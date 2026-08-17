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

template<typename V, size_t Columns, size_t Rows>
void TestEquality()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(Columns);
	CAPTURE(Rows);

	typedef matrix<V, Columns, Rows> mat;

	mat a = mat::zero;
	V value = (V)1;
	for(size_t r = 0; r < Rows; ++r)
		for(size_t c = 0; c < Columns; ++c)
		{
			a[r][c] = value;
			value += 1;
		}

	mat b = a;

	REQUIRE(a == a);
	REQUIRE(a == b);
	REQUIRE_FALSE(a != b);

	/* flipping any single element must break equality */
	for(size_t r = 0; r < Rows; ++r)
		for(size_t c = 0; c < Columns; ++c)
		{
			mat modified = a;
			modified[r][c] += (V)1;

			REQUIRE(modified != a);
			REQUIRE_FALSE(modified == a);
		}
}

TEST_CASE("matrix<V,C,R>: operator== / operator!=", "[matrix]")
{
	TestEquality<float, 1, 1>();
	TestEquality<float, 2, 2>();
	TestEquality<float, 3, 3>();
	TestEquality<float, 4, 4>();
	TestEquality<float, 2, 4>();
	TestEquality<float, 4, 2>();
	TestEquality<float, 3, 4>();

	TestEquality<double, 3, 3>();
	TestEquality<double, 4, 4>();

	TestEquality<int32_t, 3, 3>();
	TestEquality<int32_t, 4, 4>();
	TestEquality<int32_t, 2, 3>();

	TestEquality<uint32_t, 4, 4>();
	TestEquality<int64_t, 4, 4>();
}

TEST_CASE("matrix<V,C,R>: zero is not equal to identity", "[matrix]")
{
	REQUIRE(matrix<float, 4, 4>::zero != matrix<float, 4, 4>::identity);
	REQUIRE(matrix<double, 3, 3>::zero != matrix<double, 3, 3>::identity);
	REQUIRE_FALSE(matrix<float, 4, 4>::zero == matrix<float, 4, 4>::identity);
}
