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

#include <mintersect.h>
#include <mcommon.h>

using mst::math::aabb;
using mst::math::intersect;
using mst::math::intersects;
using mst::math::ray;
using mst::math::vector;

/* intersect()/intersects() forward to ray<T,N>::direction, which - like ray's constructor -
	only supports floating point value types, so T is restricted to float/double throughout. */

/* axis-aligned hits, one per axis and direction --------------------------------------------- */

template<typename T, size_t N>
void TestAxisAlignedHit(size_t axis, T sign)
{
	CAPTURE(mst::typename_of<T>());
	CAPTURE(N, axis, sign);

	vector<T, N> origin = vector<T, N>::zero;
	origin[axis] = sign * (T)5;

	vector<T, N> direction = vector<T, N>::zero;
	direction[axis] = -sign;

	ray<T, N> r(origin, direction);

	// the generic aabb<V,E> primary template (dimensions other than the 2/3 specializations)
	// only has a default constructor; min/max must be set directly
	aabb<T, N> box;
	for(size_t i = 0; i < N; ++i)
	{
		box.min[i] = (T)-1;
		box.max[i] = (T)1;
	}

	T tNear = (T)0, tFar = (T)0;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE(intersects(r, box));

	REQUIRE_THAT(tNear, Catch::Matchers::WithinAbs((double)4, 1e-4));
	REQUIRE_THAT(tFar, Catch::Matchers::WithinAbs((double)6, 1e-4));
}

template<typename T, size_t N>
void TestAxisAlignedHits()
{
	for(size_t axis = 0; axis < N; ++axis)
	{
		TestAxisAlignedHit<T, N>(axis, (T)1);
		TestAxisAlignedHit<T, N>(axis, (T)-1);
	}
}

TEST_CASE("intersect(ray, aabb): axis-aligned ray hits box from either side of every axis",
	"[intersect][ray][aabb]")
{
	TestAxisAlignedHits<float, 2>();
	TestAxisAlignedHits<float, 3>();
	TestAxisAlignedHits<float, 4>();

	TestAxisAlignedHits<double, 2>();
	TestAxisAlignedHits<double, 3>();
	TestAxisAlignedHits<double, 4>();
}

/* origin inside the box: tNear is negative, tFar >= 0 --------------------------------------- */

TEST_CASE("intersect(ray, aabb): ray originating inside the box hits with a negative tNear",
	"[intersect][ray][aabb]")
{
	ray<float, 3> r(vector<float, 3>(0.f, 0.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE(intersects(r, box));
	REQUIRE_THAT(tNear, Catch::Matchers::WithinAbs(-1.0, 1e-5));
	REQUIRE_THAT(tFar, Catch::Matchers::WithinAbs(1.0, 1e-5));
}

/* the box is entirely behind the ray's origin: tFar < 0 branch ------------------------------ */

TEST_CASE("intersect(ray, aabb): box entirely behind the ray origin is not a hit",
	"[intersect][ray][aabb]")
{
	ray<float, 3> r(vector<float, 3>(5.f, 0.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE_FALSE(intersect(r, box, tNear, tFar));
	REQUIRE_FALSE(intersects(r, box));
}

/* ray parallel to a slab (direction[axis] == 0) ---------------------------------------------- */

TEST_CASE("intersect(ray, aabb): ray parallel to an axis, inside that axis' slab, still hits",
	"[intersect][ray][aabb]")
{
	// travels along +z; x and y are stationary and already within [-1, 1]
	ray<float, 3> r(vector<float, 3>(0.f, 0.f, -5.f), vector<float, 3>(0.f, 0.f, 1.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE_THAT(tNear, Catch::Matchers::WithinAbs(4.0, 1e-4));
	REQUIRE_THAT(tFar, Catch::Matchers::WithinAbs(6.0, 1e-4));
}

TEST_CASE(
	"intersect(ray, aabb): ray parallel to an axis, above that axis' slab, misses",
	"[intersect][ray][aabb]")
{
	// travels along +z, but x = 5 is above the box's [-1, 1] slab on x
	ray<float, 3> r(vector<float, 3>(5.f, 0.f, -5.f), vector<float, 3>(0.f, 0.f, 1.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE_FALSE(intersect(r, box, tNear, tFar));
	REQUIRE_FALSE(intersects(r, box));
}

TEST_CASE(
	"intersect(ray, aabb): ray parallel to an axis, below that axis' slab, misses",
	"[intersect][ray][aabb]")
{
	// travels along +z, but x = -5 is below the box's [-1, 1] slab on x - this exercises the
	// "origin < boxMin" side of the parallel-slab check, as distinct from "origin > boxMax"
	ray<float, 3> r(vector<float, 3>(-5.f, 0.f, -5.f), vector<float, 3>(0.f, 0.f, 1.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE_FALSE(intersect(r, box, tNear, tFar));
	REQUIRE_FALSE(intersects(r, box));
}

TEST_CASE(
	"intersect(ray, aabb): ray parallel to an axis, on that axis' boundary, hits (inclusive)",
	"[intersect][ray][aabb]")
{
	// x sits exactly on the box's min-x edge
	ray<float, 3> r(vector<float, 3>(-1.f, 0.f, -5.f), vector<float, 3>(0.f, 0.f, 1.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	REQUIRE(intersects(r, box));
}

/* diagonal ray: exercises the t0/t1 swap and both tMin/tMax update branches on every axis ---- */

TEST_CASE(
	"intersect(ray, aabb): diagonal ray exercises the swap and update branches on every axis",
	"[intersect][ray][aabb]")
{
	// direction has a negative component on every axis, forcing the t0 > t1 swap each time
	vector<double, 3> dir = vector<double, 3>(-1.0, -1.0, -1.0).normalized();
	ray<double, 3> r(vector<double, 3>(5.0, 5.0, 5.0), dir);
	aabb<double, 3> box(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	double tNear, tFar;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE(tNear > 0.0);
	REQUIRE(tFar > tNear);

	vector<double, 3> enterPoint = r.position + dir * tNear;
	// the entry point should lie on the box's surface (at least one coordinate at +1)
	bool onSurface = false;
	for(size_t i = 0; i < 3; ++i)
	{
		if(std::fabs(enterPoint[i] - 1.0) < 1e-9)
		{
			onSurface = true;
		}
	}
	REQUIRE(onSurface);
}

/* diagonal ray that misses: the two per-axis intervals don't overlap (tMin > tMax) ----------- */

TEST_CASE("intersect(ray, aabb): diagonal ray whose per-axis intervals don't overlap misses",
	"[intersect][ray][aabb]")
{
	// travels toward the box on x, but passes well above it on y - never inside both slabs
	// at the same time
	vector<float, 2> dir = vector<float, 2>(1.f, 0.1f).normalized();
	ray<float, 2> r(vector<float, 2>(-10.f, 10.f), dir);
	aabb<float, 2> box(-1.f, 1.f, -1.f, 1.f);

	float tNear, tFar;
	REQUIRE_FALSE(intersect(r, box, tNear, tFar));
	REQUIRE_FALSE(intersects(r, box));
}

/* pure miss: ray travels parallel to the box, offset away from it entirely ------------------- */

TEST_CASE("intersect(ray, aabb): ray travelling away from an offset box misses",
	"[intersect][ray][aabb]")
{
	ray<float, 3> r(vector<float, 3>(0.f, 10.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	REQUIRE_FALSE(intersects(r, box));
}

/* grazing the box exactly at a single point: tMin == tMax is still a hit (non-strict compare) */

TEST_CASE("intersect(ray, aabb): ray grazing exactly one corner is a hit (tNear == tFar)",
	"[intersect][ray][aabb]")
{
	// a diagonal ray whose per-axis entry/exit intervals happen to touch at a single instant:
	// starting at (0, -2) along (1, 1), the box's x-entry (t=1) coincides with its y-exit
	// (t=3) once rescaled for the unit direction, so the ray only ever touches corner (3, 1)
	vector<float, 2> dir = vector<float, 2>(1.f, 1.f).normalized();
	ray<float, 2> r(vector<float, 2>(0.f, -2.f), dir);
	aabb<float, 2> box(1.f, 3.f, 1.f, 3.f);

	float tNear, tFar;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE_THAT((double)tNear, Catch::Matchers::WithinAbs((double)tFar, 1e-4));

	vector<float, 2> touchPoint = r.position + dir * tNear;
	REQUIRE_THAT((double)touchPoint[0], Catch::Matchers::WithinAbs(3.0, 1e-4));
	REQUIRE_THAT((double)touchPoint[1], Catch::Matchers::WithinAbs(1.0, 1e-4));
}

/* degenerate (zero-volume) box: min == max on every axis ------------------------------------- */

TEST_CASE("intersect(ray, aabb): axis-aligned ray through a degenerate (point) box hits exactly",
	"[intersect][ray][aabb]")
{
	ray<float, 3> r(vector<float, 3>(-5.f, 0.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	aabb<float, 3> box(vector<float, 3>::zero, vector<float, 3>::zero);

	float tNear, tFar;
	REQUIRE(intersect(r, box, tNear, tFar));
	REQUIRE_THAT((double)tNear, Catch::Matchers::WithinAbs(5.0, 1e-4));
	REQUIRE_THAT((double)tFar, Catch::Matchers::WithinAbs(5.0, 1e-4));
}

TEST_CASE(
	"intersect(ray, aabb): axis-aligned ray offset from a degenerate (point) box misses",
	"[intersect][ray][aabb]")
{
	ray<float, 3> r(vector<float, 3>(-5.f, 1.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	aabb<float, 3> box(vector<float, 3>::zero, vector<float, 3>::zero);

	REQUIRE_FALSE(intersects(r, box));
}

/* 1-dimensional case: only the "parallel to slab" branch can never trigger (direction is */
/* always +-1 for a unit ray in 1D), but the general slab logic still applies ------------------ */

TEST_CASE("intersect(ray, aabb): 1-dimensional ray/box hit and miss", "[intersect][ray][aabb]")
{
	aabb<float, 1> box;
	box.min[0] = -1.f;
	box.max[0] = 1.f;

	ray<float, 1> hitRay(vector<float, 1>((float)-5), vector<float, 1>((float)1));
	float tNear, tFar;
	REQUIRE(intersect(hitRay, box, tNear, tFar));
	REQUIRE_THAT((double)tNear, Catch::Matchers::WithinAbs(4.0, 1e-4));
	REQUIRE_THAT((double)tFar, Catch::Matchers::WithinAbs(6.0, 1e-4));

	ray<float, 1> missRay(vector<float, 1>((float)-5), vector<float, 1>((float)-1));
	REQUIRE_FALSE(intersects(missRay, box));
}

/* intersect() and intersects() must always agree -------------------------------------------- */

TEST_CASE("intersect(ray, aabb): intersects() agrees with intersect() on both hit and miss",
	"[intersect][ray][aabb]")
{
	aabb<float, 3> box(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	ray<float, 3> hitRay(vector<float, 3>(-5.f, 0.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));
	ray<float, 3> missRay(vector<float, 3>(5.f, 0.f, 0.f), vector<float, 3>(1.f, 0.f, 0.f));

	float tNear, tFar;
	REQUIRE(intersect(hitRay, box, tNear, tFar) == intersects(hitRay, box));
	REQUIRE(intersect(missRay, box, tNear, tFar) == intersects(missRay, box));

	REQUIRE(intersects(hitRay, box));
	REQUIRE_FALSE(intersects(missRay, box));
}
