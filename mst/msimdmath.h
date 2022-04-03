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
#include <mx_simdmath.h>

namespace mst {
namespace simd {

class float4 : private _Details::vectorf
{
	typedef _Details::vectorf base;

public:
#if _MST_HAS_INLINE_VARIABLES

	inline static const float4 zero = float4(_Details::vector_constants<__m128>::zero);

	inline static const float4 one = float4(_Details::vector_constants<__m128>::one);

#endif

	inline float4() noexcept = default;

	inline float4(float value) noexcept
		: base(value)
	{ }

	inline float4(float x, float y, float z, float w) noexcept
		: base(x, y, z, w)
	{ }

	[[nodiscard]] inline float4 operator+(const float4& other) noexcept
	{
		return float4{ base::add(other) };
	}

	[[nodiscard]] inline float4& operator+=(const float4 other) noexcept
	{
		base::set(base::add(other));
		return *this;
	}

	[[nodiscard]] inline float4 operator-(const float4& other) noexcept
	{
		return float4{ base::sub(other) };
	}

	[[nodiscard]] inline float4& operator-=(const float4 other) noexcept
	{
		base::set(base::sub(other));
		return *this;
	}

	[[nodiscard]] inline float4 operator*(const float4& other) noexcept
	{
		return float4{ base::mul(other) };
	}

	[[nodiscard]] inline float4& operator*=(const float4 other) noexcept
	{
		base::set(base::mul(other));
		return *this;
	}

	[[nodiscard]] inline float4 operator*(float value) noexcept
	{
		return float4{ base::mul(value) };
	}

	[[nodiscard]] inline float4& operator*=(float value) noexcept
	{
		base::set(base::mul(value));
		return *this;
	}

	[[nodiscard]] inline float4 operator/(const float4& other) noexcept
	{
		return float4{ base::div(other) };
	}

	[[nodiscard]] inline float4& operator/=(const float4 other) noexcept
	{
		base::set(base::div(other));
		return *this;
	}

	[[nodiscard]] inline float4 operator-() const noexcept
	{
		return float4{ base::negate() };
	}

	// reciprocal
	[[nodiscard]] inline float4 rcp() const noexcept
	{
		return float4{ base::rcp() };
	}

	// reciprocal estamated
	[[nodiscard]] inline float4 rep_est() const noexcept
	{
		return float4{ base::rcp_est() };
	}

private:
	float4(__m128 data)
		: base(data)
	{ }
};

class float3 : private _Details::vectorf
{
	typedef _Details::vectorf base;

public:
	inline float3() = default;

	inline float3(float value)
		: base(value)
	{ }

	inline float3(float x, float y, float z)
		: base(x, y, z, 0)
	{ }
};

class float2 : private _Details::vectorf
{
	typedef _Details::vectorf base;

public:
	inline float2() = default;

	inline float2(float value)
		: base(value)
	{ }

	inline float2(float x, float y)
		: base(x, y, 0, 0)
	{ }
};

} // namespace simd
} // namespace mst