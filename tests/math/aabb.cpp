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

using mst::math::aabb;
using mst::math::vector;

/* aabb<V,2> ------------------------------------------------------------------------------- */

template<typename V>
void TestAabb2Default()
{
	CAPTURE(mst::typename_of<V>());

	aabb<V, 2> box;
	REQUIRE(box.min == vector<V, 2>::zero);
	REQUIRE(box.max == vector<V, 2>::zero);
	REQUIRE(box.dimensions == 2);
}

TEST_CASE("aabb<V,2>: default constructor sets min and max to zero", "[aabb]")
{
	TestAabb2Default<float>();
	TestAabb2Default<double>();
	TestAabb2Default<int32_t>();
}

template<typename V>
void TestAabb2VectorConstructor()
{
	CAPTURE(mst::typename_of<V>());

	vector<V, 2> min((V)-1, (V)-2);
	vector<V, 2> max((V)3, (V)4);

	aabb<V, 2> box(min, max);
	REQUIRE(box.min == min);
	REQUIRE(box.max == max);
}

TEST_CASE("aabb<V,2>: min/max vector constructor sets min and max", "[aabb]")
{
	TestAabb2VectorConstructor<float>();
	TestAabb2VectorConstructor<double>();
	TestAabb2VectorConstructor<int32_t>();
}

template<typename V>
void TestAabb2ComponentConstructor()
{
	CAPTURE(mst::typename_of<V>());

	aabb<V, 2> box((V)-1, (V)3, (V)-2, (V)4);
	REQUIRE(box.min == vector<V, 2>((V)-1, (V)-2));
	REQUIRE(box.max == vector<V, 2>((V)3, (V)4));
}

TEST_CASE("aabb<V,2>: left/right/bottom/top constructor sets min and max", "[aabb]")
{
	TestAabb2ComponentConstructor<float>();
	TestAabb2ComponentConstructor<double>();
	TestAabb2ComponentConstructor<int32_t>();
}

TEST_CASE("aabb<V,2>: contains() is inclusive on every edge and rejects outside points", "[aabb]")
{
	aabb<float, 2> box(-1.f, 1.f, -2.f, 2.f);

	/* inside */
	REQUIRE(box.contains(vector<float, 2>(0.f, 0.f)));

	/* on each edge (min/max inclusive) */
	REQUIRE(box.contains(vector<float, 2>(-1.f, 0.f)));
	REQUIRE(box.contains(vector<float, 2>(1.f, 0.f)));
	REQUIRE(box.contains(vector<float, 2>(0.f, -2.f)));
	REQUIRE(box.contains(vector<float, 2>(0.f, 2.f)));

	/* on each corner */
	REQUIRE(box.contains(vector<float, 2>(-1.f, -2.f)));
	REQUIRE(box.contains(vector<float, 2>(1.f, -2.f)));
	REQUIRE(box.contains(vector<float, 2>(-1.f, 2.f)));
	REQUIRE(box.contains(vector<float, 2>(1.f, 2.f)));

	/* just outside on each axis independently */
	REQUIRE_FALSE(box.contains(vector<float, 2>(-1.1f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 2>(1.1f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 2>(0.f, -2.1f)));
	REQUIRE_FALSE(box.contains(vector<float, 2>(0.f, 2.1f)));

	/* outside on both axes at once */
	REQUIRE_FALSE(box.contains(vector<float, 2>(-5.f, -5.f)));
	REQUIRE_FALSE(box.contains(vector<float, 2>(5.f, 5.f)));
}

/* aabb<V,3> ------------------------------------------------------------------------------- */

template<typename V>
void TestAabb3Default()
{
	CAPTURE(mst::typename_of<V>());

	aabb<V, 3> box;
	REQUIRE(box.min == vector<V, 3>::zero);
	REQUIRE(box.max == vector<V, 3>::zero);
	REQUIRE(box.dimensions == 3);
}

TEST_CASE("aabb<V,3>: default constructor sets min and max to zero", "[aabb]")
{
	TestAabb3Default<float>();
	TestAabb3Default<double>();
	TestAabb3Default<int32_t>();
}

template<typename V>
void TestAabb3VectorConstructor()
{
	CAPTURE(mst::typename_of<V>());

	vector<V, 3> min((V)-1, (V)-2, (V)-3);
	vector<V, 3> max((V)4, (V)5, (V)6);

	aabb<V, 3> box(min, max);
	REQUIRE(box.min == min);
	REQUIRE(box.max == max);
}

TEST_CASE("aabb<V,3>: min/max vector constructor sets min and max", "[aabb]")
{
	TestAabb3VectorConstructor<float>();
	TestAabb3VectorConstructor<double>();
	TestAabb3VectorConstructor<int32_t>();
}

template<typename V>
void TestAabb3ComponentConstructor()
{
	CAPTURE(mst::typename_of<V>());

	aabb<V, 3> box((V)-1, (V)4, (V)-2, (V)5, (V)-3, (V)6);
	REQUIRE(box.min == vector<V, 3>((V)-1, (V)-2, (V)-3));
	REQUIRE(box.max == vector<V, 3>((V)4, (V)5, (V)6));
}

TEST_CASE("aabb<V,3>: left/right/bottom/top/front/back constructor sets min and max", "[aabb]")
{
	TestAabb3ComponentConstructor<float>();
	TestAabb3ComponentConstructor<double>();
	TestAabb3ComponentConstructor<int32_t>();
}

TEST_CASE("aabb<V,3>: contains() is inclusive on every edge and rejects outside points", "[aabb]")
{
	aabb<float, 3> box(-1.f, 1.f, -2.f, 2.f, -3.f, 3.f);

	/* inside */
	REQUIRE(box.contains(vector<float, 3>(0.f, 0.f, 0.f)));

	/* on each face (min/max inclusive) */
	REQUIRE(box.contains(vector<float, 3>(-1.f, 0.f, 0.f)));
	REQUIRE(box.contains(vector<float, 3>(1.f, 0.f, 0.f)));
	REQUIRE(box.contains(vector<float, 3>(0.f, -2.f, 0.f)));
	REQUIRE(box.contains(vector<float, 3>(0.f, 2.f, 0.f)));
	REQUIRE(box.contains(vector<float, 3>(0.f, 0.f, -3.f)));
	REQUIRE(box.contains(vector<float, 3>(0.f, 0.f, 3.f)));

	/* every corner */
	for(float x : { -1.f, 1.f })
		for(float y : { -2.f, 2.f })
			for(float z : { -3.f, 3.f })
			{
				CAPTURE(x, y, z);
				REQUIRE(box.contains(vector<float, 3>(x, y, z)));
			}

	/* just outside on each axis independently */
	REQUIRE_FALSE(box.contains(vector<float, 3>(-1.1f, 0.f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(1.1f, 0.f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(0.f, -2.1f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(0.f, 2.1f, 0.f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(0.f, 0.f, -3.1f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(0.f, 0.f, 3.1f)));

	/* outside on every axis at once */
	REQUIRE_FALSE(box.contains(vector<float, 3>(-5.f, -5.f, -5.f)));
	REQUIRE_FALSE(box.contains(vector<float, 3>(5.f, 5.f, 5.f)));
}

/* generic aabb<V,N> primary template (dimensions other than the 2/3 specializations) -------- */

template<typename V, size_t E>
void TestAabbGeneric()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	/* only the default constructor is available on the unspecialized template */
	aabb<V, E> box;
	REQUIRE(box.dimensions == E);

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(box.min[i] == (V)0);
		REQUIRE(box.max[i] == (V)0);
	}

	for(size_t i = 0; i < E; ++i)
	{
		box.min[i] = (V)-1;
		box.max[i] = (V)1;
	}

	vector<V, E> inside((V)0);
	REQUIRE(box.contains(inside));

	for(size_t i = 0; i < E; ++i)
	{
		vector<V, E> onMin = inside;
		onMin[i] = (V)-1;
		REQUIRE(box.contains(onMin));

		vector<V, E> onMax = inside;
		onMax[i] = (V)1;
		REQUIRE(box.contains(onMax));

		vector<V, E> belowMin = inside;
		belowMin[i] = (V)-2;
		REQUIRE_FALSE(box.contains(belowMin));

		vector<V, E> aboveMax = inside;
		aboveMax[i] = (V)2;
		REQUIRE_FALSE(box.contains(aboveMax));
	}
}

TEST_CASE("aabb<V,E>: unspecialized primary template supports arbitrary dimensions", "[aabb]")
{
	/* mst::math::vector only supports 1-4 elements, so those are the only dimensions
		the generic (non 2/3-specialized) aabb template can be instantiated with */
	TestAabbGeneric<float, 1>();
	TestAabbGeneric<float, 4>();
	TestAabbGeneric<double, 1>();
	TestAabbGeneric<double, 4>();
	TestAabbGeneric<int32_t, 1>();
	TestAabbGeneric<int32_t, 4>();
}

TEST_CASE("aabb<V,E>: copy construction and assignment copy min/max", "[aabb]")
{
	aabb<float, 2> box(-1.f, 1.f, -2.f, 2.f);

	aabb<float, 2> copyConstructed(box);
	REQUIRE(copyConstructed.min == box.min);
	REQUIRE(copyConstructed.max == box.max);

	aabb<float, 2> copyAssigned;
	copyAssigned = box;
	REQUIRE(copyAssigned.min == box.min);
	REQUIRE(copyAssigned.max == box.max);
}

TEST_CASE("aabb<V,E>: vector_type alias matches mst::math::vector<V,E>", "[aabb]")
{
	static_assert(std::is_same<aabb<float, 2>::vector_type, vector<float, 2>>::value,
		"aabb<V,2>::vector_type should be vector<V,2>");
	static_assert(std::is_same<aabb<float, 3>::vector_type, vector<float, 3>>::value,
		"aabb<V,3>::vector_type should be vector<V,3>");
	static_assert(std::is_same<aabb<float, 4>::vector_type, vector<float, 4>>::value,
		"aabb<V,4>::vector_type should be vector<V,4>");

	SUCCEED();
}
