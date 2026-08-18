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
#include <initializer_list>
#include <type_traits>

namespace mst {

// A fixed-capacity, contiguously stored sequence container. Storage for MaxSize elements is
// embedded in the object (no heap allocation); pushing past capacity or popping/indexing an
// empty container is a checked error (MST_ASSERT).
template<typename T, size_t MaxSize>
class static_map
{
	static_assert(MaxSize != 0, "static_map<T, MaxSize>: MaxSize is zero");
	static_assert(::std::is_trivially_copyable<T>::value,
		"static_map<T, MaxSize>: T must be trivially copyable");

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* iterator;
	typedef const T* const_iterator;

	constexpr static_map() = default;

	constexpr static_map(std::initializer_list<T> initList)
	{
		MST_ASSERT(initList.size() <= MaxSize, "initializer_list exceeds static_map capacity");

		for(const T& value : initList)
		{
			m_data[m_size++] = value;
		}
	}

	constexpr static_map(const static_map&) = default;
	constexpr static_map(static_map&&) = default;
	constexpr static_map& operator=(const static_map&) = default;
	constexpr static_map& operator=(static_map&&) = default;

	[[nodiscard]] constexpr iterator begin()
	{
		return m_data;
	}

	[[nodiscard]] constexpr const_iterator begin() const
	{
		return m_data;
	}

	[[nodiscard]] constexpr const_iterator cbegin() const
	{
		return m_data;
	}

	[[nodiscard]] constexpr iterator end()
	{
		return m_data + m_size;
	}

	[[nodiscard]] constexpr const_iterator end() const
	{
		return m_data + m_size;
	}

	[[nodiscard]] constexpr const_iterator cend() const
	{
		return m_data + m_size;
	}

	[[nodiscard]] constexpr pointer data()
	{
		return m_data;
	}

	[[nodiscard]] constexpr const_pointer data() const
	{
		return m_data;
	}

	[[nodiscard]] constexpr bool empty() const
	{
		return m_size == 0;
	}

	[[nodiscard]] constexpr bool full() const
	{
		return m_size == MaxSize;
	}

	[[nodiscard]] constexpr size_t size() const
	{
		return m_size;
	}

	[[nodiscard]] constexpr size_t capacity() const
	{
		return MaxSize;
	}

	[[nodiscard]] constexpr size_t max_size() const
	{
		return MaxSize;
	}

	constexpr void clear()
	{
		m_size = 0;
	}

	constexpr void push_back(const T& value)
	{
		MST_ASSERT(!full(), "cannot call push_back on a full static_map");

		m_data[m_size++] = value;
	}

	constexpr void pop_back()
	{
		MST_ASSERT(!empty(), "cannot call pop_back on an empty static_map");

		--m_size;
	}

	[[nodiscard]] constexpr reference operator[](size_t index)
	{
		MST_ASSERT(index < m_size, "static_map index out of range");

		return m_data[index];
	}

	[[nodiscard]] constexpr const_reference operator[](size_t index) const
	{
		MST_ASSERT(index < m_size, "static_map index out of range");

		return m_data[index];
	}

	[[nodiscard]] constexpr reference front()
	{
		MST_ASSERT(!empty(), "cannot call front() on an empty static_map");

		return m_data[0];
	}

	[[nodiscard]] constexpr const_reference front() const
	{
		MST_ASSERT(!empty(), "cannot call front() on an empty static_map");

		return m_data[0];
	}

	[[nodiscard]] constexpr reference back()
	{
		MST_ASSERT(!empty(), "cannot call back() on an empty static_map");

		return m_data[m_size - 1];
	}

	[[nodiscard]] constexpr const_reference back() const
	{
		MST_ASSERT(!empty(), "cannot call back() on an empty static_map");

		return m_data[m_size - 1];
	}

private:
	T m_data[MaxSize] = {};
	size_t m_size = 0;

}; // class static_map

} // namespace mst
