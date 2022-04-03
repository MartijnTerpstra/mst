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
#include <cstring>

#if _MST_HAS_INVOKE
#include <functional>
#endif

#ifndef MST_DEFAULT_COLONY_PAGE_SIZE
#define MST_DEFAULT_COLONY_PAGE_SIZE 8192
#endif // !MST_DEFAULT_COLONY_PAGE_SIZE



namespace mst {

template<typename T>
class colony_iterator;

template<typename T>
class colony_const_iterator;

template<typename T>
class colony_reverse_iterator;

template<typename T>
class colony_const_reverse_iterator;

template<typename T, int32_t ElementsPerPage = MST_DEFAULT_COLONY_PAGE_SIZE>
class colony
{
	static_assert(ElementsPerPage > 0 && (ElementsPerPage & (ElementsPerPage - 1)) == 0,
		"ElementsPerPage must be a power of two");

	struct FreeListNode
	{
		int32_t prev, next;
	};

	union ElemType
	{
		typename std::aligned_storage<sizeof(T), alignof(T)>::type elem;
		FreeListNode node;
	};

	static_assert(std::is_trivially_destructible<FreeListNode>::value,
		"FreeListNode must be trivially destructible");

	friend class colony_iterator<T>;
	friend class colony_const_iterator<T>;
	friend class colony_reverse_iterator<T>;
	friend class colony_const_reverse_iterator<T>;

public:
	typedef colony_iterator<T> iterator;
	typedef colony_const_iterator<T> const_iterator;

	typedef colony_reverse_iterator<T> reverse_iterator;
	typedef colony_const_reverse_iterator<T> const_reverse_iterator;

	inline colony() noexcept
		: m_pages(nullptr)
		, m_elementCount(0)
		, m_capacity(0)
		, m_freeListHead(-1)
		, m_skips(&m_capacity)
	{ }

	inline ~colony()
	{
		destroy_all();
	}

	void clear() noexcept
	{
		if(m_pages)
		{ // m_skips,m_capacity+1
			auto index = m_skips[0];

			while(index != m_capacity)
			{
				get_impl(index).~T();
				new(&get_free_impl(index)) FreeListNode{};

				m_skips[index] = 1;

				index = index + m_skips[index + 1] + 1;
			}

			set_free_prev(0, -1);
			set_free_next(0, -1);

			m_elementCount = 0;
			m_skips[0] = m_capacity;
			m_skips[m_capacity - 1] = m_capacity;
			m_freeListHead = 0;
		}
	}

	_MST_NODISCARD inline bool empty() const noexcept
	{
		return m_elementCount == 0;
	}

	template<typename Fn>
	inline void foreach(Fn func) _MST_NOEXCEPT_OP_INVOCABLE(Fn, const T&)
	{
		auto index = m_skips[0];

		while(index != m_capacity)
		{
#if _MST_HAS_INVOKE
			std::invoke(func, get_impl(index));
#else
			func(get_impl(index));
#endif

			index = index + m_skips[index + 1] + 1;
		}
	}

	template<typename Fn>
	inline void foreach(Fn func) const _MST_NOEXCEPT_OP_INVOCABLE(Fn, const T&)
	{
		auto index = m_skips[0];

		while(index != m_capacity)
		{
#if _MST_HAS_INVOKE
			std::invoke(func, get_impl(index));
#else
			func(get_impl(index));
#endif

			index = index + m_skips[index + 1] + 1;
		}
	}

	template<typename... Args>
	inline iterator emplace(Args&&... args)
	{
		int32_t newIndex;
		if(m_elementCount == m_capacity) // _MST_UNLIKELY
		{
			newIndex = create_new();
		}
		else
		{
			newIndex = get_from_free_list();
			get_free_impl(newIndex).~FreeListNode();
		}

		new(&get_impl(newIndex)) T(std::forward<Args>(args)...);

		return iterator(*this, newIndex);
	}

	inline iterator erase(const_iterator it) noexcept
	{
		MST_ASSERT(it.m_container == this, "iterator is not attached to this container");
		MST_ASSERT(it.m_index >= 0, "iterator out of range");
		MST_ASSERT(it.m_index < m_capacity, "iterator out of range");
		MST_ASSERT(m_skips[it.m_index] == 0, "iterator invalid");

		get_impl(it.m_index).~T();
		new(&get_free_impl(it.m_index)) FreeListNode{};

		const auto nextIndex = add_to_free_list(it.m_index);

		return iterator(*this, nextIndex);
	}

	inline int32_t erase(int32_t index) noexcept
	{
		MST_ASSERT(index >= 0, "iterator out of range");
		MST_ASSERT(index < m_capacity, "iterator out of range");
		MST_ASSERT(m_skips[index] == 0, "iterator invalid");

		get_impl(index).~T();
		new(&get_free_impl(index)) FreeListNode{};

		const auto nextIndex = add_to_free_list(index);

		return nextIndex;
	}

	inline T& operator[](int32_t index) noexcept
	{
		MST_ASSERT(index >= 0, "iterator out of range");
		MST_ASSERT(index < m_capacity, "iterator out of range");
		MST_ASSERT(m_skips[index] == 0, "iterator invalid");

		return get_impl(index);
	}

	inline const T& operator[](int32_t index) const noexcept
	{
		MST_ASSERT(index >= 0, "iterator out of range");
		MST_ASSERT(index < m_capacity, "iterator out of range");
		MST_ASSERT(m_skips[index] == 0, "iterator invalid");

		return get_impl(index);
	}

	_MST_NODISCARD inline size_t capacity() const noexcept
	{
		return static_cast<size_t>(m_capacity);
	}

	_MST_NODISCARD inline size_t size() const noexcept
	{
		return static_cast<size_t>(m_elementCount);
	}

	_MST_NODISCARD inline iterator begin() noexcept
	{
		return iterator(*this, m_skips[0]);
	}

	_MST_NODISCARD inline iterator end() noexcept
	{
		return iterator(*this, m_capacity);
	}

	_MST_NODISCARD inline const_iterator begin() const noexcept
	{
		return const_iterator(*this, m_skips[0]);
	}

	_MST_NODISCARD inline const_iterator end() const noexcept
	{
		return const_iterator(*this, m_capacity);
	}

	_MST_NODISCARD inline const_iterator cbegin() const noexcept
	{
		return begin();
	}

	_MST_NODISCARD inline const_iterator cend() const noexcept
	{
		return end();
	}

private:
	_MST_NODISCARD inline int32_t create_new()
	{
		MST_ASSERT(m_capacity == m_elementCount, "invalid create_new() call");

		if(m_capacity == 0) // [[unlikely]]
		{
			m_pages = new ElemType*[1];

			m_pages[0] = new ElemType[ElementsPerPage];

			for(int32_t e = 1; e < ElementsPerPage; ++e)
			{
				new(&m_pages[0][e].node) FreeListNode{};
			}

			m_skips = new int32_t[ElementsPerPage + 1];
			m_capacity = ElementsPerPage;
			memset(m_skips + 2, 1, (ElementsPerPage - 3) * sizeof(int32_t));
			m_skips[0] = 0;
			m_skips[1] = ElementsPerPage - 1;
			m_skips[ElementsPerPage - 1] = ElementsPerPage - 1;
			m_skips[ElementsPerPage] = 0;
			m_freeListHead = 1;
			set_free_prev(1, -1);
			set_free_next(1, -1);
			return m_elementCount++;
		}

		m_capacity += ElementsPerPage;

		const auto pageCount = m_capacity >> _MST_GET_SHIFT(ElementsPerPage);
		const auto oldPageCount = m_elementCount >> _MST_GET_SHIFT(ElementsPerPage);

		const auto newPages = new ElemType*[(size_t)pageCount];
		const auto newSkips = new int32_t[size_t(m_capacity + 1)];

		for(int32_t i = 0; i < oldPageCount; ++i)
		{
			newPages[i] = m_pages[i];
		}

		newPages[oldPageCount] = new ElemType[ElementsPerPage];

		for(int32_t e = 1; e < ElementsPerPage; ++e)
		{
			new(&newPages[oldPageCount][e].node) FreeListNode{};
		}

		memset(newSkips, 0, size_t(m_elementCount + 1) * sizeof(int32_t));
		memset(newSkips + m_elementCount + 2, 1, size_t(ElementsPerPage - 3) * sizeof(int32_t));
		newSkips[m_elementCount + 1] = ElementsPerPage - 1;
		newSkips[m_capacity - 1] = ElementsPerPage - 1;
		newSkips[m_capacity] = 0;

		delete[] m_pages;
		delete[] m_skips;

		m_pages = newPages;
		m_skips = newSkips;
		m_freeListHead = m_elementCount + 1;

		set_free_prev(m_elementCount + 1, -1);
		set_free_next(m_elementCount + 1, -1);

		return m_elementCount++;
	}

	_MST_NODISCARD inline int32_t get_from_free_list() noexcept
	{
		MST_ASSERT(m_freeListHead != -1, "no free list elements");

		++m_elementCount;

		int32_t newIndex = m_freeListHead;

		const auto rightIsDestroyed = m_skips[m_freeListHead + 1] != 0;

		if(rightIsDestroyed)
		{
			const auto freeBlockSize = m_skips[m_freeListHead];
			m_skips[m_freeListHead + 1] = freeBlockSize - 1;
			m_skips[m_freeListHead + freeBlockSize - 1] = freeBlockSize - 1;

			const auto freeNext = get_free_next(m_freeListHead);
			++m_freeListHead;
			if(freeNext != -1)
			{
				set_free_prev(freeNext, m_freeListHead);
			}

			set_free_next(m_freeListHead, freeNext);
			set_free_prev(m_freeListHead, -1);
		}
		else
		{
			m_freeListHead = get_free_next(m_freeListHead);
			if(m_freeListHead != -1)
			{
				set_free_prev(m_freeListHead, -1);
			}
		}

		m_skips[newIndex] = 0;

		return newIndex;
	}

	_MST_NODISCARD inline int32_t add_to_free_list(int32_t index) noexcept
	{
		--m_elementCount;

		const auto leftIndex = std::max(index - 1, 0);
		const auto rightIndex = index + 1;

		const uint8_t leftIsDestroyed = m_skips[leftIndex] != 0;
		const uint8_t rightIsDestroyed = m_skips[rightIndex] != 0;

		switch(leftIsDestroyed | (rightIsDestroyed << 1))
		{
		case 0: // none
			m_skips[index] = 1;

			if(m_freeListHead != -1)
			{
				set_free_prev(m_freeListHead, index);
			}
			set_free_next(index, m_freeListHead);
			set_free_prev(index, -1);

			m_freeListHead = index;
			return index + 1;
		case 1: // end
		{
			const auto skipBlockSize = m_skips[leftIndex];
			m_skips[index - skipBlockSize] = skipBlockSize + 1;
			m_skips[index] = skipBlockSize + 1;
			return index + 1;
		}
		case 2: // begin
		{
			const auto skipBlockSize = m_skips[rightIndex];
			m_skips[index + skipBlockSize] = skipBlockSize + 1;
			m_skips[index] = skipBlockSize + 1;

			const auto prev = get_free_prev(rightIndex);
			const auto next = get_free_next(rightIndex);

			if(m_freeListHead == rightIndex)
			{
				m_freeListHead = index;
			}
			else if(prev != -1)
			{
				set_free_next(prev, index);
			}

			set_free_prev(index, prev);
			set_free_next(index, next);

			if(next != -1)
			{
				set_free_prev(next, index);
			}

			return index + skipBlockSize + 1;
		}
		// case 3: // both
		default: {
			m_skips[index] = 1;

			const auto skipBlockSizeLeft = m_skips[leftIndex];
			const auto skipBlockSizeRight = m_skips[rightIndex];

			m_skips[index - skipBlockSizeLeft] = skipBlockSizeLeft + skipBlockSizeRight + 1;
			m_skips[index + skipBlockSizeRight] = skipBlockSizeLeft + skipBlockSizeRight + 1;

			const auto prev = get_free_prev(rightIndex);
			const auto next = get_free_next(rightIndex);

			if(m_freeListHead == rightIndex)
			{
				m_freeListHead = next;
				set_free_prev(next, -1);
			}
			else
			{
				set_free_next(prev, next);
				if(next != -1)
				{
					set_free_prev(next, prev);
				}
			}
			return index + skipBlockSizeRight + 1;
		}
		}
	}

	_MST_NODISCARD inline int32_t get_free_prev(int32_t index) const noexcept
	{
		return get_free_impl(index).prev;
	}

	inline void set_free_prev(int32_t index, int32_t newPrev) noexcept
	{
		get_free_impl(index).prev = newPrev;
	}

	_MST_NODISCARD inline int32_t get_free_next(int32_t index) const noexcept
	{
		return get_free_impl(index).next;
	}

	inline void set_free_next(int32_t index, int32_t newNext) noexcept
	{
		get_free_impl(index).next = newNext;
	}

	_MST_NODISCARD inline T& get_impl(int32_t index) noexcept
	{
		const auto pageIndex = index >> _MST_GET_SHIFT(ElementsPerPage);
		const auto elemIndex = index & (ElementsPerPage - 1);

		return get_impl(pageIndex, elemIndex);
	}

	_MST_NODISCARD inline T& get_impl(int32_t pageIndex, int32_t elemIndex) noexcept
	{
		return *reinterpret_cast<T*>(&m_pages[pageIndex][elemIndex].elem);
	}

	_MST_NODISCARD inline const T& get_impl(int32_t index) const noexcept
	{
		const auto pageIndex = index >> _MST_GET_SHIFT(ElementsPerPage);
		const auto elemIndex = index & (ElementsPerPage - 1);

		return get_impl(pageIndex, elemIndex);
	}

	_MST_NODISCARD inline const T& get_impl(int32_t pageIndex, int32_t elemIndex) const noexcept
	{
		return *reinterpret_cast<const T*>(&m_pages[pageIndex][elemIndex].elem);
	}

	_MST_NODISCARD inline FreeListNode& get_free_impl(int32_t index) noexcept
	{
		const auto pageIndex = index >> _MST_GET_SHIFT(ElementsPerPage);
		const auto elemIndex = index & (ElementsPerPage - 1);

		return m_pages[pageIndex][elemIndex].node;
	}

	_MST_NODISCARD inline const FreeListNode& get_free_impl(int32_t index) const noexcept
	{
		const auto pageIndex = index >> _MST_GET_SHIFT(ElementsPerPage);
		const auto elemIndex = index & (ElementsPerPage - 1);

		return m_pages[pageIndex][elemIndex].node;
	}

	inline void destroy_impl(T&, std::true_type) noexcept
	{
		// do nothing
	}

	inline void destroy_impl(T& t, std::false_type) noexcept
	{
		t.~T();
	}

	inline void destroy(T& t) noexcept
	{
		destroy_impl(t, typename std::is_trivially_destructible<T>::type{});
	}

	inline void destroy_pages_impl(ElemType**, int32_t, std::true_type) noexcept
	{
		// do nothing
	}

	inline void destroy_pages_impl(ElemType** pages, int32_t pageCount, std::false_type) noexcept
	{
		for(int32_t p = 0; p < pageCount; ++p)
		{
			for(int32_t e; e < ElementsPerPage; ++e)
			{
				pages[p].elements[e].elem.~T();
			}
		}
	}

	inline void destroy_pages(ElemType** pages, int32_t pageCount) noexcept
	{
		destroy_pages_impl(pages, pageCount, typename std::is_trivially_destructible<T>::type{});
	}

	inline void destroy_all()
	{
		if(m_pages)
		{ // m_skips,m_capacity+1
			auto index = m_skips[0];

			while(index != m_capacity)
			{
				get_impl(index).~T();

				index = index + m_skips[index + 1] + 1;
			}

			const auto pageCount = m_capacity >> _MST_GET_SHIFT(ElementsPerPage);

			for(int32_t i = 0; i < pageCount; ++i)
			{
				delete[] m_pages[i];
			}

			delete[] m_pages;
			delete[] m_skips;
		}
	}

private:
	ElemType** m_pages;
	int32_t m_elementCount, m_capacity;
	int32_t m_freeListHead;
	int32_t* m_skips;

}; // class colony

template<typename T>
class colony_iterator
{
	friend class colony<T>;
	friend class colony_const_iterator<T>;

public:
	inline colony_iterator() noexcept
		: m_container(nullptr)
		, m_index(0)
	{ }

	inline colony_iterator(colony<T>& container, int32_t index) noexcept
		: m_container(&container)
		, m_index(index)
	{ }

	colony_iterator(const colony_iterator&) = default;
	colony_iterator(colony_iterator&&) = default;

	colony_iterator& operator=(const colony_iterator&) = default;
	colony_iterator& operator=(colony_iterator&&) = default;

	_MST_NODISCARD inline colony_iterator& operator++() noexcept
	{
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement end iterator");

		m_index += m_container->m_skips[m_index + 1] + 1;
		return *this;
	}

	_MST_NODISCARD inline colony_iterator operator++(int) noexcept
	{
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement end iterator");

		const auto retval = *this;
		m_index += m_container->m_skips[m_index + 1] + 1;
		return retval;
	}

	_MST_NODISCARD inline colony_iterator& operator--() noexcept
	{
		MST_ASSERT(m_index > 0, "cannot decrement begin iterator");

		m_index += m_container->m_skips[m_index - 1] - 1;
		return *this;
	}

	_MST_NODISCARD inline colony_iterator operator--(int) noexcept
	{
		MST_ASSERT(m_index > 0, "cannot decrement begin iterator");

		const auto retval = *this;
		m_index += m_container->m_skips[m_index - 1] - 1;
		return retval;
	}

	_MST_NODISCARD inline bool operator==(const colony_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index == other.m_index;
	}

	_MST_NODISCARD inline bool operator!=(const colony_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	_MST_NODISCARD inline T& operator*() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return m_container->get_impl(m_index);
	}

	_MST_NODISCARD inline T* operator->() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return &m_container->get_impl(m_index);
	}

	_MST_NODISCARD int32_t idx() const noexcept
	{
		return m_index;
	}

	_MST_NODISCARD T* ptr() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return &m_container->get_impl(m_index);
	}

private:
	colony<T>* m_container;
	int32_t m_index;
};

template<typename T>
class colony_const_iterator
{
	friend class colony<T>;

public:
	inline colony_const_iterator() noexcept
		: m_container(nullptr)
		, m_index(0)
	{ }

	inline colony_const_iterator(const colony<T>& container, int32_t index) noexcept
		: m_container(&container)
		, m_index(index)
	{ }

	inline colony_const_iterator(const colony_iterator<T>& other) noexcept
		: m_container(other.m_container)
		, m_index(other.m_index)
	{ }

	_MST_NODISCARD inline colony_const_iterator& operator++() noexcept
	{
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement end iterator");

		m_index += m_container->m_skips[m_index + 1] + 1;
		return *this;
	}

	_MST_NODISCARD inline colony_const_iterator operator++(int) noexcept
	{
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement end iterator");

		const auto retval = *this;
		m_index += m_container->m_skips[m_index + 1] + 1;
		return retval;
	}

	_MST_NODISCARD inline bool operator==(const colony_const_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index == other.m_index;
	}

	_MST_NODISCARD inline bool operator!=(const colony_const_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	_MST_NODISCARD inline const T& operator*() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return m_container->get_impl(m_index);
	}

	_MST_NODISCARD inline const T* operator->() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return &m_container->get_impl(m_index);
	}

	_MST_NODISCARD int32_t idx() const noexcept
	{
		return m_index;
	}

	_MST_NODISCARD const T* ptr() const noexcept
	{
		MST_ASSERT(m_index >= 0, "cannot decrement iterator");
		MST_ASSERT(m_index < m_container->m_capacity, "cannot decrement iterator");
		MST_ASSERT(m_container->m_skips[m_index] == 0, "cannot decrement iterator");

		return &m_container->get_impl(m_index);
	}

private:
	const colony<T>* m_container;
	int32_t m_index;
};

} // namespace mst