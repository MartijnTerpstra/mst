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

typedef vector<float, 3> float3;
typedef quaternion<float> quatf;

TEST_CASE("quaternion<V>: default constructor is the identity rotation", "[quaternion]")
{
	quatf q;

	REQUIRE_THAT(q, mst::test_util::approx_equal(quatf::identity, 0.0001f));
	REQUIRE(q.w == 1.0f);
	REQUIRE(q.x == 0.0f);
	REQUIRE(q.y == 0.0f);
	REQUIRE(q.z == 0.0f);
}

TEST_CASE("quaternion<V>: component constructor sets w,x,y,z directly", "[quaternion]")
{
	quatf q(1, 2, 3, 4);

	REQUIRE(q.w == 1);
	REQUIRE(q.x == 2);
	REQUIRE(q.y == 3);
	REQUIRE(q.z == 4);

	/* operator[] indexes in the same w,x,y,z order */
	REQUIRE(q[0] == 1);
	REQUIRE(q[1] == 2);
	REQUIRE(q[2] == 3);
	REQUIRE(q[3] == 4);
}

TEST_CASE("quaternion<V>: single-value constructor broadcasts to every component", "[quaternion]")
{
	quatf q(5.0f);

	REQUIRE(q.w == 5.0f);
	REQUIRE(q.x == 5.0f);
	REQUIRE(q.y == 5.0f);
	REQUIRE(q.z == 5.0f);
}

TEST_CASE("quaternion<V>: array constructor matches the w,x,y,z component order", "[quaternion]")
{
	const float values[4] = { 1, 2, 3, 4 };
	quatf q(values);

	REQUIRE(q.w == 1);
	REQUIRE(q.x == 2);
	REQUIRE(q.y == 3);
	REQUIRE(q.z == 4);
}

TEST_CASE("quaternion<V>: converting constructor casts between value types", "[quaternion]")
{
	quaternion<double> qd(1.5, 2.5, 3.5, 4.5);
	quatf qf(qd);

	REQUIRE(qf.w == 1.5f);
	REQUIRE(qf.x == 2.5f);
	REQUIRE(qf.y == 3.5f);
	REQUIRE(qf.z == 4.5f);
}

TEST_CASE("quaternion<V>: axis-angle constructor matches the closed-form rotation quaternion",
	"[quaternion]")
{
	/* a 90 degree rotation around the y axis: w = cos(45deg), y = sin(45deg) */
	quatf q(degrees<float>(90), float3(0, 1, 0));

	const float halfSqrt2 = 0.70710678f;

	REQUIRE_THAT(q, mst::test_util::approx_equal(quatf(halfSqrt2, 0, halfSqrt2, 0), 0.0001f));
}

TEST_CASE(
	"quaternion<V>: matrix constructor round-trips with the axis-angle constructor", "[quaternion]")
{
	quatf q(degrees<float>(73), float3(1, 2, 3).normalized());

	matrix<float, 3, 3> m(q);
	quatf roundTripped(m);

	/* a quaternion and its negation represent the same rotation (double cover) -- normalize the
		sign before comparing */
	if(roundTripped.dot(q) < 0)
	{
		roundTripped = -roundTripped;
	}

	REQUIRE_THAT(roundTripped, mst::test_util::approx_equal(q, 0.0001f));
}

TEST_CASE("quaternion<V>: identity static member", "[quaternion]")
{
	REQUIRE(quatf::identity.w == 1.0f);
	REQUIRE(quatf::identity.x == 0.0f);
	REQUIRE(quatf::identity.y == 0.0f);
	REQUIRE(quatf::identity.z == 0.0f);
}

TEST_CASE("quaternion<V>: get_conjugate negates the vector part", "[quaternion]")
{
	quatf q(1, 2, 3, 4);

	REQUIRE_THAT(q.get_conjugate(), mst::test_util::approx_equal(quatf(1, -2, -3, -4), 0.0001f));

	/* the conjugate of the conjugate is the original quaternion */
	REQUIRE_THAT(q.get_conjugate().get_conjugate(), mst::test_util::approx_equal(q, 0.0001f));
}

TEST_CASE("quaternion<V>: get_inverse is the conjugate scaled by 1/squared_length", "[quaternion]")
{
	quatf q(1, 2, 3, 4);

	quatf expected = q.get_conjugate();
	expected *= 1.0f / q.squared_length();

	REQUIRE_THAT(q.get_inverse(), mst::test_util::approx_equal(expected, 0.0001f));

	/* for a unit quaternion the inverse equals the conjugate */
	quatf unitQ = q.normalized();
	REQUIRE_THAT(unitQ.get_inverse(), mst::test_util::approx_equal(unitQ.get_conjugate(), 0.0001f));
}

TEST_CASE("quaternion<V>: identity's inverse and conjugate are itself", "[quaternion]")
{
	REQUIRE_THAT(
		quatf::identity.get_conjugate(), mst::test_util::approx_equal(quatf::identity, 0.0001f));
	REQUIRE_THAT(
		quatf::identity.get_inverse(), mst::test_util::approx_equal(quatf::identity, 0.0001f));
}

TEST_CASE("quaternion<V>: direction getters on identity", "[quaternion]")
{
	REQUIRE_THAT(quatf::identity.get_right_direction(),
		mst::test_util::approx_equal(float3(1, 0, 0), 0.0001f));
	REQUIRE_THAT(quatf::identity.get_left_direction(),
		mst::test_util::approx_equal(float3(-1, 0, 0), 0.0001f));
	REQUIRE_THAT(
		quatf::identity.get_up_direction(), mst::test_util::approx_equal(float3(0, 1, 0), 0.0001f));
	REQUIRE_THAT(quatf::identity.get_down_direction(),
		mst::test_util::approx_equal(float3(0, -1, 0), 0.0001f));
	REQUIRE_THAT(quatf::identity.get_forward_direction(),
		mst::test_util::approx_equal(float3(0, 0, 1), 0.0001f));
	REQUIRE_THAT(quatf::identity.get_backward_direction(),
		mst::test_util::approx_equal(float3(0, 0, -1), 0.0001f));
}

TEST_CASE("quaternion<V>: rotate_point matches the direction getters", "[quaternion]")
{
	/* rotate_point() rotates the local axes into the quaternion's basis vectors, matching the
		identity direction getters above */
	quatf q(degrees<float>(37), float3(1, -2, 3).normalized());

	REQUIRE_THAT(q.rotate_point(float3(1, 0, 0)),
		mst::test_util::approx_equal(q.get_right_direction(), 0.0001f));
	REQUIRE_THAT(q.rotate_point(float3(-1, 0, 0)),
		mst::test_util::approx_equal(q.get_left_direction(), 0.0001f));
	REQUIRE_THAT(q.rotate_point(float3(0, 1, 0)),
		mst::test_util::approx_equal(q.get_up_direction(), 0.0001f));
	REQUIRE_THAT(q.rotate_point(float3(0, -1, 0)),
		mst::test_util::approx_equal(q.get_down_direction(), 0.0001f));
	REQUIRE_THAT(q.rotate_point(float3(0, 0, 1)),
		mst::test_util::approx_equal(q.get_forward_direction(), 0.0001f));
	REQUIRE_THAT(q.rotate_point(float3(0, 0, -1)),
		mst::test_util::approx_equal(q.get_backward_direction(), 0.0001f));
}

TEST_CASE("quaternion<V>: rotate_point preserves vector length", "[quaternion]")
{
	quatf q(degrees<float>(51), float3(2, 1, -1).normalized());
	float3 v(3, -4, 5);

	float3 rotated = q.rotate_point(v);

	REQUIRE(std::fabs(rotated.length() - v.length()) < 0.0001f);
}

TEST_CASE(
	"quaternion<V>: to_axis_angle round-trips with the axis-angle constructor", "[quaternion]")
{
	float3 axis = float3(1, 2, -2).normalized();
	degrees<float> angle(120);

	quatf q(angle, axis);

	vector<float, 4> axisAngle = q.to_axis_angle();

	REQUIRE_THAT(
		float3(axisAngle.x, axisAngle.y, axisAngle.z), mst::test_util::approx_equal(axis, 0.0001f));
	REQUIRE(std::fabs(axisAngle.w - mst::math::radians<float>(angle).count()) < 0.0001f);
}

TEST_CASE("quaternion<V>: to_axis_angle on identity returns a zero angle", "[quaternion]")
{
	vector<float, 4> axisAngle = quatf::identity.to_axis_angle();

	REQUIRE(axisAngle.w == 0.0f);
	REQUIRE(axisAngle.x == 0.0f);
	REQUIRE(axisAngle.y == 0.0f);
	REQUIRE(axisAngle.z == 0.0f);
}

TEST_CASE("quaternion<V>: normalize / normalized produce a unit quaternion", "[quaternion]")
{
	quatf q(1, 2, 3, 4);

	quatf normalized = q.normalized();
	REQUIRE(std::fabs(normalized.length() - 1.0f) < 0.0001f);

	/* normalized() does not mutate the original */
	REQUIRE(q.w == 1);
	REQUIRE(q.x == 2);

	/* normalize() mutates in place and returns a reference to itself */
	quatf& selfRef = q.normalize();
	REQUIRE(&selfRef == &q);
	REQUIRE(std::fabs(q.length() - 1.0f) < 0.0001f);
	REQUIRE_THAT(q, mst::test_util::approx_equal(normalized, 0.0001f));
}

TEST_CASE("quaternion<V>: length and squared_length are consistent", "[quaternion]")
{
	quatf q(1, 2, 3, 4);

	REQUIRE(q.squared_length() == 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4);
	REQUIRE(std::fabs(q.length() * q.length() - q.squared_length()) < 0.0001f);
}

TEST_CASE("quaternion<V>: rotate / rotated combine rotations via quaternion multiplication",
	"[quaternion]")
{
	quatf q(degrees<float>(40), float3(0, 1, 0));

	/* rotating the identity by q results in q itself */
	quatf rotatedIdentity = quatf::identity.rotated(q);
	REQUIRE_THAT(rotatedIdentity, mst::test_util::approx_equal(q, 0.0001f));

	quatf mutated = quatf::identity;
	mutated.rotate(q);
	REQUIRE_THAT(mutated, mst::test_util::approx_equal(q, 0.0001f));

	/* rotated() does not mutate the original */
	quatf original = quatf::identity;
	original.rotated(q);
	REQUIRE_THAT(original, mst::test_util::approx_equal(quatf::identity, 0.0001f));

	/* combining two rotations and applying it to a point matches applying them one at a time */
	quatf q2(degrees<float>(25), float3(1, 0, 0));
	quatf combined = quatf::identity;
	combined.rotate(q);
	combined.rotate(q2);

	float3 v(1, 2, 3);
	REQUIRE_THAT(combined.rotate_point(v),
		mst::test_util::approx_equal(q2.rotate_point(q.rotate_point(v)), 0.0001f));
}

TEST_CASE("quaternion<V>: look_at orients forward toward the target", "[quaternion]")
{
	quatf q;
	q.look_at(float3(0, 0, 5), float3(0, 1, 0));

	REQUIRE_THAT(q.get_forward_direction(), mst::test_util::approx_equal(float3(0, 0, 1), 0.0001f));
	REQUIRE_THAT(q.get_right_direction(), mst::test_util::approx_equal(float3(1, 0, 0), 0.0001f));
	REQUIRE_THAT(q.get_up_direction(), mst::test_util::approx_equal(float3(0, 1, 0), 0.0001f));

	quatf q2;
	q2.look_at(float3(5, 0, 0), float3(0, 1, 0));

	REQUIRE_THAT(
		q2.get_forward_direction(), mst::test_util::approx_equal(float3(1, 0, 0), 0.0001f));
	REQUIRE_THAT(q2.get_right_direction(), mst::test_util::approx_equal(float3(0, 0, -1), 0.0001f));
}

TEST_CASE("quaternion<V>: dot product", "[quaternion]")
{
	quatf left(1, 2, 3, 4);
	quatf right(5, 6, 7, 8);

	REQUIRE(left.dot(right) == 1 * 5 + 2 * 6 + 3 * 7 + 4 * 8);

	/* dot product with itself equals the squared length */
	REQUIRE(left.dot(left) == left.squared_length());

	/* dot product is commutative */
	REQUIRE(left.dot(right) == right.dot(left));
}

TEST_CASE("quaternion<V>: slerp interpolates between two orientations", "[quaternion]")
{
	quatf from = quatf::identity;
	quatf to(degrees<float>(90), float3(0, 1, 0));

	REQUIRE_THAT(from.slerp(to, 0.0f), mst::test_util::approx_equal(from, 0.0001f));
	REQUIRE_THAT(from.slerp(to, 1.0f), mst::test_util::approx_equal(to, 0.0001f));

	/* interpolating a quaternion with itself returns itself at every step */
	REQUIRE_THAT(to.slerp(to, 0.5f), mst::test_util::approx_equal(to, 0.0001f));

	/* the halfway point is a 45 degree rotation, and stays a unit quaternion */
	quatf halfway = from.slerp(to, 0.5f);
	REQUIRE(std::fabs(halfway.length() - 1.0f) < 0.0001f);
	REQUIRE_THAT(
		halfway, mst::test_util::approx_equal(quatf(degrees<float>(45), float3(0, 1, 0)), 0.0001f));
}

TEST_CASE("quaternion<V>: operator* combines rotations, operator*= matches", "[quaternion]")
{
	quatf q(degrees<float>(40), float3(0, 1, 0));

	/* identity is the neutral element */
	REQUIRE_THAT(q * quatf::identity, mst::test_util::approx_equal(q, 0.0001f));
	REQUIRE_THAT(quatf::identity * q, mst::test_util::approx_equal(q, 0.0001f));

	quatf q2(degrees<float>(25), float3(1, 0, 0));
	quatf product = q * q2;

	quatf mutated = q;
	mutated *= q2;
	REQUIRE_THAT(mutated, mst::test_util::approx_equal(product, 0.0001f));

	/* scalar multiplication scales every component */
	quatf scaled = q * 2.0f;
	REQUIRE_THAT(
		scaled, mst::test_util::approx_equal(quatf(q.w * 2, q.x * 2, q.y * 2, q.z * 2), 0.0001f));
}

TEST_CASE("quaternion<V>: operator+ adds componentwise", "[quaternion]")
{
	quatf left(1, 2, 3, 4);
	quatf right(5, 6, 7, 8);

	REQUIRE_THAT(left + right, mst::test_util::approx_equal(quatf(6, 8, 10, 12), 0.0001f));
}

TEST_CASE("quaternion<V>: unary operator- negates every component", "[quaternion]")
{
	quatf q(1, -2, 3, -4);

	REQUIRE_THAT(-q, mst::test_util::approx_equal(quatf(-1, 2, -3, 4), 0.0001f));
}

TEST_CASE("quaternion<V>: operator/= scales down every component", "[quaternion]")
{
	quatf q(2, 4, 6, 8);
	q /= 2.0f;

	REQUIRE_THAT(q, mst::test_util::approx_equal(quatf(1, 2, 3, 4), 0.0001f));
}
