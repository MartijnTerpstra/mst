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
#include <mdebug.h>
#include <chrono>
#include <atomic>
#include <mx_threading.h>

namespace mst {
namespace threading {


class critical_section
{
public:
	critical_section()
	{
		_Myatomic = 0;
	}

	inline void enter() const
	{
		while(_Myatomic.exchange(1) == 0)
			;
	}

	inline void leave() const
	{
		_Myatomic.store(0, std::memory_order_release);
	}

private:
	// do not copy
	critical_section(const critical_section&) = delete;
	critical_section& operator=(const critical_section&) = delete;

private:
	mutable ::std::atomic_uint32_t _Myatomic;

}; // class critical_section

class rw_lock
{
public:
	rw_lock();
	~rw_lock();

	bool try_lock_read() const;
	bool try_lock_write() const;

	void lock_read() const;
	void lock_write() const;

	void unlock_read() const;
	void unlock_write() const;

private:
	mutable void* _Myhandle;
};

class recursive_mutex
{
public:
	recursive_mutex(bool initalOwner = false);

	~recursive_mutex();

	void signal() const;

	template<class _Rep, class _Period>
	inline bool wait(const ::std::chrono::duration<_Rep, _Period>& waitDuration) const
	{
		return wait(std::chrono::duration_cast<std::chrono::milliseconds>(waitDuration).count());
	}

	template<class _Clock, class _Duration>
	inline bool wait_until(const ::std::chrono::time_point<_Clock, _Duration>& timePoint) const
	{
		return _Wait_until(timePoint.time_since_epoch());
	}

	bool wait(int64_t milliseconds = -1) const;

private:
	// do not copy
	recursive_mutex(const recursive_mutex&) = delete;
	recursive_mutex& operator=(const recursive_mutex&) = delete;

private:
	template<class _Rep, class _Period>
	inline bool _Wait_until(const ::std::chrono::duration<_Rep, _Period>& durationFromEpoch) const
	{
		return _Wait_until(
			::std::chrono::duration_cast<::std::chrono::milliseconds>(durationFromEpoch).count());
	}

	bool _Wait_until(long long durationFromEpoch) const;

private:
	void* _Myhandle;
	mutable uint64_t _Myholder;
	mutable uint32_t _Mycount;
};



class threading_object
{
public:
	template<class _Rep, class _Period>
	inline bool wait(const ::std::chrono::duration<_Rep, _Period>& waitDuration) const
	{
		return wait(std::chrono::duration_cast<std::chrono::milliseconds>(waitDuration).count());
	}

	template<class _Clock, class _Duration>
	inline bool wait_until(const ::std::chrono::time_point<_Clock, _Duration>& timePoint) const
	{
		return _Wait_until(timePoint.time_since_epoch());
	}

	bool wait(int64_t milliseconds = -1) const;

	inline void* handle() const
	{
		return _Myhandle;
	}

protected:
	threading_object();

	// not supposed to be virtual
	~threading_object();

private:
	// do not copy
	threading_object(const threading_object&) = delete;
	threading_object& operator=(const threading_object&) = delete;

private:
	template<class _Rep, class _Period>
	inline bool _Wait_until(const ::std::chrono::duration<_Rep, _Period>& durationFromEpoch) const
	{
		return _Wait_until(
			::std::chrono::duration_cast<::std::chrono::milliseconds>(durationFromEpoch).count());
	}

	bool _Wait_until(int64_t durationFromEpoch) const;

protected:
	void* _Myhandle;
};


class mutex : public threading_object
{
public:
	mutex(bool initalOwner = false);

	void signal() const;
};

class semaphore : public threading_object
{
public:
	semaphore(uint32_t initialCount = 0);

	void signal(uint32_t count = 1) const;
};

template<typename LockableType>
class lock_guard
{
public:
	explicit lock_guard(const LockableType& lock)
		: _Mylock(&lock)
	{
		_Mylock->wait();
	}

	lock_guard(lock_guard&& other)
		: _Mylock(other._Mylock)
	{
		other._Mylock = nullptr;
	}

	~lock_guard()
	{
		if(_Mylock)
		{
			_Mylock->signal();
		}
	}

private:
	lock_guard(const lock_guard&) = delete;
	void operator=(const lock_guard&) = delete;

private:
	const LockableType* _Mylock;

}; // class lock_guard<LockableType>

template<>
class lock_guard<critical_section>
{
	explicit lock_guard(const critical_section& lock)
		: _Mylock(&lock)
	{
		_Mylock->enter();
	}

	lock_guard(lock_guard&& other) noexcept
		: _Mylock(other._Mylock)
	{
		other._Mylock = nullptr;
	}

	~lock_guard()
	{
		if(_Mylock)
		{
			_Mylock->leave();
		}
	}

private:
	// no copy
	lock_guard(const lock_guard&) = delete;
	// no copy
	void operator=(const lock_guard&) = delete;

private:
	const critical_section* _Mylock;

}; // class lock_guard<critical_section>

} // namespace threading
} // namespace mst