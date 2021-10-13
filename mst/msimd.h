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
#include <emmintrin.h>

// SSE insintrics wrapper

namespace mst {
namespace SSE2 {

// arithmetics
inline __m128 operator+(const __m128 _A, const __m128 _B)
{
	return _mm_add_ps(_A, _B);
}

inline __m128 operator-(const __m128 _A, const __m128 _B)
{
	return _mm_sub_ps(_A, _B);
}

inline __m128 operator*(const __m128 _A, const __m128 _B)
{
	return _mm_mul_ps(_A, _B);
}

inline __m128 operator/(const __m128 _A, const __m128 _B)
{
	return _mm_div_ps(_A, _B);
}

inline __m128& operator+=(__m128& _A, const __m128 _B)
{
	_A = _mm_add_ps(_A, _B);
	return _A;
}

inline __m128& operator-=(__m128& _A, const __m128 _B)
{
	_A = _mm_sub_ps(_A, _B);
	return _A;
}

inline __m128& operator*=(__m128& _A, const __m128 _B)
{
	_A = _mm_mul_ps(_A, _B);
	return _A;
}

inline __m128& operator/=(__m128& _A, const __m128 _B)
{
	_A = _mm_div_ps(_A, _B);
	return _A;
}

inline __m128 operator-(const __m128 _A)
{
	return _mm_sub_ps(_mm_setzero_ps(), _A);
}

// end of arithmathics

// start of logical operator overloads

inline __m128 operator|(const __m128 _A, const __m128 _B)
{
	return _mm_or_ps(_A, _B);
}

inline __m128 operator&(const __m128 _A, const __m128 _B)
{
	return _mm_and_ps(_A, _B);
}

inline __m128& operator|=(__m128& _A, const __m128 _B)
{
	_A = _mm_or_ps(_A, _B);
	return _A;
}

inline __m128& operator&(__m128& _A, const __m128 _B)
{
	_A = _mm_and_ps(_A, _B);
	return _A;
}

// end of logical operator overloads

// comparisons overloads

inline __m128 operator<(const __m128 _A, const __m128 _B)
{
	return _mm_cmplt_ps(_A, _B);
}

inline __m128 operator<=(const __m128 _A, const __m128 _B)
{
	return _mm_cmple_ps(_A, _B);
}

inline __m128 operator>(const __m128 _A, const __m128 _B)
{
	return _mm_cmpgt_ps(_A, _B);
}

inline __m128 operator>=(const __m128 _A, const __m128 _B)
{
	return _mm_cmpge_ps(_A, _B);
}

// end of comparisons

// math functions
/* */
inline __m128 sqrtf(const __m128 _A)
{
	return _mm_sqrt_ps(_A);
}

/* warning! only 22 bit precision */
inline __m128 fast_sqrtf(const __m128 _A)
{
	return _mm_mul_ps(_mm_sqrt_ps(_A), _A);
}

/* warning! only 22 bit precision */
inline __m128 rsqrtf(const __m128 _A)
{
	return _mm_rsqrt_ps(_A);
}

/* warning! only 22 bit precision */
inline __m128 rcp(const __m128 _A)
{
	return _mm_rcp_ps(_A);
}

// end of math functions

inline __m128(max)(const __m128 _A, const __m128 _B)
{
	return _mm_max_ps(_A, _B);
}

template<typename _Ty>
inline __m128(min)(const __m128 _A, const __m128 _B)
{
	return _mm_min_ps(_A, _B);
}

struct MST_ALIGN(16) SSEfloat3
{
	SSEfloat3() = default;
	SSEfloat3(const __m128 ax, const __m128 ay, const __m128 az)
		: x(ax)
		, y(ay)
		, z(az)
	{ }

	inline void set(const __m128 ax, const __m128 ay, const __m128 az)
	{
		x = ax, y = ay, z = az;
	}

	inline __m128 dot(const SSEfloat3& vec) const
	{
		return (x * vec.x + y * vec.y + z * vec.z);
	}
	inline void quick_normalize()
	{
		__m128 l = _mm_rsqrt_ps(x * x + y * y + z * z);
		*this *= l;
	}
	inline void slow_normalize()
	{
		__m128 l = _mm_div_ps(_mm_set_ps1(1.0f), _mm_sqrt_ps(x * x + y * y + z * z));
		*this *= l;
	}

	inline __m128 squared_length()
	{
		return ((x * x) + (y * y) + (z * z));
	}

	inline SSEfloat3& operator+=(const SSEfloat3& vec)
	{
		x += vec.x, y += vec.y, z += vec.z;
		return *this;
	}
	inline SSEfloat3& operator-=(const SSEfloat3& vec)
	{
		x -= vec.x, y -= vec.y, z -= vec.z;
		return *this;
	}
	inline SSEfloat3& operator*=(const SSEfloat3& vec)
	{
		x *= vec.x, y *= vec.y, z *= vec.z;
		return *this;
	}
	inline SSEfloat3& operator/=(const SSEfloat3& vec)
	{
		x /= vec.x, y /= vec.y, z /= vec.z;
		return *this;
	}
	inline SSEfloat3& operator*=(const __m128 val)
	{
		x *= val, y *= val, z *= val;
		return *this;
	}
	inline SSEfloat3& operator/=(const __m128 val)
	{
		x /= val, y /= val, z /= val;
		return *this;
	}

	inline SSEfloat3 operator-() const
	{
		return SSEfloat3(_mm_setzero_ps() - x, _mm_setzero_ps() - y, _mm_setzero_ps() - z);
	}

	inline friend SSEfloat3 operator+(const SSEfloat3& vec1, const SSEfloat3& vec2)
	{
		return SSEfloat3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
	}
	inline friend SSEfloat3 operator-(const SSEfloat3& vec1, const SSEfloat3& vec2)
	{
		return SSEfloat3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
	}
	inline friend SSEfloat3 operator*(const SSEfloat3& vec1, const SSEfloat3& vec2)
	{
		return SSEfloat3(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
	}
	inline friend SSEfloat3 operator/(const SSEfloat3& vec1, const SSEfloat3& vec2)
	{
		return SSEfloat3(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z);
	}

	inline friend SSEfloat3 operator*(const SSEfloat3& vec, const __m128 val)
	{
		return SSEfloat3(vec.x * val, vec.y * val, vec.z * val);
	}
	inline friend SSEfloat3 operator/(const SSEfloat3& vec, const __m128 val)
	{
		return SSEfloat3(vec.x / val, vec.y / val, vec.z / val);
	}
	inline friend SSEfloat3 operator*(const __m128 val, const SSEfloat3& vec)
	{
		return SSEfloat3(vec.x * val, vec.y * val, vec.z * val);
	}
	inline friend SSEfloat3 operator/(const __m128 val, const SSEfloat3& vec)
	{
		return SSEfloat3(val / vec.x, val / vec.y, val / vec.z);
	}

	inline __m128& operator[](size_t index)
	{
		CHECK_ARR_RANGE(index, 3, "index out of range");
		return (&x)[index];
	}

	inline const __m128& operator[](size_t index) const
	{
		CHECK_ARR_RANGE(index, 3, "index out of range");
		return (&x)[index];
	}

#ifdef MMATH_H
	inline float3 to_float3(size_t index) const
	{
		CHECK_ARR_RANGE(index, 4, "index out of range");
		return cfloat3(x.m128_f32[index], y.m128_f32[index], z.m128_f32[index]);
	}
#endif

#ifdef MMATH2_H
	inline ::mst::math::vector<float, 3> to_float3(size_t index) const
	{
		CHECK_ARR_RANGE(index, 4, "index out of range");
		return ::mst::math::vector<float, 3>(
			x.m128_f32[index], y.m128_f32[index], z.m128_f32[index]);
	}
#endif

	__m128 x, y, z;
}; // struct SSEfloat3

struct MST_ALIGN(16) SSEfloat2
{
	SSEfloat2() = default;
	SSEfloat2(const __m128 ax, const __m128 ay)
		: x(ax)
		, y(ay)
	{ }

	inline void set(const __m128 ax, const __m128 ay)
	{
		x = ax, y = ay;
	}

	inline __m128 dot(const SSEfloat2& vec) const
	{
		return (x * vec.x + y * vec.y);
	}

	inline SSEfloat2& operator+=(const SSEfloat2& vec)
	{
		x += vec.x, y += vec.y;
		return *this;
	}
	inline SSEfloat2& operator-=(const SSEfloat2& vec)
	{
		x -= vec.x, y -= vec.y;
		return *this;
	}
	inline SSEfloat2& operator*=(const SSEfloat2& vec)
	{
		x *= vec.x, y *= vec.y;
		return *this;
	}
	inline SSEfloat2& operator/=(const SSEfloat2& vec)
	{
		x /= vec.x, y /= vec.y;
		return *this;
	}
	inline SSEfloat2& operator*=(const __m128 val)
	{
		x *= val, y *= val;
		return *this;
	}
	inline SSEfloat2& operator/=(const __m128 val)
	{
		x /= val, y /= val;
		return *this;
	}

	inline friend SSEfloat2 operator+(const SSEfloat2& vec1, const SSEfloat2& vec2)
	{
		return SSEfloat2(vec1.x + vec2.x, vec1.y + vec2.y);
	}
	inline friend SSEfloat2 operator-(const SSEfloat2& vec1, const SSEfloat2& vec2)
	{
		return SSEfloat2(vec1.x - vec2.x, vec1.y - vec2.y);
	}
	inline friend SSEfloat2 operator*(const SSEfloat2& vec1, const SSEfloat2& vec2)
	{
		return SSEfloat2(vec1.x * vec2.x, vec1.y * vec2.y);
	}
	inline friend SSEfloat2 operator/(const SSEfloat2& vec1, const SSEfloat2& vec2)
	{
		return SSEfloat2(vec1.x / vec2.x, vec1.y / vec2.y);
	}

	inline friend SSEfloat2 operator*(const SSEfloat2& vec, const __m128 val)
	{
		return SSEfloat2(vec.x * val, vec.y * val);
	}
	inline friend SSEfloat2 operator/(const SSEfloat2& vec, const __m128 val)
	{
		return SSEfloat2(vec.x / val, vec.y / val);
	}
	inline friend SSEfloat2 operator*(const __m128 val, const SSEfloat2& vec)
	{
		return SSEfloat2(vec.x * val, vec.y * val);
	}
	inline friend SSEfloat2 operator/(const __m128 val, const SSEfloat2& vec)
	{
		return SSEfloat2(val / vec.x, val / vec.y);
	}

	inline __m128& operator[](size_t index)
	{
		CHECK_ARR_RANGE(index, 2, "index out of range");
		return (&x)[index];
	}

	inline const __m128& operator[](size_t index) const
	{
		CHECK_ARR_RANGE(index, 2, "index out of range");
		return (&x)[index];
	}

	__m128 x, y;

}; // struct SSEfloat2

struct MST_ALIGN(16) SSEcolor
{
	SSEcolor() = default;
	SSEcolor(const __m128 ar, const __m128 ag, const __m128 ab)
		: r(ar)
		, g(ag)
		, b(ab)
	{ }

	inline void set(const __m128 ar, const __m128 ag, const __m128 ab)
	{
		r = ar, g = ag, b = ab;
	}

	inline SSEcolor& operator+=(const SSEcolor& clr)
	{
		r += clr.r, g += clr.g, b += clr.b;
		return *this;
	}
	inline SSEcolor& operator-=(const SSEcolor& clr)
	{
		r -= clr.r, g -= clr.g, b -= clr.b;
		return *this;
	}
	inline SSEcolor& operator*=(const SSEcolor& clr)
	{
		r *= clr.r, g *= clr.g, b *= clr.b;
		return *this;
	}
	inline SSEcolor& operator/=(const SSEcolor& clr)
	{
		r /= clr.r, g /= clr.g, b /= clr.b;
		return *this;
	}
	inline SSEcolor& operator*=(const __m128 val)
	{
		r *= val, g *= val, b *= val;
		return *this;
	}
	inline SSEcolor& operator/=(const __m128 val)
	{
		r /= val, g /= val, b /= val;
		return *this;
	}

	inline friend SSEcolor operator+(const SSEcolor& clr1, const SSEcolor& clr2)
	{
		return SSEcolor(clr1.r + clr2.r, clr1.g + clr2.g, clr1.b + clr2.b);
	}
	inline friend SSEcolor operator-(const SSEcolor& clr1, const SSEcolor& clr2)
	{
		return SSEcolor(clr1.r - clr2.r, clr1.g - clr2.g, clr1.b - clr2.b);
	}
	inline friend SSEcolor operator*(const SSEcolor& clr1, const SSEcolor& clr2)
	{
		return SSEcolor(clr1.r * clr2.r, clr1.g * clr2.g, clr1.b * clr2.b);
	}
	inline friend SSEcolor operator/(const SSEcolor& clr1, const SSEcolor& clr2)
	{
		return SSEcolor(clr1.r / clr2.r, clr1.g / clr2.g, clr1.b / clr2.b);
	}

	inline friend SSEcolor operator*(const SSEcolor& clr, const __m128 val)
	{
		return SSEcolor(clr.r * val, clr.g * val, clr.b * val);
	}
	inline friend SSEcolor operator/(const SSEcolor& clr, const __m128 val)
	{
		return SSEcolor(clr.r / val, clr.g / val, clr.b / val);
	}
	inline friend SSEcolor operator*(const __m128 val, const SSEcolor& clr)
	{
		return SSEcolor(clr.r * val, clr.g * val, clr.b * val);
	}
	inline friend SSEcolor operator/(const __m128 val, const SSEcolor& clr)
	{
		return SSEcolor(val / clr.r, val / clr.g, val / clr.b);
	}

	inline __m128& operator[](size_t index)
	{
		CHECK_ARR_RANGE(index, 3, "index out of range");
		return (&r)[index];
	}

	inline const __m128& operator[](size_t index) const
	{
		CHECK_ARR_RANGE(index, 3, "index out of range");
		return (&r)[index];
	}

#ifdef MCOLOR_H
	inline ::mst::math::color3 to_color3(size_t index) const
	{
		CHECK_ARR_RANGE(index, 4, "index out of range");
		return ::mst::math::ccolor3(r.m128_f32[index], g.m128_f32[index], b.m128_f32[index]);
	}
#endif

	__m128 r, g, b;

}; // struct SSEcolor

} // namespace SSE2
} // namespace mst