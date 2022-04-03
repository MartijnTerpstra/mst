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
#include <mdebug.h>

namespace std {

template<typename _Ty, size_t _MaxSize>
class staticmap
{
public:
	typedef _Ty* pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty* iterator;
	typedef const _Ty* const_iterator;

	staticmap()
	{
		_MyEnd = _MyBegin;
	}

	inline iterator begin()
	{
		return _MyBegin;
	}

	inline const_iterator begin() const
	{
		return _MyBegin;
	}

	inline const_iterator cbegin() const
	{
		return _MyBegin;
	}

	inline iterator end()
	{
		return _MyEnd;
	}

	inline const_iterator end() const
	{
		return _MyEnd;
	}

	inline const_iterator cend() const
	{
		return _MyEnd;
	}

	inline bool empty() const
	{
		return _MyEnd == _MyBegin;
	}

	inline void pop_back()
	{
		_CHECK_IF(empty(), "cannot call pop_back on empty staticmap");
		_MyEnd--;
	}

	inline void push_back(const _Ty& _Value)
	{
		_CHECK_IF(size() == _MaxSize, "cannot call push_back on full staticmap");
		memcpy(_MyEnd, _Value, sizeof(_Ty));
		_MyEnd++;
	}

	inline size_t size() const
	{
		return _MyEnd - _MyBegin;
	}

	inline reference operator[](size_t _Idx)
	{
		_CHECK_ARR_RANGE(_Idx, size(), "staticmap index out of range");
		return _Ty[_Idx];
	}

	inline const_reference operator[](size_t _Idx) const
	{
		_CHECK_ARR_RANGE(_Idx, size(), "staticmap index out of range");
		return _Ty[_Idx];
	}

	_Ty _MyBegin[_MaxSize];
	pointer _MyEnd;
};

};