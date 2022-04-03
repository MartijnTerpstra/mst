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
#include <thread>
#include <mx_platform.h>
#include <chrono>
#include <atomic>

namespace mst {
namespace threading {
namespace slim {

class wait_object;

class wait_object
{
	MST_NON_COPYABLE(wait_object);

protected:
	wait_object() noexcept = default;
	virtual ~wait_object() = default;

public:
	[[nodiscard]] inline bool try_wait() const noexcept;

	inline void wait() const noexcept;

	template<typename RepType, typename PeriodType>
	inline bool wait_for(
		const ::std::chrono::duration<RepType, PeriodType>& duration) const noexcept;

	template<typename ClockType, typename DurationType>
	inline bool wait_until(
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) const noexcept;

public:
	// statics

	template<size_t WaitObjectCount>
	inline static void wait_all(const wait_object* (&waitObjects)[WaitObjectCount]) noexcept;

	template<size_t WaitObjectCount>
	inline static void wait_all(wait_object* (&waitObjects)[WaitObjectCount]) noexcept;

	inline static void wait_all(
		const wait_object* const* waitObjects, size_t waitObjectCount) noexcept;

	template<size_t WaitObjectCount>
	inline static size_t wait_any(const wait_object* (&waitObjects)[WaitObjectCount]) noexcept;

	template<size_t WaitObjectCount>
	inline static size_t wait_any(wait_object* (&waitObjects)[WaitObjectCount]) noexcept;

	inline static size_t wait_any(
		const wait_object* const* waitObjects, size_t waitObjectCount) noexcept;

	template<typename RepType, typename PeriodType, size_t WaitObjectCount>
	inline static bool wait_all_for(const wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename RepType, typename PeriodType, size_t WaitObjectCount>
	inline static bool wait_all_for(wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename RepType, typename PeriodType>
	inline static bool wait_all_for(const wait_object* const* waitObjects, size_t waitObjectCount,
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename ClockType, typename DurationType, size_t WaitObjectCount>
	inline static bool wait_all_until(const wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

	template<typename ClockType, typename DurationType, size_t WaitObjectCount>
	inline static bool wait_all_until(wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

	template<typename ClockType, typename DurationType>
	inline static bool wait_all_until(const wait_object* const* waitObjects, size_t waitObjectCount,
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

	template<typename RepType, typename PeriodType, size_t WaitObjectCount>
	inline static size_t wait_any_for(const wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename RepType, typename PeriodType, size_t WaitObjectCount>
	inline static size_t wait_any_for(wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename RepType, typename PeriodType>
	inline static size_t wait_any_for(const wait_object* const* waitObjects, size_t waitObjectCount,
		const ::std::chrono::duration<RepType, PeriodType>& duration) noexcept;

	template<typename ClockType, typename DurationType, size_t WaitObjectCount>
	inline static size_t wait_any_until(const wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

	template<typename ClockType, typename DurationType, size_t WaitObjectCount>
	inline static size_t wait_any_until(wait_object* (&waitObjects)[WaitObjectCount],
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

	template<typename ClockType, typename DurationType>
	inline static size_t wait_any_until(const wait_object* const* waitObjects,
		size_t waitObjectCount,
		const ::std::chrono::time_point<ClockType, DurationType>& timePoint) noexcept;

protected:
	virtual bool _Try_wait() const noexcept = 0;
};

class semaphore final : public wait_object
{
	MST_NON_COPYABLE(semaphore);

public:
	semaphore(uint32_t initialCount = 0) noexcept;

	void signal() const noexcept;

	void signal(uint32_t count) const noexcept;

protected:
	inline bool _Try_wait() const noexcept override;

private:
	mutable ::std::atomic_uint32_t m_counter;
};

class mutex final : public wait_object
{
	MST_NON_COPYABLE(mutex);

public:
	mutex(bool owned) noexcept;

	void signal() const noexcept;

	inline void lock() const noexcept
	{
		wait();
	}

	inline void unlock() const noexcept
	{
		signal();
	}

protected:
	inline bool _Try_wait() const noexcept override;

private:
	mutable ::std::atomic_uint32_t m_counter;
};

class recursive_mutex final : public wait_object
{
	MST_NON_COPYABLE(recursive_mutex);

public:
	recursive_mutex(bool owned) noexcept;

	void signal() const noexcept;

	inline void lock() const noexcept
	{
		wait();
	}

	inline void unlock() const noexcept
	{
		signal();
	}

protected:
	inline bool _Try_wait() const noexcept override;

private:
	mutable ::std::atomic_uint32_t m_counter;
	mutable volatile uint64_t m_tid;
	mutable volatile uint32_t m_recursiveCounter;
};

class event final : public wait_object
{
	MST_NON_COPYABLE(event);

public:
	event(bool set, bool manual) noexcept;

	void set() const noexcept;

	void reset() const noexcept;

protected:
	inline bool _Try_wait() const noexcept override;

private:
	mutable ::std::atomic_uint32_t m_counter;
	const bool m_manualReset;
};

} // slim
} // namespace threading
} // namespace mst

#include <mx_threading_slim.inl>