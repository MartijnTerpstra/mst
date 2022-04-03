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

#include <type_traits>
namespace mst {
namespace _Details {

template<typename T>
_MST_CONSTEXPR17 int32_t _Get_shift_value_cepr(T value)
{
	for(int32_t i = 0; i < sizeof(T) * 8; ++i)
	{
		if(value >> i == 1)
			return i;
	}

	return -1;
}

template<int64_t Value>
struct _Get_shift_value_signed
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 :
		(Value >> 30 == 1 ? 30 :
		(Value >> 31 == 1 ? 31 :
		(Value >> 32 == 1 ? 32 :
		(Value >> 33 == 1 ? 33 :
		(Value >> 34 == 1 ? 34 :
		(Value >> 35 == 1 ? 35 :
		(Value >> 36 == 1 ? 36 :
		(Value >> 37 == 1 ? 37 :
		(Value >> 38 == 1 ? 38 :
		(Value >> 39 == 1 ? 39 :
		(Value >> 40 == 1 ? 40 :
		(Value >> 41 == 1 ? 41 :
		(Value >> 42 == 1 ? 42 :
		(Value >> 43 == 1 ? 43 :
		(Value >> 44 == 1 ? 44 :
		(Value >> 45 == 1 ? 45 :
		(Value >> 46 == 1 ? 46 :
		(Value >> 47 == 1 ? 47 :
		(Value >> 48 == 1 ? 48 :
		(Value >> 49 == 1 ? 49 :
		(Value >> 50 == 1 ? 50 :
		(Value >> 51 == 1 ? 51 :
		(Value >> 52 == 1 ? 52 : 
		(Value >> 53 == 1 ? 53 : 
		(Value >> 54 == 1 ? 54 : 
		(Value >> 55 == 1 ? 55 : 
		(Value >> 56 == 1 ? 56 :
		(Value >> 57 == 1 ? 57 :
		(Value >> 58 == 1 ? 58 :
		(Value >> 59 == 1 ? 59 :
		(Value >> 60 == 1 ? 60 :
		(Value >> 61 == 1 ? 61 :
		(Value >> 62 == 1 ? 62 : 63)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
	// clang-format on
};

template<uint64_t Value>
struct _Get_shift_value_unsigned
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 :
		(Value >> 30 == 1 ? 30 :
		(Value >> 31 == 1 ? 31 :
		(Value >> 32 == 1 ? 32 :
		(Value >> 33 == 1 ? 33 :
		(Value >> 34 == 1 ? 34 :
		(Value >> 35 == 1 ? 35 :
		(Value >> 36 == 1 ? 36 :
		(Value >> 37 == 1 ? 37 :
		(Value >> 38 == 1 ? 38 :
		(Value >> 39 == 1 ? 39 :
		(Value >> 40 == 1 ? 40 :
		(Value >> 41 == 1 ? 41 :
		(Value >> 42 == 1 ? 42 :
		(Value >> 43 == 1 ? 43 :
		(Value >> 44 == 1 ? 44 :
		(Value >> 45 == 1 ? 45 :
		(Value >> 46 == 1 ? 46 :
		(Value >> 47 == 1 ? 47 :
		(Value >> 48 == 1 ? 48 :
		(Value >> 49 == 1 ? 49 :
		(Value >> 50 == 1 ? 50 :
		(Value >> 51 == 1 ? 51 :
		(Value >> 52 == 1 ? 52 : 
		(Value >> 53 == 1 ? 53 : 
		(Value >> 54 == 1 ? 54 : 
		(Value >> 55 == 1 ? 55 : 
		(Value >> 56 == 1 ? 56 :
		(Value >> 57 == 1 ? 57 :
		(Value >> 58 == 1 ? 58 :
		(Value >> 59 == 1 ? 59 :
		(Value >> 60 == 1 ? 60 :
		(Value >> 61 == 1 ? 61 :
		(Value >> 62 == 1 ? 62 : 63)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
	// clang-format on
};

#define _MST_GET_SHIFT(x)                                                                          \
	(std::conditional_t<std::is_signed_v<decltype(x)>,                                             \
		::mst::_Details::_Get_shift_value_signed<(int64_t)x>,                                      \
		::mst::_Details::_Get_shift_value_unsigned<(uint64_t)x>>::value)

} // namespace _Details
} // namespace mst