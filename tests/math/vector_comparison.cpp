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

template<typename V, size_t E>
void TestEquality()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	typedef vector<V, E> vector_type;

	vector_type a = vector_type::zero;
	V value = (V)1;
	for(size_t i = 0; i < E; ++i)
	{
		a[i] = value;
		value += 1;
	}

	vector_type b = a;

	REQUIRE(a == a);
	REQUIRE(a == b);
	REQUIRE_FALSE(a != b);

	/* flipping any single element must break equality */
	for(size_t i = 0; i < E; ++i)
	{
		vector_type modified = a;
		modified[i] += (V)1;

		REQUIRE(modified != a);
		REQUIRE_FALSE(modified == a);
	}
}

TEST_CASE("vector<V,E>: operator== / operator!=", "[vector]")
{
	TestEquality<float, 1>();
	TestEquality<float, 2>();
	TestEquality<float, 3>();
	TestEquality<float, 4>();

	TestEquality<double, 2>();
	TestEquality<double, 4>();

	TestEquality<int32_t, 1>();
	TestEquality<int32_t, 3>();
	TestEquality<int32_t, 4>();

	TestEquality<uint32_t, 4>();
	TestEquality<int64_t, 4>();
}

TEST_CASE("vector<V,E>: zero is not equal to a non-zero vector", "[vector]")
{
	REQUIRE(vector<float, 4>::zero != vector<float, 4>(1, 0, 0, 0));
	REQUIRE(vector<double, 3>::zero != vector<double, 3>(0, 0, 1));
	REQUIRE_FALSE(vector<float, 4>::zero == vector<float, 4>(0, 0, 0, 1));
}
