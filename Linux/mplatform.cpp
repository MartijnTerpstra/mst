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
#include <mutex>
#include <cpuid.h>

#include <string.h>

#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <set>
#include <map>

static std::string get_os_name_init()
{
	struct utsname buffer;

	if(uname(&buffer) != 0)
	{
		abort();
	}
	return buffer.sysname;
}

const char* mst::platform::_Details::get_os_name_impl() noexcept
{
	static const ::std::string osname = get_os_name_init();

	return osname.c_str();
}

static std::string get_os_version_string_init()
{
	struct utsname buffer;

	if(uname(&buffer) != 0)
	{
		abort();
	}

	if(strstr(buffer.version, "Ubuntu"))
	{
		return std::string("Ubuntu");
	}

	return buffer.version;
}

const char* mst::platform::_Details::get_os_version_string_impl() noexcept
{
	static const ::std::string osversion = get_os_version_string_init();

	return osversion.c_str();
}

struct SpecialFolderPaths
{
	std::string downloads;
	std::string dektop;
	std::string mydocuments;
	std::string temp;
	std::string recycleBin;
};

static SpecialFolderPaths get_special_folders_init()
{
	const std::string home = std::getenv("HOME");
	std::ifstream userDirs(home + "/.config/user-dirs.dirs");

	if(userDirs.fail())
		return SpecialFolderPaths{};

	std::string line;
	std::map<std::string, std::string> mappings;
	while(!userDirs.eof())
	{
		std::getline(userDirs, line);

		if(line.find("XDG_") == 0)
		{
			const auto midIdx = line.find_first_of('=');
			const auto key = line.substr(0, midIdx);
			auto value = line.substr(midIdx + 2, line.length() - 3 - midIdx);

			if(value.find("$HOME/") == 0)
			{
				value = home + value.substr(5);
			}

			mappings[key] = std::move(value);
		}
	}

	std::string temp = "/tmp";

	std::string recycleBin;
	struct stat st;
	if(stat((home + "/.local/share/Trash/files").c_str(), &st) == 0 && S_ISDIR(st.st_mode))
	{
		recycleBin = home + "/.local/share/Trash/files";
	}

	return SpecialFolderPaths{ mappings["XDG_DOWNLOAD_DIR"], mappings["XDG_DESKTOP_DIR"],
		mappings["XDG_DOCUMENTS_DIR"], temp, recycleBin };
}

static const SpecialFolderPaths& get_special_folders_impl()
{
	static const SpecialFolderPaths paths = get_special_folders_init();

	return paths;
}

bool mst::platform::_Details::get_downloads_folder_impl(char* path) noexcept
{
	strcpy(path, get_special_folders_impl().downloads.c_str());
	return path[0] != 0;
}

bool mst::platform::_Details::get_desktop_folder_impl(char* path) noexcept
{
	strcpy(path, get_special_folders_impl().dektop.c_str());
	return path[0] != 0;
}

bool mst::platform::_Details::get_mydocuments_folder_impl(char* path) noexcept
{
	strcpy(path, get_special_folders_impl().mydocuments.c_str());
	return path[0] != 0;
}

bool mst::platform::_Details::get_temp_folder_impl(char* path) noexcept
{
	strcpy(path, get_special_folders_impl().temp.c_str());
	return path[0] != 0;
}

bool mst::platform::_Details::get_recycle_bin_folder_impl(char* path) noexcept
{
	strcpy(path, get_special_folders_impl().recycleBin.c_str());
	return path[0] != 0;
}

bool mst::platform::_Details::create_directory_impl(const char* path) noexcept
{
	return mkdir(path, 0777) == 0;
}

bool mst::platform::_Details::get_current_directory_impl(char* path) noexcept
{
	return getcwd(path, 1024) != nullptr;
}

bool mst::platform::_Details::set_current_directory_impl(const char* path) noexcept
{
	return chdir(path) == 0;
}

static inline uint32_t get_page_size_init()
{
	return static_cast<uint32_t>(sysconf(_SC_PAGE_SIZE));
}

uint32_t mst::platform::_Details::get_page_size_impl() noexcept
{
	static const uint32_t pageSize = get_page_size_init();

	return pageSize;
}

struct ProcCpuInfo
{
	uint32_t coreCount;
	uint32_t threadCount;
};

static ProcCpuInfo get_proc_cpuinfo_init() noexcept
{
	// parse /proc/cpuinfo
	std::ifstream cpuinfo("/proc/cpuinfo");

	std::set<std::string> cores;
	uint32_t threadCount = 0;
	std::string line;
	std::string coreId;
	std::string pyhsicalId;
	while(!cpuinfo.eof())
	{
		std::getline(cpuinfo, line);

		if(line.find("processor") == 0)
		{
			++threadCount;
		}
		if(line.find("core id") == 0)
		{
			coreId = line.substr(line.find_first_of(':') + 1);
			if(!pyhsicalId.empty())
			{
				cores.insert(pyhsicalId + ":" + coreId);
				coreId.clear();
				pyhsicalId.clear();
			}
		}
		if(line.find("physical id") == 0)
		{
			pyhsicalId = line.substr(line.find_first_of(':') + 1);
			if(!coreId.empty())
			{
				cores.insert(pyhsicalId + ":" + coreId);
				coreId.clear();
				pyhsicalId.clear();
			}
		}
	}

	return ProcCpuInfo{ static_cast<uint32_t>(cores.size()), threadCount };
}

static const ProcCpuInfo& get_proc_cpuinfo_impl() noexcept
{
	static const ProcCpuInfo cpuInfo = get_proc_cpuinfo_init();

	return cpuInfo;
}

uint32_t mst::platform::_Details::get_processor_core_count_impl() noexcept
{
	return get_proc_cpuinfo_impl().coreCount;
}

uint32_t mst::platform::_Details::get_processor_thread_count_impl() noexcept
{
	return get_proc_cpuinfo_impl().threadCount;
}

#define EDX_MMX_bit		 0x800000	// 23 bit
#define EDX_SSE_bit		 0x2000000	// 25 bit
#define EDX_SSE2_bit	 0x4000000	// 26 bit
#define EDX_3DnowExt_bit 0x40000000 // 30 bit
#define EDX_3Dnow_bit	 0x80000000 // 31 bit
#define EDX_MMXplus_bit	 0x400000	// 22 bit

#define ECX_SSE3_bit  0x1	   // 0 bit
#define ECX_SSSE3_bit 0x200	   // 9 bit
#define ECX_SSE41_bit 0x80000  // 19 bit
#define ECX_SSE42_bit 0x100000 // 20 bit

#define ECX_SSE4A_bit 0x40	// 6 bit
#define ECX_SSE5_bit  0x800 // 11 bit

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

	uint32_t CPUInfo[4];
	uint32_t dwECX = 0;
	uint32_t dwEDX = 0;
	uint32_t dwEBX = 0;

	__get_cpuid(0, CPUInfo, CPUInfo + 1, CPUInfo + 2, CPUInfo + 3);

	int id_count = CPUInfo[0];

	if(id_count >= 1)
	{
		__get_cpuid(1, &CPUInfo[0], &CPUInfo[1], &CPUInfo[2], &CPUInfo[3]);
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
		memset(CPUInfo, 0, sizeof(CPUInfo));
		CPUInfo[0] = 7;
		__get_cpuid(0, &CPUInfo[0], &CPUInfo[1], &CPUInfo[2], &CPUInfo[3]);
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