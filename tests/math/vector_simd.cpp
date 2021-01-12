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

#include <numeric>
#include <mcommon.h>

using mst::math::vector;

TEST_CASE("vector<V,E>: base support", "[vector][simd]")
{
	REQUIRE(vector<float, 1>(0).x == 0);
	REQUIRE(vector<float, 2>(0).x == 0);
	REQUIRE(vector<float, 3>(0).x == 0);
	REQUIRE(vector<float, 4>(0).x == 0);

	REQUIRE(vector<uint32_t, 1>(0).x == 0);
	REQUIRE(vector<uint32_t, 2>(0).x == 0);
	REQUIRE(vector<uint32_t, 3>(0).x == 0);
	REQUIRE(vector<uint32_t, 4>(0).x == 0);

	REQUIRE(vector<int32_t, 1>(0).x == 0);
	REQUIRE(vector<int32_t, 2>(0).x == 0);
	REQUIRE(vector<int32_t, 3>(0).x == 0);
	REQUIRE(vector<int32_t, 4>(0).x == 0);

	REQUIRE(vector<double, 1>(0).x == 0);
	REQUIRE(vector<double, 2>(0).x == 0);
	REQUIRE(vector<double, 3>(0).x == 0);
	REQUIRE(vector<double, 4>(0).x == 0);

	REQUIRE(vector<long double, 1>(0).x == 0);
	REQUIRE(vector<long double, 2>(0).x == 0);
	REQUIRE(vector<long double, 3>(0).x == 0);
	REQUIRE(vector<long double, 4>(0).x == 0);

	REQUIRE(vector<uint64_t, 1>(0).x == 0);
	REQUIRE(vector<uint64_t, 2>(0).x == 0);
	REQUIRE(vector<uint64_t, 3>(0).x == 0);
	REQUIRE(vector<uint64_t, 4>(0).x == 0);

	REQUIRE(vector<int64_t, 1>(0).x == 0);
	REQUIRE(vector<int64_t, 2>(0).x == 0);
	REQUIRE(vector<int64_t, 3>(0).x == 0);
	REQUIRE(vector<int64_t, 4>(0).x == 0);
}

template<typename V, size_t E>
void TestArithmetics()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);
	typedef vector<V, E> vector_type;

	vector_type left = vector_type::zero;
	vector_type right = vector_type::zero;

	V lvalues[E];
	std::iota(std::begin(lvalues), std::end(lvalues), (V)0);

	V rvalues[E];
	std::iota(std::begin(rvalues), std::end(rvalues), (V)E);

	for(size_t i = 0; i < E; ++i)
	{
		left[i] = lvalues[i];
		right[i] = rvalues[i];
	}

	auto r = left + right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] + rvalues[i]);
	}

	r = left - right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] - rvalues[i]);
	}

	r = left * right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] * rvalues[i]);
	}

	r = left / right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] / rvalues[i]);
	}

	r = left;
	r += right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] + rvalues[i]);
	}

	r = left;
	r -= right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] - rvalues[i]);
	}

	r = left;
	r *= right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] * rvalues[i]);
	}

	r = left;
	r /= right;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(r[i] == lvalues[i] / rvalues[i]);
	}
}

TEST_CASE("vector<V,E>: SIMD arithmetics", "[vector][simd]")
{
	TestArithmetics<float, 1>();
	TestArithmetics<float, 2>();
	TestArithmetics<float, 3>();
	TestArithmetics<float, 4>();

	TestArithmetics<uint32_t, 1>();
	TestArithmetics<uint32_t, 2>();
	TestArithmetics<uint32_t, 3>();
	TestArithmetics<uint32_t, 4>();

	TestArithmetics<int32_t, 1>();
	TestArithmetics<int32_t, 2>();
	TestArithmetics<int32_t, 3>();
	TestArithmetics<int32_t, 4>();

	TestArithmetics<double, 1>();
	TestArithmetics<double, 2>();
	TestArithmetics<double, 3>();
	TestArithmetics<double, 4>();
}