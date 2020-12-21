//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <mplatform.h>
#include <mdebug.h>
#include <vector>
#include <thread>
#include <intrin.h>

#include <Windows.h>
#include <ShlObj.h>
#include <VersionHelpers.h>
#include <direct.h>

#define WINDOWS_10_PLATFORM_DEFINE 0x0A00

const char* mst::platform::_Details::get_os_name_impl() noexcept
{
	return "Windows";
}

const char* mst::platform::_Details::get_os_version_string_impl() noexcept
{
	if(IsWindowsServer())
	{
		if(IsWindowsVersionOrGreater(10, 0, 0))
		{
			return "Server 2014";
		}

		if(IsWindowsVersionOrGreater(6, 3, 0))
		{
			return "Server 2012 R2";
		}

		if(IsWindowsVersionOrGreater(6, 2, 0))
		{
			return "Server 2012";
		}

		if(IsWindowsVersionOrGreater(6, 1, 1))
		{
			return "Server 2008 R2 SP1";
		}

		if(IsWindowsVersionOrGreater(6, 1, 0))
		{
			return "Server 2008 R2";
		}

		if(IsWindowsVersionOrGreater(6, 0, 2))
		{
			return "Server 2008 SP2";
		}

		if(IsWindowsVersionOrGreater(6, 0, 1))
		{
			return "Server 2008 SP1";
		}

		if(IsWindowsVersionOrGreater(6, 0, 0))
		{
			return "Server 2008";
		}

		if(IsWindowsVersionOrGreater(5, 2, 2))
		{
			return "Server 2003 SP2";
		}

		if(IsWindowsVersionOrGreater(5, 2, 1))
		{
			return "Server 2003 SP1";
		}

		if(IsWindowsVersionOrGreater(5, 2, 0))
		{
			return "Server 2003";
		}

		if(IsWindowsVersionOrGreater(5, 1, 4))
		{
			return "Server 2000 SP4";
		}

		if(IsWindowsVersionOrGreater(5, 1, 3))
		{
			return "Server 2000 SP3";
		}

		if(IsWindowsVersionOrGreater(5, 1, 2))
		{
			return "Server 2000 SP2";
		}

		if(IsWindowsVersionOrGreater(5, 1, 1))
		{
			return "Server 2000 SP1";
		}

		if(IsWindowsVersionOrGreater(5, 1, 0))
		{
			return "Server 2000";
		}
	}
	else
	{
		if(IsWindowsVersionOrGreater(
			   HIBYTE(WINDOWS_10_PLATFORM_DEFINE), LOBYTE(WINDOWS_10_PLATFORM_DEFINE), 0))
		{
			return "10";
		}

		if(IsWindows8Point1OrGreater())
		{
			return "8.1";
		}

		if(IsWindows8OrGreater())
		{
			return "8.0";
		}

		if(IsWindows7SP1OrGreater())
		{
			return "7 SP1";
		}

		if(IsWindows7OrGreater())
		{
			return "7";
		}

		if(IsWindowsVistaSP2OrGreater())
		{
			return "Vista SP2";
		}

		if(IsWindowsVistaSP1OrGreater())
		{
			return "Vista SP1";
		}

		if(IsWindowsVistaOrGreater())
		{
			return "Vista";
		}

		if(IsWindowsXPSP3OrGreater())
		{
			return "XP SP3";
		}

		if(IsWindowsXPSP2OrGreater())
		{
			return "XP SP2";
		}

		if(IsWindowsXPSP1OrGreater())
		{
			return "XP SP1";
		}

		if(IsWindowsXPOrGreater())
		{
			return "XP";
		}
	}

	return "Unknown";
}

static bool _Get_special_folder_impl(const KNOWNFOLDERID& folderID, char* path) noexcept
{
	PWSTR str;
	if(FAILED(SHGetKnownFolderPath(folderID, 0, nullptr, &str)))
	{
		return false;
	}

	if(WideCharToMultiByte(CP_UTF8, 0, str, -1, path, 1024, nullptr, nullptr) == 0)
	{
		CoTaskMemFree(str);
		return false;
	}

	CoTaskMemFree(str);
	return true;
}

bool mst::platform::_Details::get_downloads_folder_impl(char* path) noexcept
{
	return _Get_special_folder_impl(FOLDERID_Downloads, path);
}

bool mst::platform::_Details::get_desktop_folder_impl(char* path) noexcept
{
	return _Get_special_folder_impl(FOLDERID_Desktop, path);
}

bool mst::platform::_Details::get_mydocuments_folder_impl(char* path) noexcept
{
	return _Get_special_folder_impl(FOLDERID_Documents, path);
}

bool mst::platform::_Details::get_temp_folder_impl(char* path) noexcept
{
	if(_Get_special_folder_impl(FOLDERID_LocalAppData, path))
	{
		strcat_s(path, 1024, "\\TEMP");
		return true;
	}
	return false;
}

bool mst::platform::_Details::get_recycle_bin_folder_impl(char* path) noexcept
{
	return _Get_special_folder_impl(FOLDERID_RecycleBinFolder, path);
}

bool mst::platform::_Details::create_directory_impl(const char* path) noexcept
{
	return _mkdir(path) == 0;
}

bool mst::platform::_Details::get_current_directory_impl(char* path) noexcept
{
	return GetCurrentDirectoryA(1024, path) != 0;
}

bool mst::platform::_Details::set_current_directory_impl(const char* path) noexcept
{
	return SetCurrentDirectoryA(path) != FALSE;
}


static uint32_t get_page_size_init() noexcept
{
	SYSTEM_INFO info;

	GetSystemInfo(&info);

	return static_cast<uint32_t>(info.dwPageSize);
}

uint32_t mst::platform::_Details::get_page_size_impl() noexcept
{
	static const uint32_t pageSize = get_page_size_init();

	return pageSize;
}

static uint32_t get_processor_core_count_init() noexcept
{
	DWORD size = 0;

	GetLogicalProcessorInformation(nullptr, &size);

	if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return 0;

	std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> infos(
		size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));

	if(!GetLogicalProcessorInformation(infos.data(), &size))
		return 0;

	uint32_t coreCount = 0;

	for(auto& info : infos)
	{
		if(info.Relationship == RelationProcessorCore)
			++coreCount;
	}

	return coreCount;
}

uint32_t mst::platform::_Details::get_processor_core_count_impl() noexcept
{
	static const uint32_t pageSize = get_processor_core_count_init();

	return pageSize;
}

static inline uint32_t get_processor_thread_count_init() noexcept
{
	SYSTEM_INFO info;

	GetSystemInfo(&info);

	return static_cast<uint32_t>(info.dwNumberOfProcessors);
}

uint32_t mst::platform::_Details::get_processor_thread_count_impl() noexcept
{
	static const uint32_t pageSize = get_processor_thread_count_init();

	return pageSize;
}

#define EDX_MMX_bit	 0x800000  // 23 bit
#define EDX_SSE_bit	 0x2000000 // 25 bit
#define EDX_SSE2_bit 0x4000000 // 26 bit

#define ECX_SSE3_bit  0x1	   // 0 bit
#define ECX_SSSE3_bit 0x200	   // 9 bit
#define ECX_SSE41_bit 0x80000  // 19 bit
#define ECX_SSE42_bit 0x100000 // 20 bit

#define ECX_AES_bit 0x2000000  // 25 bit
#define ECX_AVX_bit 0x10000000 // 28 bit

#define EBX_AVX2_bit	 (1U << 5U)	 // 5 bit
#define EBX_AVX512F_bit	 (1U << 16U) // 25 bit
#define EBX_AVX512ER_bit (1U << 27U) // 27 bit
#define EBX_AVX512PF_bit (1U << 26U) // 26 bit
#define EBX_AVX512VL_bit (1U << 31U) // 26 bit
#define EBX_AVX512DQ_bit (1U << 17U) // 26 bit
#define EBX_AVX512BW_bit (1U << 30U) // 26 bit

static inline mst::flag<mst::platform::processor_feature_flags> processor_features_init() noexcept
{
	using mst::platform::processor_feature_flags;

	int CPUInfo[4];
	uint32_t dwECX = 0;
	uint32_t dwEDX = 0;
	uint32_t dwEBX = 0;

	__cpuid(CPUInfo, 0);

	int id_count = CPUInfo[0];

	if(id_count >= 1)
	{
		__cpuid(CPUInfo, 1);
		dwECX = static_cast<uint32_t>(CPUInfo[2]);
		dwEDX = static_cast<uint32_t>(CPUInfo[3]);
	}

	mst::flag<processor_feature_flags> features;

	if(ECX_AES_bit & dwECX)
		features.enable(processor_feature_flags::aes);

	if(EDX_MMX_bit & dwEDX)
		features.enable(processor_feature_flags::mmx);

	if(EDX_SSE_bit & dwEDX)
		features.enable(processor_feature_flags::sse);

	if(EDX_SSE2_bit & dwEDX)
		features.enable(processor_feature_flags::sse2);

	if(ECX_SSE3_bit & dwECX)
		features.enable(processor_feature_flags::sse3);

	if(ECX_SSSE3_bit & dwECX)
		features.enable(processor_feature_flags::ssse3);

	if(ECX_SSE41_bit & dwECX)
		features.enable(processor_feature_flags::sse4_1);

	if(ECX_SSE42_bit & dwECX)
		features.enable(processor_feature_flags::sse4_2);

	if(ECX_AVX_bit & dwECX)
		features.enable(processor_feature_flags::avx);

	const uint32_t ref = 43806655;

	if(id_count >= 7)
	{
		__cpuid(CPUInfo, 7);
		dwEBX = static_cast<uint32_t>(CPUInfo[1]);
	}

	if(EBX_AVX2_bit & dwEBX)
		features.enable(processor_feature_flags::avx2);

	if(EBX_AVX512F_bit & dwEBX)
		features.enable(processor_feature_flags::avx512f);

	if(EBX_AVX512ER_bit & dwEBX)
		features.enable(processor_feature_flags::avx512er);

	if(EBX_AVX512PF_bit & dwEBX)
		features.enable(processor_feature_flags::avx512pf);

	if(EBX_AVX512VL_bit & dwEBX)
		features.enable(processor_feature_flags::avx512vl);

	if(EBX_AVX512DQ_bit & dwEBX)
		features.enable(processor_feature_flags::avx512dq);

	if(EBX_AVX512BW_bit & dwEBX)
		features.enable(processor_feature_flags::avx512bw);

	return features;
}

mst::flag<mst::platform::processor_feature_flags>
mst::platform::_Details::processor_features_impl() noexcept
{
	static flag<processor_feature_flags> features = processor_features_init();

	return features;
}