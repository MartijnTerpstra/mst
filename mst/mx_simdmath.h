#pragma once

#if !_MST_HAS_AVX2
#error "AVX2 support required"
#endif

#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace mst {
namespace _Details {

inline void make_zero_impl(__m128& value)
{
	value = _mm_setzero_ps();
}

inline void make_one_impl(__m128& value)
{
	value = _mm_set_ps1(1.0f);
}

template<typename T>
struct vector_constants
{
	static const T zero;
	static const T one;

	static T make_zero() noexcept
	{
		T retval;
		make_zero_impl(retval);
		return retval;
	}

	static T make_one() noexcept
	{
		T retval;
		make_zero_impl(retval);
		return retval;
	}
};

template<typename T>
const T vector_constants<T>::zero = vector_constants<T>::make_zero();

template<typename T>
const T vector_constants<T>::one = vector_constants<T>::make_one();

class vectorf
{
protected:
	inline vectorf() noexcept = default;

	inline vectorf(float value) noexcept
		: m_data(_mm_set_ps1(value))
	{ }

	inline vectorf(float x, float y, float z, float w) noexcept
		: m_data(_mm_set_ps(w, z, y, x))
	{ }

	inline vectorf(__m128 data) noexcept
		: m_data(data)
	{ }

	[[nodiscard]] inline __m128 add(const vectorf& other) const noexcept
	{
		return _mm_add_ps(m_data, other.m_data);
	}

	[[nodiscard]] inline __m128 sub(const vectorf& other) const noexcept
	{
		return _mm_sub_ps(m_data, other.m_data);
	}

	[[nodiscard]] inline __m128 mul(const vectorf& other) const noexcept
	{
		return _mm_mul_ps(m_data, other.m_data);
	}

	[[nodiscard]] inline __m128 mul(float value) const noexcept
	{
		const auto value4 = _mm_set_ps1(value);
		return _mm_mul_ps(m_data, value4);
	}

	[[nodiscard]] inline __m128 div(const vectorf& other) const noexcept
	{
		return _mm_div_ps(m_data, other.m_data);
	}

	[[nodiscard]] inline __m128 negate() const noexcept
	{
		return _mm_sub_ps(_mm_setzero_ps(), m_data);
	}

	inline void set(__m128 data) noexcept
	{
		m_data = data;
	}

	[[nodiscard]] __m128 rcp() const noexcept
	{
		return _mm_div_ps(vector_constants<__m128>::one, m_data);
	}

	[[nodiscard]] __m128 rcp_est() const noexcept
	{
		return _mm_rcp_ps(m_data);
	}

protected:
	__m128 m_data;
};

class vectord
{
protected:
	inline vectord() = default;

	inline vectord(double value)
		: m_data(_mm256_set1_pd(value))
	{ }

	inline vectord(double x, double y, double z, double w)
		: m_data(_mm256_set_pd(w, z, y, x))
	{ }

	inline vectord(__m256d data)
		: m_data(data)
	{ }

private:
	__m256d m_data;
};

}
}