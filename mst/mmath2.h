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

#ifndef MMATH2_H
#define MMATH2_H

#ifdef MMATH_H
#error "either include mmath.h or mmath2.h"
#endif

#include <mcore.h>
#include <mdebug.h>
#include <cmath>
#include <mx_math2.h>

#if MST_MATH_NO_SIMD
#define _MST_MATH_SIMD_ENABLED 0
#elif MST_NO_SIMD
#define _MST_MATH_SIMD_ENABLED 0
#elif _MST_HAS_ARM
#define _MST_MATH_SIMD_ENABLED 0
#else
#define _MST_MATH_SIMD_ENABLED 1

#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#endif // MST_MATH_NO_SIMD

#if MST_MATH_ALL_VECTORS_MATRICES_SIMD
#define _MST_ALL_VECTORS_SIMD  1
#define _MST_ALL_MATRICES_SIMD 1
#endif // MST_MATH_ALL_VECTORS_MATRICES_SIMD

#define NOMINMAX 1
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace mst {
namespace math {

template<typename _Value_type, size_t _Elems>
class vector;

template<typename _Value_type, size_t _Columns, size_t _Rows>
class matrix;

template<typename _Value_type>
class quaternion;

template<typename _Value_type, size_t _Dimensions>
class aabb;

template<typename _Value_type, size_t _Dimensions>
class ray;

template<typename T>
struct get_value_type : ::mst::math::_Details::_Get_value_type<T>
{ };

template<typename _Value_type>
class degrees;

template<typename _Value_type>
class radians;

#if _MST_HAS_TEMPLATE_ALIASES
template<class T>
using get_value_type_t = typename get_value_type<T>::type;
#endif

} // namespace math
} // namespace mst

#include <mmath_algorithm.h>

namespace mst {
namespace math {

/* Depricated, use constexpr versions
#ifndef PI
#define PI		(3.141592653589793238462f)
#endif
*/

template<typename _Value_type>
inline constexpr _Value_type pi()
{
	return (
		_Value_type)3.141592653589793238462643383279502884197169399375105820974944592307816406286L;
}

inline constexpr float pi_f()
{
	return pi<float>();
}

inline constexpr double pi_d()
{
	return pi<double>();
}

inline constexpr long double pi_ld()
{
	return pi<long double>();
}

template<typename T>
T epsilon()
{
	static_assert(::std::is_floating_point<T>::value, "T must be a floating point type");
	return (T)_MST_EPSILON;
}

} // namespace math
} // namespace mst

#include <mx_math_angle.h>
#include <mx_math_vector.h>
#include <mx_math_quaternion.h>
#include <mx_math_matrix.h>
#include <mx_math_aabb.h>
#include <mx_math_ray.h>

#include <mx_math_vector.inl>
#include <mx_math_quaternion.inl>
#include <mx_math_matrix.inl>

#if _MST_MATH_SIMD_ENABLED

#include <mx_math_vector_simd.inl>
#include <mx_math_matrix_simd.inl>

#endif

#endif