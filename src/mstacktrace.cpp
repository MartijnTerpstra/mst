//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
//                                                                                          //
//      Permission is hereby granted, free of charge, to any person obtaining a copy        //
//      of this software and associated documentation files (the "Software"), to deal       //
//      in the Software without restriction, including without limitation the rights        //
//      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell           //
//      copies of the Software, and to permit persons to whom the Software is               //
//      furnished to do so, subject to the following conditions:                            //
//                                                                                          //
//      The above copyright notice and this permission notice shall be included in          //
//      all copies or substantial portions of the Software.                                 //
//                                                                                          //
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          //
//      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            //
//      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE         //
//      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              //
//      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       //
//      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           //
//      THE SOFTWARE.                                                                       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#include <mstacktrace.h>
#include <mplatform.h>

#if MST_PLATFORM_WINDOWS

#include <mutex>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "Dbghelp.lib")

namespace {

// DbgHelp's Sym* functions aren't thread safe for a given process handle, so access is
// serialized through this single handler.
struct SymbolHandler
{
	SymbolHandler() noexcept
		: process(::GetCurrentProcess())
	{
		::SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
		initialized = ::SymInitialize(process, nullptr, TRUE) != FALSE;
	}

	~SymbolHandler()
	{
		if(initialized)
			::SymCleanup(process);
	}

	HANDLE process;
	bool initialized;
	::std::mutex mutex;
};

SymbolHandler& GetSymbolHandler() noexcept
{
	static SymbolHandler handler;
	return handler;
}

} // namespace

size_t mst::_Details::stacktrace_capture_impl(
	void** frames, size_t frameCount, size_t skip) noexcept
{
	if(frameCount == 0)
		return 0;

	(void)GetSymbolHandler();

	// +1 drops the frame for this call to CaptureStackBackTrace itself
	const auto captured = ::CaptureStackBackTrace(
		static_cast<DWORD>(skip + 1), static_cast<DWORD>(frameCount), frames, nullptr);

	return static_cast<size_t>(captured);
}

mst::_Details::stacktrace_symbol_info mst::_Details::stacktrace_resolve_impl(void* frame) noexcept
{
	stacktrace_symbol_info info;

	auto& handler = GetSymbolHandler();

	if(!handler.initialized)
		return info;

	::std::lock_guard<::std::mutex> lock(handler.mutex);

	const auto address = reinterpret_cast<DWORD64>(frame);

	alignas(SYMBOL_INFO) char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
	auto* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	DWORD64 nameDisplacement = 0;
	if(::SymFromAddr(handler.process, address, &nameDisplacement, symbol))
	{
		info.description.assign(symbol->Name);
	}

	IMAGEHLP_LINE64 line;
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	DWORD lineDisplacement = 0;
	if(::SymGetLineFromAddr64(handler.process, address, &lineDisplacement, &line))
	{
		info.sourceFile.assign(line.FileName);
		info.sourceLine = static_cast<uint32_t>(line.LineNumber);
	}

	return info;
}

#elif MST_PLATFORM_LINUX || MST_PLATFORM_MAC

#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>

namespace {

::std::string Demangle(const char* mangledName) noexcept
{
	if(mangledName == nullptr)
		return ::std::string();

	int status = 0;
	const ::std::unique_ptr<char, void (*)(void*)> demangled{
		abi::__cxa_demangle(mangledName, nullptr, nullptr, &status), &::std::free
	};

	if(status == 0 && demangled)
		return ::std::string(demangled.get());

	return ::std::string(mangledName);
}

#if MST_PLATFORM_LINUX

// ET_DYN (shared objects and PIE executables) needs the runtime address rebased to the file's
// link-time address space before it's handed to addr2line; ET_EXEC doesn't.
bool TryReadElfIsPositionIndependent(const char* path, bool& isPositionIndependent) noexcept
{
	::std::FILE* file = ::std::fopen(path, "rb");
	if(file == nullptr)
		return false;

	unsigned char header[18];
	const auto bytesRead = ::std::fread(header, 1, sizeof(header), file);
	::std::fclose(file);

	if(bytesRead != sizeof(header) || header[0] != 0x7F || header[1] != 'E' || header[2] != 'L' ||
		header[3] != 'F')
	{
		return false;
	}

	// e_type: 2-byte little-endian field at offset 16, ET_DYN == 3
	const uint16_t elfType =
		static_cast<uint16_t>(header[16]) | (static_cast<uint16_t>(header[17]) << 8);

	isPositionIndependent = (elfType == 3);
	return true;
}

// best-effort file/line resolution via addr2line; leaves sourceFile/sourceLine untouched on
// any failure (missing tool, stripped binary, unresolvable address, ...)
void ResolveSourceLocation(const char* modulePath, void* moduleBase, void* absoluteAddress,
	::mst::_Details::stacktrace_symbol_info& info) noexcept
{
	if(modulePath == nullptr || *modulePath == '\0')
		return;

	bool isPositionIndependent = false;
	if(!TryReadElfIsPositionIndependent(modulePath, isPositionIndependent))
		return;

	const auto queryAddress = isPositionIndependent
		? reinterpret_cast<uintptr_t>(absoluteAddress) - reinterpret_cast<uintptr_t>(moduleBase)
		: reinterpret_cast<uintptr_t>(absoluteAddress);

	::std::string command = "addr2line -e \"";
	command += modulePath;
	command += "\" -C 0x";

	char addressBuffer[2 * sizeof(uintptr_t) + 1];
	::std::snprintf(addressBuffer, sizeof(addressBuffer), "%llx",
		static_cast<unsigned long long>(queryAddress));
	command += addressBuffer;

	::std::FILE* pipe = ::popen(command.c_str(), "r");
	if(pipe == nullptr)
		return;

	::std::array<char, 1024> line{};
	const auto* result = ::std::fgets(line.data(), static_cast<int>(line.size()), pipe);
	::pclose(pipe);

	if(result == nullptr)
		return;

	::std::string fileAndLine(line.data());
	while(!fileAndLine.empty() && (fileAndLine.back() == '\n' || fileAndLine.back() == '\r'))
	{
		fileAndLine.pop_back();
	}

	if(fileAndLine.empty() || fileAndLine == "??:0" || fileAndLine == "??:?")
		return;

	const auto separator = fileAndLine.find_last_of(':');
	if(separator == ::std::string::npos)
		return;

	info.sourceFile = fileAndLine.substr(0, separator);

	const auto lineNumberText = fileAndLine.substr(separator + 1);
	if(!lineNumberText.empty() &&
		lineNumberText.find_first_not_of("0123456789") == ::std::string::npos)
	{
		info.sourceLine =
			static_cast<uint32_t>(::std::strtoul(lineNumberText.c_str(), nullptr, 10));
	}
}

#endif // MST_PLATFORM_LINUX

} // namespace

size_t mst::_Details::stacktrace_capture_impl(
	void** frames, size_t frameCount, size_t skip) noexcept
{
	if(frameCount == 0)
		return 0;

	// backtrace() can't skip frames itself, so over-capture and trim; +1 drops this function's
	// own frame for the call to backtrace()
	const size_t totalSkip = skip + 1;

	::std::vector<void*> rawFrames(frameCount + totalSkip);

	const auto rawCount = ::backtrace(rawFrames.data(), static_cast<int>(rawFrames.size()));
	if(rawCount <= 0 || static_cast<size_t>(rawCount) <= totalSkip)
		return 0;

	const auto usableCount = static_cast<size_t>(rawCount) - totalSkip;
	const auto resultCount = usableCount < frameCount ? usableCount : frameCount;

	for(size_t i = 0; i < resultCount; ++i)
	{
		frames[i] = rawFrames[totalSkip + i];
	}

	return resultCount;
}

mst::_Details::stacktrace_symbol_info mst::_Details::stacktrace_resolve_impl(void* frame) noexcept
{
	stacktrace_symbol_info info;

	Dl_info dlInfo{};
	if(::dladdr(frame, &dlInfo) == 0)
		return info;

	if(dlInfo.dli_sname != nullptr)
	{
		info.description = Demangle(dlInfo.dli_sname);
	}

#if MST_PLATFORM_LINUX
	ResolveSourceLocation(dlInfo.dli_fname, dlInfo.dli_fbase, frame, info);
#endif

	return info;
}

#else

#error "stacktrace not implemented for this platform"

#endif
