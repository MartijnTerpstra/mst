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

#include "mx_platform.h"
#include <mthreading_slim.h>

namespace mst {
namespace threading {
namespace slim {

inline bool wait_object::try_wait() const noexcept
{
	return _Try_wait();
}

inline void wait_object::wait() const noexcept
{
	for(;;)
	{
		if(_Try_wait())
		{
			return;
		}

		::std::this_thread::yield();
	}
}

template<typename RepType, typename PeriodType>
inline bool wait_object::wait_for(
	const ::std::chrono::duration<RepType, PeriodType>& duration) const noexcept
{
	typedef ::std::chrono::duration<RepType, PeriodType> DurationType;

	if(duration.count() == 0)
	{
		return _Try_wait();
	}
	else
	{
		auto start = ::std::chrono::high_resolution_clock::now();

		while(1)
		{
			if(_Try_wait())
			{
				return true;
			}
			if(::std::chrono::duration_cast<DurationType>(
				   ::std::chrono::high_resolution_clock::now() - start)
					.count() > duration)
			{
				return false;
			}

			::std::this_thread::yield();
		}
	}
}

template<typename ClockType, typename DurationType>
inline bool wait_object::wait_until(
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) const noexcept
{
	if(ClockType::now() > timePoint)
	{
		return _Try_wait();
	}
	else
	{

		while(1)
		{
			if(_Try_wait())
			{
				return true;
			}
			if(ClockType::now() > timePoint)
			{
				return false;
			}

			::std::this_thread::yield();
		}
	}
}

template<size_t WaitObjectCount>
inline void wait_object::wait_all(const wait_object* (&waitObjects)[WaitObjectCount]) noexcept
{
	wait_object::wait_all(waitObjects, WaitObjectCount);
}

inline void wait_object::wait_all(
	const wait_object* const* waitObjects, size_t waitObjectCount) noexcept
{
	for(size_t i = 0; i < waitObjectCount; ++i)
	{
		waitObjects[i]->wait();
	}
}

template<size_t WaitObjectCount>
inline size_t wait_object::wait_any(const wait_object* (&waitObjects)[WaitObjectCount]) noexcept
{
	return wait_object::wait_any(waitObjects, WaitObjectCount);
}

template<size_t WaitObjectCount>
inline size_t wait_object::wait_any(wait_object* (&waitObjects)[WaitObjectCount]) noexcept
{
	return wait_object::wait_any(waitObjects, WaitObjectCount);
}

inline size_t wait_object::wait_any(
	const wait_object* const* waitObjects, size_t waitObjectCount) noexcept
{
	if(waitObjectCount == 0)
		return (size_t)-1;

	for(;;)
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			if(waitObjects[i]->_Try_wait())
			{
				return i;
			}
		}

		std::this_thread::yield();
	}
}

template<typename RepType, typename PeriodType, size_t WaitObjectCount>
inline bool wait_object::wait_all_for(const wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	return wait_object::wait_all_for(waitObjects, WaitObjectCount, duration);
}

template<typename RepType, typename PeriodType, size_t WaitObjectCount>
inline bool wait_object::wait_all_for(wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	return wait_object::wait_all_for(waitObjects, WaitObjectCount, duration);
}

template<typename RepType, typename PeriodType>
inline bool wait_object::wait_all_for(const wait_object* const* waitObjects, size_t waitObjectCount,
	const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	if(waitObjectCount == 0)
		return true;

	if(duration.count() == 0)
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			if(waitObjects[i]->_Try_wait())
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		auto start = ::std::chrono::high_resolution_clock::now();

		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			while(1)
			{
				if(waitObjects[i]->_Try_wait())
				{
					break;
				}

				if((::std::chrono::high_resolution_clock::now() - start) > duration)
				{
					return false;
				}

				std::this_thread::yield();
			}
		}
		return true;
	}
}

template<typename ClockType, typename DurationType, size_t WaitObjectCount>
inline bool wait_object::wait_all_until(const wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	return wait_object::wait_all_until(waitObjects, WaitObjectCount, timePoint);
}

template<typename ClockType, typename DurationType, size_t WaitObjectCount>
inline bool wait_object::wait_all_until(wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	return wait_object::wait_all_until(waitObjects, WaitObjectCount, timePoint);
}

template<typename ClockType, typename DurationType>
inline bool wait_object::wait_all_until(const wait_object* const* waitObjects,
	size_t waitObjectCount,
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	if(waitObjectCount == 0)
		return true;

	if(ClockType::now() > timePoint)
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			if(waitObjects[i]->_Try_wait())
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			while(1)
			{
				if(waitObjects[i]->_Try_wait())
				{
					break;
				}

				if(ClockType::now() > timePoint)
				{
					return false;
				}

				std::this_thread::yield();
			}
		}
		return true;
	}
}

template<typename RepType, typename PeriodType, size_t WaitObjectCount>
inline size_t(wait_object::wait_any_for)(const wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	return wait_object::wait_any_for(waitObjects, WaitObjectCount, duration);
}

template<typename RepType, typename PeriodType, size_t WaitObjectCount>
inline size_t(wait_object::wait_any_for)(wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	return wait_object::wait_any_for(waitObjects, WaitObjectCount, duration);
}

template<typename RepType, typename PeriodType>
inline size_t(wait_object::wait_any_for)(const wait_object* const* waitObjects,
	size_t waitObjectCount, const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept
{
	if(waitObjectCount == 0)
		return (size_t)-1;

	if(duration.count() == 0)
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			if(waitObjects[i]->_Try_wait())
			{
				return i;
			}
		}
		return (size_t)-1;
	}
	else
	{
		auto start = ::std::chrono::high_resolution_clock::now();

		for(;;)
		{
			for(size_t i = 0; i < waitObjectCount; ++i)
			{
				if(waitObjects[i]->_Try_wait())
				{
					return i;
				}
			}

			if((::std::chrono::high_resolution_clock::now() - start) > duration)
			{
				return (size_t)-1;
			}

			std::this_thread::yield();
		}
	}
}

template<typename ClockType, typename DurationType, size_t WaitObjectCount>
inline size_t(wait_object::wait_any_until)(const wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	return wait_object::wait_any_until(waitObjects, WaitObjectCount, timePoint);
}

template<typename ClockType, typename DurationType, size_t WaitObjectCount>
inline size_t(wait_object::wait_any_until)(wait_object* (&waitObjects)[WaitObjectCount],
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	return wait_object::wait_any_until(waitObjects, WaitObjectCount, timePoint);
}

template<typename ClockType, typename DurationType>
inline size_t mst::threading::slim::wait_object::wait_any_until(
	const wait_object* const* waitObjects, size_t waitObjectCount,
	const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept
{
	if(waitObjectCount == 0)
		return -1;

	if(ClockType::now() > timePoint)
	{
		for(size_t i = 0; i < waitObjectCount; ++i)
		{
			if(waitObjects[i]->_Try_wait())
			{
				return i;
			}
		}
		return -1;
	}
	else
	{
		while(1)
		{
			for(size_t i = 0; i < waitObjectCount; ++i)
			{
				if(waitObjects[i]->_Try_wait())
				{
					return i;
				}
			}

			if(ClockType::now() > timePoint)
			{
				return -1;
			}

			std::this_thread::yield();
		}
	}
}

inline semaphore::semaphore(uint32_t initialCount) noexcept
	: m_counter(initialCount)
{ }

inline void semaphore::signal() const noexcept
{
	++m_counter;
}

inline void semaphore::signal(uint32_t count) const noexcept
{
	m_counter += count;
}

inline bool semaphore::_Try_wait() const noexcept
{
	uint32_t currentValue = m_counter.load();

	return (currentValue != 0) && m_counter.compare_exchange_strong(currentValue, currentValue - 1);
}

inline mutex::mutex(bool owned) noexcept
	: m_counter(owned ? 1U : 0)
{ }

inline void mutex::signal() const noexcept
{
	m_counter.store(0);
}

inline bool mutex::_Try_wait() const noexcept
{
	return m_counter.exchange(1U) == 0;
}

inline recursive_mutex::recursive_mutex(bool owned) noexcept
	: m_counter(owned ? 1U : 0)
	, m_tid(owned ? ::mst::_Details::get_current_thread_id() : UINT64_MAX)
	, m_recursiveCounter(owned ? 1U : 0)
{ }

inline void recursive_mutex::signal() const noexcept
{
	if((--m_recursiveCounter) == 0)
	{
		m_tid = UINT64_MAX;
		m_counter.store(0);
	}
}

inline bool recursive_mutex::_Try_wait() const noexcept
{
	const uint32_t tid = ::mst::_Details::get_current_thread_id();

	if(m_tid == tid)
	{
		++m_recursiveCounter;
		return true;
	}

	const bool success = m_counter.exchange(1U) == 0;

	if(success)
	{
		m_tid = tid;
		++m_recursiveCounter;
	}

	return success;
}

inline event::event(bool set, bool manual) noexcept
	: m_counter(set ? 0 : 1U)
	, m_manualReset(manual)
{ }

inline void event::set() const noexcept
{
	m_counter.store(0);
}

inline void event::reset() const noexcept
{
	m_counter.store(1U);
}

inline bool event::_Try_wait() const noexcept
{
	if(m_manualReset)
	{
		return m_counter.load() == 0;
	}
	else
	{
		return m_counter.exchange(1U) == 0;
	}
}

} // slim
} // namespace threading
} // namespace mst