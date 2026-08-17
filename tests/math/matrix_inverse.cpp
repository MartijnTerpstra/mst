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

/* get_minor / get_determinant / get_inverse only exist for square, signed floating-point
	matrices (see _GetMatrixBase in mx_math_matrix.h) */

TEST_CASE("matrix<V,C,R>: get_minor removes the given column and row", "[matrix]")
{
	typedef vector<float, 3> row3;
	matrix<float, 3, 3> m(row3(1, 2, 3), row3(4, 5, 6), row3(7, 8, 9));

	auto m00 = m.get_minor(0, 0);
	REQUIRE(m00 == matrix<float, 2, 2>(vector<float, 2>(5, 6), vector<float, 2>(8, 9)));

	auto m12 = m.get_minor(1, 2);
	REQUIRE(m12 == matrix<float, 2, 2>(vector<float, 2>(1, 3), vector<float, 2>(4, 6)));

	auto m22 = m.get_minor(2, 2);
	REQUIRE(m22 == matrix<float, 2, 2>(vector<float, 2>(1, 2), vector<float, 2>(4, 5)));
}

TEST_CASE("matrix<V,C,R>: get_determinant of a 1x1 matrix", "[matrix]")
{
	REQUIRE(matrix<float, 1, 1>(7.f).get_determinant() == 7.f);
	REQUIRE(matrix<double, 1, 1>(-3.0).get_determinant() == -3.0);
}

TEST_CASE("matrix<V,C,R>: get_determinant against hand-computed examples", "[matrix]")
{
	/* det([[4,7],[2,6]]) = 4*6 - 7*2 = 10 */
	matrix<float, 2, 2> m2(vector<float, 2>(4, 7), vector<float, 2>(2, 6));
	REQUIRE(m2.get_determinant() == 10.f);

	/* det of the tridiagonal [[2,-1,0],[-1,2,-1],[0,-1,2]] is 4 */
	matrix<double, 3, 3> m3(vector<double, 3>(2, -1, 0), vector<double, 3>(-1, 2, -1),
		vector<double, 3>(0, -1, 2));
	REQUIRE(m3.get_determinant() == 4.0);

	/* the determinant of an (upper) triangular matrix is the product of its diagonal */
	matrix<float, 4, 4> triangular(vector<float, 4>(2, 1, 3, 4), vector<float, 4>(0, 3, 1, 2),
		vector<float, 4>(0, 0, 4, 5), vector<float, 4>(0, 0, 0, 5));
	REQUIRE(triangular.get_determinant() == 2.f * 3.f * 4.f * 5.f);
}

TEST_CASE("matrix<V,C,R>: get_determinant of identity is 1", "[matrix]")
{
	REQUIRE(matrix<float, 1, 1>::identity.get_determinant() == 1.f);
	REQUIRE(matrix<float, 2, 2>::identity.get_determinant() == 1.f);
	REQUIRE(matrix<float, 3, 3>::identity.get_determinant() == 1.f);
	REQUIRE(matrix<float, 4, 4>::identity.get_determinant() == 1.f);
}

TEST_CASE("matrix<V,C,R>: get_inverse of identity is identity", "[matrix]")
{
	REQUIRE(matrix<float, 1, 1>::identity.get_inverse() == matrix<float, 1, 1>::identity);
	REQUIRE(matrix<float, 2, 2>::identity.get_inverse() == matrix<float, 2, 2>::identity);
	REQUIRE(matrix<float, 3, 3>::identity.get_inverse() == matrix<float, 3, 3>::identity);
	REQUIRE(matrix<float, 4, 4>::identity.get_inverse() == matrix<float, 4, 4>::identity);
}

TEST_CASE("matrix<V,C,R>: get_inverse against a hand-computed example", "[matrix]")
{
	/* inverse of [[4,7],[2,6]] is (1/10)*[[6,-7],[-2,4]] */
	matrix<float, 2, 2> m(vector<float, 2>(4, 7), vector<float, 2>(2, 6));
	matrix<float, 2, 2> expected(vector<float, 2>(.6f, -.7f), vector<float, 2>(-.2f, .4f));

	REQUIRE_THAT(m.get_inverse(), mst::test_util::approx_equal(expected, 0.0001f));
}

template<typename V, size_t N>
void TestInverseRoundTrip(const matrix<V, N, N>& m)
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(N);

	auto inv = m.get_inverse();

	REQUIRE_THAT(m * inv, mst::test_util::approx_equal(matrix<V, N, N>::identity, (V)0.0005));
	REQUIRE_THAT(inv * m, mst::test_util::approx_equal(matrix<V, N, N>::identity, (V)0.0005));
}

TEST_CASE("matrix<V,C,R>: get_inverse round-trips to identity", "[matrix]")
{
	TestInverseRoundTrip<float, 1>(matrix<float, 1, 1>(4.f));

	TestInverseRoundTrip<float, 2>(
		matrix<float, 2, 2>(vector<float, 2>(4, 7), vector<float, 2>(2, 6)));

	TestInverseRoundTrip<double, 3>(matrix<double, 3, 3>(vector<double, 3>(2, -1, 0),
		vector<double, 3>(-1, 2, -1), vector<double, 3>(0, -1, 2)));

	TestInverseRoundTrip<float, 4>(matrix<float, 4, 4>(vector<float, 4>(5, 1, 0, 2),
		vector<float, 4>(1, 6, 2, 0), vector<float, 4>(0, 1, 7, 3), vector<float, 4>(2, 0, 1, 8)));

	TestInverseRoundTrip<double, 4>(matrix<double, 4, 4>(vector<double, 4>(3, 0, 2, -1),
		vector<double, 4>(1, 2, 0, -2), vector<double, 4>(4, 0, 6, -3),
		vector<double, 4>(5, 0, 2, 0)));
}
