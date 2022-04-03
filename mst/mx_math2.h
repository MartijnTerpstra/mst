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

#ifndef _MST_MDET
#define _MST_MDET ::mst::math::_Details::
#endif

#if _MST_USING_VC_COMPILER
#define _MST_MATH_SVML_SUPPORT 1
#elif _MST_USING_GCC_COMPILER
#define _MST_MATH_SVML_SUPPORT 0
#elif _MST_USING_CLANG_COMPILER
#define _MST_MATH_SVML_SUPPORT 0
#else
#error "Compiler not supported"
#endif

namespace mst {
namespace math {

template<typename _Value_type, size_t _Elems>
class vector;

template<typename _Value_type, size_t _Columns, size_t _Rows>
class matrix;

template<typename _Value_type, size_t _Dimensions>
struct vector_type
{
	typedef vector<_Value_type, _Dimensions> type;
};

template<typename _Value_type>
struct vector_type<_Value_type, 1>
{
	typedef _Value_type type;
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
struct matrix_type
{
	typedef matrix<_Value_type, _Columns, _Rows> type;
};

template<typename _Value_type>
struct matrix_type<_Value_type, 1, 1>
{
	typedef _Value_type type;
};

namespace _Details {

template<typename _Value_type>
class _Math_base
{
protected:
	typedef ::mst::math::vector<_Value_type, 1> vector1_type;
	typedef ::mst::math::vector<_Value_type, 2> vector2_type;
	typedef ::mst::math::vector<_Value_type, 3> vector3_type;
	typedef ::mst::math::vector<_Value_type, 4> vector4_type;
};

template<typename _Ty, bool _Is_arithmetic = ::std::is_arithmetic<_Ty>::value>
struct _Get_value_type
{
	typedef typename _Ty::value_type type;
};

template<typename _Ty>
struct _Get_value_type<_Ty, true>
{
	typedef _Ty type;
};

} // namespace _Details

} // namespace math
} // namespace mst