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
#include <cstring>
#include <msimdmath.h>

using mst::simd::float2;
using mst::simd::float3;
using mst::simd::float4;

namespace {

// float4 deliberately exposes no accessors (it wraps a private __m128), so reading its raw
// object representation is the only way to verify arithmetic results from outside the class.
std::array<float, 4> extract(const float4& value)
{
	static_assert(sizeof(value) == sizeof(std::array<float, 4>), "unexpected float4 layout");
	std::array<float, 4> result;
	std::memcpy(result.data(), &value, sizeof(result));
	return result;
}

} // namespace

TEST_CASE("simd::float4: value construction broadcasts to every lane", "[simd][simdmath]")
{
	float4 v(2.0f);
	REQUIRE(extract(v) == std::array<float, 4>{ 2, 2, 2, 2 });
}

TEST_CASE("simd::float4: per-component construction", "[simd][simdmath]")
{
	float4 v(1, 2, 3, 4);
	REQUIRE(extract(v) == std::array<float, 4>{ 1, 2, 3, 4 });
}

TEST_CASE("simd::float4: zero and one constants", "[simd][simdmath]")
{
	REQUIRE(extract(float4::zero) == std::array<float, 4>{ 0, 0, 0, 0 });
	REQUIRE(extract(float4::one) == std::array<float, 4>{ 1, 1, 1, 1 });
}

TEST_CASE("simd::float4: addition and subtraction", "[simd][simdmath]")
{
	float4 a(1, 2, 3, 4);
	float4 b(10, 20, 30, 40);

	REQUIRE(extract(a + b) == std::array<float, 4>{ 11, 22, 33, 44 });
	REQUIRE(extract(b - a) == std::array<float, 4>{ 9, 18, 27, 36 });

	float4 c(1, 2, 3, 4);
	c += b;
	REQUIRE(extract(c) == std::array<float, 4>{ 11, 22, 33, 44 });

	c -= b;
	REQUIRE(extract(c) == std::array<float, 4>{ 1, 2, 3, 4 });
}

TEST_CASE("simd::float4: multiplication and division", "[simd][simdmath]")
{
	float4 a(1, 2, 3, 4);
	float4 b(10, 20, 30, 40);

	REQUIRE(extract(a * b) == std::array<float, 4>{ 10, 40, 90, 160 });
	REQUIRE(extract(b / a) == std::array<float, 4>{ 10, 10, 10, 10 });

	float4 c(1, 2, 3, 4);
	c *= b;
	REQUIRE(extract(c) == std::array<float, 4>{ 10, 40, 90, 160 });

	c /= b;
	REQUIRE(extract(c) == std::array<float, 4>{ 1, 2, 3, 4 });
}

TEST_CASE("simd::float4: scalar multiplication", "[simd][simdmath]")
{
	float4 a(1, 2, 3, 4);

	REQUIRE(extract(a * 2.0f) == std::array<float, 4>{ 2, 4, 6, 8 });

	float4 b(1, 2, 3, 4);
	b *= 2.0f;
	REQUIRE(extract(b) == std::array<float, 4>{ 2, 4, 6, 8 });
}

TEST_CASE("simd::float4: unary negation", "[simd][simdmath]")
{
	float4 a(1, -2, 3, -4);
	REQUIRE(extract(-a) == std::array<float, 4>{ -1, 2, -3, 4 });
}

TEST_CASE("simd::float4: rcp is an exact reciprocal", "[simd][simdmath]")
{
	float4 a(1, 2, 4, 8);
	REQUIRE(extract(a.rcp()) == std::array<float, 4>{ 1.0f, 0.5f, 0.25f, 0.125f });
}

TEST_CASE("simd::float4: rep_est is an approximate reciprocal", "[simd][simdmath]")
{
	float4 a(1, 2, 4, 8);
	const auto est = extract(a.rep_est());

	REQUIRE(est[0] == Catch::Approx(1.0f).epsilon(0.01));
	REQUIRE(est[1] == Catch::Approx(0.5f).epsilon(0.01));
	REQUIRE(est[2] == Catch::Approx(0.25f).epsilon(0.01));
	REQUIRE(est[3] == Catch::Approx(0.125f).epsilon(0.01));
}

TEST_CASE("simd::float3 and simd::float2: construction", "[simd][simdmath]")
{
	// float3/float2 currently expose no operations beyond construction (no accessors,
	// no arithmetic) - these are smoke tests confirming the constructors compile and run.
	float3 broadcast3(1.0f);
	float3 components3(1, 2, 3);
	float2 broadcast2(1.0f);
	float2 components2(1, 2);

	(void)broadcast3;
	(void)components3;
	(void)broadcast2;
	(void)components2;
}
