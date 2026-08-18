//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <set_assertions.h>

#include <array>
#include <cmath>
#include <msimd.h>

// __m128 is not a type in namespace mst::SSE2, so argument-dependent lookup can't find
// the free-function operators overloaded for it; bring them into scope explicitly.
using namespace mst::SSE2;

namespace {

// __m128's internal layout differs between MSVC (named .m128_f32 members) and GCC/Clang
// (an opaque vector type), so tests extract lanes portably through _mm_store_ps instead.
std::array<float, 4> to_array(const __m128 value)
{
	alignas(16) std::array<float, 4> result;
	_mm_store_ps(result.data(), value);
	return result;
}

bool all_lanes_set(const __m128 mask)
{
	return _mm_movemask_ps(mask) == 0b1111;
}

bool all_lanes_clear(const __m128 mask)
{
	return _mm_movemask_ps(mask) == 0;
}

} // namespace

TEST_CASE("SSE2: arithmetic operators", "[simd][sse2]")
{
	const __m128 a = _mm_set_ps(4, 3, 2, 1);
	const __m128 b = _mm_set_ps(40, 30, 20, 10);

	REQUIRE(to_array(a + b) == std::array<float, 4>{ 11, 22, 33, 44 });
	REQUIRE(to_array(b - a) == std::array<float, 4>{ 9, 18, 27, 36 });
	REQUIRE(to_array(a * b) == std::array<float, 4>{ 10, 40, 90, 160 });
	REQUIRE(to_array(b / a) == std::array<float, 4>{ 10, 10, 10, 10 });
	REQUIRE(to_array(-a) == std::array<float, 4>{ -1, -2, -3, -4 });
}

TEST_CASE("SSE2: compound assignment operators", "[simd][sse2]")
{
	const __m128 b = _mm_set_ps(40, 30, 20, 10);

	__m128 v = _mm_set_ps(4, 3, 2, 1);
	v += b;
	REQUIRE(to_array(v) == std::array<float, 4>{ 11, 22, 33, 44 });

	v = _mm_set_ps(4, 3, 2, 1);
	v -= b;
	REQUIRE(to_array(v) == std::array<float, 4>{ -9, -18, -27, -36 });

	v = _mm_set_ps(4, 3, 2, 1);
	v *= b;
	REQUIRE(to_array(v) == std::array<float, 4>{ 10, 40, 90, 160 });

	v = b;
	v /= _mm_set_ps(4, 3, 2, 1);
	REQUIRE(to_array(v) == std::array<float, 4>{ 10, 10, 10, 10 });
}

TEST_CASE("SSE2: bitwise operators", "[simd][sse2]")
{
	// bit patterns of 1.0f and 2.0f chosen so AND/OR land on other well-defined, non-NaN
	// floats (0.0f and +Infinity respectively) instead of unpredictable NaN payloads.
	const __m128 one = _mm_castsi128_ps(_mm_set1_epi32(0x3F800000));
	const __m128 two = _mm_castsi128_ps(_mm_set1_epi32(0x40000000));

	const auto anded = to_array(one & two);
	for(float lane : anded)
		REQUIRE(lane == 0.0f);

	const auto ored = to_array(one | two);
	for(float lane : ored)
		REQUIRE(std::isinf(lane));

	__m128 v = one;
	v &= two;
	REQUIRE(to_array(v)[0] == 0.0f);

	v = one;
	v |= two;
	REQUIRE(std::isinf(to_array(v)[0]));
}

TEST_CASE("SSE2: comparison operators produce all-set or all-clear masks", "[simd][sse2]")
{
	const __m128 small = _mm_set_ps1(1.0f);
	const __m128 large = _mm_set_ps1(2.0f);

	REQUIRE(all_lanes_set(small < large));
	REQUIRE(all_lanes_clear(large < small));

	REQUIRE(all_lanes_set(small <= small));
	REQUIRE(all_lanes_clear(large <= small));

	REQUIRE(all_lanes_set(large > small));
	REQUIRE(all_lanes_clear(small > large));

	REQUIRE(all_lanes_set(small >= small));
	REQUIRE(all_lanes_clear(small >= large));
}

TEST_CASE("SSE2: sqrtf", "[simd][sse2]")
{
	const __m128 v = _mm_set_ps(16, 9, 4, 1);
	REQUIRE(to_array(mst::SSE2::sqrtf(v)) == std::array<float, 4>{ 1, 2, 3, 4 });
}

TEST_CASE("SSE2: fast_sqrtf, rsqrtf and rcp are low-precision approximations", "[simd][sse2]")
{
	const __m128 v = _mm_set_ps1(4.0f);

	const auto fastSqrt = to_array(mst::SSE2::fast_sqrtf(v));
	for(float lane : fastSqrt)
		REQUIRE(lane == Catch::Approx(4.0f * 2.0f).epsilon(0.01)); // x * sqrt(x)

	const auto rsqrt = to_array(mst::SSE2::rsqrtf(v));
	for(float lane : rsqrt)
		REQUIRE(lane == Catch::Approx(0.5f).epsilon(0.01));

	const auto rcp = to_array(mst::SSE2::rcp(v));
	for(float lane : rcp)
		REQUIRE(lane == Catch::Approx(0.25f).epsilon(0.01));
}

TEST_CASE("SSE2: max and min", "[simd][sse2]")
{
	const __m128 a = _mm_set_ps(4, 3, 2, 1);
	const __m128 b = _mm_set_ps(1, 2, 3, 4);

	REQUIRE(to_array((mst::SSE2::max)(a, b)) == std::array<float, 4>{ 4, 3, 3, 4 });
	REQUIRE(to_array((mst::SSE2::min)(a, b)) == std::array<float, 4>{ 1, 2, 2, 1 });
}

TEST_CASE("SSE2::SSEfloat3: construction, set and dot", "[simd][sse2]")
{
	SSEfloat3 v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	REQUIRE(to_array(v.x) == std::array<float, 4>{ 1, 1, 1, 1 });
	REQUIRE(to_array(v.y) == std::array<float, 4>{ 2, 2, 2, 2 });
	REQUIRE(to_array(v.z) == std::array<float, 4>{ 3, 3, 3, 3 });

	v.set(_mm_set_ps1(4), _mm_set_ps1(5), _mm_set_ps1(6));
	REQUIRE(to_array(v.x) == std::array<float, 4>{ 4, 4, 4, 4 });

	SSEfloat3 other(_mm_set_ps1(1), _mm_set_ps1(1), _mm_set_ps1(1));
	// (4*1 + 5*1 + 6*1) = 15
	REQUIRE(to_array(v.dot(other)) == std::array<float, 4>{ 15, 15, 15, 15 });

	REQUIRE(to_array(v.squared_length()) == std::array<float, 4>{ 77, 77, 77, 77 }); // 16+25+36
}

TEST_CASE("SSE2::SSEfloat3: normalization", "[simd][sse2]")
{
	SSEfloat3 v(_mm_set_ps1(3), _mm_set_ps1(0), _mm_set_ps1(4)); // length 5

	SSEfloat3 slow = v;
	slow.slow_normalize();
	REQUIRE(to_array(slow.x)[0] == Catch::Approx(0.6f));
	REQUIRE(to_array(slow.z)[0] == Catch::Approx(0.8f));

	SSEfloat3 fast = v;
	fast.quick_normalize();
	REQUIRE(to_array(fast.x)[0] == Catch::Approx(0.6f).epsilon(0.01));
	REQUIRE(to_array(fast.z)[0] == Catch::Approx(0.8f).epsilon(0.01));
}

TEST_CASE("SSE2::SSEfloat3: arithmetic operators", "[simd][sse2]")
{
	SSEfloat3 a(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	SSEfloat3 b(_mm_set_ps1(10), _mm_set_ps1(20), _mm_set_ps1(30));

	SSEfloat3 sum = a + b;
	REQUIRE(to_array(sum.x)[0] == 11);
	REQUIRE(to_array(sum.y)[0] == 22);
	REQUIRE(to_array(sum.z)[0] == 33);

	SSEfloat3 diff = b - a;
	REQUIRE(to_array(diff.x)[0] == 9);

	SSEfloat3 prod = a * b;
	REQUIRE(to_array(prod.x)[0] == 10);

	SSEfloat3 quot = b / a;
	REQUIRE(to_array(quot.x)[0] == 10);

	SSEfloat3 neg = -a;
	REQUIRE(to_array(neg.x)[0] == -1);

	SSEfloat3 scaledMul = a * _mm_set_ps1(2);
	REQUIRE(to_array(scaledMul.x)[0] == 2);
	SSEfloat3 scaledMulLeft = _mm_set_ps1(2) * a;
	REQUIRE(to_array(scaledMulLeft.x)[0] == 2);

	SSEfloat3 scaledDiv = a / _mm_set_ps1(2);
	REQUIRE(to_array(scaledDiv.x)[0] == Catch::Approx(0.5f));
	SSEfloat3 scaledDivLeft = _mm_set_ps1(2) / a;
	REQUIRE(to_array(scaledDivLeft.x)[0] == 2);

	SSEfloat3 c(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	c += b;
	REQUIRE(to_array(c.x)[0] == 11);
	c -= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
	c *= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(10.0f));
	c /= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
	c *= _mm_set_ps1(2);
	REQUIRE(to_array(c.x)[0] == Catch::Approx(2.0f));
	c /= _mm_set_ps1(2);
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
}

TEST_CASE("SSE2::SSEfloat3: operator[], const and non-const", "[simd][sse2]")
{
	SSEfloat3 v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));

	REQUIRE(to_array(v[0])[0] == 1);
	REQUIRE(to_array(v[1])[0] == 2);
	REQUIRE(to_array(v[2])[0] == 3);

	v[0] = _mm_set_ps1(100);
	REQUIRE(to_array(v.x)[0] == 100);

	const SSEfloat3& cv = v;
	REQUIRE(to_array(cv[0])[0] == 100);
}

TEST_CASE("SSE2::SSEfloat3: operator[] out of range", "[!shouldfail][simd][sse2]")
{
	SSEfloat3 v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	(void)v[3];
}

TEST_CASE("SSE2::SSEfloat3: const operator[] out of range", "[!shouldfail][simd][sse2]")
{
	const SSEfloat3 v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	(void)v[3];
}

TEST_CASE("SSE2::SSEfloat2: construction, set, dot and arithmetic", "[simd][sse2]")
{
	SSEfloat2 v(_mm_set_ps1(3), _mm_set_ps1(4));
	REQUIRE(to_array(v.x)[0] == 3);
	REQUIRE(to_array(v.y)[0] == 4);

	v.set(_mm_set_ps1(1), _mm_set_ps1(2));
	REQUIRE(to_array(v.x)[0] == 1);

	SSEfloat2 other(_mm_set_ps1(1), _mm_set_ps1(1));
	REQUIRE(to_array(v.dot(other))[0] == 3); // 1*1 + 2*1

	SSEfloat2 a(_mm_set_ps1(1), _mm_set_ps1(2));
	SSEfloat2 b(_mm_set_ps1(10), _mm_set_ps1(20));

	REQUIRE(to_array((a + b).x)[0] == 11);
	REQUIRE(to_array((b - a).x)[0] == 9);
	REQUIRE(to_array((a * b).x)[0] == 10);
	REQUIRE(to_array((b / a).x)[0] == 10);
	REQUIRE(to_array((a * _mm_set_ps1(2)).x)[0] == 2);
	REQUIRE(to_array((_mm_set_ps1(2) * a).x)[0] == 2);
	REQUIRE(to_array((a / _mm_set_ps1(2)).x)[0] == Catch::Approx(0.5f));
	REQUIRE(to_array((_mm_set_ps1(2) / a).x)[0] == 2);

	SSEfloat2 c(_mm_set_ps1(1), _mm_set_ps1(2));
	c += b;
	REQUIRE(to_array(c.x)[0] == 11);
	c -= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
	c *= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(10.0f));
	c /= b;
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
	c *= _mm_set_ps1(2);
	REQUIRE(to_array(c.x)[0] == Catch::Approx(2.0f));
	c /= _mm_set_ps1(2);
	REQUIRE(to_array(c.x)[0] == Catch::Approx(1.0f));
}

TEST_CASE("SSE2::SSEfloat2: operator[], const and non-const", "[simd][sse2]")
{
	SSEfloat2 v(_mm_set_ps1(3), _mm_set_ps1(4));

	REQUIRE(to_array(v[0])[0] == 3);
	REQUIRE(to_array(v[1])[0] == 4);

	v[0] = _mm_set_ps1(100);
	REQUIRE(to_array(v.x)[0] == 100);

	const SSEfloat2& cv = v;
	REQUIRE(to_array(cv[0])[0] == 100);
}

TEST_CASE("SSE2::SSEfloat2: operator[] out of range", "[!shouldfail][simd][sse2]")
{
	SSEfloat2 v(_mm_set_ps1(3), _mm_set_ps1(4));
	(void)v[2];
}

TEST_CASE("SSE2::SSEfloat2: const operator[] out of range", "[!shouldfail][simd][sse2]")
{
	const SSEfloat2 v(_mm_set_ps1(3), _mm_set_ps1(4));
	(void)v[2];
}

TEST_CASE("SSE2::SSEcolor: construction, set and arithmetic", "[simd][sse2]")
{
	SSEcolor v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	REQUIRE(to_array(v.r)[0] == 1);
	REQUIRE(to_array(v.g)[0] == 2);
	REQUIRE(to_array(v.b)[0] == 3);

	v.set(_mm_set_ps1(4), _mm_set_ps1(5), _mm_set_ps1(6));
	REQUIRE(to_array(v.r)[0] == 4);

	SSEcolor a(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	SSEcolor b(_mm_set_ps1(10), _mm_set_ps1(20), _mm_set_ps1(30));

	REQUIRE(to_array((a + b).r)[0] == 11);
	REQUIRE(to_array((b - a).r)[0] == 9);
	REQUIRE(to_array((a * b).r)[0] == 10);
	REQUIRE(to_array((b / a).r)[0] == 10);
	REQUIRE(to_array((a * _mm_set_ps1(2)).r)[0] == 2);
	REQUIRE(to_array((_mm_set_ps1(2) * a).r)[0] == 2);
	REQUIRE(to_array((a / _mm_set_ps1(2)).r)[0] == Catch::Approx(0.5f));
	REQUIRE(to_array((_mm_set_ps1(2) / a).r)[0] == 2);

	SSEcolor c(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	c += b;
	REQUIRE(to_array(c.r)[0] == 11);
	c -= b;
	REQUIRE(to_array(c.r)[0] == Catch::Approx(1.0f));
	c *= b;
	REQUIRE(to_array(c.r)[0] == Catch::Approx(10.0f));
	c /= b;
	REQUIRE(to_array(c.r)[0] == Catch::Approx(1.0f));
	c *= _mm_set_ps1(2);
	REQUIRE(to_array(c.r)[0] == Catch::Approx(2.0f));
	c /= _mm_set_ps1(2);
	REQUIRE(to_array(c.r)[0] == Catch::Approx(1.0f));
}

TEST_CASE("SSE2::SSEcolor: operator[], const and non-const", "[simd][sse2]")
{
	SSEcolor v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));

	REQUIRE(to_array(v[0])[0] == 1);
	REQUIRE(to_array(v[1])[0] == 2);
	REQUIRE(to_array(v[2])[0] == 3);

	v[0] = _mm_set_ps1(100);
	REQUIRE(to_array(v.r)[0] == 100);

	const SSEcolor& cv = v;
	REQUIRE(to_array(cv[0])[0] == 100);
}

TEST_CASE("SSE2::SSEcolor: operator[] out of range", "[!shouldfail][simd][sse2]")
{
	SSEcolor v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	(void)v[3];
}

TEST_CASE("SSE2::SSEcolor: const operator[] out of range", "[!shouldfail][simd][sse2]")
{
	const SSEcolor v(_mm_set_ps1(1), _mm_set_ps1(2), _mm_set_ps1(3));
	(void)v[3];
}
