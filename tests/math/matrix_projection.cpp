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
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <set_assertions.h>

#include "helpers.h"

#include <mmath2.h>
#include <mcommon.h>

using mst::math::vector;
using mst::math::matrix;
using mst::math::aabb;
using mst::math::radians;
using mst::math::degrees;

TEST_CASE("matrix<V,4,4>: create_perspective(fov,aspect,near,far)", "[matrix]")
{
	typedef matrix<float, 4, 4> mat4;

	/* fov=90 deg, aspect=1 -> cotangent(45deg) == 1 */
	auto m = mat4::create_perspective(radians<float>(degrees<float>(90)), 1.f, 1.f, 100.f);

	mat4 expected = mat4::zero;
	expected[0].x = 1.f;
	expected[1].y = 1.f;
	expected[2].z = 100.f / 99.f;
	expected[2].w = 1.f;
	expected[3].z = -100.f / 99.f;

	REQUIRE_THAT(m, mst::test_util::approx_equal(expected, 0.0001f));
}

TEST_CASE("matrix<V,4,4>: create_perspective(fov,width,height,near,far)", "[matrix]")
{
	typedef matrix<float, 4, 4> mat4;

	auto viaAspect =
		mat4::create_perspective(radians<float>(degrees<float>(90)), 1.f, 1.f, 100.f);
	auto viaWidthHeight =
		mat4::create_perspective(radians<float>(degrees<float>(90)), 16.f, 16.f, 1.f, 100.f);

	/* width==height means aspect==1, same as the test above */
	REQUIRE_THAT(viaWidthHeight, mst::test_util::approx_equal(viaAspect, 0.0001f));
}

TEST_CASE("matrix<V,4,4>: create_perspective maps near/far to depth 0/1 after perspective divide",
	"[matrix]")
{
	/* usable directly via plain point * M -- no .get_transpose() needed */
	typedef vector<float, 4> float4;

	auto m =
		matrix<float, 4, 4>::create_perspective(radians<float>(degrees<float>(90)), 1.f, 1.f, 100.f);

	auto nearPoint = float4(0, 0, 1.f, 1) * m;
	auto farPoint = float4(0, 0, 100.f, 1) * m;

	REQUIRE_THAT(nearPoint.z / nearPoint.w, Catch::Matchers::WithinAbs(0.f, 0.0001f));
	REQUIRE_THAT(farPoint.z / farPoint.w, Catch::Matchers::WithinAbs(1.f, 0.0001f));
}

TEST_CASE("matrix<V,4,4>: create_orthographic(aabb<V,2>, near, far)", "[matrix]")
{
	typedef matrix<float, 4, 4> mat4;
	typedef vector<float, 2> float2;

	aabb<float, 2> box(float2(-2, -3), float2(2, 5));

	auto m = mat4::create_orthographic(box, 1.f, 5.f);

	mat4 expected = mat4::zero;
	expected[0][0] = 0.5f;
	expected[1][1] = 0.25f;
	expected[2][2] = 0.25f;
	expected[3][0] = 0.f;
	expected[3][1] = -0.25f;
	expected[3][2] = -0.25f;
	expected[3][3] = 1.f;

	REQUIRE_THAT(m, mst::test_util::approx_equal(expected, 0.0001f));
}

TEST_CASE("matrix<V,4,4>: create_orthographic(aabb<V,2>) and (aabb<V,3>) agree", "[matrix]")
{
	/* the same box and near/far range, expressed either way, must produce the same matrix --
		this is the property that was violated before the [2][2]/[2][3] sign fix */
	typedef vector<float, 2> float2;
	typedef vector<float, 3> float3;

	aabb<float, 2> box2(float2(-2, -3), float2(2, 5));
	aabb<float, 3> box3(float3(-2, -3, 1), float3(2, 5, 5));

	auto viaAabb2 = matrix<float, 4, 4>::create_orthographic(box2, 1.f, 5.f);
	auto viaAabb3 = matrix<float, 4, 4>::create_orthographic(box3);

	REQUIRE_THAT(viaAabb2, mst::test_util::approx_equal(viaAabb3, 0.0001f));
}

TEST_CASE("matrix<V,4,4>: create_orthographic maps near/far to depth 0/1", "[matrix]")
{
	/* usable directly via plain point * M -- no .get_transpose() needed */
	typedef vector<float, 2> float2;
	typedef vector<float, 4> float4;

	aabb<float, 2> box(float2(-2, -3), float2(2, 5));
	auto m = matrix<float, 4, 4>::create_orthographic(box, 1.f, 5.f);

	auto nearPoint = float4(0, 1, 1, 1) * m;
	auto farPoint = float4(0, 1, 5, 1) * m;

	REQUIRE_THAT(nearPoint.z, Catch::Matchers::WithinAbs(0.f, 0.0001f));
	REQUIRE_THAT(farPoint.z, Catch::Matchers::WithinAbs(1.f, 0.0001f));
	REQUIRE(nearPoint.w == 1.f);
	REQUIRE(farPoint.w == 1.f);
}

TEST_CASE("matrix<V,4,4>: create_orthographic(aabb<V,3>)", "[matrix]")
{
	typedef matrix<float, 4, 4> mat4;
	typedef vector<float, 3> float3;

	aabb<float, 3> box(float3(-2, -3, 1), float3(2, 5, 5));

	auto m = mat4::create_orthographic(box);

	mat4 expected = mat4::zero;
	expected[0][0] = 0.5f;
	expected[1][1] = 0.25f;
	expected[2][2] = 0.25f;
	expected[3][0] = 0.f;
	expected[3][1] = -0.25f;
	expected[3][2] = -0.25f;
	expected[3][3] = 1.f;

	REQUIRE_THAT(m, mst::test_util::approx_equal(expected, 0.0001f));
}
