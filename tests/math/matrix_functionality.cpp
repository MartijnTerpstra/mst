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

// #define MST_MATH_ALL_VECTORS_MATRICES_SIMD 1
#include <mmath2.h>
#include <mcommon.h>

using mst::math::vector;
using mst::math::matrix;
using mst::math::quaternion;

TEST_CASE("matrix<V,C,R>: type support", "[matrix][simd]")
{
	REQUIRE(matrix<float, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<int32_t, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<double, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<long double, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<int64_t, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 4>(0)[0][0] == 0);
}

TEST_CASE("matrix<V,C,R>: constructor", "[matrix][simd]")
{
	vector<float, 3> fposition;
	quaternion<float> fquaternion;

	vector<double, 3> dposition;
	quaternion<double> dquaternion;

	vector<long double, 3> ldposition;
	quaternion<long double> ldquaternion;

	matrix<float, 3, 3>{ fquaternion };
	matrix<float, 3, 4>{ fposition, fquaternion };
	matrix<float, 4, 4>{ fposition, fquaternion };

	matrix<double, 3, 3>{ dquaternion };
	matrix<double, 3, 4>{ dposition, dquaternion };
	matrix<double, 4, 4>{ dposition, dquaternion };

	matrix<long double, 3, 3>{ ldquaternion };
	matrix<long double, 3, 4>{ ldposition, ldquaternion };
	matrix<long double, 4, 4>{ ldposition, ldquaternion };
}

TEST_CASE("matrix<V,C,R>: row constructors set the expected elements", "[matrix]")
{
	typedef vector<float, 4> float4;

	matrix<float, 4, 1> m1(float4(1, 2, 3, 4));
	REQUIRE(m1[0] == float4(1, 2, 3, 4));

	matrix<float, 4, 2> m2(float4(1, 2, 3, 4), float4(5, 6, 7, 8));
	REQUIRE(m2[0] == float4(1, 2, 3, 4));
	REQUIRE(m2[1] == float4(5, 6, 7, 8));

	matrix<float, 4, 3> m3(float4(1, 2, 3, 4), float4(5, 6, 7, 8), float4(9, 10, 11, 12));
	REQUIRE(m3[0] == float4(1, 2, 3, 4));
	REQUIRE(m3[1] == float4(5, 6, 7, 8));
	REQUIRE(m3[2] == float4(9, 10, 11, 12));

	matrix<float, 4, 4> m4(float4(1, 2, 3, 4), float4(5, 6, 7, 8), float4(9, 10, 11, 12),
		float4(13, 14, 15, 16));
	REQUIRE(m4[0] == float4(1, 2, 3, 4));
	REQUIRE(m4[1] == float4(5, 6, 7, 8));
	REQUIRE(m4[2] == float4(9, 10, 11, 12));
	REQUIRE(m4[3] == float4(13, 14, 15, 16));

	typedef vector<float, 3> float3;

	/* matrix<3,4> is 4 rows of 3 (3 orientation rows + 1 position row); matrix<4,4> can be
		built from one plus an explicit 4th column (typically (0,0,0,1)) */
	matrix<float, 3, 4> m34(
		float3(1, 2, 3), float3(5, 6, 7), float3(9, 10, 11), float3(13, 14, 15));
	matrix<float, 4, 4> m4b(m34, float4(4, 8, 12, 16));
	REQUIRE(m4b == m4);
}

TEST_CASE("matrix<V,C,R>: fill constructor sets every element", "[matrix]")
{
	matrix<float, 3, 2> m(4.f);

	for(size_t r = 0; r < 2; ++r)
		for(size_t c = 0; c < 3; ++c)
		{
			REQUIRE(m[r][c] == 4.f);
		}
}

TEST_CASE("matrix<V,C,R>: converting constructor casts every element", "[matrix]")
{
	matrix<double, 3, 2> src(0.0);
	src[0] = { 1.5, 2.5, 3.5 };
	src[1] = { 4.5, 5.5, 6.5 };

	matrix<float, 3, 2> dst(src);

	for(size_t r = 0; r < 2; ++r)
		for(size_t c = 0; c < 3; ++c)
		{
			REQUIRE(dst[r][c] == static_cast<float>(src[r][c]));
		}
}

TEST_CASE("matrix<V,C,R>: zero and identity statics", "[matrix]")
{
	matrix<float, 3, 4> z = matrix<float, 3, 4>::zero;

	for(size_t r = 0; r < 4; ++r)
		for(size_t c = 0; c < 3; ++c)
		{
			REQUIRE(z[r][c] == 0);
		}

	/* identity/get_determinant/get_inverse are only available for signed floating-point
		matrix types (see _GetMatrixBase) -- integral/unsigned matrices only expose zero() */
	matrix<float, 4, 4> id4 = matrix<float, 4, 4>::identity;
	matrix<double, 2, 2> id2 = matrix<double, 2, 2>::identity;
	matrix<double, 1, 1> id1 = matrix<double, 1, 1>::identity;
	matrix<float, 3, 4> id34 = matrix<float, 3, 4>::identity;

	for(size_t r = 0; r < 4; ++r)
		for(size_t c = 0; c < 4; ++c)
		{
			REQUIRE(id4[r][c] == (r == c ? 1.f : 0.f));
		}

	for(size_t r = 0; r < 2; ++r)
		for(size_t c = 0; c < 2; ++c)
		{
			REQUIRE(id2[r][c] == (r == c ? 1.0 : 0.0));
		}

	REQUIRE(id1[0][0] == 1.0);

	/* rows 0-2 (orientation) form a 3x3 identity; row 3 (position) is zero */
	for(size_t r = 0; r < 3; ++r)
		for(size_t c = 0; c < 3; ++c)
		{
			REQUIRE(id34[r][c] == (r == c ? 1.f : 0.f));
		}
	REQUIRE(id34[3] == vector<float, 3>(0, 0, 0));
}

TEST_CASE("matrix<V,C,R>: operator[] reads and writes elements", "[matrix]")
{
	matrix<int, 3, 2> m(0);

	m[0][0] = 1;
	m[0][1] = 2;
	m[0][2] = 3;
	m[1][0] = 4;
	m[1][1] = 5;
	m[1][2] = 6;

	REQUIRE(m[0][0] == 1);
	REQUIRE(m[0][1] == 2);
	REQUIRE(m[0][2] == 3);
	REQUIRE(m[1][0] == 4);
	REQUIRE(m[1][1] == 5);
	REQUIRE(m[1][2] == 6);

	const auto& cm = m;
	REQUIRE(cm[1][2] == 6);
}

TEST_CASE("matrix<V,C,R>: data() and size()", "[matrix]")
{
	matrix<int, 3, 2> m(0);
	m[0] = { 1, 2, 3 };
	m[1] = { 4, 5, 6 };

	REQUIRE(m.size() == 6);

	/* data() is laid out row-major: row0 (columns 0..2), then row1 */
	const int* data = m.data();
	for(int i = 0; i < 6; ++i)
	{
		REQUIRE(data[i] == i + 1);
	}

	matrix<int, 3, 2>& mref = m;
	REQUIRE(mref.data() == data);
}

TEST_CASE("matrix<V,C,R>: begin/end iteration", "[matrix]")
{
	matrix<int, 3, 2> m(0);
	m[0] = { 1, 2, 3 };
	m[1] = { 4, 5, 6 };

	int rowIndex = 0;
	for(auto& row : m)
	{
		REQUIRE(row == m[rowIndex]);
		++rowIndex;
	}
	REQUIRE(rowIndex == 2);

	REQUIRE(std::distance(m.begin(), m.end()) == 2);
	REQUIRE(std::distance(m.cbegin(), m.cend()) == 2);

	const auto& cm = m;
	REQUIRE(std::distance(cm.begin(), cm.end()) == 2);
}
