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
#include <mx_process.h>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace mst {
namespace platform {

using process_id_t = uint32_t;

struct process_info
{
	process_id_t pid = 0;
	process_id_t parent_pid = 0;
	::std::string name;
	::std::string path;
};

// id of the calling process
inline process_id_t current_process_id() noexcept
{
	return ::mst::_Details::get_current_process_id_impl();
}

// looks up a process by id; returns std::nullopt if no such process exists (or it could not be
// queried, e.g. insufficient privileges). `path` may come back empty even on success if the
// executable path itself couldn't be resolved.
inline ::std::optional<process_info> get_process_info(process_id_t pid) noexcept
{
	::mst::_Details::process_info_impl info;

	if(!::mst::_Details::get_process_info_impl(pid, info))
		return ::std::nullopt;

	return process_info{ info.pid, info.parentProcessId, ::std::move(info.name),
		::std::move(info.path) };
}

// finds every currently running process whose executable name matches `name` exactly
// (case-insensitive on Windows, case-sensitive on Linux/macOS). `name` includes the file
// extension where the platform has one (e.g. "notepad.exe" on Windows, "bash" on Linux).
// process names aren't unique, so this can return more than one pid, or none.
inline ::std::vector<process_id_t> find_pids_by_name(const ::std::string& name) noexcept
{
	return ::mst::_Details::find_process_ids_by_name_impl(name);
}

// a handle to either the calling process or one spawned via process::create(). Move-only: the
// underlying native resources (a Windows process handle; a POSIX zombie slot pending reap) are
// released exactly once, on destruction.
class process
{
public:
	process() noexcept = default;

	process(const process&) = delete;
	process& operator=(const process&) = delete;

	process(process&& other) noexcept
		: _Pid(other._Pid)
		, _Handle(other._Handle)
		, _Owned(other._Owned)
		, _ExitCode(other._ExitCode)
	{
		other._Pid = 0;
		other._Handle = nullptr;
		other._Owned = false;
		other._ExitCode.reset();
	}

	process& operator=(process&& other) noexcept
	{
		if(this != &other)
		{
			_Close();

			_Pid = other._Pid;
			_Handle = other._Handle;
			_Owned = other._Owned;
			_ExitCode = other._ExitCode;

			other._Pid = 0;
			other._Handle = nullptr;
			other._Owned = false;
			other._ExitCode.reset();
		}
		return *this;
	}

	~process()
	{
		_Close();
	}

	// wraps the calling process. This instance never owns a waitable handle to itself, so
	// wait()/wait_for() always return false and exit_code() always returns std::nullopt; use
	// is_running() (always true) or info() to inspect it instead.
	static process current() noexcept
	{
		process result;
		result._Pid = ::mst::_Details::get_current_process_id_impl();
		return result;
	}

	// wraps an existing process by id, without taking ownership of it - as far as this instance
	// is concerned, it's exactly as unrelated to the calling process as current()'s target is
	// "related" to it. `pid` isn't validated eagerly, so this always succeeds even for a pid
	// that doesn't exist (or no longer does); is_running()/info() reflect that. Same
	// restrictions as current(): wait()/wait_for() always return false and exit_code() always
	// returns std::nullopt.
	static process from_pid(process_id_t pid) noexcept
	{
		process result;
		result._Pid = pid;
		return result;
	}

	// spawns `executablePath` (searched via the usual OS rules if not an absolute path) as a
	// child process with the given command-line arguments (not including argv[0]) and, if
	// non-empty, `workingDirectory` as its initial directory. Returns std::nullopt if the
	// process could not be started.
	static ::std::optional<process> create(const ::std::string& executablePath,
		const ::std::vector<::std::string>& arguments = {},
		const ::std::string& workingDirectory = ::std::string()) noexcept
	{
		process result;

		if(!::mst::_Details::create_process_impl(
			   executablePath, arguments, workingDirectory, result._Pid, result._Handle))
		{
			return ::std::nullopt;
		}

		result._Owned = true;

		return result;
	}

	// false for a default-constructed (moved-from) instance
	explicit operator bool() const noexcept
	{
		return _Pid != 0;
	}

	process_id_t id() const noexcept
	{
		return _Pid;
	}

	::std::optional<process_info> info() const noexcept
	{
		if(_Pid == 0)
			return ::std::nullopt;

		return get_process_info(_Pid);
	}

	// blocks until the process exits; returns false immediately for a process this instance
	// doesn't own a waitable handle for (e.g. current())
	bool wait() const noexcept
	{
		return _Wait(-1);
	}

	// like wait(), but gives up (returning false) once `timeout` elapses instead of blocking
	// indefinitely
	template<class _Rep, class _Period>
	bool wait_for(const ::std::chrono::duration<_Rep, _Period>& timeout) const noexcept
	{
		return _Wait(::std::chrono::duration_cast<::std::chrono::milliseconds>(timeout).count());
	}

	// for an owned process this is a non-blocking wait() attempt; for current() it queries
	// get_process_info() instead, since there's no handle to poll
	bool is_running() const noexcept
	{
		if(_Pid == 0)
			return false;

		if(!_Owned)
			return get_process_info(_Pid).has_value();

		return !exit_code().has_value();
	}

	// the process' exit code, once known. Only ever set for an owned process (one returned by
	// create()) that has actually exited; this makes one non-blocking wait attempt on its own if
	// wait()/wait_for() wasn't already called.
	::std::optional<int> exit_code() const noexcept
	{
		if(!_ExitCode.has_value() && _Owned)
		{
			_Wait(0);
		}

		return _ExitCode;
	}

private:
	bool _Wait(int64_t timeoutMilliseconds) const noexcept
	{
		if(_ExitCode.has_value())
			return true;

		if(!_Owned || _Pid == 0)
			return false;

		int exitCode = 0;
		if(!::mst::_Details::wait_process_impl(_Pid, _Handle, timeoutMilliseconds, exitCode))
			return false;

		_ExitCode = exitCode;
		return true;
	}

	void _Close() noexcept
	{
		if(_Owned && _Pid != 0)
		{
			::mst::_Details::close_process_impl(_Pid, _Handle);
		}
	}

	process_id_t _Pid = 0;
	void* _Handle = nullptr;
	bool _Owned = false;
	mutable ::std::optional<int> _ExitCode;
};

} // namespace platform
} // namespace mst
