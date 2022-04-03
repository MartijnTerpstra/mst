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

#if !_MST_MATH_SIMD_ENABLED

#error "Should only be included when simd is enabled"

#else

namespace mst {
namespace math {

//////////////////////////////////
// vector<double, 2>            //
//////////////////////////////////

template<>
inline vector<double, 2>& operator+=(
	vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	_Left._Simd = _mm_add_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 2>& operator-=(
	vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	_Left._Simd = _mm_sub_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 2>& operator*=(
	vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	_Left._Simd = _mm_mul_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 2>& operator/=(
	vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	_Left._Simd = _mm_div_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 2>& operator*=(vector<double, 2>& _Left, double _Right) noexcept
{
	_Left._Simd = _mm_mul_pd(_Left._Simd, _mm_set1_pd(_Right));
	return _Left;
}

template<>
inline vector<double, 2>& operator/=(vector<double, 2>& _Left, double _Right) noexcept
{
	_Right = 1 / _Right;
	_Left._Simd = _mm_mul_pd(_Left._Simd, _mm_set1_pd(_Right));
	return _Left;
}

template<>
inline vector<double, 2> operator+(
	const vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	vector<double, 2> retval;
	retval._Simd = _mm_add_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 2> operator-(
	const vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	vector<double, 2> retval;
	retval._Simd = _mm_sub_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 2> operator*(
	const vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	vector<double, 2> retval;
	retval._Simd = _mm_mul_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 2> operator/(
	const vector<double, 2>& _Left, const vector<double, 2>& _Right) noexcept
{
	vector<double, 2> retval;
	retval._Simd = _mm_div_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 2> operator*(const vector<double, 2>& _Left, double _Right) noexcept
{
	vector<double, 2> retval;
	retval._Simd = _mm_mul_pd(_Left._Simd, _mm_set1_pd(_Right));
	return retval;
}

template<>
inline vector<double, 2> operator/(const vector<double, 2>& _Left, double _Right) noexcept
{
	_Right = 1 / _Right;

	vector<double, 2> retval;
	retval._Simd = _mm_mul_pd(_Left._Simd, _mm_set1_pd(_Right));
	return retval;
}

//////////////////////////////////
// vector<int64_t, 2>            //
//////////////////////////////////

template<>
inline vector<int64_t, 2>& operator+=(
	vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	_Left._Simd = _mm_add_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 2>& operator-=(
	vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	_Left._Simd = _mm_sub_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 2>& operator*=(
	vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	_Left._Simd = _mm_mullo_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 2>& operator*=(vector<int64_t, 2>& _Left, int64_t _Right) noexcept
{
	_Left._Simd = _mm_mullo_epi64(_Left._Simd, _mm_set1_epi64x(_Right));
	return _Left;
}

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int64_t, 2>& operator/=(
	vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	_Left._Simd = _mm_div_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 2>& operator/=(vector<int64_t, 2>& _Left, int64_t _Right) noexcept
{
	_Left._Simd = _mm_div_epi64(_Left._Simd, _mm_set1_epi64x(_Right));
	return _Left;
}
#endif // _MST_MATH_SVML_SUPPORT

template<>
inline vector<int64_t, 2> operator+(
	const vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_add_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 2> operator-(
	const vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_sub_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 2> operator*(
	const vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_mullo_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 2> operator*(const vector<int64_t, 2>& _Left, int64_t _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_mullo_epi64(_Left._Simd, _mm_set1_epi64x(_Right));
	return retval;
}

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int64_t, 2> operator/(
	const vector<int64_t, 2>& _Left, const vector<int64_t, 2>& _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_div_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 2> operator/(const vector<int64_t, 2>& _Left, int64_t _Right) noexcept
{
	vector<int64_t, 2> retval;
	retval._Simd = _mm_div_epi64(_Left._Simd, _mm_set1_epi64x(_Right));
	return retval;
}
#endif // _MST_MATH_SVML_SUPPORT

//////////////////////////////////
// vector<float, 4>             //
//////////////////////////////////

template<>
inline vector<float, 4>& operator+=(
	vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	_Left._Simd = _mm_add_ps(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<float, 4>& operator-=(
	vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	_Left._Simd = _mm_sub_ps(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<float, 4>& operator*=(
	vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	_Left._Simd = _mm_mul_ps(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<float, 4>& operator/=(
	vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	_Left._Simd = _mm_div_ps(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<float, 4>& operator*=(vector<float, 4>& _Left, float _Right) noexcept
{
	_Left._Simd = _mm_mul_ps(_Left._Simd, _mm_broadcast_ss(&_Right));
	return _Left;
}

template<>
inline vector<float, 4>& operator/=(vector<float, 4>& _Left, float _Right) noexcept
{
	_Right = 1 / _Right;
	_Left._Simd = _mm_mul_ps(_Left._Simd, _mm_broadcast_ss(&_Right));
	return _Left;
}

template<>
inline vector<float, 4> operator+(
	const vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	vector<float, 4> retval;
	retval._Simd = _mm_add_ps(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<float, 4> operator-(
	const vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	vector<float, 4> retval;
	retval._Simd = _mm_sub_ps(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<float, 4> operator*(
	const vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	vector<float, 4> retval;
	retval._Simd = _mm_mul_ps(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<float, 4> operator/(
	const vector<float, 4>& _Left, const vector<float, 4>& _Right) noexcept
{
	vector<float, 4> retval;
	retval._Simd = _mm_div_ps(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<float, 4> operator*(const vector<float, 4>& _Left, float _Right) noexcept
{
	vector<float, 4> retval;
	retval._Simd = _mm_mul_ps(_Left._Simd, _mm_broadcast_ss(&_Right));
	return retval;
}

template<>
inline vector<float, 4> operator/(const vector<float, 4>& _Left, float _Right) noexcept
{
	_Right = 1 / _Right;

	vector<float, 4> retval;
	retval._Simd = _mm_mul_ps(_Left._Simd, _mm_broadcast_ss(&_Right));
	return retval;
}

//////////////////////////////////
// vector<int32_t, 4>             //
//////////////////////////////////

template<>
inline vector<int32_t, 4>& operator+=(
	vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm_add_epi32(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int32_t, 4>& operator-=(
	vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm_sub_epi32(_Left._Simd, _Right._Simd);
	return _Left;
}

#if _MST_HAS_AVX

template<>
inline vector<int32_t, 4>& operator*=(
	vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm_mullo_epi32(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int32_t, 4>& operator*=(vector<int32_t, 4>& _Left, int32_t _Right) noexcept
{
	_Left._Simd = _mm_mullo_epi32(_Left._Simd, _mm_set1_epi32(_Right));
	return _Left;
}

#endif

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int32_t, 4>& operator/=(
	vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm_div_epi32(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int32_t, 4>& operator/=(vector<int32_t, 4>& _Left, int32_t _Right) noexcept
{
	_Left._Simd = _mm_div_epi32(_Left._Simd, _mm_set1_epi32(_Right));
	return _Left;
}
#endif // _MST_MATH_SVML_SUPPORT

template<>
inline vector<int32_t, 4> operator+(
	const vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_add_epi32(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int32_t, 4> operator-(
	const vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_sub_epi32(_Left._Simd, _Right._Simd);
	return retval;
}

#if _MST_HAS_AVX

template<>
inline vector<int32_t, 4> operator*(
	const vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_mullo_epi32(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int32_t, 4> operator*(const vector<int32_t, 4>& _Left, int32_t _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_mullo_epi32(_Left._Simd, _mm_set1_epi32(_Right));
	return retval;
}

#endif

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int32_t, 4> operator/(
	const vector<int32_t, 4>& _Left, const vector<int32_t, 4>& _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_div_epi32(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int32_t, 4> operator/(const vector<int32_t, 4>& _Left, int32_t _Right) noexcept
{
	vector<int32_t, 4> retval;
	retval._Simd = _mm_div_epi32(_Left._Simd, _mm_set1_epi32(_Right));
	return retval;
}
#endif // _MST_MATH_SVML_SUPPORT

#if _MST_HAS_AVX2

//////////////////////////////////
// vector<double, 4>            //
//////////////////////////////////

template<>
inline vector<double, 4>& operator+=(
	vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_add_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 4>& operator-=(
	vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_sub_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 4>& operator*=(
	vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_mul_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 4>& operator/=(
	vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_div_pd(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<double, 4>& operator*=(vector<double, 4>& _Left, double _Right) noexcept
{
	_Left._Simd = _mm256_mul_pd(_Left._Simd, _mm256_broadcast_sd(&_Right));
	return _Left;
}

template<>
inline vector<double, 4>& operator/=(vector<double, 4>& _Left, double _Right) noexcept
{
	_Right = 1 / _Right;
	_Left._Simd = _mm256_mul_pd(_Left._Simd, _mm256_broadcast_sd(&_Right));
	return _Left;
}

template<>
inline vector<double, 4> operator+(
	const vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	vector<double, 4> retval;
	retval._Simd = _mm256_add_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 4> operator-(
	const vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	vector<double, 4> retval;
	retval._Simd = _mm256_sub_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 4> operator*(
	const vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	vector<double, 4> retval;
	retval._Simd = _mm256_mul_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 4> operator/(
	const vector<double, 4>& _Left, const vector<double, 4>& _Right) noexcept
{
	vector<double, 4> retval;
	retval._Simd = _mm256_div_pd(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<double, 4> operator*(const vector<double, 4>& _Left, double _Right) noexcept
{
	vector<double, 4> retval;
	retval._Simd = _mm256_mul_pd(_Left._Simd, _mm256_broadcast_sd(&_Right));
	return retval;
}

template<>
inline vector<double, 4> operator/(const vector<double, 4>& _Left, double _Right) noexcept
{
	_Right = 1 / _Right;

	vector<double, 4> retval;
	retval._Simd = _mm256_mul_pd(_Left._Simd, _mm256_broadcast_sd(&_Right));
	return retval;
}

//////////////////////////////////
// vector<int64_t, 4>            //
//////////////////////////////////

template<>
inline vector<int64_t, 4>& operator+=(
	vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_add_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 4>& operator-=(
	vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_sub_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 4>& operator*=(
	vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_mullo_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 4>& operator*=(vector<int64_t, 4>& _Left, int64_t _Right) noexcept
{
	_Left._Simd = _mm256_mullo_epi64(_Left._Simd, _mm256_set1_epi64x(_Right));
	return _Left;
}

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int64_t, 4>& operator/=(
	vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	_Left._Simd = _mm256_div_epi64(_Left._Simd, _Right._Simd);
	return _Left;
}

template<>
inline vector<int64_t, 4>& operator/=(vector<int64_t, 4>& _Left, int64_t _Right) noexcept
{
	_Left._Simd = _mm256_div_epi64(_Left._Simd, _mm256_set1_epi64x(_Right));
	return _Left;
}
#endif // _MST_MATH_SVML_SUPPORT

template<>
inline vector<int64_t, 4> operator+(
	const vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_add_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 4> operator-(
	const vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_sub_epi64(_Left._Simd, _Right._Simd);
	return retval;
}

template<>
inline vector<int64_t, 4> operator*(
	const vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_mullo_epi64(_Left._Simd, _Right._Simd);
	return retval;
}
template<>
inline vector<int64_t, 4> operator*(const vector<int64_t, 4>& _Left, int64_t _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_mullo_epi64(_Left._Simd, _mm256_set1_epi64x(_Right));
	return retval;
}

#if _MST_MATH_SVML_SUPPORT
template<>
inline vector<int64_t, 4> operator/(
	const vector<int64_t, 4>& _Left, const vector<int64_t, 4>& _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_div_epi64(_Left._Simd, _Right._Simd);
	return retval;
}


template<>
inline vector<int64_t, 4> operator/(const vector<int64_t, 4>& _Left, int64_t _Right) noexcept
{
	vector<int64_t, 4> retval;
	retval._Simd = _mm256_div_epi64(_Left._Simd, _mm256_set1_epi64x(_Right));
	return retval;
}
#endif // _MST_MATH_SVML_SUPPORT

#endif // _MST_HAS_AVX2

#if _MST_ALL_VECTORS_SIMD


#endif

} // namespace math
} // namespace mst

#endif