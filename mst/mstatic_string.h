//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mcore.h>
#include <string>
#include <mdebug.h>
#include <string_view>
#include <mx_is_string_type.h>

namespace mst {

template<typename CharT, size_t MaxElements>
class basic_static_string
{
	static_assert(::std::is_pod<CharT>::value,
		"basic_static_string<CharT, MaxElements>: CharT is not plain old data");
	static_assert(MaxElements != 0, "basic_static_string<CharT, MaxElements>: MaxElements is zero");

public:
	typedef CharT* pointer;
	typedef CharT* iterator;
	typedef const CharT* const_iterator;
	typedef CharT& reference;
	typedef const CharT& const_reference;

public:
	static const size_t npos = (size_t)-1;

	typedef CharT value_type;

	constexpr basic_static_string() = default;

	constexpr basic_static_string(const CharT* cStr)
	{
		MST_ASSERT(cStr, "ptr == nullptr");

		MST_ASSERT(
			std::char_traits<CharT>::length(cStr) <= max_size(), "cStr: length is out of range");

		while(cStr[m_length])
		{
			m_ptr[m_length] = cStr[m_length];
			++m_length;
		}
	}

	template<typename Traits>
	constexpr basic_static_string(std::basic_string_view<CharT, Traits> stringView)
		: m_length(stringView.length())
	{
		MST_ASSERT(stringView.length() <= max_size(), "stringView: length is out of range");

		for(size_t i = 0; i < m_length; ++i)
		{
			m_ptr[i] = stringView[i];
		}
	}

	template<typename Traits, typename Allocator>
	constexpr basic_static_string(const ::std::basic_string<CharT, Traits, Allocator>& stdString)
		: m_length(stdString.length())
	{
		auto bg = stdString.begin();

		for(size_t i = 0; i < m_length; ++i)
		{
			m_ptr[i] = *bg++;
		}
	}

	constexpr basic_static_string(const basic_static_string& other) = default;

	template<size_t OtherMaxElements>
	constexpr basic_static_string(const ::mst::basic_static_string<CharT, OtherMaxElements>& other)
		: m_length(other.length())
	{
		MST_ASSERT(other.length() > max_size(), "other: length is out of range");

		auto bg = other.begin();

		for(size_t i = 0; i < m_length; ++i)
		{
			m_ptr[i] = *bg++;
		}
	}

	template<typename IteratorType>
	constexpr basic_static_string(IteratorType begin, IteratorType end)
	{
		while(begin != end)
		{
			m_ptr[m_length++] = *begin++;
		}
	}

	constexpr basic_static_string& operator=(const basic_static_string& other)
	{
		memcpy(m_ptr, other.m_ptr, sizeof(other.m_ptr));

		return *this;
	}

	template<size_t OtherMaxElements>
	constexpr basic_static_string& operator=(
		const ::mst::basic_static_string<CharT, OtherMaxElements>& other)
	{
		clear();

		auto begin = other.begin();
		auto end = other.end();

		while(begin != end)
		{
			m_ptr[m_length++] = *begin++;
		}

		return *this;
	}

	constexpr basic_static_string& operator=(const CharT* cStr)
	{
		MST_ASSERT(cStr, "cStr == nullptr");

		MST_ASSERT(
			std::char_traits<CharT>::length(cStr) <= max_size(), "cStr: length is out of range");

		clear();

		while(cStr[m_length])
		{
			m_ptr[m_length] = cStr[m_length];
			++m_length;
		}

		return *this;
	}

	constexpr basic_static_string& operator=(const ::std::string& stdStr)
	{
		MST_ASSERT(stdStr.length() <= max_size(), "stdStr: length is out of range");

		memset(m_ptr, 0, sizeof(m_ptr));
		m_length = 0;

		while(stdStr[m_length])
		{
			m_ptr[m_length] = stdStr[m_length];
			++m_length;
		}

		return *this;
	}

	[[nodiscard]] constexpr iterator begin()
	{
		return m_ptr;
	}

	[[nodiscard]] constexpr iterator end()
	{
		return m_ptr + m_length;
	}

	[[nodiscard]] constexpr const_iterator begin() const
	{
		return m_ptr;
	}

	[[nodiscard]] constexpr const_iterator end() const
	{
		return m_ptr + m_length;
	}

	[[nodiscard]] constexpr const_iterator cbegin() const
	{
		return m_ptr;
	}

	[[nodiscard]] constexpr const_iterator cend() const
	{
		return m_ptr + m_length;
	}

	[[nodiscard]] constexpr CharT* data()
	{
		return m_ptr;
	}

	[[nodiscard]] constexpr const CharT* data() const
	{
		return m_ptr;
	}

	[[nodiscard]] constexpr const CharT* c_str() const
	{
		return data();
	}

	template<typename T, typename Traits = std::char_traits<T>,
		typename Allocator = std::allocator<T>>
	[[nodiscard]] inline ::std::basic_string<T, Traits, Allocator> str() const
	{
		return ::std::basic_string<T, Traits, Allocator>(m_ptr, m_length);
	}

	[[nodiscard]] constexpr reference operator[](size_t index)
	{
		MST_ASSERT(index < m_length, "index out of range");

		return m_ptr[index];
	}

	[[nodiscard]] constexpr const_reference operator[](size_t index) const
	{
		MST_ASSERT(index < m_length, "index out of range");

		return m_ptr[index];
	}

	[[nodiscard]] constexpr bool empty() const
	{
		return m_length == 0;
	}

	[[nodiscard]] constexpr size_t size() const
	{
		return m_length;
	}

	[[nodiscard]] constexpr size_t length() const
	{
		return m_length;
	}

	[[nodiscard]] constexpr inline size_t capacity() const
	{
		return MaxElements - 1;
	}

	[[nodiscard]] constexpr inline size_t max_size() const
	{
		return MaxElements - 1;
	}

	constexpr void clear()
	{
		for(size_t i = 0; i < m_length; ++i)
		{
			m_ptr[i] = (CharT)0;
		}
	}

	constexpr void push_back(const CharT& value)
	{
		MST_ASSERT(size() < max_size(), "container is already at max size");

		m_ptr[m_length++] = value;
	}

	constexpr void pop_back()
	{
		MST_ASSERT(!empty(), "container is empty");

		m_ptr[m_length--] = (CharT)0;
	}

	constexpr void insert(const_iterator iter, const CharT& value)
	{
		MST_ASSERT(iter >= begin() && iter < end(), "iterator is not from this container");
		MST_ASSERT(size() < max_size(), "container is already at max size");

		const auto offset = static_cast<size_t>(iter - begin());

		::std::char_traits<CharT>::move(m_ptr + offset + 1, m_ptr + offset, m_length - offset - 1);

		m_ptr[offset] = value;
	}

	template<size_t OtherMaxElements>
	constexpr basic_static_string& append(const basic_static_string<CharT, OtherMaxElements>& other)
	{
		size_t prevSize = size();
		size_t otherSize = other.size();

		MST_ASSERT(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<CharT>::copy(m_ptr + prevSize, other.data(), otherSize);

		return *this;
	}

	template<typename Traits, typename Alloc>
	constexpr basic_static_string& append(const ::std::basic_string<CharT, Traits, Alloc>& other)
	{
		size_t prevSize = size();
		size_t otherSize = other.size();

		MST_ASSERT(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<CharT>::copy(m_ptr + prevSize, other.data(), otherSize);

		return *this;
	}

	constexpr basic_static_string& append(const CharT* cStr)
	{
		MST_ASSERT(cStr != nullptr, "invalid nullptr value");
		size_t prevSize = size();
		size_t otherSize = ::std::char_traits<CharT>::length(cStr);

		MST_ASSERT(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<CharT>::copy(m_ptr + prevSize, cStr, otherSize);

		return *this;
	}

	constexpr basic_static_string& operator+=(const CharT& character)
	{
		push_back(character);
		return *this;
	}

	constexpr basic_static_string& operator+=(const basic_static_string& other)
	{
		return append(other);
	}

	constexpr basic_static_string& operator+=(const CharT* cStr)
	{
		return append(cStr);
	}

private:
	CharT m_ptr[MaxElements] = {};
	size_t m_length = 0;

}; // class basic_static_string

template<typename T, size_t MaxElements>
::std::basic_ostream<T>& operator<<(
	::std::basic_ostream<T>& ostr, const ::mst::basic_static_string<T, MaxElements>& str)
{
	return ostr << str.c_str();
}

template<typename T, size_t MaxElements>
inline bool operator==(const ::mst::basic_static_string<T, MaxElements>& l, const T* r)
{
	return strcmp(l.c_str(), r) == 0;
}

template<typename T, size_t MaxElements>
inline bool operator!=(const ::mst::basic_static_string<T, MaxElements>& l, const T* r)
{
	return strcmp(l.c_str(), r) != 0;
}

template<typename T, size_t MaxElements>
inline bool operator==(const T* l, const ::mst::basic_static_string<T, MaxElements>& r)
{
	return strcmp(l, r.c_str()) == 0;
}

template<typename T, size_t MaxElements>
inline bool operator!=(const T* l, const ::mst::basic_static_string<T, MaxElements>& r)
{
	return strcmp(l, r.c_str()) != 0;
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator==(const ::mst::basic_static_string<T, MaxElements>& l,
	const ::std::basic_string<T, Traits, Allocator>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), l.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator!=(const ::mst::basic_static_string<T, MaxElements>& l,
	const ::std::basic_string<T, Traits, Allocator>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), l.size() * sizeof(T)) != 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator==(const ::std::basic_string<T, Traits, Allocator>& l,
	const ::mst::basic_static_string<T, MaxElements>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), r.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator!=(const ::std::basic_string<T, Traits, Allocator>& l,
	const ::mst::basic_static_string<T, MaxElements>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), r.size() * sizeof(T)) != 0);
}

template<typename T, size_t MaxElements>
struct is_string_type<::mst::basic_static_string<T, MaxElements>>
	: public ::std::integral_constant<bool, true>
{ };

template<size_t MaxElements>
using static_string = basic_static_string<char, MaxElements>;

template<size_t MaxElements>
using wstatic_string = basic_static_string<wchar_t, MaxElements>;

template<size_t MaxElements>
using u16static_string = basic_static_string<char16_t, MaxElements>;

template<size_t MaxElements>
using u32static_string = basic_static_string<char32_t, MaxElements>;

} // namespace mst

namespace std {

template<typename T, size_t MaxElements>
struct hash<::mst::basic_static_string<T, MaxElements>>
{
	size_t operator()(const ::mst::basic_static_string<T, MaxElements>& value)
	{
		return std::hash<std::basic_string_view<T>>{ value.data(), value.length() };
	}
};

}