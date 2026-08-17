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

/* Independent (from-spec, not copied from the library) reference implementations, used to
	cross-check the library's operator* rather than just checking it against itself. */

/* a: M rows x K cols, b: K rows x N cols -> M rows x N cols, standard matrix multiplication */
template<typename V, size_t M, size_t K, size_t N>
matrix<V, N, M> ReferenceMultiply(const matrix<V, K, M>& a, const matrix<V, N, K>& b)
{
	matrix<V, N, M> result = matrix<V, N, M>::zero;

	for(size_t row = 0; row < M; ++row)
		for(size_t col = 0; col < N; ++col)
		{
			V sum = (V)0;
			for(size_t k = 0; k < K; ++k)
			{
				sum += a[row][k] * b[k][col];
			}
			result[row][col] = sum;
		}

	return result;
}

/* v: length K row-vector, m: K rows x N cols -> length N row-vector */
template<typename V, size_t K, size_t N>
vector<V, N> ReferenceMultiply(const vector<V, K>& v, const matrix<V, N, K>& m)
{
	vector<V, N> result = vector<V, N>::zero;

	for(size_t col = 0; col < N; ++col)
	{
		V sum = (V)0;
		for(size_t k = 0; k < K; ++k)
		{
			sum += v[k] * m[k][col];
		}
		result[col] = sum;
	}

	return result;
}

TEST_CASE("matrix<V,C,R>: rectangular multiplication against a hand-computed example", "[matrix]")
{
	/* classic textbook example: [[1,2,3],[4,5,6]] * [[7,8],[9,10],[11,12]] = [[58,64],[139,154]] */
	typedef vector<int, 3> row3;
	typedef vector<int, 2> row2;

	matrix<int, 3, 2> a(row3(1, 2, 3), row3(4, 5, 6));
	matrix<int, 2, 3> b(row2(7, 8), row2(9, 10), row2(11, 12));

	matrix<int, 2, 2> result = a * b;
	matrix<int, 2, 2> expected(row2(58, 64), row2(139, 154));

	REQUIRE(result == expected);
	REQUIRE(result == ReferenceMultiply(a, b));
}

TEST_CASE("matrix<V,C,R>: vector * rectangular matrix against a hand-computed example", "[matrix]")
{
	typedef vector<int, 2> row2;

	vector<int, 3> v(1, 2, 3);
	matrix<int, 2, 3> b(row2(7, 8), row2(9, 10), row2(11, 12));

	vector<int, 2> result = v * b;

	REQUIRE(result == vector<int, 2>(58, 64));
	REQUIRE(result == ReferenceMultiply(v, b));
}

template<typename V, size_t M, size_t K, size_t N>
void TestRectangularMultiplication()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(M);
	CAPTURE(K);
	CAPTURE(N);

	matrix<V, K, M> a = matrix<V, K, M>::zero;
	matrix<V, N, K> b = matrix<V, N, K>::zero;

	V value = (V)1;
	for(size_t r = 0; r < M; ++r)
		for(size_t c = 0; c < K; ++c)
		{
			a[r][c] = value;
			value += 1;
		}

	value = (V)1;
	for(size_t r = 0; r < K; ++r)
		for(size_t c = 0; c < N; ++c)
		{
			b[r][c] = value;
			value += 1;
		}

	REQUIRE(a * b == ReferenceMultiply(a, b));
}

TEST_CASE("matrix<V,C,R>: rectangular multiplication matches reference across shapes", "[matrix]")
{
	TestRectangularMultiplication<float, 2, 3, 4>();
	TestRectangularMultiplication<float, 4, 3, 2>();
	TestRectangularMultiplication<float, 1, 4, 1>();
	TestRectangularMultiplication<float, 3, 1, 3>();

	TestRectangularMultiplication<double, 2, 3, 4>();

	TestRectangularMultiplication<int32_t, 2, 3, 4>();
	TestRectangularMultiplication<uint32_t, 4, 2, 3>();
	TestRectangularMultiplication<int64_t, 3, 4, 2>();
}

template<typename V, size_t N>
void TestSquareMultiplication()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(N);

	typedef matrix<V, N, N> mat;

	mat left = mat::zero;
	mat right = mat::zero;

	V value = (V)1;
	for(size_t r = 0; r < N; ++r)
		for(size_t c = 0; c < N; ++c)
		{
			left[r][c] = value;
			right[r][c] = value + (V)1;
			value += 1;
		}

	REQUIRE(left * right == ReferenceMultiply(left, right));
	REQUIRE(right * left == ReferenceMultiply(right, left));
}

TEST_CASE("matrix<V,C,R>: square multiplication matches reference", "[matrix]")
{
	TestSquareMultiplication<float, 1>();
	TestSquareMultiplication<float, 2>();
	TestSquareMultiplication<float, 3>();
	TestSquareMultiplication<float, 4>();

	TestSquareMultiplication<double, 2>();
	TestSquareMultiplication<double, 3>();
	TestSquareMultiplication<double, 4>();

	TestSquareMultiplication<int32_t, 4>();
	TestSquareMultiplication<uint32_t, 4>();
	TestSquareMultiplication<int64_t, 4>();
}

template<typename V, size_t N, size_t M>
void TestIdentityMultiplication()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(N);
	CAPTURE(M);

	/* A is N cols x M rows, i.e. an M x N matrix */
	matrix<V, N, M> a = matrix<V, N, M>::zero;

	V value = (V)1;
	for(size_t r = 0; r < M; ++r)
		for(size_t c = 0; c < N; ++c)
		{
			a[r][c] = value;
			value += 1;
		}

	REQUIRE(a * matrix<V, N, N>::identity == a);
	REQUIRE(matrix<V, M, M>::identity * a == a);
}

TEST_CASE("matrix<V,C,R>: multiplying by identity is a no-op", "[matrix]")
{
	TestIdentityMultiplication<float, 4, 4>();
	TestIdentityMultiplication<float, 3, 4>();
	TestIdentityMultiplication<float, 4, 3>();
	TestIdentityMultiplication<float, 2, 3>();
	TestIdentityMultiplication<double, 3, 4>();
	TestIdentityMultiplication<long double, 4, 4>();
}

TEST_CASE("matrix<V,C,R>: multiplication is associative", "[matrix]")
{
	typedef matrix<float, 4, 4> mat4;
	typedef matrix<double, 3, 3> dmat3;

	mat4 a(vector<float, 4>(1, 2, 0, 3), vector<float, 4>(0, 1, 4, 1), vector<float, 4>(2, 0, 1, 0),
		vector<float, 4>(1, 1, 1, 1));
	mat4 b(vector<float, 4>(2, 0, 1, 1), vector<float, 4>(1, 3, 0, 2), vector<float, 4>(0, 1, 2, 0),
		vector<float, 4>(1, 0, 0, 1));
	mat4 c(vector<float, 4>(1, 1, 0, 0), vector<float, 4>(0, 2, 1, 0), vector<float, 4>(1, 0, 3, 1),
		vector<float, 4>(0, 1, 0, 2));

	REQUIRE_THAT((a * b) * c, mst::test_util::approx_equal(a * (b * c), 0.0001f));

	dmat3 x(vector<double, 3>(1, 2, 3), vector<double, 3>(0, 1, 4), vector<double, 3>(5, 6, 0));
	dmat3 y(vector<double, 3>(2, 0, 1), vector<double, 3>(1, 1, 0), vector<double, 3>(0, 3, 2));
	dmat3 z(vector<double, 3>(1, 0, 2), vector<double, 3>(3, 1, 1), vector<double, 3>(0, 2, 1));

	REQUIRE_THAT((x * y) * z, mst::test_util::approx_equal(x * (y * z), 0.0000001));
}

TEST_CASE("matrix<V,C,R>: vector * identity is a no-op", "[matrix]")
{
	vector<float, 4> v(1, 2, 3, 4);
	REQUIRE((v * matrix<float, 4, 4>::identity) == v);

	vector<double, 3> dv(5, 6, 7);
	REQUIRE((dv * matrix<double, 3, 3>::identity) == dv);
}

TEST_CASE("matrix<V,C,R>: vector * matrix matches reference across shapes", "[matrix]")
{
	{
		vector<float, 3> v(1, 2, 3);
		matrix<float, 4, 3> m(vector<float, 4>(1, 2, 3, 4), vector<float, 4>(5, 6, 7, 8),
			vector<float, 4>(9, 10, 11, 12));

		REQUIRE(v * m == ReferenceMultiply(v, m));
	}
	{
		vector<int, 4> v(1, 2, 3, 4);
		matrix<int, 2, 4> m(vector<int, 2>(1, 2), vector<int, 2>(3, 4), vector<int, 2>(5, 6),
			vector<int, 2>(7, 8));

		REQUIRE(v * m == ReferenceMultiply(v, m));
	}
}
