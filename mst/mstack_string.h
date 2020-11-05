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
#include <mdll_string.h>
#include <string>
#include <mdebug.h>
#include <mx_is_string_type.h>

#ifndef _MSTACK_STRING_H
#define _MSTACK_STRING_H
#endif // !_MSTACK_STRING_H

namespace mst {
namespace dll_safe {

template<typename T>
class string;

}; // namespace dll_safe

template<typename T, size_t MaxElements>
class stack_string;

template<size_t MaxElements>
struct is_string_type<::mst::stack_string<char, MaxElements>>
	: public ::std::integral_constant<bool, true>
{ };

template<size_t MaxElements>
struct is_wstring_type<::mst::stack_string<wchar_t, MaxElements>>
	: public ::std::integral_constant<bool, true>
{ };

}; // namespace mst

namespace mst {

template<typename T, size_t MaxElements>
class stack_string
{
	static_assert(::std::is_pod<T>::value, "stack_string<T, MaxElements>: T is not plain old data");
	static_assert(MaxElements != 0, "string<T, MaxElements>: MaxElements is zero");

public:
	typedef T* pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

public:
	static const size_t npos = (size_t)-1;

	typedef T value_type;

	inline stack_string()
	{
		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;
	}

	inline stack_string(const T* cStr)
	{
		CHECK_IF(cStr == nullptr, "ptr == nullptr");

		CHECK_IF(strlen(cStr) >= MaxElements, "cStr: length is out of range");

		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		while(cStr[_Mylength])
		{
			_Myptr[_Mylength] = cStr[_Mylength];
			++_Mylength;
		}
	}

	template<typename Traits, typename Allocator>
	inline stack_string(const ::std::basic_string<T, Traits, Allocator>& stdString)
	{
		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		auto bg = stdString.begin();
		auto ed = stdString.end();

		while(bg != ed)
		{
			_Myptr[_Mylength++] = *bg++;
		}
	}

	inline stack_string(const stack_string& other)
	{
		memcpy(_Myptr, other._Myptr, sizeof(other._Myptr));
		_Mylength = other._Mylength;
	}

	template<size_t OtherMaxElements>
	inline stack_string(const ::mst::stack_string<T, OtherMaxElements>& other)
	{
		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		auto begin = other.begin();
		auto end = other.end();

		while(begin != end)
		{
			_Myptr[_Mylength++] = *begin++;
		}
	}

	template<typename IteratorType>
	inline stack_string(IteratorType begin, IteratorType end)
	{
		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		while(begin != end)
		{
			_Myptr[_Mylength++] = *begin++;
		}
	}

	inline stack_string& operator=(const stack_string& other)
	{
		memcpy(_Myptr, other._Myptr, sizeof(other._Myptr));

		return *this;
	}

	template<size_t OtherMaxElements>
	inline stack_string& operator=(const ::mst::stack_string<T, OtherMaxElements>& other)
	{
		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		auto begin = other.begin();
		auto end = other.end();

		while(begin != end)
		{
			_Myptr[_Mylength++] = *begin++;
		}

		return *this;
	}

	inline stack_string& operator=(const T* cStr)
	{
		CHECK_IF(cStr == nullptr, "ptr == nullptr");

		CHECK_IF(strlen(cStr) >= MaxElements, "cStr: length is out of range");

		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		while(cStr[_Mylength])
		{
			_Myptr[_Mylength] = cStr[_Mylength];
			++_Mylength;
		}

		return *this;
	}

	inline stack_string& operator=(const ::std::string& stdStr)
	{
		CHECK_IF(stdStr.length() >= MaxElements, "stdStr: length is out of range");

		memset(_Myptr, 0, sizeof(_Myptr));
		_Mylength = 0;

		while(stdStr[_Mylength])
		{
			_Myptr[_Mylength] = stdStr[_Mylength];
			++_Mylength;
		}

		return *this;
	}

	inline iterator begin()
	{
		return _Myptr;
	}

	inline iterator end()
	{
		return _Myptr + _Mylength;
	}

	inline const_iterator begin() const
	{
		return _Myptr;
	}

	inline const_iterator end() const
	{
		return _Myptr + _Mylength;
	}

	inline const_iterator cbegin() const
	{
		return _Myptr;
	}

	inline const_iterator cend() const
	{
		return _Myptr + _Mylength;
	}

	inline T* data()
	{
		return _Myptr;
	}

	inline const T* data() const
	{
		return _Myptr;
	}

	inline const T* c_str() const
	{
		return data();
	}

	inline ::std::string std_string() const
	{
		return ::std::string(_Myptr, _Mylength);
	}

	inline reference operator[](size_t index)
	{
		CHECK_ARR_RANGE(index, _Mylength, "index out of range");

		return _Myptr[index];
	}

	inline const_reference operator[](size_t index) const
	{
		CHECK_ARR_RANGE(index, _Mylength, "index out of range");

		return _Myptr[index];
	}

	inline bool empty() const
	{
		return _Mylength == 0;
	}

	inline size_t size() const
	{
		return _Mylength;
	}

	inline size_t length() const
	{
		return _Mylength;
	}

	constexpr inline size_t capacity() const
	{
		return MaxElements - 1;
	}

	constexpr inline size_t max_size() const
	{
		return MaxElements - 1;
	}

	inline void push_back(const T& value)
	{
		CHECK_IF(size() >= max_size(), "container is already at max size");

		_Myptr[_Mylength++] = value;
	}

	inline void pop_back()
	{
		CHECK_IF(empty(), "container is empty");

		_Myptr[_Mylength--] = (T)0;
	}

	inline void insert(const_iterator iter, const T& value)
	{
		CHECK_ITER_RANGE(iter, begin(), end(), "iterator is not from this container");
		CHECK_IF(size() >= max_size(), "container is already at max size");

		size_t offset = (size_t)(iter - begin());

		::std::char_traits<T>::move(_Myptr + offset + 1, _Myptr + offset, _Mylength - offset - 1);

		_Myptr[offset] = value;
	}

	stack_string& append(const stack_string& other)
	{
		CHECK_IF(this == &other, "cannot append to itself");

		size_t prevSize = size();
		size_t otherSize = other.size();
		CHECK_IF(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<T>::copy(_Myptr + prevSize, other.data(), otherSize);

		return *this;
	}

	template<typename Traits, typename Alloc>
	stack_string& append(const ::std::basic_string<T, Traits, Alloc>& other)
	{
		size_t prevSize = size();
		size_t otherSize = other.size();
		CHECK_IF(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<T>::copy(_Myptr + prevSize, other.data(), otherSize);

		return *this;
	}

	stack_string& append(const T* cStr)
	{
		CHECK_IF(cStr == nullptr, "invalid nullptr value");
		size_t prevSize = size();
		size_t otherSize = ::std::char_traits<T>::length(cStr);
		CHECK_IF(prevSize + otherSize >= max_size(), "appending out of bounds");

		::std::char_traits<T>::copy(_Myptr + prevSize, cStr, otherSize);

		return *this;
	}

	stack_string& operator+=(const T& character)
	{
		push_back(character);
		return *this;
	}

	stack_string& operator+=(const stack_string& other)
	{
		return append(other);
	}

	stack_string& operator+=(const T* cStr)
	{
		return append(cStr);
	}


	inline stack_string(const ::mst::dll_safe::string<T>& dllString);
	inline stack_string& operator=(const ::mst::dll_safe::string<T>& dllString);

private:
	T _Myptr[MaxElements];
	size_t _Mylength;

}; // class stack_string

}; // namespace mst

namespace std {

template<typename T, size_t MaxElements>
::std::basic_ostream<T>& operator<<(
	::std::basic_ostream<T>& ostr, const ::mst::stack_string<T, MaxElements>& str)
{
	return ostr << str.c_str();
}

};

template<typename T, size_t MaxElements>
inline bool operator==(const ::mst::stack_string<T, MaxElements>& l, const T* r)
{
	return strcmp(l.c_str(), r) == 0;
}

template<typename T, size_t MaxElements>
inline bool operator!=(const ::mst::stack_string<T, MaxElements>& l, const T* r)
{
	return strcmp(l.c_str(), r) != 0;
}

template<typename T, size_t MaxElements>
inline bool operator==(const T* l, const ::mst::stack_string<T, MaxElements>& r)
{
	return strcmp(l, r.c_str()) == 0;
}

template<typename T, size_t MaxElements>
inline bool operator!=(const T* l, const ::mst::stack_string<T, MaxElements>& r)
{
	return strcmp(l, r.c_str()) != 0;
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator==(const ::mst::stack_string<T, MaxElements>& l,
	const ::std::basic_string<T, Traits, Allocator>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), l.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator!=(const ::mst::stack_string<T, MaxElements>& l,
	const ::std::basic_string<T, Traits, Allocator>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), l.size() * sizeof(T)) != 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator==(const ::std::basic_string<T, Traits, Allocator>& l,
	const ::mst::stack_string<T, MaxElements>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), r.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements, typename Traits, typename Allocator>
inline bool operator!=(const ::std::basic_string<T, Traits, Allocator>& l,
	const ::mst::stack_string<T, MaxElements>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), r.size() * sizeof(T)) != 0);
}

#if defined(_MDLL_STRING_H) && defined(_MSTACK_STRING_H)
#include <mx_dll_string_stack_string.inl>
#endif