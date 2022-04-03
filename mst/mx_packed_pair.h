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

namespace mst {
namespace _Details {

template<typename T1, typename T2>
struct _Packed_pair : T1
{
	_Packed_pair(T1&& t1, T2&& t2)
		: T1(::std::move(t1))
		, _Mysecond(::std::move(t2))
	{ }

	_Packed_pair(const T1& t1, T2&& t2)
		: T1(t1)
		, _Mysecond(::std::move(t2))
	{ }

	_Packed_pair(T1&& t1, const T2& t2)
		: T1(::std::move(t1))
		, _Mysecond(t2)
	{ }

	_Packed_pair(const T1& t1, const T2& t2)
		: T1(t1)
		, _Mysecond(t2)
	{ }

	_Packed_pair(T1&& t1)
		: T1(::std::move(t1))
		, _Mysecond()
	{ }

	_Packed_pair(_Packed_pair&& other)
		: T1(::std::move(other._Get_first()))
		, _Mysecond(::std::move(other._Get_second()))
	{ }

	_Packed_pair()
		: T1()
		, _Mysecond()
	{ }

	T1& _Get_first()
	{
		return *this;
	}

	const T1& _Get_first() const
	{
		return *this;
	}

	T2& _Get_second()
	{
		return _Mysecond;
	}

	const T2& _Get_second() const
	{
		return _Mysecond;
	}

private:
	T2 _Mysecond;
};

} // namespace _Details
} // namespace mst