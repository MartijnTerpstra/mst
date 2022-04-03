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

#include <mmath_algorithm.h>

namespace mst {
namespace math {
namespace _Details {

template<typename T>
constexpr T lerp_impl(
	const T& u, const T& v, typename _Cref_value<T>::type s, _Scalar_type) noexcept
{
	return (u * (ConvertTo<T>(1) - s)) + (v * s);
}

template<typename T>
_MST_CONSTEXPR17 T lerp_impl(
	const T& u, const T& v, typename _Cref_value<T>::type s, _Math_type) noexcept
{
	T retval;

	auto uIt = begin(u);
	auto vIt = begin(v);
	auto outIt = begin(retval);
	auto endIt = end(u);

	for(; uIt != endIt; ++uIt, ++vIt, ++outIt)
	{
		*outIt = lerp_impl(
			*uIt, *vIt, s, typename _Math_traits<std::decay_t<decltype(*uIt)>>::math_type{});
	}

	return retval;
}

template<typename Ty>
constexpr Ty _Reflect(const Ty& i, const Ty& n, _Math_type) noexcept
{
	return i - Ty(2.0) * n * n.dot(i);
}

template<typename T>
constexpr T _Saturate(const T& val, _Scalar_type) noexcept
{
	return (val > (T)0.0 ? (val < (T)1.0 ? val : (T)1.0) : (T)0.0);
}

template<typename T>
_MST_CONSTEXPR17 T _Saturate(const T& val, _Math_type) noexcept
{
	T retval;

	auto inIt = begin(val);
	auto outIt = begin(retval);
	auto endIt = end(val);

	while(inIt != endIt)
	{
		*outIt++ = _Saturate(
			*inIt++, typename _Details::_Math_traits<std::decay_t<decltype(*inIt)>>::math_type{});
	}

	return retval;
}

template<typename T>
_MST_CONSTEXPR17 T _Smoothstep(
	const T& minim, const T& maxim, typename _Cref_value<T>::type val, _Math_type) noexcept
{
	T t = ::mst::math::saturate((T(val) - minim) / (maxim - minim));
	return t * t *
		   (_MST_MDET _Cvalue<T>::create_object(3.0) - (_Cvalue<T>::create_object(2.0) * t));
}

template<typename T>
_MST_CONSTEXPR17 T _Smoothstep(
	const T& minim, const T& maxim, typename _Cref_value<T>::type val, _Scalar_type) noexcept
{
	T t = ::mst::math::saturate((val - minim) / (maxim - minim));
	return t * t * (T(3.0) - (T(2.0) * t));
}

template<typename ForwardIterator>
_MST_CONSTEXPR17 auto _Average(ForwardIterator begin, ForwardIterator end) noexcept
	-> std::decay_t<decltype(*begin)>
{
	typedef std::decay_t<decltype(*begin)> ValueType;

	MST_ASSERT(begin != end, "An empty sequence cannot have an average");

	auto retval = *begin++;
	int32_t count = 1;
	while(begin != end)
	{
		retval += *begin++;
		++count;
	}

	return retval / ConvertTo<ValueType>(count);
}

template<typename T>
_MST_CONSTEXPR17 T _Frac(const T& val, _MST_MDET _Math_type) noexcept
{
	static_assert(::std::is_floating_point<typename _MST_MDET _Math_traits<T>::value_type>::value,
		"frac() cannot be used with integer types");

	T retval;

	auto inIt = begin(val);
	auto outIt = begin(retval);
	auto endIt = end(val);

	while(inIt != endIt)
	{
		*outIt++ = _Frac(
			*inIt++, typename _Details::_Math_traits<std::decay_t<decltype(*inIt)>>::math_type{});
	}

	return retval;
}

template<typename T>
_MST_CONSTEXPR17 T _Frac(const T& val, _MST_MDET _Scalar_type) noexcept
{
	static_assert(::std::is_floating_point<T>::value,
		"::mst::math::frac() can only be used with floating point types");
	return val - floor(val);
}

template<typename T>
constexpr T _Clamp(const T& val, const T& minim, const T& maxim, _MST_MDET _Scalar_type) noexcept
{
	return (minim > val ? minim : (maxim < val ? maxim : val));
}

template<typename T>
_MST_CONSTEXPR17 T _Clamp(const T& val, typename _MST_MDET _Cref_value<T>::type minim,
	typename _MST_MDET _Cref_value<T>::type maxim, _MST_MDET _Math_type) noexcept
{
	T retval;
	for(size_t i = 0; i < T::dimensions; ++i)
	{
		retval.data()[i] = _Clamp(val.data()[i], minim, maxim, _MST_MDET _Scalar_type());

		// retval.data()[i] = (minim > val.data()[i] ? minim : (maxim < val.data()[i] ? maxim :
		// val[i]));
	}

	return retval;
}

template<typename T>
_MST_CONSTEXPR17 T _Clamp(
	const T& val, const T& minim, const T& maxim, _MST_MDET _Math_type) noexcept
{
	T retval;
	for(size_t i = 0; i < T::dimensions; ++i)
	{
		// retval.data()[i] = (minim.data()[i] > val.data()[i] ? minim.data()[i] : (maxim.data()[i]
		// < val.data()[i] ? maxim.data()[i] : val.data()[i]));
		retval.data()[i] =
			_Clamp(val.data()[i], minim.data()[i], maxim.data()[i], _MST_MDET _Scalar_type());
	}

	return retval;
}

} // namespace _Details
} // namespace math
} // namespace mst

template<typename T>
constexpr T mst::math::lerp(
	const T& u, const T& v, typename _MST_MDET _Cref_value<T>::type s) noexcept
{
	return _Details::lerp_impl(u, v, s, typename _Details::_Math_traits<T>::math_type());
}

template<typename T>
constexpr T mst::math::lerp(const T& u, const T& v, const T& w,
	typename _MST_MDET _Cref_value<T>::type s, typename _MST_MDET _Cref_value<T>::type t) noexcept
{
	return lerp(lerp(u, v, s), w, t);
}

template<typename T>
constexpr T mst::math::cubic_interp(const T& pre_u, const T& u, const T& v, const T& post_v,
	typename _MST_MDET _Cref_value<T>::type s) noexcept
{
	return u + _MST_MDET _Cvalue<T>::create(0.5) * s *
				   (v - pre_u +
					   s * (_MST_MDET _Cvalue<T>::create(2.0) * pre_u -
							   _MST_MDET _Cvalue<T>::create(5.0) * u +
							   _MST_MDET _Cvalue<T>::create(4.0) * v - post_v +
							   s * (_MST_MDET _Cvalue<T>::create(3.0) * (u - v) + post_v - pre_u)));
}

template<typename T>
constexpr T mst::math::cubic_interp(
	const T (&uv)[4], typename _MST_MDET _Cref_value<T>::type s) noexcept
{
	return uv[1] + _MST_MDET _Cvalue<T>::create(0.5) * s *
					   (uv[2] - uv[0] +
						   s * (_MST_MDET _Cvalue<T>::create(2.0) * uv[0] -
								   _MST_MDET _Cvalue<T>::create(5.0) * uv[1] +
								   _MST_MDET _Cvalue<T>::create(4.0) * uv[2] - uv[3] +
								   s * (_MST_MDET _Cvalue<T>::create(3.0) * (uv[1] - uv[2]) +
										   uv[3] - uv[0])));
}

template<typename Ty>
constexpr Ty mst::math::reflect(const Ty& i, const Ty& n) noexcept
{
	return _Details::_Reflect(i, n, typename _Details::_Math_traits<Ty>::math_type{});
}

/* returns clamps all the values to the range: [0,1] */
template<typename _Ty>
constexpr _Ty mst::math::saturate(const _Ty& val) noexcept
{
	return _Details::_Saturate(val, typename _Details::_Math_traits<_Ty>::math_type{});
}

template<typename T>
_MST_CONSTEXPR17 T mst::math::smoothstep(
	const T& minim, const T& maxim, typename _MST_MDET _Cref_value<T>::type val) noexcept
{
	return _MST_MDET _Smoothstep(minim, maxim, val);
}

template<typename _Ty>
_MST_CONSTEXPR17 _Ty mst::math::frac(const _Ty& val) noexcept
{
	return _Details::_Frac(val, typename _Details::_Math_traits<_Ty>::math_type{});
}

template<typename T>
constexpr T mst::math::clamp(const T& val, typename _MST_MDET _Cref_value<T>::type minim,
	typename _MST_MDET _Cref_value<T>::type maxim) noexcept
{
	return _MST_MDET _Clamp(val, minim, maxim, typename _MST_MDET _Math_traits<T>::math_type());
}

template<typename T>
constexpr T mst::math::clamp(const T& val, const T& minim, const T& maxim) noexcept
{
	return _MST_MDET _Clamp(val, minim, maxim, typename _MST_MDET _Math_traits<T>::math_type());
}

template<typename ForwardIterator>
constexpr auto mst::math::average(ForwardIterator begin, ForwardIterator end) noexcept
	-> std::decay_t<decltype(*begin)>
{
	return _MST_MDET _Average(begin, end);
}

template<typename T>
_MST_CONSTEXPR17 T mst::math::greatest_common_devisor(const T& left, const T& right) noexcept
{
	T remaining;
	while(right != 0)
	{
		remaining = left % right;
		left = right;
		right = remaining;
	}
	return left;
}

constexpr bool mst::math::is_pow_2(uint8_t value) noexcept
{
	return value != 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(int8_t value) noexcept
{
	return value > 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(uint16_t value) noexcept
{
	return value != 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(int16_t value) noexcept
{
	return value > 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(uint32_t value) noexcept
{
	return value != 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(int32_t value) noexcept
{
	return value > 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(uint64_t value) noexcept
{
	return value != 0 && (value & (value - 1)) == 0;
}

constexpr bool mst::math::is_pow_2(int64_t value) noexcept
{
	return value > 0 && (value & (value - 1)) == 0;
}