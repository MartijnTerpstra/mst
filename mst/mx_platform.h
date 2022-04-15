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

#define MST_PLATFORM_WINDOWS 0
#define MST_PLATFORM_LINUX	 0
#define MST_PLATFORM_MAC	 0
#define MST_PLATFORM_IOS	 0
#define MST_PLATFORM_ANDROID 0

#if defined(__WINDOWS__) || defined(_WIN16) || defined(_WIN32)
#undef MST_PLATFORM_WINDOWS
#define MST_PLATFORM_WINDOWS 1

#elif defined(__APPLE__)

#undef MST_PLATFORM_MAC
#define MST_PLATFORM_MAC 1

#elif defined(__linux__) || defined(__unix) || defined(__linux)

#undef MST_PLATFORM_LINUX
#define MST_PLATFORM_LINUX 1

#else

#error "Operating system not supported"

#endif

namespace mst {

namespace _Details {

const char* get_os_name_impl() noexcept;

const char* get_os_version_string_impl() noexcept;

bool get_downloads_folder_impl(char* path) noexcept;
bool get_desktop_folder_impl(char* path) noexcept;
bool get_mydocuments_folder_impl(char* path) noexcept;
bool get_temp_folder_impl(char* path) noexcept;
bool get_recycle_bin_folder_impl(char* path) noexcept;
bool get_current_directory_impl(char* path) noexcept;
bool set_current_directory_impl(const char* path) noexcept;
uint32_t get_page_size_impl() noexcept;
uint32_t get_processor_core_count_impl() noexcept;
uint32_t get_processor_thread_count_impl() noexcept;
uint64_t processor_features_impl() noexcept;

uint64_t get_current_thread_id() noexcept;

} // namespace _Details
} // namespace mst