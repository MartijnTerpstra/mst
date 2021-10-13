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
#include <initializer_list>

namespace mst {

template<typename T>
class array_view
{
public:
	inline array_view()
		: _Mybegin(nullptr)
		, _Mysize(0)
	{ }

	inline array_view(const array_view& other) = default;

	template<typename T2>
	inline array_view(const T2* ptr, size_t count)
		: _Mybegin(ptr)
		, _Mysize(count)
	{
		static_assert(std::is_same<T2, typename std::remove_const<T>::type>::value,
			"pointer must point to the actional type, to prevent base class shenanigans");
	}

	template<typename T2,
		typename std::enable_if<
			std::is_convertible<T2*, typename std::remove_const<T>::type*>::value, int>::type = 0>
	inline array_view(const T2& value)
		: _Mybegin(&value)
		, _Mysize(1)
	{ }

	template<size_t _Arrsize>
	inline array_view(const T (&_Arr)[_Arrsize])
		: _Mybegin(std::addressof(_Arr[0]))
		, _Mysize(_Arrsize)
	{ }

	template<typename _Container,
		typename std::enable_if<
			!std::is_convertible<_Container*, typename std::remove_const<T>::type*>::value,
			int>::type = 0>
	inline array_view(const _Container& _Cont)
		: _Mybegin(_Cont.data())
		, _Mysize(_Cont.size())
	{ }

	inline array_view(std::initializer_list<T> _Initlist)
		: _Mybegin(_Initlist.begin())
		, _Mysize(_Initlist.size())
	{ }

	[[nodiscard]] inline size_t size() const
	{
		return _Mysize;
	}

	[[nodiscard]] inline bool empty() const
	{
		return _Mysize == 0;
	}

	[[nodiscard]] inline const T& operator[](size_t index) const
	{
		MST_ASSERT(index < _Mysize, "index out of range");
		return _Mybegin[index];
	}

	[[nodiscard]] inline const T* begin() const
	{
		return _Mybegin;
	}

	[[nodiscard]] inline const T* end() const
	{
		return _Mybegin + _Mysize;
	}

	[[nodiscard]] inline const T* data() const
	{
		return _Mybegin;
	}

	[[nodiscard]] inline const T* cbegin() const
	{
		return _Mybegin;
	}

	[[nodiscard]] inline const T* cend() const
	{
		return _Mybegin + _Mysize;
	}

private:
	// Non const to enable copy assignment, which is required by some containers

	const T* _Mybegin;
	size_t _Mysize;

}; // class array_view

} // namespace mst
