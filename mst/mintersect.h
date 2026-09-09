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

#pragma once

#include <mcore.h>
#include <mmath2.h>
#include <limits>
#include <type_traits>

namespace mst::math {

/* Ray/axis-aligned-bounding-box intersection test, using the slab method.

	On a hit, tNear and tFar receive the ray parameters (r.position + t * r.direction) at which
	the ray enters and exits the box. tNear can be negative when the ray's origin is inside the
	box; tFar is always >= tNear on a hit.

	Returns false when the ray misses the box entirely, or when the box lies entirely behind the
	ray's origin (tFar < 0) - unlike an infinite line, a ray only extends in one direction. */
template<typename T, size_t N>
_MST_NODISCARD _MST_CONSTEXPR17 bool intersect(
	const ray<T, N>& r, const aabb<T, N>& box, T& tNear, T& tFar) noexcept
{
	static_assert(std::is_floating_point<T>::value,
		"mst::math::intersect(ray, aabb) requires a floating point value type");

	T tMin = -std::numeric_limits<T>::infinity();
	T tMax = std::numeric_limits<T>::infinity();

	for(size_t i = 0; i < N; ++i)
	{
		const T origin = r.position[i];
		const T direction = r.direction[i];
		const T boxMin = box.min[i];
		const T boxMax = box.max[i];

		if(direction == (T)0)
		{
			// the ray is parallel to this slab: it can only pass through it when the origin
			// already lies within the slab's bounds on this axis
			if(origin < boxMin || origin > boxMax)
			{
				return false;
			}

			continue;
		}

		const T invDirection = (T)1 / direction;
		T t0 = (boxMin - origin) * invDirection;
		T t1 = (boxMax - origin) * invDirection;

		if(t0 > t1)
		{
			const T tmp = t0;
			t0 = t1;
			t1 = tmp;
		}

		if(t0 > tMin)
		{
			tMin = t0;
		}

		if(t1 < tMax)
		{
			tMax = t1;
		}

		if(tMin > tMax)
		{
			return false;
		}
	}

	if(tMax < (T)0)
	{
		// the box is entirely behind the ray's origin
		return false;
	}

	tNear = tMin;
	tFar = tMax;
	return true;
}

/* Convenience overload of intersect() for callers that only need to know whether the ray hits
	the box, without the hit distances. */
template<typename T, size_t N>
_MST_NODISCARD _MST_CONSTEXPR17 bool intersects(const ray<T, N>& r, const aabb<T, N>& box) noexcept
{
	T tNear, tFar;
	return intersect(r, box, tNear, tFar);
}

} // namespace mst::math
