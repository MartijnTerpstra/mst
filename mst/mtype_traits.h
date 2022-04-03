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

#include <limits.h>
#include <type_traits>
#include <mx_pre_type_traits.h>
#include <cstddef>

namespace mst {

template<typename T>
struct numeric_traits
{ };

template<>
struct numeric_traits<char>
{
	typedef char value_type;
	typedef char& reference_type;
	static const char zero = 0;
	static const char max = CHAR_MAX;
	static const char min = CHAR_MIN;
	static const size_t bits = 8;
};

template<>
struct numeric_traits<signed char>
{
	typedef signed char value_type;
	typedef signed char& reference_type;
	static const signed char zero = (signed char)0;
	static const signed char max = SCHAR_MAX;
	static const signed char min = SCHAR_MIN;
	static const size_t bits = 8;
};

template<>
struct numeric_traits<unsigned char>
{
	typedef unsigned char value_type;
	typedef unsigned char& reference_type;
	static const unsigned char zero = (unsigned char)0;
	static const unsigned char max = UCHAR_MAX;
	static const unsigned char min = (unsigned char)0;
	static const size_t bits = 8;
};

template<>
struct numeric_traits<short>
{
	typedef short value_type;
	typedef short& reference_type;
	static const short zero = (short)0;
	static const short max = SHRT_MAX;
	static const short min = SHRT_MIN;
	static const size_t bits = 16;
};

template<>
struct numeric_traits<unsigned short>
{
	typedef unsigned short value_type;
	typedef unsigned short& reference_type;
	static const unsigned short zero = (unsigned short)0;
	static const unsigned short max = USHRT_MAX;
	static const unsigned short min = (unsigned short)0;
	static const size_t bits = 16;
};

template<>
struct numeric_traits<int>
{
	typedef int value_type;
	typedef int& reference_type;
	static const int zero = (int)0;
	static const int max = INT_MAX;
	static const int min = INT_MIN;
	static const size_t bits = 32;
};

template<>
struct numeric_traits<unsigned int>
{
	typedef unsigned int value_type;
	typedef unsigned int& reference_type;
	static const unsigned int zero = (unsigned int)0;
	static const unsigned int max = UINT_MAX;
	static const unsigned int min = (unsigned int)0;
	static const size_t bits = 32;
};

template<>
struct numeric_traits<long>
{
	typedef long value_type;
	typedef long& reference_type;
	static const long zero = 0L;
	static const long max = LONG_MAX;
	static const long min = LONG_MIN;
	static const size_t bits = 32;
};

template<>
struct numeric_traits<unsigned long>
{
	typedef unsigned long value_type;
	typedef unsigned long& reference_type;
	static const unsigned long zero = 0UL;
	static const unsigned long max = ULONG_MAX;
	static const unsigned long min = 0UL;
	static const size_t bits = 32;
};

template<>
struct numeric_traits<long long>
{
	typedef long long value_type;
	typedef long long& reference_type;
	static const long long zero = (long long)0;
	static const long long max = LLONG_MAX;
	static const long long min = LLONG_MIN;
	static const size_t bits = 64;
};

template<>
struct numeric_traits<unsigned long long>
{
	typedef unsigned long long value_type;
	typedef unsigned long long& reference_type;
	static const unsigned long long zero = (long long)0;
	static const unsigned long long max = ULLONG_MAX;
	static const unsigned long long min = (long long)0;
	static const size_t bits = 64;
};

#if _MST_HAS_TEMPLATE_AUTO

template<auto Value>
struct is_power_of_2
	: public std::integral_constant<bool, (Value > 0 && (Value & (Value - 1)) == 0)>::type
{ };

template<auto Value>
struct to_bit_shift : public std::integral_constant<int32_t, _MST_GET_SHIFT(Value)>::type
{ };

#if _MST_HAS_INLINE_VARIABLES

template<auto Value>
_MST_INLINE_VAR constexpr const bool is_power_of_2_v = is_power_of_2<Value>::value;

template<auto Value>
_MST_INLINE_VAR constexpr const int32_t to_bit_shift_v = to_bit_shift<Value>::value;

#endif // !_MST_HAS_INLINE_VARIABLES

#endif // !_MST_HAS_TEMPLATE_AUTO

template<typename T, T Value>
struct is_power_of_2_cpp11
	: public std::integral_constant<bool, (Value > 0 && (Value & (Value - 1)) == 0)>::type
{ };

template<typename T, T Value>
struct to_bit_shift_cpp11 : public std::integral_constant<int32_t, _MST_GET_SHIFT(Value)>::type
{ };

}

#include <mx_is_pointer_to_const.h>
#include <mx_pointer.h>
#include <mx_func_type_traits.h>