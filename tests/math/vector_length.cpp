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
void TestSquaredLength()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> v = vector<V, E>::zero;
	V expected = (V)0;
	for(size_t i = 0; i < E; ++i)
	{
		V value = (V)(i + 1);
		v[i] = value;
		expected += value * value;
	}

	REQUIRE(v.squared_length() == expected);
}

TEST_CASE("vector<V,E>: squared_length", "[vector]")
{
	TestSquaredLength<float, 1>();
	TestSquaredLength<float, 2>();
	TestSquaredLength<float, 3>();
	TestSquaredLength<float, 4>();

	TestSquaredLength<double, 3>();
	TestSquaredLength<double, 4>();

	/* squared_length has no floating-point restriction */
	TestSquaredLength<int32_t, 3>();
	TestSquaredLength<uint32_t, 4>();
	TestSquaredLength<int64_t, 4>();
}

template<typename V, size_t E>
void TestSquaredDistance()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> a = vector<V, E>::zero;
	vector<V, E> b = vector<V, E>::zero;

	for(size_t i = 0; i < E; ++i)
	{
		a[i] = (V)(i + 1);
		b[i] = (V)(2 * (i + 1));
	}

	V expected = (V)0;
	for(size_t i = 0; i < E; ++i)
	{
		const V diff = b[i] - a[i];
		expected += diff * diff;
	}

	REQUIRE(a.squared_distance(b) == expected);
	/* distance is symmetric */
	REQUIRE(a.squared_distance(b) == b.squared_distance(a));
	REQUIRE(a.squared_distance(a) == (V)0);
}

TEST_CASE("vector<V,E>: squared_distance", "[vector]")
{
	TestSquaredDistance<float, 1>();
	TestSquaredDistance<float, 2>();
	TestSquaredDistance<float, 3>();
	TestSquaredDistance<float, 4>();

	TestSquaredDistance<double, 4>();
	TestSquaredDistance<int32_t, 3>();
	TestSquaredDistance<int64_t, 4>();
}

template<typename V>
inline V AbsOf(V value)
{
	return value < (V)0 ? -value : value;
}

template<typename V, size_t E>
void TestLength()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	const V eps = mst::math::epsilon<V>() * (V)100;

	vector<V, E> v = vector<V, E>::zero;
	V sq = (V)0;
	for(size_t i = 0; i < E; ++i)
	{
		V value = (V)(i + 1);
		v[i] = value;
		sq += value * value;
	}

	V len = v.length();

	REQUIRE(AbsOf(len * len - sq) < eps);
	REQUIRE(AbsOf(len - std::sqrt(sq)) < eps);
}

TEST_CASE("vector<V,E>: length", "[vector]")
{
	TestLength<float, 1>();
	TestLength<float, 2>();
	TestLength<float, 3>();
	TestLength<float, 4>();

	TestLength<double, 2>();
	TestLength<double, 3>();
	TestLength<double, 4>();
}

template<typename V, size_t E>
void TestDistance()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	const V eps = mst::math::epsilon<V>() * (V)100;

	vector<V, E> a = vector<V, E>::zero;
	vector<V, E> b = vector<V, E>::zero;

	for(size_t i = 0; i < E; ++i)
	{
		a[i] = (V)(i + 1);
		b[i] = (V)(2 * (i + 1));
	}

	REQUIRE(AbsOf(a.distance(b) * a.distance(b) - a.squared_distance(b)) < eps);
	REQUIRE(a.distance(a) == (V)0);
}

TEST_CASE("vector<V,E>: distance", "[vector]")
{
	TestDistance<float, 1>();
	TestDistance<float, 2>();
	TestDistance<float, 3>();
	TestDistance<float, 4>();

	TestDistance<double, 3>();
	TestDistance<double, 4>();
}

template<typename V, size_t E>
void TestNormalize()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	const V eps = mst::math::epsilon<V>() * (V)100;

	vector<V, E> v = vector<V, E>::zero;
	for(size_t i = 0; i < E; ++i)
	{
		v[i] = (V)(i + 1);
	}

	auto normalized = v.normalized();
	REQUIRE(AbsOf(normalized.length() - (V)1) < eps);

	/* normalized() does not modify the original */
	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(v[i] == (V)(i + 1));
	}

	auto& selfRef = v.normalize();
	REQUIRE(&selfRef == &v);
	REQUIRE(AbsOf(v.length() - (V)1) < eps);

	/* normalize() and normalized() must agree on direction */
	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(AbsOf(v[i] - normalized[i]) < eps);
	}
}

TEST_CASE("vector<V,E>: normalize / normalized", "[vector]")
{
	TestNormalize<float, 1>();
	TestNormalize<float, 2>();
	TestNormalize<float, 3>();
	TestNormalize<float, 4>();

	TestNormalize<double, 2>();
	TestNormalize<double, 3>();
	TestNormalize<double, 4>();
}
