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

#include <atomic>

namespace mst {
namespace _Details {

template<typename T>
struct queue_elem_type
{
	std::atomic_uint32_t state{ 0 };

	// Data state:
	// 0: dead, can be pushed on
	// 1: reserved for push
	// 2: pushed, can be popped
	// 3: reserved for pop

	typename std::aligned_storage<sizeof(T), alignof(T)>::type type;
};

template<typename T>
struct queue_buffer
{
	// static_assert(ElemCount >= 4, "ElemCount should be at least 4");
	// static_assert(is_power_of_2<ElemCount>::value, "ElemCount must be a power of two");

	// const static uint32_t CountMask = ElemCount - 1;

	static const uint32_t c_elemJump = 3;

	queue_buffer(uint32_t elemCount)
		: m_count(elemCount)
		, m_countMask(elemCount - 1)
		, m_countShift(get_shift_from_count(elemCount))
		, m_buffer(new queue_elem_type<T>[elemCount])
	{
		assert(elemCount >= 4);
		assert(m_count != 0 && (m_count & (m_count - 1)) == 0);
	}

	~queue_buffer()
	{
		delete[] m_buffer;
	}

	queue_buffer(const queue_buffer&) = delete;
	queue_buffer& operator=(const queue_buffer&) = delete;

	[[nodiscard]] inline bool try_push(const T& elem) noexcept(
		std::is_nothrow_copy_constructible<T>::value)
	{
		uint64_t tmp, oldtail;
		uint32_t tmpState;
		bool isFull;

		do
		{

			tmp = oldtail = m_tail.load(std::memory_order_relaxed);

			if(!find_tail(tmp, tmpState, isFull))
			{
				if(isFull)
					return false;

				continue;
			}

		} while(!m_buffer[(tmp * c_elemJump) & m_countMask].state.compare_exchange_weak(
			tmpState, state(tmp, 1), std::memory_order_acq_rel, std::memory_order_relaxed));

		m_tail.store(tmp + 1, std::memory_order_relaxed);

		new(&get_impl(tmp)) T(elem);

		m_buffer[(tmp * c_elemJump) & m_countMask].state.store(
			state(tmp, 2), std::memory_order_release);

		return true;
	}

	[[nodiscard]] inline bool try_push(T&& elem) noexcept(
		std::is_nothrow_move_constructible<T>::value)
	{
		uint64_t tmp, oldtail;
		uint32_t tmpState;
		bool isFull;

		do
		{

			tmp = oldtail = m_tail.load(std::memory_order_relaxed);

			if(!find_tail(tmp, tmpState, isFull))
			{
				if(isFull)
					return false;

				continue;
			}

		} while(!m_buffer[(tmp * c_elemJump) & m_countMask].state.compare_exchange_weak(
			tmpState, state(tmp, 1), std::memory_order_acq_rel, std::memory_order_relaxed));

		m_tail.store(tmp + 1, std::memory_order_relaxed);

		new(&get_impl(tmp)) T(std::move(elem));

		m_buffer[(tmp * c_elemJump) & m_countMask].state.store(
			state(tmp, 2), std::memory_order_release);

		return true;
	}

	[[nodiscard]] inline bool try_pop(T& elem) noexcept
	{
		uint64_t tmp, oldhead;
		uint32_t tmpState;
		bool moveHead;

		do
		{
			tmp = oldhead = m_head.load(std::memory_order_relaxed);

			if(!find_head(tmp, tmpState, moveHead))
				return false;

		} while(!m_buffer[(tmp * c_elemJump) & m_countMask].state.compare_exchange_weak(
			tmpState, state(tmp, 3), std::memory_order_acq_rel, std::memory_order_relaxed));

		if(moveHead)
			m_head.compare_exchange_weak(oldhead, tmp + 1, std::memory_order_relaxed);

		elem = std::move(get_impl(tmp));

		get_impl(tmp).~T();

		m_buffer[(tmp * c_elemJump) & m_countMask].state.store(
			next_state(tmp, 0), std::memory_order_release);

		return true;
	}

	[[nodiscard]] inline bool try_delete() noexcept
	{
		uint64_t tmp, oldhead;
		uint32_t tmpState;
		bool moveHead;

		do
		{
			tmp = oldhead = m_head.load(std::memory_order_relaxed);

			if(!find_head(tmp, tmpState, moveHead))
				return false;

		} while(!m_buffer[(tmp * c_elemJump) & m_countMask].state.compare_exchange_weak(
			tmpState, state(tmp, 3), std::memory_order_acq_rel, std::memory_order_relaxed));

		if(moveHead)
			m_head.compare_exchange_weak(oldhead, tmp + 1, std::memory_order_relaxed);

		get_impl(tmp).~T();

		m_buffer[(tmp * c_elemJump) & m_countMask].state.store(
			next_state(tmp, 0), std::memory_order_release);

		return true;
	}

	[[nodiscard]] inline bool confirm_empty() const noexcept
	{
		// atomic_thread_fence(std::memory_order_acquire);
		for(uint32_t i = 0; i < m_count; ++i)
		{ // m_buffer,m_count
			const auto dataState = m_buffer[i].state.load(std::memory_order_acquire) >> 30;

			if(dataState != 0)
			{
				return false;
			}
		}

		return true;
	}

	[[nodiscard]] size_t capacity() const noexcept
	{
		return m_count;
	}

	[[nodiscard]] size_t size_approx() const noexcept
	{
		const auto tail = m_tail.load(std::memory_order_relaxed);
		const auto head = m_head.load(std::memory_order_relaxed);

		if(tail < head)
			return 0;

		return tail - head;
	}

private:
	[[nodiscard]] inline bool find_tail(uint64_t& tmp, uint32_t& tmpState, bool& isFull) noexcept
	{
		uint32_t prev = 0;
		isFull = false;

		while(true)
		{
			const uint32_t bufferState =
				m_buffer[(tmp * c_elemJump) & m_countMask].state.load(std::memory_order_relaxed);
			tmpState = state(tmp, 0);

			const uint32_t bufferGen = generation_from_state(bufferState);
			const uint32_t tmpGen = generation_from_index(tmp);

			if(bufferState == tmpState)
				return true;

			if(bufferGen < tmpGen)
			{
				isFull = true;
				return false;
			}

			if(bufferGen <= tmpGen && bufferGen < prev)
				return false;

			prev = bufferGen;
			++tmp;
		}
	}

	[[nodiscard]] inline bool find_head(uint64_t& tmp, uint32_t& tmpState, bool& moveHead) noexcept
	{
		uint32_t prev = 0;
		moveHead = true;

		while(true)
		{
			const uint32_t bufferState =
				m_buffer[(tmp * c_elemJump) & m_countMask].state.load(std::memory_order_relaxed);
			tmpState = state(tmp, 2);

			const uint32_t bufferGen = generation_from_state(bufferState);
			const uint32_t dataState = datastate_from_state(bufferState);
			const uint32_t tmpGen = generation_from_index(tmp);

			if(dataState < 2 && bufferGen == tmpGen)
				moveHead = false;

			if(bufferState == tmpState)
				return true;

			if(bufferGen < tmpGen)
				return false;
			// moveHead = false;

			if(bufferGen <= tmpGen && bufferGen < prev)
				return false;

			prev = bufferGen;
			++tmp;
		}
	}

	[[nodiscard]] inline uint32_t state(uint64_t idx, uint32_t dataState) const noexcept
	{
		return uint32_t((idx & 0xFFFFFFFF) >> m_countShift) | (dataState << 30);
	}

	[[nodiscard]] inline uint32_t next_state(uint64_t idx, uint32_t dataState) const noexcept
	{
		return uint32_t(((idx & 0xFFFFFFFF) >> m_countShift) + 1) | (dataState << 30);
	}

	[[nodiscard]] inline uint32_t generation_from_state(uint32_t state) const noexcept
	{
		return state & 0x3FFFFFFF;
	}

	[[nodiscard]] inline uint32_t generation_from_index(uint64_t idx) const noexcept
	{
		return uint32_t((idx & 0xFFFFFFFF) >> m_countShift);
	}

	[[nodiscard]] inline uint32_t datastate_from_state(uint32_t state) const noexcept
	{
		return state >> 30;
	}

	[[nodiscard]] inline T& get_impl(uint64_t index) noexcept
	{
		return *reinterpret_cast<T*>(&m_buffer[(index * c_elemJump) & m_countMask].type);
	}

	[[nodiscard]] inline const T& get_impl(uint64_t index) const noexcept
	{
		return *reinterpret_cast<const T*>(&m_buffer[(index * c_elemJump) & m_countMask].type);
	}

	[[nodiscard]] inline static uint32_t get_shift_from_count(uint32_t elemCount) noexcept
	{
		for(uint32_t shift = 0; shift < sizeof(elemCount) * 8; ++shift)
		{
			if(elemCount >> shift == 1)
				return shift;
		}

		return 0;
	}

	const uint32_t m_count, m_countMask, m_countShift;
	alignas(64) std::atomic_uint64_t m_head{ 0 };
	alignas(64) std::atomic_uint64_t m_tail{ 0 };
	queue_elem_type<T>* const m_buffer;
};

}
}