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

using mst::math::ray;
using mst::math::vector;

/* ray<V,E>'s constructor calls vector::length()/normalize(), which only exist for
	floating point value types, so V is restricted to float/double throughout. */

template<typename V, size_t E>
void TestRayDefaultConstructor()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	ray<V, E> r{};
	REQUIRE(r.position == vector<V, E>::zero);
	REQUIRE(r.direction == vector<V, E>::zero);
}

TEST_CASE("ray<V,E>: default constructor value-initializes position and direction to zero", "[ray]")
{
	TestRayDefaultConstructor<float, 1>();
	TestRayDefaultConstructor<float, 2>();
	TestRayDefaultConstructor<float, 3>();
	TestRayDefaultConstructor<float, 4>();

	TestRayDefaultConstructor<double, 1>();
	TestRayDefaultConstructor<double, 2>();
	TestRayDefaultConstructor<double, 3>();
	TestRayDefaultConstructor<double, 4>();
}

TEST_CASE("ray<V,E>: default constructor is usable in a constexpr context", "[ray]")
{
	constexpr ray<float, 3> r{};
	static_assert(r.position.x == 0.f && r.position.y == 0.f && r.position.z == 0.f,
		"default constructed position should be zero");
	static_assert(r.direction.x == 0.f && r.direction.y == 0.f && r.direction.z == 0.f,
		"default constructed direction should be zero");

	SUCCEED();
}

template<typename V, size_t E>
void TestRayConstructor()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> pos = vector<V, E>::zero;
	for(size_t i = 0; i < E; ++i)
	{
		pos[i] = (V)(i + 1);
	}

	/* axis-aligned unit vector: exactly length 1 for every value type/dimension */
	vector<V, E> dir = vector<V, E>::zero;
	dir[0] = (V)1;

	ray<V, E> r(pos, dir);
	REQUIRE(r.position == pos);
	REQUIRE(r.direction == dir);
}

TEST_CASE("ray<V,E>: constructor sets position and direction for a unit-length direction", "[ray]")
{
	TestRayConstructor<float, 1>();
	TestRayConstructor<float, 2>();
	TestRayConstructor<float, 3>();
	TestRayConstructor<float, 4>();

	TestRayConstructor<double, 1>();
	TestRayConstructor<double, 2>();
	TestRayConstructor<double, 3>();
	TestRayConstructor<double, 4>();
}

TEST_CASE("ray<V,E>: constructor accepts a normalized, non-axis-aligned direction", "[ray]")
{
	vector<double, 3> pos(1.0, 2.0, 3.0);
	vector<double, 3> dir = vector<double, 3>(3.0, 4.0, 0.0).normalized();

	REQUIRE(std::fabs(dir.length() - 1.0) < 1e-12);

	ray<double, 3> r(pos, dir);
	REQUIRE(r.position == pos);
	REQUIRE(r.direction == dir);
}

TEST_CASE("ray<V,E>: constructor accepts a direction length within epsilon of 1", "[ray]")
{
	/* _MST_EPSILON is 1e-5, so a length just inside that tolerance must be accepted */
	vector<float, 3> pos = vector<float, 3>::zero;
	vector<float, 3> dir(1.000005f, 0.f, 0.f);

	ray<float, 3> r(pos, dir);
	REQUIRE(r.direction == dir);
}

TEST_CASE("ray<V,E>: constructor rejects a direction that is too long", "[!shouldfail][ray]")
{
	vector<float, 3> pos = vector<float, 3>::zero;
	vector<float, 3> dir(2.f, 0.f, 0.f);

	ray<float, 3> r(pos, dir);
	(void)r;
}

TEST_CASE("ray<V,E>: constructor rejects a direction that is too short", "[!shouldfail][ray]")
{
	vector<float, 3> pos = vector<float, 3>::zero;
	vector<float, 3> dir(0.5f, 0.f, 0.f);

	ray<float, 3> r(pos, dir);
	(void)r;
}

TEST_CASE("ray<V,E>: constructor rejects a zero-length direction", "[!shouldfail][ray]")
{
	vector<float, 3> pos = vector<float, 3>::zero;
	vector<float, 3> dir = vector<float, 3>::zero;

	ray<float, 3> r(pos, dir);
	(void)r;
}

TEST_CASE("ray<V,E>: constructor rejects a direction just outside the epsilon tolerance",
	"[!shouldfail][ray]")
{
	vector<float, 3> pos = vector<float, 3>::zero;
	vector<float, 3> dir(1.00002f, 0.f, 0.f);

	ray<float, 3> r(pos, dir);
	(void)r;
}

TEST_CASE("ray<V,E>: copy construction and assignment copy position and direction", "[ray]")
{
	vector<float, 3> pos(1.f, 2.f, 3.f);
	vector<float, 3> dir(0.f, 1.f, 0.f);
	ray<float, 3> r(pos, dir);

	ray<float, 3> copyConstructed(r);
	REQUIRE(copyConstructed.position == r.position);
	REQUIRE(copyConstructed.direction == r.direction);

	ray<float, 3> copyAssigned{};
	copyAssigned = r;
	REQUIRE(copyAssigned.position == r.position);
	REQUIRE(copyAssigned.direction == r.direction);
}

TEST_CASE("ray<V,E>: vector_type alias matches mst::math::vector<V,E>", "[ray]")
{
	static_assert(std::is_same<ray<float, 2>::vector_type, vector<float, 2>>::value,
		"ray<V,2>::vector_type should be vector<V,2>");
	static_assert(std::is_same<ray<float, 3>::vector_type, vector<float, 3>>::value,
		"ray<V,3>::vector_type should be vector<V,3>");
	static_assert(std::is_same<ray<double, 4>::vector_type, vector<double, 4>>::value,
		"ray<V,4>::vector_type should be vector<V,4>");

	SUCCEED();
}
