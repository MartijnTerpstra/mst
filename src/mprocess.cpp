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

#include <mprocess.h>
#include <mplatform.h>

#if MST_PLATFORM_WINDOWS

#include <Windows.h>
#include <TlHelp32.h>

uint32_t mst::_Details::get_current_process_id_impl() noexcept
{
	return static_cast<uint32_t>(::GetCurrentProcessId());
}

namespace {

bool ResolveProcessPath(uint32_t pid, ::std::string& path) noexcept
{
	const HANDLE process = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if(process == nullptr)
		return false;

	char buffer[1024];
	DWORD size = static_cast<DWORD>(sizeof(buffer));

	const bool success = ::QueryFullProcessImageNameA(process, 0, buffer, &size) != FALSE;

	::CloseHandle(process);

	if(!success)
		return false;

	path.assign(buffer, size);
	return true;
}

} // namespace

bool mst::_Details::get_process_info_impl(uint32_t pid, process_info_impl& outInfo) noexcept
{
	const HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(snapshot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	bool found = false;

	if(::Process32First(snapshot, &entry))
	{
		do
		{
			if(entry.th32ProcessID == pid)
			{
				outInfo.pid = pid;
				outInfo.parentProcessId = static_cast<uint32_t>(entry.th32ParentProcessID);
				outInfo.name = entry.szExeFile;
				found = true;
				break;
			}
		} while(::Process32Next(snapshot, &entry));
	}

	::CloseHandle(snapshot);

	if(!found)
		return false;

	if(!ResolveProcessPath(pid, outInfo.path))
	{
		outInfo.path.clear();
	}

	return true;
}

::std::vector<uint32_t> mst::_Details::find_process_ids_by_name_impl(
	const ::std::string& name) noexcept
{
	::std::vector<uint32_t> result;

	const HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(snapshot == INVALID_HANDLE_VALUE)
		return result;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if(::Process32First(snapshot, &entry))
	{
		do
		{
			if(::_stricmp(entry.szExeFile, name.c_str()) == 0)
			{
				result.push_back(static_cast<uint32_t>(entry.th32ProcessID));
			}
		} while(::Process32Next(snapshot, &entry));
	}

	::CloseHandle(snapshot);

	return result;
}

namespace {

// only quotes an argument (escaping embedded double quotes) when it actually needs it, i.e.
// when it contains whitespace/quotes or is empty; plain tokens like "/C" are left bare. Not a
// full implementation of the (famously fiddly) Windows command-line quoting rules around
// trailing backslashes, but good enough for the common case of plain arguments/paths - and
// leaving unremarkable tokens unquoted matters in practice: cmd.exe's /C parses the raw command
// line itself rather than via argv, and stops recognizing /C as the switch it is if it's quoted.
void AppendArgument(::std::string& commandLine, const ::std::string& argument) noexcept
{
	const bool needsQuoting =
		argument.empty() || argument.find_first_of(" \t\"") != ::std::string::npos;

	if(!needsQuoting)
	{
		commandLine += argument;
		return;
	}

	commandLine += '"';

	for(const char c : argument)
	{
		if(c == '"')
			commandLine += '\\';

		commandLine += c;
	}

	commandLine += '"';
}

} // namespace

bool mst::_Details::create_process_impl(const ::std::string& executablePath,
	const ::std::vector<::std::string>& arguments, const ::std::string& workingDirectory,
	uint32_t& outPid, void*& outHandle) noexcept
{
	::std::string commandLine;
	AppendArgument(commandLine, executablePath);

	for(const auto& argument : arguments)
	{
		commandLine += ' ';
		AppendArgument(commandLine, argument);
	}

	STARTUPINFOA startupInfo{};
	startupInfo.cb = sizeof(startupInfo);

	PROCESS_INFORMATION processInfo{};

	// lpCommandLine must point at a writable buffer; commandLine.data() (non-const overload,
	// C++17) is exactly that
	const auto success = ::CreateProcessA(nullptr, commandLine.data(), nullptr, nullptr, FALSE, 0,
		nullptr, workingDirectory.empty() ? nullptr : workingDirectory.c_str(), &startupInfo,
		&processInfo);

	if(!success)
		return false;

	::CloseHandle(processInfo.hThread);

	outPid = static_cast<uint32_t>(processInfo.dwProcessId);
	outHandle = processInfo.hProcess;

	return true;
}

bool mst::_Details::wait_process_impl(
	uint32_t pid, void* handle, int64_t timeoutMilliseconds, int& outExitCode) noexcept
{
	(void)pid;

	if(handle == nullptr)
		return false;

	const DWORD timeout =
		timeoutMilliseconds < 0 ? INFINITE : static_cast<DWORD>(timeoutMilliseconds);

	if(::WaitForSingleObject(handle, timeout) != WAIT_OBJECT_0)
		return false;

	DWORD exitCode = 0;
	if(!::GetExitCodeProcess(handle, &exitCode))
		return false;

	outExitCode = static_cast<int>(exitCode);
	return true;
}

void mst::_Details::close_process_impl(uint32_t pid, void* handle) noexcept
{
	(void)pid;

	if(handle != nullptr)
	{
		::CloseHandle(handle);
	}
}

#elif MST_PLATFORM_LINUX || MST_PLATFORM_MAC

#include <cerrno>
#include <chrono>
#include <thread>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

uint32_t mst::_Details::get_current_process_id_impl() noexcept
{
	return static_cast<uint32_t>(::getpid());
}

bool mst::_Details::create_process_impl(const ::std::string& executablePath,
	const ::std::vector<::std::string>& arguments, const ::std::string& workingDirectory,
	uint32_t& outPid, void*& outHandle) noexcept
{
	outHandle = nullptr;

	::std::vector<char*> argv;
	argv.reserve(arguments.size() + 2);
	argv.push_back(const_cast<char*>(executablePath.c_str()));
	for(const auto& argument : arguments)
	{
		argv.push_back(const_cast<char*>(argument.c_str()));
	}
	argv.push_back(nullptr);

	// fork()/execvp() can't report a failing exec (bad path, non-executable, ...) through a
	// return value - the failure happens in the child, after fork() has already returned
	// successfully in the parent. Use the classic self-pipe trick: the write end is marked
	// FD_CLOEXEC, so it closes on its own the moment execvp() succeeds; if execvp() instead
	// fails, the child writes its errno through the pipe before exiting, which the parent
	// reads (getting either an errno, meaning failure, or EOF, meaning the exec went through).
	int pipeFds[2];
	if(::pipe(pipeFds) != 0)
		return false;

	::fcntl(pipeFds[0], F_SETFD, FD_CLOEXEC);
	::fcntl(pipeFds[1], F_SETFD, FD_CLOEXEC);

	const pid_t pid = ::fork();

	if(pid < 0)
	{
		::close(pipeFds[0]);
		::close(pipeFds[1]);
		return false;
	}

	if(pid == 0)
	{
		// child: on any failure below, bail out without running any C++ destructors/atexit
		// handlers meant for the parent
		::close(pipeFds[0]);

		if(!workingDirectory.empty() && ::chdir(workingDirectory.c_str()) != 0)
		{
			const int error = errno;
			(void)::write(pipeFds[1], &error, sizeof(error));
			::_exit(127);
		}

		::execvp(executablePath.c_str(), argv.data());

		// execvp() only returns on failure
		const int error = errno;
		(void)::write(pipeFds[1], &error, sizeof(error));
		::_exit(127);
	}

	::close(pipeFds[1]);

	int childError = 0;
	const bool execFailed = ::read(pipeFds[0], &childError, sizeof(childError)) > 0;
	::close(pipeFds[0]);

	if(execFailed)
	{
		// reap the child now so it doesn't linger as a zombie; nothing else will ever wait() it
		int status = 0;
		::waitpid(pid, &status, 0);
		return false;
	}

	outPid = static_cast<uint32_t>(pid);
	return true;
}

bool mst::_Details::wait_process_impl(
	uint32_t pid, void* handle, int64_t timeoutMilliseconds, int& outExitCode) noexcept
{
	(void)handle;

	int status = 0;

	if(timeoutMilliseconds < 0)
	{
		if(::waitpid(static_cast<pid_t>(pid), &status, 0) < 0)
			return false;
	}
	else
	{
		// waitpid() has no native timeout, so poll it with WNOHANG until the deadline
		const auto deadline =
			::std::chrono::steady_clock::now() + ::std::chrono::milliseconds(timeoutMilliseconds);

		for(;;)
		{
			const auto result = ::waitpid(static_cast<pid_t>(pid), &status, WNOHANG);

			if(result > 0)
				break;

			if(result < 0)
				return false;

			if(::std::chrono::steady_clock::now() >= deadline)
				return false;

			::std::this_thread::sleep_for(::std::chrono::milliseconds(1));
		}
	}

	if(WIFEXITED(status))
	{
		outExitCode = WEXITSTATUS(status);
	}
	else if(WIFSIGNALED(status))
	{
		// mirror the common shell convention for processes killed by a signal
		outExitCode = 128 + WTERMSIG(status);
	}
	else
	{
		outExitCode = -1;
	}

	return true;
}

void mst::_Details::close_process_impl(uint32_t pid, void* handle) noexcept
{
	(void)handle;

	// best-effort, non-blocking reap: only actually collects the child (and its zombie table
	// entry) if it has already exited; a still-running, never-waited child is left running
	int status = 0;
	::waitpid(static_cast<pid_t>(pid), &status, WNOHANG);
}

#if MST_PLATFORM_LINUX

#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

bool mst::_Details::get_process_info_impl(uint32_t pid, process_info_impl& outInfo) noexcept
{
	const ::std::string procDir = "/proc/" + ::std::to_string(pid);

	struct stat st;
	if(::stat(procDir.c_str(), &st) != 0)
		return false;

	uint32_t parentPid = 0;

	::std::ifstream status(procDir + "/status");
	if(!status.fail())
	{
		::std::string line;
		while(::std::getline(status, line))
		{
			if(line.find("PPid:") == 0)
			{
				parentPid = static_cast<uint32_t>(::std::strtoul(line.c_str() + 5, nullptr, 10));
				break;
			}
		}
	}

	char pathBuffer[4096];
	::std::string path;

	const auto len = ::readlink((procDir + "/exe").c_str(), pathBuffer, sizeof(pathBuffer) - 1);
	if(len > 0)
	{
		path.assign(pathBuffer, static_cast<size_t>(len));
	}

	::std::string name = path;
	const auto slash = path.find_last_of('/');
	if(slash != ::std::string::npos)
	{
		name = path.substr(slash + 1);
	}

	if(name.empty())
	{
		// exe symlink couldn't be resolved (e.g. kernel thread, permission denied); fall back to
		// the (possibly truncated) comm name
		::std::ifstream comm(procDir + "/comm");
		if(!comm.fail())
		{
			::std::getline(comm, name);
		}
	}

	outInfo.pid = pid;
	outInfo.parentProcessId = parentPid;
	outInfo.name = ::std::move(name);
	outInfo.path = ::std::move(path);

	return true;
}

::std::vector<uint32_t> mst::_Details::find_process_ids_by_name_impl(
	const ::std::string& name) noexcept
{
	::std::vector<uint32_t> result;

	::DIR* proc = ::opendir("/proc");
	if(proc == nullptr)
		return result;

	while(const auto* entry = ::readdir(proc))
	{
		const auto* dname = entry->d_name;
		if(dname[0] < '0' || dname[0] > '9')
			continue;

		char* end = nullptr;
		const auto pid = static_cast<uint32_t>(::std::strtoul(dname, &end, 10));
		if(end == nullptr || *end != '\0')
			continue;

		process_info_impl info;
		if(get_process_info_impl(pid, info) && info.name == name)
		{
			result.push_back(pid);
		}
	}

	::closedir(proc);

	return result;
}

#elif MST_PLATFORM_MAC

#include <libproc.h>
#include <sys/sysctl.h>

bool mst::_Details::get_process_info_impl(uint32_t pid, process_info_impl& outInfo) noexcept
{
	int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, static_cast<int>(pid) };
	struct kinfo_proc kp;
	size_t size = sizeof(kp);

	if(::sysctl(mib, 4, &kp, &size, nullptr, 0) != 0 || size == 0)
		return false;

	char pathBuffer[PROC_PIDPATHINFO_MAXSIZE];
	::std::string path;

	const auto pathLen = ::proc_pidpath(static_cast<pid_t>(pid), pathBuffer, sizeof(pathBuffer));
	if(pathLen > 0)
	{
		path.assign(pathBuffer, static_cast<size_t>(pathLen));
	}

	::std::string name = path;
	const auto slash = path.find_last_of('/');
	if(slash != ::std::string::npos)
	{
		name = path.substr(slash + 1);
	}

	if(name.empty())
	{
		// proc_pidpath() needs the target process to be owned by us (or elevated privileges);
		// fall back to the (possibly truncated) short name sysctl always provides
		name = kp.kp_proc.p_comm;
	}

	outInfo.pid = pid;
	outInfo.parentProcessId = static_cast<uint32_t>(kp.kp_eproc.e_ppid);
	outInfo.name = ::std::move(name);
	outInfo.path = ::std::move(path);

	return true;
}

::std::vector<uint32_t> mst::_Details::find_process_ids_by_name_impl(
	const ::std::string& name) noexcept
{
	::std::vector<uint32_t> result;

	const auto bufferSize = ::proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
	if(bufferSize <= 0)
		return result;

	// the process list can grow between the sizing call and the fetch call, so pad the buffer
	::std::vector<pid_t> pids(static_cast<size_t>(bufferSize) / sizeof(pid_t) + 16);

	const auto actualSize = ::proc_listpids(
		PROC_ALL_PIDS, 0, pids.data(), static_cast<int>(pids.size() * sizeof(pid_t)));
	if(actualSize <= 0)
		return result;

	const auto count = static_cast<size_t>(actualSize) / sizeof(pid_t);

	for(size_t i = 0; i < count; ++i)
	{
		if(pids[i] == 0)
			continue;

		process_info_impl info;
		if(get_process_info_impl(static_cast<uint32_t>(pids[i]), info) && info.name == name)
		{
			result.push_back(static_cast<uint32_t>(pids[i]));
		}
	}

	return result;
}

#else
#error "mprocess process_info lookup not implemented for this platform"
#endif

#else

#error "mprocess not implemented for this platform"

#endif
