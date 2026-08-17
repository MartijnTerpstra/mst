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

template<typename V>
void TestDot()
{
	CAPTURE(mst::typename_of<V>());

	typedef vector<V, 3> vector_type;

	vector_type left(2, -3, 5);
	vector_type right(1, 4, -2);

	REQUIRE(left.dot(right) == (V)(2 * 1 + -3 * 4 + 5 * -2));

	/* dot product with itself equals the squared length */
	REQUIRE(left.dot(left) == left.squared_length());

	/* dot product is commutative */
	REQUIRE(left.dot(right) == right.dot(left));

	/* dot product with the zero vector is zero */
	REQUIRE(left.dot(vector_type::zero) == (V)0);
}

TEST_CASE("vector<V,3>: dot", "[vector]")
{
	TestDot<float>();
	TestDot<double>();
	TestDot<int32_t>();
	TestDot<int64_t>();
}

template<typename V>
void TestCross()
{
	CAPTURE(mst::typename_of<V>());

	typedef vector<V, 3> vector_type;

	const vector_type unitX(1, 0, 0);
	const vector_type unitY(0, 1, 0);
	const vector_type unitZ(0, 0, 1);

	REQUIRE(unitX.cross(unitY) == unitZ);
	REQUIRE(unitY.cross(unitZ) == unitX);
	REQUIRE(unitZ.cross(unitX) == unitY);

	vector_type left(2, -3, 5);
	vector_type right(1, 4, -2);

	vector_type expected(
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);

	REQUIRE(left.cross(right) == expected);

	/* cross product with itself is the zero vector */
	REQUIRE(left.cross(left) == vector_type::zero);

	/* cross product is anti-commutative */
	REQUIRE(left.cross(right) == -right.cross(left));

	/* the cross product is perpendicular to both inputs */
	REQUIRE(left.cross(right).dot(left) == (V)0);
	REQUIRE(left.cross(right).dot(right) == (V)0);
}

TEST_CASE("vector<V,3>: cross", "[vector]")
{
	TestCross<float>();
	TestCross<double>();
	TestCross<int32_t>();
}
