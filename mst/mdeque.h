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
#include <mdebug.h>

#include <new>
#include <utility>
#include <maligned_malloc.h>
#include <iterator>
#include <stdexcept>
#include <initializer_list>

#if _MST_HAS_BITOPS
#include <bit>
#endif

namespace mst {

template<typename T>
class deque_iterator;

template<typename T>
class deque_const_iterator;

// std::vector-like expanding ring buffer: a single contiguous, power-of-two sized buffer that
// wraps around, so push/pop on either end are O(1) without ever shuffling elements. Because the
// capacity is always a power of two (or zero), wrapping an index is a plain AND mask instead of
// a modulo.
template<typename T>
class deque
{
	friend class deque_iterator<T>;
	friend class deque_const_iterator<T>;

public:
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef deque_iterator<T> iterator;
	typedef deque_const_iterator<T> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	inline deque() noexcept
		: m_data(nullptr)
		, m_capacity(0)
		, m_head(0)
		, m_size(0)
	{ }

	inline explicit deque(size_type count)
		: deque()
	{
		resize(count);
	}

	inline deque(size_type count, const T& value)
		: deque()
	{
		resize(count, value);
	}

	inline deque(std::initializer_list<T> initList)
		: deque()
	{
		reserve(initList.size());

		for(const auto& value : initList)
		{
			push_back(value);
		}
	}

	inline deque(const deque& other)
		: m_data(nullptr)
		, m_capacity(0)
		, m_head(0)
		, m_size(0)
	{
		if(!other.empty())
		{
			m_data = _Allocate(_Next_pow2(other.m_size));
			m_capacity = _Next_pow2(other.m_size);

			for(size_type i = 0; i < other.m_size; ++i)
			{
				new(m_data + i) T(other[i]);
				++m_size;
			}
		}
	}

	inline deque(deque&& other) noexcept
		: m_data(other.m_data)
		, m_capacity(other.m_capacity)
		, m_head(other.m_head)
		, m_size(other.m_size)
	{
		other.m_data = nullptr;
		other.m_capacity = 0;
		other.m_head = 0;
		other.m_size = 0;
	}

	inline ~deque()
	{
		_Destroy_all();
		_Deallocate(m_data);
	}

	inline deque& operator=(const deque& other)
	{
		if(this != &other)
		{
			deque tmp(other);
			swap(tmp);
		}
		return *this;
	}

	inline deque& operator=(deque&& other) noexcept
	{
		if(this != &other)
		{
			_Destroy_all();
			_Deallocate(m_data);

			m_data = other.m_data;
			m_capacity = other.m_capacity;
			m_head = other.m_head;
			m_size = other.m_size;

			other.m_data = nullptr;
			other.m_capacity = 0;
			other.m_head = 0;
			other.m_size = 0;
		}
		return *this;
	}

	inline deque& operator=(std::initializer_list<T> initList)
	{
		clear();
		reserve(initList.size());

		for(const auto& value : initList)
		{
			push_back(value);
		}
		return *this;
	}

	_MST_NODISCARD inline bool empty() const noexcept
	{
		return m_size == 0;
	}

	_MST_NODISCARD inline size_type size() const noexcept
	{
		return m_size;
	}

	_MST_NODISCARD inline size_type capacity() const noexcept
	{
		return m_capacity;
	}

	inline void reserve(size_type newCapacity)
	{
		if(newCapacity > m_capacity)
		{
			_Grow(_Next_pow2(newCapacity));
		}
	}

	inline void shrink_to_fit()
	{
		const auto newCapacity = _Next_pow2(m_size);

		if(newCapacity < m_capacity)
		{
			if(newCapacity == 0)
			{
				_Destroy_all();
				_Deallocate(m_data);
				m_data = nullptr;
				m_capacity = 0;
				m_head = 0;
			}
			else
			{
				_Grow(newCapacity);
			}
		}
	}

	inline void clear() noexcept
	{
		_Destroy_all();
		m_size = 0;
		m_head = 0;
	}

	_MST_NODISCARD inline reference operator[](size_type index) noexcept
	{
		MST_ASSERT(index < m_size, "index out of range");

		return m_data[_To_physical(index)];
	}

	_MST_NODISCARD inline const_reference operator[](size_type index) const noexcept
	{
		MST_ASSERT(index < m_size, "index out of range");

		return m_data[_To_physical(index)];
	}

	_MST_NODISCARD inline reference at(size_type index)
	{
		if(index >= m_size)
		{
			throw std::out_of_range("mst::deque::at: index out of range");
		}

		return m_data[_To_physical(index)];
	}

	_MST_NODISCARD inline const_reference at(size_type index) const
	{
		if(index >= m_size)
		{
			throw std::out_of_range("mst::deque::at: index out of range");
		}

		return m_data[_To_physical(index)];
	}

	_MST_NODISCARD inline reference front() noexcept
	{
		MST_ASSERT(!empty(), "cannot call front() on an empty deque");

		return m_data[m_head];
	}

	_MST_NODISCARD inline const_reference front() const noexcept
	{
		MST_ASSERT(!empty(), "cannot call front() on an empty deque");

		return m_data[m_head];
	}

	_MST_NODISCARD inline reference back() noexcept
	{
		MST_ASSERT(!empty(), "cannot call back() on an empty deque");

		return m_data[_To_physical(m_size - 1)];
	}

	_MST_NODISCARD inline const_reference back() const noexcept
	{
		MST_ASSERT(!empty(), "cannot call back() on an empty deque");

		return m_data[_To_physical(m_size - 1)];
	}

	inline void push_back(const T& value)
	{
		emplace_back(value);
	}

	inline void push_back(T&& value)
	{
		emplace_back(std::move(value));
	}

	template<typename... Args>
	inline reference emplace_back(Args&&... args)
	{
		if(m_size == m_capacity)
		{
			_Grow(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		const auto physIndex = _To_physical(m_size);
		new(m_data + physIndex) T(std::forward<Args>(args)...);
		++m_size;

		return m_data[physIndex];
	}

	inline void pop_back() noexcept
	{
		MST_ASSERT(!empty(), "cannot call pop_back() on an empty deque");

		m_data[_To_physical(m_size - 1)].~T();
		--m_size;
	}

	inline void push_front(const T& value)
	{
		emplace_front(value);
	}

	inline void push_front(T&& value)
	{
		emplace_front(std::move(value));
	}

	template<typename... Args>
	inline reference emplace_front(Args&&... args)
	{
		if(m_size == m_capacity)
		{
			_Grow(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		m_head = _Mask(m_head - 1);
		new(m_data + m_head) T(std::forward<Args>(args)...);
		++m_size;

		return m_data[m_head];
	}

	inline void pop_front() noexcept
	{
		MST_ASSERT(!empty(), "cannot call pop_front() on an empty deque");

		m_data[m_head].~T();
		m_head = _Mask(m_head + 1);
		--m_size;
	}

	inline iterator insert(const_iterator pos, const T& value)
	{
		return emplace(pos, value);
	}

	inline iterator insert(const_iterator pos, T&& value)
	{
		return emplace(pos, std::move(value));
	}

	inline iterator insert(const_iterator pos, size_type count, const T& value)
	{
		const auto idx = (size_type)(pos - cbegin());

		for(size_type i = 0; i < count; ++i)
		{
			emplace(begin() + (difference_type)(idx + i), value);
		}

		return begin() + (difference_type)idx;
	}

	template<typename... Args>
	inline iterator emplace(const_iterator pos, Args&&... args)
	{
		const auto idx = (size_type)(pos - cbegin());

		MST_ASSERT(idx <= m_size, "iterator out of range");

		if(m_size == m_capacity)
		{
			_Grow(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		const auto elemsBefore = idx;
		const auto elemsAfter = m_size - idx;

		if(elemsBefore <= elemsAfter)
		{
			m_head = _Mask(m_head - 1);
			++m_size;

			for(size_type i = 0; i < elemsBefore; ++i)
			{
				new(&(*this)[i]) T(std::move((*this)[i + 1]));
				(*this)[i + 1].~T();
			}
		}
		else
		{
			++m_size;

			for(size_type i = m_size - 1; i > idx; --i)
			{
				new(&(*this)[i]) T(std::move((*this)[i - 1]));
				(*this)[i - 1].~T();
			}
		}

		new(&(*this)[idx]) T(std::forward<Args>(args)...);

		return begin() + (difference_type)idx;
	}

	inline iterator erase(const_iterator pos) noexcept
	{
		return erase(pos, pos + 1);
	}

	inline iterator erase(const_iterator first, const_iterator last) noexcept
	{
		const auto firstIdx = (size_type)(first - cbegin());
		const auto lastIdx = (size_type)(last - cbegin());

		MST_ASSERT(firstIdx <= lastIdx, "invalid erase range");
		MST_ASSERT(lastIdx <= m_size, "erase range out of bounds");

		const auto count = lastIdx - firstIdx;

		if(count == 0)
		{
			return begin() + (difference_type)firstIdx;
		}

		const auto elemsBefore = firstIdx;
		const auto elemsAfter = m_size - lastIdx;

		if(elemsBefore <= elemsAfter)
		{
			for(size_type i = firstIdx; i > 0; --i)
			{
				(*this)[i - 1 + count] = std::move((*this)[i - 1]);
			}

			for(size_type i = 0; i < count; ++i)
			{
				pop_front();
			}
		}
		else
		{
			for(size_type i = lastIdx; i < m_size; ++i)
			{
				(*this)[i - count] = std::move((*this)[i]);
			}

			for(size_type i = 0; i < count; ++i)
			{
				pop_back();
			}
		}

		return begin() + (difference_type)firstIdx;
	}

	inline void resize(size_type newSize)
	{
		if(newSize > m_size)
		{
			reserve(newSize);

			while(m_size < newSize)
			{
				emplace_back();
			}
		}
		else
		{
			while(m_size > newSize)
			{
				pop_back();
			}
		}
	}

	inline void resize(size_type newSize, const T& value)
	{
		if(newSize > m_size)
		{
			reserve(newSize);

			while(m_size < newSize)
			{
				push_back(value);
			}
		}
		else
		{
			while(m_size > newSize)
			{
				pop_back();
			}
		}
	}

	inline void swap(deque& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_capacity, other.m_capacity);
		std::swap(m_head, other.m_head);
		std::swap(m_size, other.m_size);
	}

	_MST_NODISCARD inline iterator begin() noexcept
	{
		return iterator(*this, 0);
	}

	_MST_NODISCARD inline const_iterator begin() const noexcept
	{
		return const_iterator(*this, 0);
	}

	_MST_NODISCARD inline const_iterator cbegin() const noexcept
	{
		return const_iterator(*this, 0);
	}

	_MST_NODISCARD inline iterator end() noexcept
	{
		return iterator(*this, m_size);
	}

	_MST_NODISCARD inline const_iterator end() const noexcept
	{
		return const_iterator(*this, m_size);
	}

	_MST_NODISCARD inline const_iterator cend() const noexcept
	{
		return const_iterator(*this, m_size);
	}

	_MST_NODISCARD inline reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	_MST_NODISCARD inline const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	_MST_NODISCARD inline const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	_MST_NODISCARD inline reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	_MST_NODISCARD inline const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	_MST_NODISCARD inline const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

private:
	_MST_NODISCARD inline size_type _Mask(size_type index) const noexcept
	{
		return index & (m_capacity - 1);
	}

	_MST_NODISCARD inline size_type _To_physical(size_type logicalIndex) const noexcept
	{
		return _Mask(m_head + logicalIndex);
	}

	_MST_NODISCARD static inline size_type _Next_pow2(size_type value) noexcept
	{
#if _MST_HAS_BITOPS
		return std::bit_ceil(value);
#else
		if(value <= 1)
		{
			return value;
		}

		--value;

		for(size_type shift = 1; shift < sizeof(size_type) * 8; shift <<= 1)
		{
			value |= value >> shift;
		}

		return value + 1;
#endif
	}

	_MST_NODISCARD static inline T* _Allocate(size_type count)
	{
		return count == 0 ? nullptr
						  : static_cast<T*>(mst::aligned_malloc(count * sizeof(T), alignof(T)));
	}

	static inline void _Deallocate(T* ptr) noexcept
	{
		if(ptr != nullptr)
		{
			mst::aligned_free(ptr);
		}
	}

	inline void _Destroy_all() noexcept
	{
		for(size_type i = 0; i < m_size; ++i)
		{
			m_data[_To_physical(i)].~T();
		}
	}

	inline void _Grow(size_type newCapacity)
	{
		MST_ASSERT(newCapacity >= m_size, "cannot shrink below the current size");
		MST_ASSERT(newCapacity == 0 || (newCapacity & (newCapacity - 1)) == 0,
			"newCapacity must be a power of two");

		T* newData = _Allocate(newCapacity);

		for(size_type i = 0; i < m_size; ++i)
		{
			T* src = m_data + _To_physical(i);
			new(newData + i) T(std::move(*src));
			src->~T();
		}

		_Deallocate(m_data);

		m_data = newData;
		m_capacity = newCapacity;
		m_head = 0;
	}

private:
	T* m_data;
	size_type m_capacity;
	size_type m_head;
	size_type m_size;

}; // class deque<T>

template<typename T>
inline void swap(deque<T>& left, deque<T>& right) noexcept
{
	left.swap(right);
}

template<typename T>
_MST_NODISCARD inline bool operator==(const deque<T>& left, const deque<T>& right)
{
	if(left.size() != right.size())
	{
		return false;
	}

	for(size_t i = 0; i < left.size(); ++i)
	{
		if(!(left[i] == right[i]))
		{
			return false;
		}
	}

	return true;
}

template<typename T>
_MST_NODISCARD inline bool operator!=(const deque<T>& left, const deque<T>& right)
{
	return !(left == right);
}

template<typename T>
class deque_iterator
{
	friend class deque<T>;
	friend class deque_const_iterator<T>;

public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;

	inline deque_iterator() noexcept
		: m_container(nullptr)
		, m_index(0)
	{ }

	inline deque_iterator(deque<T>& container, size_t index) noexcept
		: m_container(&container)
		, m_index(index)
	{ }

	deque_iterator(const deque_iterator&) = default;
	deque_iterator(deque_iterator&&) = default;

	deque_iterator& operator=(const deque_iterator&) = default;
	deque_iterator& operator=(deque_iterator&&) = default;

	_MST_NODISCARD inline reference operator*() const noexcept
	{
		return (*m_container)[m_index];
	}

	_MST_NODISCARD inline pointer operator->() const noexcept
	{
		return &(*m_container)[m_index];
	}

	_MST_NODISCARD inline reference operator[](difference_type offset) const noexcept
	{
		return (*m_container)[size_t((difference_type)m_index + offset)];
	}

	inline deque_iterator& operator++() noexcept
	{
		++m_index;
		return *this;
	}

	inline deque_iterator operator++(int) noexcept
	{
		const auto retval = *this;
		++m_index;
		return retval;
	}

	inline deque_iterator& operator--() noexcept
	{
		--m_index;
		return *this;
	}

	inline deque_iterator operator--(int) noexcept
	{
		const auto retval = *this;
		--m_index;
		return retval;
	}

	inline deque_iterator& operator+=(difference_type offset) noexcept
	{
		m_index = size_t((difference_type)m_index + offset);
		return *this;
	}

	inline deque_iterator& operator-=(difference_type offset) noexcept
	{
		m_index = size_t((difference_type)m_index - offset);
		return *this;
	}

	_MST_NODISCARD inline deque_iterator operator+(difference_type offset) const noexcept
	{
		return deque_iterator(*m_container, size_t((difference_type)m_index + offset));
	}

	_MST_NODISCARD inline deque_iterator operator-(difference_type offset) const noexcept
	{
		return deque_iterator(*m_container, size_t((difference_type)m_index - offset));
	}

	_MST_NODISCARD inline difference_type operator-(const deque_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return (difference_type)m_index - (difference_type)other.m_index;
	}

	_MST_NODISCARD friend inline deque_iterator operator+(
		difference_type offset, const deque_iterator& it) noexcept
	{
		return it + offset;
	}

	_MST_NODISCARD inline bool operator==(const deque_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index == other.m_index;
	}

	_MST_NODISCARD inline bool operator!=(const deque_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	_MST_NODISCARD inline bool operator<(const deque_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index < other.m_index;
	}

	_MST_NODISCARD inline bool operator>(const deque_iterator& other) const noexcept
	{
		return other < *this;
	}

	_MST_NODISCARD inline bool operator<=(const deque_iterator& other) const noexcept
	{
		return !(other < *this);
	}

	_MST_NODISCARD inline bool operator>=(const deque_iterator& other) const noexcept
	{
		return !(*this < other);
	}

private:
	deque<T>* m_container;
	size_t m_index;
};

template<typename T>
class deque_const_iterator
{
	friend class deque<T>;

public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;

	inline deque_const_iterator() noexcept
		: m_container(nullptr)
		, m_index(0)
	{ }

	inline deque_const_iterator(const deque<T>& container, size_t index) noexcept
		: m_container(&container)
		, m_index(index)
	{ }

	inline deque_const_iterator(const deque_iterator<T>& other) noexcept
		: m_container(other.m_container)
		, m_index(other.m_index)
	{ }

	deque_const_iterator(const deque_const_iterator&) = default;
	deque_const_iterator(deque_const_iterator&&) = default;

	deque_const_iterator& operator=(const deque_const_iterator&) = default;
	deque_const_iterator& operator=(deque_const_iterator&&) = default;

	_MST_NODISCARD inline reference operator*() const noexcept
	{
		return (*m_container)[m_index];
	}

	_MST_NODISCARD inline pointer operator->() const noexcept
	{
		return &(*m_container)[m_index];
	}

	_MST_NODISCARD inline reference operator[](difference_type offset) const noexcept
	{
		return (*m_container)[size_t((difference_type)m_index + offset)];
	}

	inline deque_const_iterator& operator++() noexcept
	{
		++m_index;
		return *this;
	}

	inline deque_const_iterator operator++(int) noexcept
	{
		const auto retval = *this;
		++m_index;
		return retval;
	}

	inline deque_const_iterator& operator--() noexcept
	{
		--m_index;
		return *this;
	}

	inline deque_const_iterator operator--(int) noexcept
	{
		const auto retval = *this;
		--m_index;
		return retval;
	}

	inline deque_const_iterator& operator+=(difference_type offset) noexcept
	{
		m_index = size_t((difference_type)m_index + offset);
		return *this;
	}

	inline deque_const_iterator& operator-=(difference_type offset) noexcept
	{
		m_index = size_t((difference_type)m_index - offset);
		return *this;
	}

	_MST_NODISCARD inline deque_const_iterator operator+(difference_type offset) const noexcept
	{
		return deque_const_iterator(*m_container, size_t((difference_type)m_index + offset));
	}

	_MST_NODISCARD inline deque_const_iterator operator-(difference_type offset) const noexcept
	{
		return deque_const_iterator(*m_container, size_t((difference_type)m_index - offset));
	}

	_MST_NODISCARD inline difference_type operator-(
		const deque_const_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return (difference_type)m_index - (difference_type)other.m_index;
	}

	_MST_NODISCARD friend inline deque_const_iterator operator+(
		difference_type offset, const deque_const_iterator& it) noexcept
	{
		return it + offset;
	}

	_MST_NODISCARD inline bool operator==(const deque_const_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index == other.m_index;
	}

	_MST_NODISCARD inline bool operator!=(const deque_const_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	_MST_NODISCARD inline bool operator<(const deque_const_iterator& other) const noexcept
	{
		MST_ASSERT(m_container == other.m_container, "invalid container iterators");

		return m_index < other.m_index;
	}

	_MST_NODISCARD inline bool operator>(const deque_const_iterator& other) const noexcept
	{
		return other < *this;
	}

	_MST_NODISCARD inline bool operator<=(const deque_const_iterator& other) const noexcept
	{
		return !(other < *this);
	}

	_MST_NODISCARD inline bool operator>=(const deque_const_iterator& other) const noexcept
	{
		return !(*this < other);
	}

private:
	const deque<T>* m_container;
	size_t m_index;
};

} // namespace mst
