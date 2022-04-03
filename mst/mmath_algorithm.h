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
#include <iterator>
#include <mx_math_algorithm.h>

namespace mst {
namespace math {

// math functions
template<typename T>
_MST_NODISCARD constexpr T lerp(
	const T& u, const T& v, typename _MST_MDET _Cref_value<T>::type s) noexcept;

template<typename T>
_MST_NODISCARD constexpr T lerp(const T& u, const T& v, const T& w,
	typename _MST_MDET _Cref_value<T>::type s, typename _MST_MDET _Cref_value<T>::type t) noexcept;

template<typename T>
_MST_NODISCARD constexpr T cubic_interp(const T& pre_u, const T& u, const T& v, const T& post_v,
	typename _MST_MDET _Cref_value<T>::type s) noexcept;

template<typename T>
_MST_NODISCARD constexpr T cubic_interp(
	const T (&uv)[4], typename _MST_MDET _Cref_value<T>::type s) noexcept;

template<typename T>
_MST_NODISCARD constexpr T reflect(const T& i, const T& n) noexcept;

/* returns clamps all the values to the range: [0,1] */
template<typename T>
_MST_NODISCARD constexpr T saturate(const T& val) noexcept;

template<typename T>
_MST_NODISCARD _MST_CONSTEXPR17 T smoothstep(
	const T& minim, const T& maxim, typename _MST_MDET _Cref_value<T>::type s) noexcept;

template<typename T>
_MST_NODISCARD _MST_CONSTEXPR17 T frac(const T& val) noexcept;

template<typename T>
_MST_NODISCARD constexpr T clamp(const T& val, typename _MST_MDET _Cref_value<T>::type minim,
	typename _MST_MDET _Cref_value<T>::type maxim) noexcept;

template<typename T>
_MST_NODISCARD constexpr T clamp(const T& val, const T& minim, const T& maxim) noexcept;

template<typename ForwardIterator>
_MST_NODISCARD constexpr auto average(ForwardIterator begin, ForwardIterator end) noexcept
	-> std::decay_t<decltype(*begin)>;

template<typename T>
_MST_NODISCARD _MST_CONSTEXPR17 T greatest_common_devisor(const T& left, const T& right) noexcept;

_MST_NODISCARD constexpr bool is_pow_2(int8_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(uint8_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(int16_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(uint16_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(int32_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(uint32_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(int64_t value) noexcept;
_MST_NODISCARD constexpr bool is_pow_2(uint64_t value) noexcept;

namespace math_functions {

using ::mst::math::lerp;
using ::mst::math::cubic_interp;
using ::mst::math::reflect;
using ::mst::math::saturate;
using ::mst::math::smoothstep;
using ::mst::math::frac;
using ::mst::math::clamp;
using ::mst::math::average;
using ::mst::math::greatest_common_devisor;

} // namespace math_functions

} // namespace math
} // namespace mst

#include <mx_math_algorithm.inl>