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
void TestTranspose()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(Columns);
	CAPTURE(Rows);

	matrix<V, Columns, Rows> m = matrix<V, Columns, Rows>::zero;

	V value = (V)1;
	for(size_t r = 0; r < Rows; ++r)
		for(size_t c = 0; c < Columns; ++c)
		{
			m[r][c] = value;
			value += 1;
		}

	matrix<V, Rows, Columns> t = m.get_transpose();

	for(size_t r = 0; r < Rows; ++r)
		for(size_t c = 0; c < Columns; ++c)
		{
			REQUIRE(t[c][r] == m[r][c]);
		}

	/* transposing twice yields the original matrix */
	REQUIRE(t.get_transpose() == m);
}

TEST_CASE("matrix<V,C,R>: get_transpose", "[matrix]")
{
	TestTranspose<float, 1, 1>();
	TestTranspose<float, 2, 2>();
	TestTranspose<float, 3, 3>();
	TestTranspose<float, 4, 4>();
	TestTranspose<float, 2, 4>();
	TestTranspose<float, 4, 2>();
	TestTranspose<float, 3, 4>();
	TestTranspose<float, 4, 3>();

	TestTranspose<double, 4, 4>();
	TestTranspose<int32_t, 3, 4>();
	TestTranspose<uint32_t, 4, 4>();
	TestTranspose<int64_t, 2, 3>();
}

TEST_CASE("matrix<V,C,R>: get_transpose of identity is identity", "[matrix]")
{
	REQUIRE(matrix<float, 4, 4>::identity.get_transpose() == matrix<float, 4, 4>::identity);
	REQUIRE(matrix<double, 3, 3>::identity.get_transpose() == matrix<double, 3, 3>::identity);
}

template<typename V, size_t Columns, size_t Rows>
void TestRowColumn()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(Columns);
	CAPTURE(Rows);

	matrix<V, Columns, Rows> m = matrix<V, Columns, Rows>::zero;

	V value = (V)1;
	for(size_t r = 0; r < Rows; ++r)
		for(size_t c = 0; c < Columns; ++c)
		{
			m[r][c] = value;
			value += 1;
		}

	for(size_t r = 0; r < Rows; ++r)
	{
		auto row = m.get_row(r);
		REQUIRE(row == m[r]);
		for(size_t c = 0; c < Columns; ++c)
		{
			REQUIRE(row[c] == m[r][c]);
		}
	}

	for(size_t c = 0; c < Columns; ++c)
	{
		auto column = m.get_column(c);
		for(size_t r = 0; r < Rows; ++r)
		{
			REQUIRE(column[r] == m[r][c]);
		}
	}
}

TEST_CASE("matrix<V,C,R>: get_row / get_column", "[matrix]")
{
	TestRowColumn<float, 1, 1>();
	TestRowColumn<float, 2, 2>();
	TestRowColumn<float, 3, 3>();
	TestRowColumn<float, 4, 4>();
	TestRowColumn<float, 2, 4>();
	TestRowColumn<float, 4, 2>();
	TestRowColumn<float, 3, 4>();
	TestRowColumn<float, 4, 3>();

	TestRowColumn<double, 4, 4>();
	TestRowColumn<int32_t, 3, 4>();
	TestRowColumn<uint32_t, 4, 4>();
	TestRowColumn<int64_t, 2, 3>();
}
