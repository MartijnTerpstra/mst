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
using mst::math::quaternion;
using mst::math::degrees;
using mst::math::euler_rotation_order;

TEST_CASE("matrix<V,C,R>: direction getters on identity", "[matrix]")
{
	typedef matrix<float, 4, 4> mat;
	typedef vector<float, 3> float3;

	auto id = mat::identity;

	REQUIRE(id.get_right_direction() == float3(1, 0, 0));
	REQUIRE(id.get_left_direction() == float3(-1, 0, 0));
	REQUIRE(id.get_up_direction() == float3(0, 1, 0));
	REQUIRE(id.get_down_direction() == float3(0, -1, 0));
	REQUIRE(id.get_forward_direction() == float3(0, 0, 1));
	REQUIRE(id.get_backward_direction() == float3(0, 0, -1));
}

TEST_CASE("matrix<V,C,R>: set_orientation with identity quaternion is identity", "[matrix]")
{
	matrix<float, 3, 3> m;
	m.set_orientation(quaternion<float>());

	REQUIRE_THAT(m, mst::test_util::approx_equal(matrix<float, 3, 3>::identity, 0.00001f));
}

TEST_CASE("matrix<V,C,R>: set_orientation matches the quaternion constructor", "[matrix]")
{
	typedef vector<float, 3> float3;

	quaternion<float> q(degrees<float>(73), float3(1, 2, 3).normalized());

	matrix<float, 3, 3> viaConstructor(q);

	matrix<float, 3, 3> viaSetter;
	viaSetter.set_orientation(q);

	REQUIRE_THAT(viaSetter, mst::test_util::approx_equal(viaConstructor, 0.00001f));
}

TEST_CASE("matrix<V,C,R>: rotated_z rotates the right/up basis", "[matrix]")
{
	typedef vector<float, 3> float3;

	auto rotated = matrix<float, 4, 4>::identity.rotated_z(degrees<float>(90));

	REQUIRE_THAT(rotated.get_right_direction(),
		mst::test_util::approx_equal(float3(0, 1, 0), 0.0001f));
	REQUIRE_THAT(
		rotated.get_up_direction(), mst::test_util::approx_equal(float3(-1, 0, 0), 0.0001f));
	REQUIRE_THAT(rotated.get_forward_direction(),
		mst::test_util::approx_equal(float3(0, 0, 1), 0.0001f));
}

TEST_CASE("matrix<V,C,R>: rotated_x/y/z round-trip with their negative angle", "[matrix]")
{
	typedef matrix<float, 4, 4> mat;

	auto original = mat::identity.rotated_z(degrees<float>(35)).rotated_x(degrees<float>(15));

	auto roundTripX = original.rotated_x(degrees<float>(20)).rotated_x(degrees<float>(-20));
	auto roundTripY = original.rotated_y(degrees<float>(20)).rotated_y(degrees<float>(-20));
	auto roundTripZ = original.rotated_z(degrees<float>(20)).rotated_z(degrees<float>(-20));

	REQUIRE_THAT(roundTripX, mst::test_util::approx_equal(original, 0.0001f));
	REQUIRE_THAT(roundTripY, mst::test_util::approx_equal(original, 0.0001f));
	REQUIRE_THAT(roundTripZ, mst::test_util::approx_equal(original, 0.0001f));
}

TEST_CASE("matrix<V,C,R>: rotated() applies each non-zero axis in order", "[matrix]")
{
	typedef matrix<float, 4, 4> mat;

	auto expected = mat::identity.rotated_x(degrees<float>(10))
						.rotated_y(degrees<float>(20))
						.rotated_z(degrees<float>(30));

	auto actual = mat::identity.rotated(
		degrees<float>(10), degrees<float>(20), degrees<float>(30), euler_rotation_order::xyz);

	REQUIRE_THAT(actual, mst::test_util::approx_equal(expected, 0.0001f));
}

TEST_CASE("matrix<V,C,R>: rotated() skips axes with a zero angle", "[matrix]")
{
	typedef matrix<float, 4, 4> mat;

	/* only the y axis has a non-zero angle -- rotated() must behave like rotated_y() alone */
	auto expected = mat::identity.rotated_y(degrees<float>(42));

	auto actual = mat::identity.rotated(
		degrees<float>(0), degrees<float>(42), degrees<float>(0), euler_rotation_order::xyz);

	REQUIRE_THAT(actual, mst::test_util::approx_equal(expected, 0.0001f));

	/* an all-zero rotation must be a no-op */
	auto noop = mat::identity.rotated(
		degrees<float>(0), degrees<float>(0), degrees<float>(0), euler_rotation_order::zyx);

	REQUIRE_THAT(noop, mst::test_util::approx_equal(mat::identity, 0.0001f));
}

TEST_CASE("matrix<V,C,R>: set_position / get_position / translate", "[matrix]")
{
	typedef vector<float, 3> float3;

	matrix<float, 4, 4> m = matrix<float, 4, 4>::identity;

	REQUIRE(m.get_position() == float3(0, 0, 0));

	m.set_position(float3(1, 2, 3));
	REQUIRE(m.get_position() == float3(1, 2, 3));

	m.translate(float3(1, 1, 1));
	REQUIRE(m.get_position() == float3(2, 3, 4));

	/* matrix<3,4> (the compact affine type) supports the same position API */
	matrix<float, 3, 4> m34 = matrix<float, 3, 4>::identity;
	m34.set_position(float3(5, 6, 7));
	REQUIRE(m34.get_position() == float3(5, 6, 7));
}

TEST_CASE("matrix<V,C,R>: position + orientation constructor", "[matrix]")
{
	typedef vector<float, 3> float3;

	float3 position(3, -1, 4);
	quaternion<float> orientation(degrees<float>(30), float3(0, 1, 0));

	matrix<float, 4, 4> m(position, orientation);

	REQUIRE(m.get_position() == position);

	matrix<float, 3, 3> expectedOrientation(orientation);
	REQUIRE_THAT(m.get_right_direction(),
		mst::test_util::approx_equal(expectedOrientation.get_right_direction(), 0.0001f));
	REQUIRE_THAT(m.get_up_direction(),
		mst::test_util::approx_equal(expectedOrientation.get_up_direction(), 0.0001f));

	/* matrix<3,4> supports the same position+orientation constructor */
	matrix<float, 3, 4> m34(position, orientation);
	REQUIRE(m34.get_position() == position);
}

TEST_CASE("matrix<V,C,R>: lookat sets forward/right/up towards the target", "[matrix]")
{
	typedef vector<float, 3> float3;

	matrix<float, 4, 4> m = matrix<float, 4, 4>::identity;
	m.lookat(float3(0, 0, 5), float3(0, 1, 0));

	REQUIRE_THAT(
		m.get_forward_direction(), mst::test_util::approx_equal(float3(0, 0, 1), 0.0001f));
	REQUIRE_THAT(m.get_right_direction(), mst::test_util::approx_equal(float3(1, 0, 0), 0.0001f));
	REQUIRE_THAT(m.get_up_direction(), mst::test_util::approx_equal(float3(0, 1, 0), 0.0001f));

	matrix<float, 4, 4> m2 = matrix<float, 4, 4>::identity;
	m2.lookat(float3(5, 0, 0), float3(0, 1, 0));

	REQUIRE_THAT(
		m2.get_forward_direction(), mst::test_util::approx_equal(float3(1, 0, 0), 0.0001f));
	REQUIRE_THAT(
		m2.get_right_direction(), mst::test_util::approx_equal(float3(0, 0, -1), 0.0001f));
	REQUIRE_THAT(m2.get_up_direction(), mst::test_util::approx_equal(float3(0, 1, 0), 0.0001f));
}

TEST_CASE("matrix<V,C,R>: scale pre-multiplies by a diagonal scale matrix", "[matrix]")
{
	/* scale() is documented (mx_math_matrix.inl) to compute `scaleMatrix * *this` -- verify it
		against that same computation done via the (independently tested) operator*, rather than
		hand-deriving the resulting element layout here. */
	typedef vector<float, 3> float3;
	typedef vector<float, 4> float4;
	typedef matrix<float, 4, 4> mat4;

	mat4 original = mat4::identity;
	original.set_position(float3(1, 2, 3));
	original = original.rotated_z(degrees<float>(20));

	mat4 scaleMatrix(
		float4(2, 0, 0, 0), float4(0, 3, 0, 0), float4(0, 0, 4, 0), float4(0, 0, 0, 1));

	mat4 expected = scaleMatrix * original;

	mat4 actual = original;
	actual.scale(float3(2, 3, 4));

	REQUIRE_THAT(actual, mst::test_util::approx_equal(expected, 0.0001f));

	mat4 uniformScaleMatrix(
		float4(2, 0, 0, 0), float4(0, 2, 0, 0), float4(0, 0, 2, 0), float4(0, 0, 0, 1));

	mat4 uniformExpected = uniformScaleMatrix * original;

	mat4 uniformActual = original;
	uniformActual.scale(2.f);

	REQUIRE_THAT(uniformActual, mst::test_util::approx_equal(uniformExpected, 0.0001f));
}
