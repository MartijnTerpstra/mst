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

#include <catch2/catch_test_macros.hpp>

#include <set_assertions.h>

#include "helpers.h"

#include <mmath2.h>
#include <mcommon.h>

using mst::math::vector;

template<typename V, size_t E>
void TestUnaryNegate()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> v = vector<V, E>::zero;
	V value = (V)1;
	for(size_t i = 0; i < E; ++i)
	{
		v[i] = value;
		value += 1;
	}

	auto negated = -v;

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(negated[i] == -v[i]);
	}

	/* negating twice yields the original vector */
	REQUIRE(-negated == v);
}

TEST_CASE("vector<V,E>: unary operator-", "[vector]")
{
	TestUnaryNegate<float, 1>();
	TestUnaryNegate<float, 2>();
	TestUnaryNegate<float, 3>();
	TestUnaryNegate<float, 4>();

	TestUnaryNegate<double, 2>();
	TestUnaryNegate<double, 4>();

	TestUnaryNegate<int32_t, 1>();
	TestUnaryNegate<int32_t, 3>();
	TestUnaryNegate<int32_t, 4>();

	TestUnaryNegate<int64_t, 4>();
}

template<typename V, size_t E>
void TestScalarArithmetic()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> v = vector<V, E>::zero;
	V value = (V)1;
	for(size_t i = 0; i < E; ++i)
	{
		v[i] = value;
		value += 1;
	}

	const V scalar = (V)3;

	auto mul = v * scalar;
	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(mul[i] == v[i] * scalar);
	}

	auto div = mul / scalar;
	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(div[i] == v[i]);
	}

	auto r = v;
	r *= scalar;
	REQUIRE(r == mul);

	r /= scalar;
	REQUIRE(r == v);
}

TEST_CASE("vector<V,E>: scalar multiply/divide (operator and compound assignment)", "[vector]")
{
	TestScalarArithmetic<float, 1>();
	TestScalarArithmetic<float, 2>();
	TestScalarArithmetic<float, 3>();
	TestScalarArithmetic<float, 4>();

	TestScalarArithmetic<double, 1>();
	TestScalarArithmetic<double, 2>();
	TestScalarArithmetic<double, 3>();
	TestScalarArithmetic<double, 4>();

	TestScalarArithmetic<int32_t, 1>();
	TestScalarArithmetic<int32_t, 3>();
	TestScalarArithmetic<int32_t, 4>();

	TestScalarArithmetic<uint32_t, 4>();
	TestScalarArithmetic<int64_t, 4>();
}

TEST_CASE("vector<V,E>: vector-vector arithmetic operators match componentwise reference",
	"[vector]")
{
	typedef vector<float, 3> float3;

	float3 left(2, -3, 5);
	float3 right(1, 4, -2);

	REQUIRE((left + right) == float3(3, 1, 3));
	REQUIRE((left - right) == float3(1, -7, 7));
	REQUIRE((left * right) == float3(2, -12, -10));
	REQUIRE((left / right) == float3(2, -0.75f, -2.5f));

	auto r = left;
	r += right;
	REQUIRE(r == float3(3, 1, 3));

	r = left;
	r -= right;
	REQUIRE(r == float3(1, -7, 7));

	r = left;
	r *= right;
	REQUIRE(r == float3(2, -12, -10));

	r = left;
	r /= right;
	REQUIRE(r == float3(2, -0.75f, -2.5f));
}
