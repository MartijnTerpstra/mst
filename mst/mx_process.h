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

#pragma once

#include <mcore.h>
#include <cstdint>
#include <string>
#include <vector>

namespace mst ::_Details {

struct process_info_impl
{
	uint32_t pid = 0;
	uint32_t parentProcessId = 0;
	::std::string name;
	::std::string path;
};

uint32_t get_current_process_id_impl() noexcept;

bool get_process_info_impl(uint32_t pid, process_info_impl& outInfo) noexcept;

::std::vector<uint32_t> find_process_ids_by_name_impl(const ::std::string& name) noexcept;

// spawns `executablePath` (searched via the usual OS rules if not an absolute path) as a child
// process with the given command-line arguments (not including argv[0]) and, if non-empty,
// `workingDirectory` as its initial directory; fills outPid/outHandle and returns true on
// success. outHandle is a native waitable handle on Windows and unused (left null) elsewhere.
bool create_process_impl(const ::std::string& executablePath,
	const ::std::vector<::std::string>& arguments, const ::std::string& workingDirectory,
	uint32_t& outPid, void*& outHandle) noexcept;

// blocks until the process identified by pid/handle exits or timeoutMilliseconds elapses
// (a negative value waits indefinitely); on success, fills outExitCode and returns true.
bool wait_process_impl(
	uint32_t pid, void* handle, int64_t timeoutMilliseconds, int& outExitCode) noexcept;

// releases any OS resources tied to a process created via create_process_impl. Best-effort: on
// platforms where an un-waited child leaves a zombie entry behind (POSIX), this performs a
// non-blocking reap, which only succeeds once the child has actually exited.
void close_process_impl(uint32_t pid, void* handle) noexcept;

} // namespace mst::_Details
