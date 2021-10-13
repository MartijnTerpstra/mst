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

namespace _Details {

template<size_t N>
struct _Mst_float_holder
{
	operator float&()
	{
		return ((float*)this)[N];
	}

	operator const float&() const
	{
		return ((const float*)this)[N];
	}

	float& operator=(const float& _Val)
	{
		((float*)this)[N] = _Val;
		return ((float*)this)[N];
	}
};

template<size_t X, size_t Y>
struct _Swizzle_proxy
{
	friend struct float3;

public:
	_Swizzle_proxy& operator=(const float2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	/* normalizes itself and returns a reference to itself */
	inline _Swizzle_proxy& normalize()
	{
		const float inv_length = 1.0f / length();
		x *= inv_length;
		y *= inv_length;
		return *this;
	}

	/* returns the normalized version of itself */
	inline float2 normalized() const
	{
		return float2(*this).normalize();
	}

	/* returns the length of the vector */
	inline float length() const
	{
		return sqrtf(square_length());
	}

	/* returns the square length of the vector */
	inline float square_length() const
	{
		return (x * x + y * y);
	}

	/* returns the distance between it and vec */
	inline float distance(const _Swizzle_proxy& vec) const
	{
		return sqrtf(square_distance(vec));
	}

	/* returns the distance between it and vec */
	inline float distance(const float2& vec) const
	{
		return sqrtf(square_distance(vec));
	}

	/* returns the square distance between it and vec */
	inline float square_distance(const _Swizzle_proxy& vec) const
	{
		const float dx = vec.x - x;
		const float dy = vec.y - y;
		return (dx * dx + dy * dy);
	}

	/* returns the square distance between it and vec */
	inline float square_distance(const float2& vec) const
	{
		const float dx = vec.x - x;
		const float dy = vec.y - y;
		return (dx * dx + dy * dy);
	}

	/* returns the dot product between it and vec */
	inline float dot(const _Swizzle_proxy& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	/* returns the dot product between it and vec */
	inline float dot(const float2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	/* swaps the signs of all axis. x will be -x etc. */
	inline void swap_signs()
	{
		x = -x;
		y = -y;
	}

	// operator overloads

	inline _Swizzle_proxy& operator+=(const _Swizzle_proxy& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator+=(const float2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator-=(const _Swizzle_proxy& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator-=(const float2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator*=(const float& val)
	{
		x *= val;
		y *= val;
		return *this;
	}

	inline _Swizzle_proxy& operator*=(const _Swizzle_proxy& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator*=(const float2& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator/=(const _Swizzle_proxy& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator/=(const float2& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	inline _Swizzle_proxy& operator/=(const float& val)
	{
		const float inv_val = 1.0f / val;
		x *= inv_val;
		y *= inv_val;
		return *this;
	}

	inline friend float2 operator+(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return cfloat2(v1.x + v2.x, v1.y + v2.y);
	}

	inline friend float2 operator+(const _Swizzle_proxy& v1, const float2& v2)
	{
		return cfloat2(v1.x + v2.x, v1.y + v2.y);
	}

	inline friend float2 operator-(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return cfloat2(v1.x - v2.x, v1.y - v2.y);
	}

	inline friend float2 operator-(const _Swizzle_proxy& v1, const float2& v2)
	{
		return cfloat2(v1.x - v2.x, v1.y - v2.y);
	}

	inline friend float2 operator*(const _Swizzle_proxy& v, float val)
	{
		return cfloat2(v.x * val, v.y * val);
	}

	inline friend float2 operator*(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return cfloat2(v1.x * v2.x, v1.y * v2.y);
	}

	inline friend float2 operator*(const _Swizzle_proxy& v1, const float2& v2)
	{
		return cfloat2(v1.x * v2.x, v1.y * v2.y);
	}

	inline friend float2 operator*(float val, const _Swizzle_proxy& v)
	{
		return (v * val);
	}

	inline friend float2 operator/(const _Swizzle_proxy& v, float val)
	{
		const float inv_val = 1.0f / val;
		return cfloat2(v.x * inv_val, v.y * inv_val);
	}

	inline friend float2 operator/(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return cfloat2(v1.x / v2.x, v1.y / v2.y);
	}

	inline friend float2 operator/(const _Swizzle_proxy& v1, const float2& v2)
	{
		return cfloat2(v1.x / v2.x, v1.y / v2.y);
	}

	inline friend float2 operator/(float val, const _Swizzle_proxy& v)
	{
		return cfloat2(val / v.x, val / v.y);
	}

	inline friend bool operator==(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y);
	}

	inline friend bool operator==(const _Swizzle_proxy& v1, const float2& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y);
	}

	inline friend bool operator!=(const _Swizzle_proxy& v1, const _Swizzle_proxy& v2)
	{
		return !(v1 == v2);
	}

	inline friend bool operator!=(const _Swizzle_proxy& v1, const float2& v2)
	{
		return !(v1 == v2);
	}

	inline float2 operator-() const
	{
		return cfloat2(-x, -y);
	}

	inline operator float2() const
	{
		return cfloat2(x, y);
	}

	inline friend std::ostream& operator<<(std::ostream& stream, const _Swizzle_proxy& value)
	{
		stream << "<" << value.x << ", " << value.y << ">";
		return stream;
	}

	inline float& operator[](size_t n)
	{
		switch(n)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			ERROR_MESG_ARG("index out of range", n);
			return x;
		}
	}

	inline const float& operator[](size_t n) const
	{
		switch(n)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			ERROR_MESG_ARG("index out of range", n);
			return x;
		}
	}

	union
	{
		_Mst_float_holder<X> x;
		_Mst_float_holder<Y> y;
	};
};

} // namespace mst::math::_Details