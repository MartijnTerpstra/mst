//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2022 Martinus Terpstra                                                 //
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

#pragma once

#include <mcore.h>
#include <string>
#include <mflag.h>
#include <mx_platform.h>

namespace mst {
namespace platform {

inline ::std::string name() noexcept
{
	return ::mst::_Details::get_os_name_impl();
}

inline ::std::string version() noexcept
{
	return ::mst::_Details::get_os_version_string_impl();
}

inline ::std::string full_name() noexcept
{
	return name() + " " + version();
}

inline ::std::string newline() noexcept
{
#if MST_PLATFORM_WINDOWS
	return "\r\n";
#elif MST_PLATFORM_LINUX
	return "\n";
#elif MST_PLATFORM_MAC
	return "\r";
#else
#error "Platform not implemented"
#endif
}

inline char directory_separator() noexcept
{
#if MST_PLATFORM_WINDOWS
	return '\\';
#elif MST_PLATFORM_LINUX
	return '/';
#elif MST_PLATFORM_MAC
	return '/';
#else
#error "Platform not implemented"
#endif
}

inline ::std::string downloads_path() noexcept
{
	char path[1024];
	if(::mst::_Details::get_downloads_folder_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline ::std::string desktop_path() noexcept
{
	char path[1024];
	if(::mst::_Details::get_desktop_folder_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline ::std::string my_documents_path() noexcept
{
	char path[1024];
	if(::mst::_Details::get_mydocuments_folder_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline ::std::string temp_path() noexcept
{
	char path[1024];
	if(::mst::_Details::get_temp_folder_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline ::std::string recycle_bin_path() noexcept
{
	char path[1024];
	if(::mst::_Details::get_recycle_bin_folder_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline ::std::string current_directory() noexcept
{
	char path[1024];
	if(::mst::_Details::get_current_directory_impl(path))
	{
		return path;
	}
	return ::std::string();
}

inline bool set_current_directory(const ::std::string& path) noexcept
{
	// true if success, false if failed
	return ::mst::_Details::set_current_directory_impl(path.c_str());
}

inline uint32_t page_size() noexcept
{
	return ::mst::_Details::get_page_size_impl();
}

inline uint32_t processor_core_count() noexcept
{
	return ::mst::_Details::get_processor_core_count_impl();
}

inline uint32_t processor_thread_count() noexcept
{
	return ::mst::_Details::get_processor_thread_count_impl();
}

enum class processor_feature_flags : uint64_t
{
	// Advanced Encryption Standard
	aes,

	// Intel MMX
	mmx,

	// Intel Streaming SIMD Extensions
	sse,

	// Intel Streaming SIMD Extensions 2
	sse2,

	// Intel Streaming SIMD Extensions 3
	sse3,

	// Intel Supplemental Streaming SIMD Extensions 3
	ssse3,

	// Intel Streaming SIMD Extensions 4.1
	sse4_1,

	// Intel Streaming SIMD Extensions 4.2
	sse4_2,

	// Intel Advanced Vector Extensions
	avx,

	// 256-bits Intel Advanced Vector Extensions 2
	avx2,

	// 512-bits Intel Advanced Vector Extensions, Foundation
	avx512f,

	// 512-bits Intel Advanced Vector Extensions, Exponential and Reciprocal Instructions
	avx512er,

	// 512-bits Intel Advanced Vector Extensions, Prefetch Instructions
	avx512pf,

	// 512-bits Intel Advanced Vector Extensions, Vector Length Extensions
	avx512vl,

	// 512-bits Intel Advanced Vector Extensions, Doubleword and Quadword Instructions
	avx512dq,

	// 512-bits Intel Advanced Vector Extensions, Byte and Word Instructions
	avx512bw,

	// ARM Advanced SIMD
	asimd,

	// ARM neon advances vector instructions
	neon
};

inline mst::flag<processor_feature_flags> processor_features() noexcept
{
	return mst::flag<processor_feature_flags>::from_storage(_Details::processor_features_impl());
}



} // namespace platform
} // namespace mst