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

TEST_CASE("vector<V,E>: single value constructor sets x", "[vector]")
{
	REQUIRE(vector<float, 1>(5).x == 5);
	REQUIRE(vector<double, 1>(-2.5).x == -2.5);
	REQUIRE(vector<int32_t, 1>(7).x == 7);
	REQUIRE(vector<uint32_t, 1>(9u).x == 9u);
}

TEST_CASE("vector<V,1>: implicit conversion to value_type", "[vector]")
{
	vector<float, 1> v(42.f);
	float f = v;
	REQUIRE(f == 42.f);

	vector<int32_t, 1> iv(-3);
	int32_t i = iv;
	REQUIRE(i == -3);
}

TEST_CASE("vector<V,E>: component constructors set the expected elements", "[vector]")
{
	typedef vector<float, 2> float2;
	typedef vector<float, 3> float3;
	typedef vector<float, 4> float4;

	float2 v2(1, 2);
	REQUIRE(v2.x == 1);
	REQUIRE(v2.y == 2);

	float3 v3(1, 2, 3);
	REQUIRE(v3.x == 1);
	REQUIRE(v3.y == 2);
	REQUIRE(v3.z == 3);

	/* vector<V,3> composed from a vector<V,2> and a trailing/leading scalar */
	REQUIRE(float3(float2(1, 2), 3) == v3);
	REQUIRE(float3(1, float2(2, 3)) == v3);

	float4 v4(1, 2, 3, 4);
	REQUIRE(v4.x == 1);
	REQUIRE(v4.y == 2);
	REQUIRE(v4.z == 3);
	REQUIRE(v4.w == 4);

	/* every way to compose a vector<V,4> from smaller vectors/scalars */
	REQUIRE(float4(float2(1, 2), float2(3, 4)) == v4);
	REQUIRE(float4(float2(1, 2), 3, 4) == v4);
	REQUIRE(float4(1, float2(2, 3), 4) == v4);
	REQUIRE(float4(1, 2, float2(3, 4)) == v4);
	REQUIRE(float4(float3(1, 2, 3), 4) == v4);
	REQUIRE(float4(1, float3(2, 3, 4)) == v4);
}

TEST_CASE("vector<V,E>: fill constructor sets every element", "[vector]")
{
	REQUIRE(vector<float, 2>(4.f) == vector<float, 2>(4.f, 4.f));
	REQUIRE(vector<float, 3>(4.f) == vector<float, 3>(4.f, 4.f, 4.f));
	REQUIRE(vector<float, 4>(4.f) == vector<float, 4>(4.f, 4.f, 4.f, 4.f));

	vector<int32_t, 3> vi(-2);
	REQUIRE(vi.x == -2);
	REQUIRE(vi.y == -2);
	REQUIRE(vi.z == -2);
}

template<typename Dst, typename Src, size_t E>
void TestConvertingConstructor()
{
	CAPTURE(mst::typename_of<Dst>());
	CAPTURE(mst::typename_of<Src>());
	CAPTURE(E);

	vector<Src, E> src;
	for(size_t i = 0; i < E; ++i)
	{
		src[i] = (Src)(i + 1);
	}

	vector<Dst, E> dst(src);

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(dst[i] == static_cast<Dst>(src[i]));
	}
}

TEST_CASE("vector<V,E>: converting constructor casts every element", "[vector]")
{
	TestConvertingConstructor<float, double, 1>();
	TestConvertingConstructor<float, double, 2>();
	TestConvertingConstructor<float, double, 3>();
	TestConvertingConstructor<float, double, 4>();

	TestConvertingConstructor<double, float, 2>();
	TestConvertingConstructor<int32_t, float, 3>();
	TestConvertingConstructor<float, int32_t, 4>();
	TestConvertingConstructor<int64_t, int32_t, 4>();
}

TEST_CASE("vector<V,E>: zero static", "[vector]")
{
	REQUIRE(vector<float, 1>::zero == vector<float, 1>(0));
	REQUIRE(vector<float, 2>::zero == vector<float, 2>(0, 0));
	REQUIRE(vector<float, 3>::zero == vector<float, 3>(0, 0, 0));
	REQUIRE(vector<float, 4>::zero == vector<float, 4>(0, 0, 0, 0));

	REQUIRE(vector<int32_t, 3>::zero == vector<int32_t, 3>(0, 0, 0));
	REQUIRE(vector<uint32_t, 4>::zero == vector<uint32_t, 4>(0, 0, 0, 0));
}

TEST_CASE("vector<V,E>: operator[] reads and writes elements", "[vector]")
{
	vector<int, 4> v(0);

	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;

	REQUIRE(v[0] == 1);
	REQUIRE(v[1] == 2);
	REQUIRE(v[2] == 3);
	REQUIRE(v[3] == 4);

	const auto& cv = v;
	REQUIRE(cv[2] == 3);
}

TEST_CASE("vector<V,E>: data() and size()", "[vector]")
{
	vector<int, 4> v(0, 0, 0, 0);
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;

	REQUIRE(v.size() == 4);

	int* data = v.data();
	for(int i = 0; i < 4; ++i)
	{
		REQUIRE(data[i] == i + 1);
	}

	const vector<int, 4>& cv = v;
	REQUIRE(cv.data() == data);
}

template<typename V, size_t E>
void TestIteration()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(E);

	vector<V, E> v = vector<V, E>::zero;
	for(size_t i = 0; i < E; ++i)
	{
		v[i] = (V)(i + 1);
	}

	/* vector_iterator only supports offsetting by a fixed amount, not subtracting one iterator
		from another, so distance is counted by walking rather than via std::distance */
	size_t count = 0;
	for(auto it = v.begin(); it != v.end(); ++it)
	{
		++count;
	}
	REQUIRE(count == E);

	count = 0;
	for(auto it = v.cbegin(); it != v.cend(); ++it)
	{
		++count;
	}
	REQUIRE(count == E);

	const vector<V, E>& cv = v;
	count = 0;
	for(auto it = cv.begin(); it != cv.end(); ++it)
	{
		++count;
	}
	REQUIRE(count == E);

	/* mutate every element through the (mutable) iterator */
	for(auto& e : v)
	{
		e += (V)10;
	}

	for(size_t i = 0; i < E; ++i)
	{
		REQUIRE(v[i] == (V)(i + 11));
	}

	/* read every element back through the const iterator */
	size_t idx = 0;
	for(auto it = cv.cbegin(); it != cv.cend(); ++it, ++idx)
	{
		REQUIRE(*it == cv[idx]);
	}
}

TEST_CASE("vector<V,E>: begin/end iteration", "[vector]")
{
	TestIteration<int, 1>();
	TestIteration<int, 2>();
	TestIteration<int, 3>();
	TestIteration<int, 4>();

	TestIteration<float, 4>();
	TestIteration<double, 3>();
}

TEST_CASE("vector_iterator: random access operations", "[vector]")
{
	vector<int, 4> v(1, 2, 3, 4);

	auto first = v.begin();
	auto last = v.end();

	REQUIRE(first + 4 == last);
	REQUIRE(4 + first == last);
	REQUIRE(first < last);
	REQUIRE(first <= last);
	REQUIRE(last > first);
	REQUIRE(last >= first);
	REQUIRE(first == v.begin());
	REQUIRE(first != last);

	auto it = first;
	REQUIRE(*it == 1);
	++it;
	REQUIRE(*it == 2);
	auto postInc = it++;
	REQUIRE(*postInc == 2);
	REQUIRE(*it == 3);
	--it;
	REQUIRE(*it == 2);
	auto postDec = it--;
	REQUIRE(*postDec == 2);
	REQUIRE(*it == 1);

	it += 3;
	REQUIRE(*it == 4);
	it -= 3;
	REQUIRE(*it == 1);
}
