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
#include <mutex>
#include <mx_lock_free_queue.h>

namespace mst {
namespace lock_free {

template<typename T>
class queue
{
	static const uint32_t c_maxBufferCount = 6;

public:
	inline queue()
		: m_pushIndex(0)
		, m_popIndex(0)
		, m_readerCounts()
		, m_writerCounts()
		, m_createBufferMutex(0)
	{
		m_buffers[0].store(new _Details::queue_buffer<T>(2048));
		for(uint32_t i = 1; i < c_maxBufferCount; ++i)
		{
			m_buffers[i].store(nullptr);
		}

		for(uint32_t i = 0; i < c_maxBufferCount; ++i)
		{
			m_readerCounts[i].store(0xC0000000);
			m_writerCounts[i].store(0);
		}
	}

	inline queue(size_t initCapacity)
		: m_pushIndex(0)
		, m_popIndex(0)
		, m_readerCounts()
		, m_writerCounts()
		, m_createBufferMutex(0)
	{
		MST_ASSERT(initCapacity <= max_size(), "Initial capacity of", initCapacity,
			"exceeds max_size of ", max_size());

		uint32_t index = 0;
		while((2048 << ((size_t)index * 2)) < initCapacity)
		{
			++index;
		}

		m_pushIndex = index;
		m_popIndex = index;
		m_buffers[index].store(new _Details::queue_buffer<T>(2048 << (index * 2)));
		for(int i = 0; i < c_maxBufferCount; ++i)
		{
			if(i == index)
				continue;

			m_buffers[i].store(nullptr);
		}

		for(int i = 0; i < c_maxBufferCount; ++i)
		{
			m_readerCounts[i].store(0xC0000000);
			m_writerCounts[i].store(0);
		}
	}

	queue(const queue&) = delete;
	queue operator=(const queue&) = delete;

	inline ~queue()
	{
		for(auto& b : m_buffers)
		{
			const auto ptr = b.load();

			if(ptr)
			{
				while(ptr->try_delete())
				{ }
				delete ptr;
			}
		}
	}

	inline void push(const T& elem)
	{
		while(true)
		{
			uint32_t index = m_pushIndex.load(std::memory_order_relaxed);

			auto buffer = m_buffers[index].load(std::memory_order_acquire);

			if(buffer == nullptr)
				continue;

			if(grab_buffer_for_push(index))
			{
				bool success = buffer->try_push(elem);

				release_buffer_for_push(index);

				if(success)
					return;

				create_buffer(index + 1);
			}
		}
	}

	inline void push(T&& elem)
	{
		while(true)
		{
			uint32_t index = m_pushIndex.load(std::memory_order_relaxed);

			auto buffer = m_buffers[index].load(std::memory_order_acquire);

			if(buffer == nullptr)
				continue;

			if(grab_buffer_for_push(index))
			{
				bool success = buffer->try_push(elem);

				release_buffer_for_push(index);

				if(success)
					return;

				create_buffer(index + 1);
			}
		}
	}

	[[nodiscard]] bool try_pop(T& elem) noexcept
	{
		uint32_t index, startindex;


		index = startindex = m_popIndex.load(std::memory_order_relaxed);

		do
		{
			const auto buffer = m_buffers[index].load(std::memory_order_acquire);

			if(buffer == nullptr)
			{
				if(index != startindex)
					return false;
				continue;
			}

			if(!grab_buffer_for_pop(index))
				continue;

			bool success = buffer->try_pop(elem);

			release_buffer_for_pop(index, buffer, !success);

			if(success)
				return true;

		} while(++index < c_maxBufferCount);

		return false;
	}

	[[nodiscard]] size_t max_size() const noexcept
	{
		return 2048 << (c_maxBufferCount - 1) * 2;
	}

	[[nodiscard]] size_t capacity_approx() const noexcept
	{
		size_t capacity = 0;
		for(auto& b : m_buffers)
		{
			const auto ptr = b.load(std::memory_order_acquire);

			if(ptr)
			{
				capacity = ptr->capacity();
			}
		}

		return capacity;
	}

	[[nodiscard]] size_t size_approx() const noexcept
	{
		size_t size = 0;
		for(auto& b : m_buffers)
		{
			const auto ptr = b.load(std::memory_order_acquire);

			if(ptr)
			{
				size += ptr->size_approx();
			}
		}

		return size;
	}

	[[nodiscard]] bool empty_approx() const noexcept
	{
		return size_approx() == 0;
	}

private:
	bool grab_buffer_for_push(uint32_t index) noexcept
	{
		auto refcount = m_writerCounts[index].load();

		do
		{
			// use next buffer
			if(refcount & 0x80000000)
				return false;

		} while(!m_writerCounts[index].compare_exchange_weak(refcount, refcount + 1));

		return true;
	}

	void release_buffer_for_push(uint32_t index) noexcept
	{
		--m_writerCounts[index];
	}

	bool grab_buffer_for_pop(uint32_t index) noexcept
	{
		auto refcount = m_readerCounts[index].load();

		do
		{
			if((refcount & 0x40000000) == 0)
				return false;

		} while(!m_readerCounts[index].compare_exchange_weak(refcount, refcount + 1));

		return true;
	}

	void release_buffer_for_pop(
		uint32_t index, _Details::queue_buffer<T>* buffer, bool tryDeleteBuffer) noexcept
	{
		if(tryDeleteBuffer)
		{
			auto refcount = m_readerCounts[index].load();

			if((refcount & 0x80000000) == 0)
			{
				if((refcount & 0x40000000) != 0 && m_writerCounts[index].load() == 0x80000000)
				{
					if(buffer->confirm_empty())
					{
						m_readerCounts[index].fetch_and(0xBFFFFFFF);
					}
				}
			}
		}

		if(--m_readerCounts[index] == 0)
		{
			while(true)
			{
				uint32_t oldIndex = index;

				// Previous buffer still exists, leave the delete to them
				if(!m_popIndex.compare_exchange_strong(
					   oldIndex, index + 1, std::memory_order_acq_rel))
					break;

				buffer = m_buffers[index].exchange(nullptr, std::memory_order_relaxed);
				MST_ASSERT(buffer, "buffer already removed");
				delete buffer;

				++index;
				if(m_readerCounts[index].load() != 0)
					break;
			}
		}
	}

	void create_buffer(uint32_t newIndex)
	{
		// You have inserted 2+ million objects, fix your algorithm
		// Spinlocking till room is available
		if(newIndex >= c_maxBufferCount)
			return;

		// grab mutex, this is OK because the coming operator new will grab a
		// lock
		while(m_createBufferMutex.exchange(1, std::memory_order_acquire) != 0)
			;

		const uint32_t currentIndex = m_pushIndex.load(std::memory_order_relaxed);

		// buffer is already created
		if(currentIndex >= newIndex)
		{
			m_createBufferMutex.store(0, std::memory_order_release);
			return;
		}

		// release memory order for the pop thread, not the push
		m_buffers[newIndex].store(
			new _Details::queue_buffer<T>(2048U << (newIndex * 2U)), std::memory_order_release);

		//
		m_pushIndex.store(newIndex, std::memory_order_relaxed);

		m_createBufferMutex.store(0, std::memory_order_release);

		const auto oldIndex = newIndex - 1;

		m_writerCounts[oldIndex].fetch_or(0x80000000);
		m_readerCounts[oldIndex].fetch_and(0x7FFFFFFF);
	}

private:
	alignas(64) std::atomic_uint32_t m_pushIndex, m_popIndex;
	alignas(64) std::atomic<_Details::queue_buffer<T>*> m_buffers[c_maxBufferCount];
	alignas(64) std::atomic_uint32_t m_readerCounts[c_maxBufferCount];
	alignas(64) std::atomic_uint32_t m_writerCounts[c_maxBufferCount];
	alignas(64) std::atomic_uint32_t m_createBufferMutex;

}; // class queue

} // namespace lock_free
} // namespace mst