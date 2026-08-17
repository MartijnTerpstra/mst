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
using mst::math::matrix;

TEST_CASE("matrix<V,C,R>: set() from a flat (row-major) array", "[matrix]")
{
	matrix<int, 3, 2> m(0);

	int values[6] = { 1, 2, 3, 4, 5, 6 };
	m.set(values);

	REQUIRE(m[0] == vector<int, 3>(1, 2, 3));
	REQUIRE(m[1] == vector<int, 3>(4, 5, 6));
}

TEST_CASE("matrix<V,C,R>: set() from a [column][row] array", "[matrix]")
{
	matrix<int, 3, 2> m(0);

	/* column-major: column 0 = (1,4), column 1 = (2,5), column 2 = (3,6) */
	int values[3][2] = { { 1, 4 }, { 2, 5 }, { 3, 6 } };
	m.set(values);

	/* must match the row-major flat set() above */
	REQUIRE(m[0] == vector<int, 3>(1, 2, 3));
	REQUIRE(m[1] == vector<int, 3>(4, 5, 6));
}

TEST_CASE("matrix<V,C,R>: set_part() copies a whole matrix at an offset", "[matrix]")
{
	matrix<int, 4, 4> dest = matrix<int, 4, 4>::zero;
	matrix<int, 2, 2> src(vector<int, 2>(1, 2), vector<int, 2>(3, 4));

	dest.set_part(src, 1, 1);

	matrix<int, 4, 4> expected = matrix<int, 4, 4>::zero;
	expected[1][1] = 1;
	expected[1][2] = 2;
	expected[2][1] = 3;
	expected[2][2] = 4;

	REQUIRE(dest == expected);
}

TEST_CASE("matrix<V,C,R>: set_part() copies only the given column/row counts", "[matrix]")
{
	matrix<int, 4, 4> dest = matrix<int, 4, 4>::zero;
	matrix<int, 3, 3> src(
		vector<int, 3>(1, 2, 3), vector<int, 3>(4, 5, 6), vector<int, 3>(7, 8, 9));

	/* only copy the top-left 2x2 block of src */
	dest.set_part(src, 2, 2, 0, 0);

	matrix<int, 4, 4> expected = matrix<int, 4, 4>::zero;
	expected[0][0] = 1;
	expected[0][1] = 2;
	expected[1][0] = 4;
	expected[1][1] = 5;

	REQUIRE(dest == expected);
}

TEST_CASE("matrix<V,C,R>: set_part() from a [column][row] array at an offset", "[matrix]")
{
	matrix<int, 4, 3> dest = matrix<int, 4, 3>::zero;

	/* column-major: column 0 = (1,3), column 1 = (2,4) */
	int values[2][2] = { { 1, 3 }, { 2, 4 } };

	dest.set_part(values, 1, 0);

	matrix<int, 4, 3> expected = matrix<int, 4, 3>::zero;
	expected[0][1] = 1;
	expected[1][1] = 3;
	expected[0][2] = 2;
	expected[1][2] = 4;

	REQUIRE(dest == expected);
}

TEST_CASE("matrix<V,C,R>: set_part() at offset (0,0) covering the whole destination", "[matrix]")
{
	matrix<float, 2, 2> dest = matrix<float, 2, 2>::zero;
	matrix<float, 2, 2> src(vector<float, 2>(1, 2), vector<float, 2>(3, 4));

	dest.set_part(src);

	REQUIRE(dest == src);
}
