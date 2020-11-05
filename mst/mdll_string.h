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
#include <mdll_malloc.h>
#include <mdebug.h>
#include <algorithm>
#include <mx_is_string_type.h>
#include <mstack_string.h>

#ifndef _MDLL_STRING_H
#define _MDLL_STRING_H
#endif // !_MDLL_STRING_H

namespace mst {

template<typename T, size_t MaxElements>
class stack_string;

namespace dll_safe {

template<typename T>
class string;

}; // namespace dll_safe

template<>
struct is_string_type<::mst::dll_safe::string<char>> : public ::std::integral_constant<bool, true>
{ };

template<>
struct is_wstring_type<::mst::dll_safe::string<wchar_t>>
	: public ::std::integral_constant<bool, true>
{ };

}; // namespace mst

namespace mst {
namespace dll_safe {

template<typename T>
class string
{
	static_assert(::std::is_pod<T>::value, "string<T>: T is not plain old data");

public:
	typedef T* pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

public:
	static const size_t npos = (size_t)-1;

	typedef T value_type;

	string()
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;
		clear();
	}

	template<typename Traits, typename Allocator>
	string(const ::std::basic_string<T, Traits, Allocator>& stdString)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t len = stdString.length();
		_Alloc(len);
		for(size_t i = 0; i < len; ++i)
		{
			_Myptr[i] = stdString[i];
		}
	}

	template<size_t MaxElements>
	inline string(const ::mst::stack_string<T, MaxElements>& stackString);

	template<typename InputIterator>
	explicit string(InputIterator beginIter, InputIterator endIter)
	{
		CHECK_IF(beginIter > endIter, "invalid iterator range");

		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		_Alloc(endIter - beginIter);

		iterator myBegin = begin();

		while(beginIter != endIter)
		{
			*myBegin++ = *beginIter++;
		}
	}

	explicit string(size_t initCount, const T& initValue)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		_Alloc(initCount);
		for(size_t i = 0; i < initCount; ++i)
		{
			_Myptr[i] = initValue;
		}
	}

	string(const string& other)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;
		_Set(other);
	}

	string(string&& other)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;
		_Set(other);
	}

	string(const T* str)
	{
		CHECK_IF(str == nullptr, "invalid nullptr value");

		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;
		_Set(str);
	}

	string(const T* str, size_t strLength)
	{
		CHECK_IF(str == nullptr, "invalid nullptr value");
		CHECK_IF(::std::char_traits<T>::length(str) < strLength, "invalid string length");

		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;
		_Set(str, strLength);
	}

	~string()
	{
		_Release();
	}

	string substr(size_t offset = 0, size_t length = npos) const
	{
		if(offset >= size())
			throw std::invalid_argument("offset is out of range");

		size_t len = size() - offset;
		if(length < len)
		{
			len = length;
		}

		return string(*this, offset, len);
	}

	const T* data() const
	{
		return _Myptr;
	}

	T* data()
	{
		return _Myptr;
	}

	const T* c_str() const
	{
		return data();
	}

	::std::basic_string<T> std_str() const
	{
		return ::std::basic_string<T>(_Myptr, _Mylength);
	}

	bool empty() const
	{
		return _Mylength == 0;
	}

	size_t size() const
	{
		return _Mylength;
	}

	size_t length() const
	{
		return _Mylength;
	}

	size_t data_size() const
	{
		return _Mylength * sizeof(T);
	}

	T& back()
	{
		CHECK_IF(empty(), "cannot return back of empty string");

		return _Myptr[_Mylength - 1];
	}

	const T& back() const
	{
		CHECK_IF(empty(), "cannot return back of empty string");

		return _Myptr[_Mylength - 1];
	}

	T& front()
	{
		CHECK_IF(empty(), "cannot return back of empty string");

		return *_Myptr;
	}

	const T& front() const
	{
		CHECK_IF(empty(), "cannot return back of empty string");

		return *_Myptr;
	}

	void clear()
	{
		_Alloc(0);
		*_Myptr = (T)0;
	}

	iterator begin()
	{
		return _Myptr;
	}

	const_iterator begin() const
	{
		return _Myptr;
	}

	iterator end()
	{
		return _Myptr + _Mylength;
	}

	const_iterator end() const
	{
		return _Myptr + _Mylength;
	}

	const_iterator cbegin() const
	{
		return begin();
	}

	const_iterator cend() const
	{
		return end();
	}

	size_t find(const T* str) const
	{
		CHECK_IF(str == nullptr, "invalid nullptr value");
		if(str[0] == 0)
		{
			return 0;
		}

		size_t strLength = ::std::char_traits<T>::length(str);
		const_iterator beginIter = begin();
		const_iterator endIter = end() - strLength;

		while(beginIter <= endIter)
		{
			for(size_t i = 0; i < strLength; ++i)
			{
				if(beginIter[i] != str[i])
				{
					goto Fail;
				}
			}

			return beginIter - begin();

		Fail:
			++beginIter;
		}

		return npos;
	}

	size_t find(const T& character) const
	{
		const_iterator beginIter = begin();
		const_iterator endIter = end();

		while(beginIter != endIter)
		{
			if(*beginIter == character)
			{
				return beginIter - begin();
			}
			++beginIter;
		}

		return npos;
	}

	size_t find_last_of(const T& character) const
	{
		const_iterator beginIter = begin() - 1;
		const_iterator endIter = end() - 1;

		while(endIter != beginIter)
		{
			if(*endIter == character)
			{
				return endIter - begin();
			}
			--endIter;
		}

		return npos;
	}

	string& append(const string& other)
	{
		CHECK_IF(this == &other, "cannot append to itself");

		size_t prevSize = size();
		size_t otherSize = other.size();
		_Alloc(prevSize + otherSize);
		::std::char_traits<T>::copy(_Myptr + prevSize, other.data(), otherSize);

		return *this;
	}

	template<typename Traits, typename Alloc>
	string& append(const ::std::basic_string<T, Traits, Alloc>& other)
	{
		size_t prevSize = size();
		size_t otherSize = other.size();
		_Alloc(prevSize + otherSize);
		::std::char_traits<T>::copy(_Myptr + prevSize, other.data(), otherSize);

		return *this;
	}

	string& append(const T* cStr)
	{
		CHECK_IF(cStr == nullptr, "invalid nullptr value");
		size_t prevSize = size();
		size_t otherSize = ::std::char_traits<T>::length(cStr);
		_Alloc(prevSize + otherSize);
		::std::char_traits<T>::copy(_Myptr + prevSize, cStr, otherSize);

		return *this;
	}

	void push_back(const T& value)
	{
		_Alloc(_Mylength + 1);
		_Myptr[_Mylength - 1] = value;
	}

	void pop_back()
	{
		_Alloc(_Mylength - 1);
	}

	iterator insert(iterator positionIter, const T& value)
	{
		CHECK_ITER_RANGE(positionIter, begin(), end(), "invalid argument: positionIter");

		const size_t offset = positionIter - begin();
		_Alloc(_Mylength + 1);

		::std::char_traits<T>::move(_Myptr + offset + 1, _Myptr + offset, _Mylength - offset - 1);

		_Myptr[offset] = value;

		return _Myptr + offset;
	}

	size_t insert(size_t posOffset, const T* str)
	{
		CHECK_IF(str == nullptr, "invalid nullptr value");
		CHECK_ARR_RANGE(posOffset, size() + 1, "invalid argument: positionIter");
		CHECK_IF(str == nullptr, "invalid argument: str");
		CHECK_IF(str >= begin() && str < end(), "invalid argument str");

		const size_t strLength = std::char_traits<T>::length(str);

		if(strLength == 0)
		{
			return posOffset;
		}

		_Alloc(_Mylength + strLength);

		::std::char_traits<T>::move(
			_Myptr + posOffset + strLength, _Myptr + posOffset, _Mylength - posOffset - strLength);

		::std::char_traits<T>::copy(_Myptr + posOffset, str, strLength);

		return posOffset;
	}

	iterator insert(iterator positionIter, const T* str)
	{
		CHECK_IF(str == nullptr, "invalid nullptr value");
		CHECK_ITER_RANGE(positionIter, begin(), end(), "invalid argument: positionIter");
		CHECK_IF(str >= begin() && str < end(), "invalid argument str");

		const size_t strLength = std::char_traits<T>::length(str);

		if(strLength == 0)
		{
			return positionIter;
		}

		const size_t offset = positionIter - begin();
		_Alloc(_Mylength + strLength);

		memmove(_Myptr + offset + strLength, _Myptr + offset, _Mylength - offset - strLength);

		memcpy(_Myptr + offset, str, strLength);

		return _Myptr + offset;
	}

	iterator insert(iterator positionIter, const ::std::basic_string<T>& str)
	{
		CHECK_ITER_RANGE(positionIter, begin(), end(), "invalid argument: positionIter");

		if(str.empty())
		{
			return positionIter;
		}

		const size_t strLength = str.length();

		const size_t offset = positionIter - begin();
		_Alloc(_Mylength + strLength);

		memmove(_Myptr + offset + strLength, _Myptr + offset, _Mylength - offset - strLength);

		memcpy(_Myptr + offset, str.data(), strLength);

		return _Myptr + offset;
	}

	void replace(
		const T& compareValue, const T& replaceValue, size_t offset = 0, size_t range = npos)
	{
		const const_iterator endIter = begin() + (::std::min)(offset + range, _Mylength);

		iterator beginIter = begin();

		while(beginIter != endIter)
		{
			*beginIter = ((*beginIter == compareValue) ? replaceValue : *beginIter);
			++beginIter;
		}
	}

	void replace(
		const T& compareValue, const T* replaceString, size_t offset = 0, size_t range = npos)
	{
		CHECK_IF(replaceString == nullptr, "invalid nullptr value");
		if(offset >= _Mylength)
		{
			return;
		}

		const const_iterator endIter = begin() + (::std::min)(offset + range, _Mylength);

		iterator beginIter = begin() + offset;

		size_t strLength = ::std::char_traits<T>::length(replaceString);

		switch(strLength)
		{
		case 0:
			_Replace_empty(beginIter, endIter, compareValue);
			break;
		case 1:
			replace(compareValue, *replaceString, offset, range);
			break;
		default: // bigger than
			_Replace_bigger(beginIter, endIter, compareValue, replaceString, strLength);
			break;
		}
	}

	string& erase(size_t pos = 0, size_t len = npos)
	{
		size_t maxLength = (pos + len < size()) ? pos + len : size();
		erase(begin() + pos, begin() + maxLength);
		return *this;
	}

	iterator erase(iterator eraseIter)
	{
		CHECK_ITER_RANGE(eraseIter, begin(), end(), "iterator out of range");

		size_t beginOffset = eraseIter - begin();

		std::char_traits<T>::copy(eraseIter, eraseIter + 1, (end() - (eraseIter + 1)));
		_Alloc(size() - 1);

		return begin() + beginOffset;
	}

	iterator erase(iterator beginIter, iterator endIter)
	{
		CHECK_ITER_RANGE(beginIter, begin(), end(), "iterator out of range");
		CHECK_ITER_RANGE(endIter, begin(), end(), "iterator out of range");
		CHECK_IF(beginIter > endIter, "iterator out of range");

		if(beginIter == endIter)
		{
			return beginIter;
		}

		size_t beginOffset = beginIter - begin();

		std::char_traits<T>::copy(beginIter, endIter, (end() - endIter));
		_Alloc(size() - (endIter - beginIter));

		return begin() + beginOffset;
	}

	string& operator=(const string& other)
	{
		_Set(other);
		return *this;
	}

	string& operator=(string&& other)
	{
		_Set(::std::move(other));
		return *this;
	}

	string& operator=(const T* other)
	{
		CHECK_IF(other == nullptr, "invalid nullptr value");
		_Set(other);
		return *this;
	}

	inline friend bool operator<(const string& left, const string& right)
	{
		if(left.size() == right.size())
		{
			return ::std::char_traits<T>::compare(left.data(), right.data(), left.size()) < 0;
		}
		return left.size() < right.size();
	}

	inline friend bool operator==(const string& left, const string& right)
	{
		if(left.size() == right.size())
		{
			return ::std::char_traits<T>::compare(left.data(), right.data(), left.size()) == 0;
		}

		return false;
	}

	inline friend bool operator==(const string& left, const T* right)
	{
		CHECK_IF(right == nullptr, "invalid string pointer");

		size_t rightSize = ::std::char_traits<T>::length(right);
		if(left.size() == rightSize)
		{
			return ::std::char_traits<T>::compare(left.data(), right, left.size()) == 0;
		}

		return false;
	}

	inline friend bool operator==(const T* left, const string& right)
	{
		CHECK_IF(left == nullptr, "invalid nullptr value");
		return (right == left);
	}

	inline friend bool operator!=(const string& left, const string& right)
	{
		if(left.size() != right.size())
		{
			return true;
		}

		return ::std::char_traits<T>::compare(left.data(), right.data(), left.size()) != 0;
	}

	inline friend bool operator!=(const string& left, const T* right)
	{
		CHECK_IF(right == nullptr, "invalid string pointer");

		size_t rightSize = ::std::char_traits<T>::length(right);
		if(left.size() != rightSize)
		{
			return true;
		}

		return ::std::char_traits<T>::compare(left.data(), right, rightSize) != 0;
	}

	inline friend bool operator!=(const T* left, const string& right)
	{
		CHECK_IF(left == nullptr, "invalid nullptr value");
		return (right != left);
	}

	inline friend string operator+(const string& left, const string& right)
	{
		return string(left, right);
	}

	inline friend string operator+(const string& left, const T* right)
	{
		CHECK_IF(right == nullptr, "invalid nullptr value");
		return string(left, right);
	}

	inline friend string operator+(const T* left, const string& right)
	{
		CHECK_IF(left == nullptr, "invalid nullptr value");
		return string(left, right);
	}

	template<typename Traits, typename Alloc>
	inline friend string operator+(
		const string& left, const ::std::basic_string<T, Traits, Alloc>& right)
	{
		return string(left, right);
	}

	template<typename Traits, typename Alloc>
	inline friend string operator+(
		const ::std::basic_string<T, Traits, Alloc>& left, const string& right)
	{
		return string(left, right);
	}

	string& operator+=(const T& character)
	{
		push_back(character);
		return *this;
	}

	string& operator+=(const string& other)
	{
		return append(other);
	}

	string& operator+=(const T* cStr)
	{
		return append(cStr);
	}

	T& operator[](size_t index)
	{
		CHECK_ARR_RANGE(index, size(), "index out of range");
		return _Myptr[index];
	}

	const T& operator[](size_t index) const
	{
		CHECK_ARR_RANGE(index, size(), "index out of range");
		return _Myptr[index];
	}

private:
	void _Set(const string& str)
	{
		_Set(str._Myptr, str._Mylength);
	}

	void _Set(string&& str)
	{
		swap(_Myptr, str._Myptr);
		_Mylength = str._Mylength;
	}

	void _Set(const T* str)
	{
		_Set(str, std::char_traits<T>::length(str));
	}

	void _Set(const T* str, size_t strLength)
	{

		_Alloc(strLength);

		_Mylength = strLength;

		::std::char_traits<T>::copy(_Myptr, str, strLength);
	}

	void _Alloc(size_t strLength)
	{
		if(_Mycapacity <= strLength)
		{
			_Myptr = ::mst::dll_safe::realloc_object(_Myptr, strLength + _Mycapacity + 1);
			_Mycapacity = strLength + _Mycapacity;
		}
		else if(_Mycapacity > strLength * 2)
		{
			_Myptr = ::mst::dll_safe::realloc_object(_Myptr, strLength + 1);
			_Mycapacity = strLength;
		}

		_Mylength = strLength;
		_Myptr[strLength] = (T)0;
	}

	void _Pre_alloc(size_t strLength)
	{
		if(_Mycapacity < strLength)
		{
			_Myptr = ::mst::dll_safe::realloc_object(_Myptr, strLength + _Mycapacity + 1);
			_Mycapacity = strLength + _Mycapacity;
		}

		_Mylength = strLength;
	}

	void _Release()
	{
		::mst::dll_safe::free_object(_Myptr);
	}

	void _Replace_bigger(iterator insertBegin, const_iterator insertEnd, const T& compareValue,
		const T* replaceString, size_t replaceStringLength)
	{
		size_t copyOffset = 0;
		size_t beginOffset = insertBegin - begin();
		size_t endOffset = insertEnd - begin();

		const size_t pushLength = replaceStringLength - 1;

		while(beginOffset != endOffset)
		{
			if(_Myptr[beginOffset] == compareValue)
			{
				_Pre_alloc(size() + pushLength);
				memmove(_Myptr + beginOffset + replaceStringLength, _Myptr + beginOffset + 1,
					_Mylength - beginOffset - pushLength);
				memcpy(_Myptr + beginOffset, replaceString, replaceStringLength);

				beginOffset += replaceStringLength;
				endOffset += replaceStringLength;
			}

			++beginOffset;
		}

		_Myptr[_Mylength] = (T)0;
	}

	void _Replace_smaller(iterator insertBegin, const_iterator insertEnd, const T* origString,
		size_t origStringLength, const T* replaceString, size_t replaceStringLength)
	{
		const size_t diffSize = replaceStringLength - 1;
	}

	void _Replace_empty(iterator insertBegin, const_iterator insertEnd, const T& compareValue)
	{
		iterator copyIter;

		while(insertBegin != insertEnd)
		{
			if(*insertBegin == compareValue)
			{
				copyIter = insertBegin++;
				while(insertBegin != insertEnd)
				{
					if(*insertBegin != compareValue)
					{
						*copyIter++ = *insertBegin;
					}

					++insertBegin;
				}
				_Alloc(size() - (insertEnd - copyIter));
				return;
			}
			++insertBegin;
		}
		// nothing happened no realloc needed
	}

	string(const string& str, size_t offset, size_t len)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		_Set(str.c_str() + offset, len);
	}

	string(const string& left, const string& right)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t leftSize = left.size();
		size_t rightSize = right.size();
		size_t initLength = leftSize + rightSize;
		_Alloc(initLength);
		::std::char_traits<T>::copy(_Myptr, left._Myptr, leftSize);
		::std::char_traits<T>::copy(_Myptr + leftSize, right._Myptr, rightSize);
	}

	template<typename Traits, typename Alloc>
	string(const string& left, const ::std::basic_string<T, Traits, Alloc>& right)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t leftSize = left.size();
		size_t rightSize = right.size();
		size_t initLength = leftSize + rightSize;
		_Alloc(initLength);
		if(_Myptr != nullptr)
		{
			::std::char_traits<T>::copy(_Myptr, left.data(), leftSize);
			::std::char_traits<T>::copy(_Myptr + leftSize, right.data(), rightSize);
		}
	}

	template<typename Traits, typename Alloc>
	string(const ::std::basic_string<T, Traits, Alloc>& left, const string& right)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t leftSize = left.size();
		size_t rightSize = right.size();
		size_t initLength = leftSize + rightSize;
		_Alloc(initLength);
		::std::char_traits<T>::copy(_Myptr, left.data(), leftSize);
		::std::char_traits<T>::copy(_Myptr + leftSize, right.data(), rightSize);
	}

	string(const string& left, const T* right)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t leftSize = left.size();
		size_t rightSize = ::std::char_traits<T>::length(right);
		size_t initLength = leftSize + rightSize;
		_Alloc(initLength);
		::std::char_traits<T>::copy(_Myptr, left._Myptr, leftSize);
		::std::char_traits<T>::copy(_Myptr + leftSize, right, rightSize);
	}

	string(const char* left, const string& right)
	{
		_Myptr = nullptr;
		_Mylength = 0;
		_Mycapacity = 0;

		size_t leftSize = ::std::char_traits<T>::length(left);
		size_t rightSize = right.size();
		size_t initLength = leftSize + rightSize;
		_Alloc(initLength);
		::std::char_traits<T>::copy(_Myptr, left, leftSize);
		::std::char_traits<T>::copy(_Myptr + leftSize, right._Myptr, rightSize);
	}

private:
	T* _Myptr;
	size_t _Mylength, _Mycapacity;

}; // class string

}; // namespace dll_safe
}; // namespace mst

namespace std {

template<typename T>
::std::basic_ostream<T>& operator<<(
	::std::basic_ostream<T>& ostr, const ::mst::dll_safe::string<T>& str)
{
	return ostr << str.c_str();
}

}; // namespace std


namespace mst {

inline const char* to_string_pointer(const ::mst::dll_safe::string<char>& _Val)
{
	return _Val.c_str();
}

inline const wchar_t* to_string_pointer(const ::mst::dll_safe::string<wchar_t>& _Val)
{
	return _Val.c_str();
}

#ifdef MCOMMON_H

template<typename T>
inline uint32_t hash32(const ::mst::dll_safe::string<T>& dllString)
{
	return ::mst::_Details::_Hash32(dllString.data(), dllString.length());
}

template<typename T>
inline uint64_t hash64(const ::mst::dll_safe::string<T>& dllString)
{
	return ::mst::_Details::_Hash64(dllString.data(), dllString.length());
}

#endif

}; // anmespace mst

#if defined(_MDLL_STRING_H) && defined(_MSTACK_STRING_H)
#include <mx_dll_string_stack_string.inl>
#endif