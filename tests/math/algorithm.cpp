//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <set_assertions.h>

#include "helpers.h"

//#define MST_MATH_ALL_VECTORS_MATRICES_SIMD 1
#include <mmath2.h>
#include <mcommon.h>

using namespace mst::math;
using namespace mst;

TEST_CASE("is_pow_2", "[math][algorithm]")
{
	for(uint32_t i = 0; i < 32; ++i)
	{
		REQUIRE(is_pow_2(1U << i));
	}

	std::mt19937 engine(100);
	std::uniform_int_distribution<uint32_t> rand;

	for(int i = 0; i < 10000; ++i)
	{
		const auto value = rand(engine);
		bool isPow2 = false;
		for(uint32_t j = 0; j < 32; ++j)
		{
			if(value == 1U << j)
			{
				isPow2 = true;
			}
		}

		REQUIRE(is_pow_2(value) == isPow2);
	}
}

TEST_CASE("to_bit_shift_cpp11", "[math][algorithm]")
{
	uint32_t value = 100;

	constexpr const uint32_t div = 8;
	const auto divShift = to_bit_shift_cpp11<uint32_t, div>::value;

	REQUIRE(value / div == value >> divShift);
	REQUIRE(value * div == value << divShift);

	REQUIRE(to_bit_shift_cpp11<uint32_t, 1>::value == 0);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 2>::value == 1);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 4>::value == 2);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 8>::value == 3);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 16>::value == 4);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 32>::value == 5);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 64>::value == 6);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 128>::value == 7);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 256>::value == 8);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 512>::value == 9);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 1024>::value == 10);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 2048>::value == 11);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 4096>::value == 12);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 8192>::value == 13);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 16384>::value == 14);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 32768>::value == 15);
	REQUIRE(to_bit_shift_cpp11<uint32_t, 65536>::value == 16);
}

#if _MST_HAS_TEMPLATE_AUTO

TEST_CASE("to_bit_shift", "[math][algorithm]")
{
	uint32_t value = 100;

	constexpr const uint32_t div = 8;
	const auto divShift = to_bit_shift<div>::value;

	REQUIRE(value / div == value >> divShift);
	REQUIRE(value * div == value << divShift);

	REQUIRE(to_bit_shift<1>::value == 0);
	REQUIRE(to_bit_shift<2>::value == 1);
	REQUIRE(to_bit_shift<4>::value == 2);
	REQUIRE(to_bit_shift<8>::value == 3);
	REQUIRE(to_bit_shift<16>::value == 4);
	REQUIRE(to_bit_shift<32>::value == 5);
	REQUIRE(to_bit_shift<64>::value == 6);
	REQUIRE(to_bit_shift<128>::value == 7);
	REQUIRE(to_bit_shift<256>::value == 8);
	REQUIRE(to_bit_shift<512>::value == 9);
	REQUIRE(to_bit_shift<1024>::value == 10);
	REQUIRE(to_bit_shift<2048>::value == 11);
	REQUIRE(to_bit_shift<4096>::value == 12);
	REQUIRE(to_bit_shift<8192>::value == 13);
	REQUIRE(to_bit_shift<16384>::value == 14);
	REQUIRE(to_bit_shift<32768>::value == 15);
	REQUIRE(to_bit_shift<65536>::value == 16);
}

#endif // _MST_HAS_TEMPLATE_AUTO

TEST_CASE("algorithm_saturate_scalar", "[math][algorithm]")
{
	REQUIRE(saturate(-1.f) == 0.0f);
	REQUIRE(saturate(.5f) == .5f);
	REQUIRE(saturate(2.f) == 1.0f);

	REQUIRE(saturate(-1.0) == 0.0);
	REQUIRE(saturate(.5) == .5);
	REQUIRE(saturate(2.0) == 1.0);

	REQUIRE(saturate(-1) == 0);
	REQUIRE(saturate(0) == 0);
	REQUIRE(saturate(1) == 1);
	REQUIRE(saturate(2) == 1);
}

TEST_CASE("algorithm_saturate_vector", "[math][algorithm]")
{
	typedef vector<float, 4> vec4;
	typedef vector<double, 4> dvec4;
	typedef vector<int, 4> ivec4;

	REQUIRE(saturate(vec4(-1.f)) == vec4(0.0f));
	REQUIRE(saturate(vec4(.5f)) == vec4(.5f));
	REQUIRE(saturate(vec4(2.f)) == vec4(1.0f));

	REQUIRE(saturate(dvec4(-1.0)) == dvec4(0.0));
	REQUIRE(saturate(dvec4(.5)) == dvec4(.5));
	REQUIRE(saturate(dvec4(2.0)) == dvec4(1.0));

	REQUIRE(saturate(ivec4(-1)) == ivec4(0));
	REQUIRE(saturate(ivec4(0)) == ivec4(0));
	REQUIRE(saturate(ivec4(1)) == ivec4(1));
	REQUIRE(saturate(ivec4(2)) == ivec4(1));
}

TEST_CASE("algorithm_saturate_matrix", "[math][algorithm]")
{
	typedef matrix<float, 4, 4> mat4x4;
	typedef matrix<double, 4, 4> dmat4x4;
	typedef matrix<int, 4, 4> imat4x4;

	REQUIRE(saturate(mat4x4(-1.f)) == mat4x4(0.0f));
	REQUIRE(saturate(mat4x4(.5f)) == mat4x4(.5f));
	REQUIRE(saturate(mat4x4(2.f)) == mat4x4(1.0f));

	REQUIRE(saturate(dmat4x4(-1.0)) == dmat4x4(0.0));
	REQUIRE(saturate(dmat4x4(.5)) == dmat4x4(.5));
	REQUIRE(saturate(dmat4x4(2.0)) == dmat4x4(1.0));

	REQUIRE(saturate(imat4x4(-1)) == imat4x4(0));
	REQUIRE(saturate(imat4x4(0)) == imat4x4(0));
	REQUIRE(saturate(imat4x4(1)) == imat4x4(1));
	REQUIRE(saturate(imat4x4(2)) == imat4x4(1));
}

TEST_CASE("algorithm_lerp_scalar", "[math][algorithm]")
{
	auto result = lerp(-1.f, 1.0f, 0.5f);
	REQUIRE(result == 0.0f);
	result = lerp(-.5f, 1.5f, .5f);
	REQUIRE(result == .5f);
	result = lerp(2.f, -6.f, 0.25);
	REQUIRE(result == 0.0f);

	auto resultd = lerp(-1.0, 1.0, 0.5);
	REQUIRE(resultd == 0.0);
	resultd = lerp(-.5, 1.5, .5);
	REQUIRE(resultd == .5);
	resultd = lerp(2.0, -6.0, 0.25);
	REQUIRE(resultd == 0.0);
}

TEST_CASE("algorithm_lerp_vector", "[math][algorithm]")
{
	typedef vector<float, 4> vec4;
	typedef vector<double, 4> dvec4;

	REQUIRE(lerp(vec4(-1.f), vec4(1.0f), 0.5f) == vec4(0.0f));
	REQUIRE(lerp(vec4(-.5f), vec4(1.5f), .5f) == vec4(.5f));
	REQUIRE(lerp(vec4(2.f), vec4(-6.f), 0.25f) == vec4(0.0f));

	REQUIRE(lerp(dvec4(-1.0), dvec4(1.0), 0.5) == dvec4(0.0));
	REQUIRE(lerp(dvec4(-.5), dvec4(1.5), 0.5) == dvec4(0.5));
	REQUIRE(lerp(dvec4(2.0), dvec4(-6.0), 0.25) == dvec4(0.0));
}

TEST_CASE("algorithm_lerp_matrix", "[math][algorithm]")
{
	typedef matrix<float, 4, 4> mat4x4;
	typedef matrix<double, 4, 4> dmat4x4;

	REQUIRE(lerp(mat4x4(-1.f), mat4x4(1.0f), 0.5f) == mat4x4(0.0f));
	REQUIRE(lerp(mat4x4(-.5f), mat4x4(1.5f), .5f) == mat4x4(.5f));
	REQUIRE(lerp(mat4x4(2.f), mat4x4(-6.f), 0.25f) == mat4x4(0.0f));

	REQUIRE(lerp(dmat4x4(-1.0), dmat4x4(1.0), 0.5) == dmat4x4(0.0));
	REQUIRE(lerp(dmat4x4(-.5), dmat4x4(1.5), 0.5) == dmat4x4(0.5));
	REQUIRE(lerp(dmat4x4(2.0), dmat4x4(-6.0), 0.25) == dmat4x4(0.0));
}