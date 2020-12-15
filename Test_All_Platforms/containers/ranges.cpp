//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <set_assertions.h>

#include <vector>
#include <mranges.h>
#include <random>

template<typename T>
void TestEmpty(T range)
{
	REQUIRE(range.begin() == range.end());

	for(auto item : range)
	{
		_MST_UNUSED(item);
		FAIL("Container should be empty");
	}
}

TEST_CASE("range: empty", "[ranges]")
{
	std::vector<int> container;

	const auto& ccontainer = container;

	TestEmpty(mst::range(container));
	TestEmpty(mst::range(container.begin(), container.end()));
	TestEmpty(mst::range(std::make_pair(container.begin(), container.end())));
	TestEmpty(mst::range(ccontainer));
	TestEmpty(mst::range(ccontainer.begin(), ccontainer.end()));
	TestEmpty(mst::range(std::make_pair(ccontainer.begin(), ccontainer.end())));
}

TEST_CASE("crange: empty", "[ranges]")
{
	std::vector<int> container;

	const auto& ccontainer = container;

	TestEmpty(mst::crange(container));
	TestEmpty(mst::crange(ccontainer));
}

template<typename T>
void TestOne(int value, T range)
{
	REQUIRE(range.begin() != range.end());

	auto next = range.begin();
	REQUIRE(++next == range.end());

	bool once = true;
	for(auto item : range)
	{
		if(once)
		{
			REQUIRE(item == value);
			once = false;
			continue;
		}
		FAIL("Container should contain one element");
	}

	if(once)
		FAIL("Container should contain one element");
}

TEST_CASE("range: one", "[ranges]")
{
	std::vector<int> container = { 1 };
	int arr[1] = { 1 };
	const int carr[1] = { 1 };

	const auto& ccontainer = container;

	TestOne(1, mst::range(container));
	TestOne(1, mst::range(container.begin(), container.end()));
	TestOne(1, mst::range(std::make_pair(container.begin(), container.end())));
	TestOne(1, mst::range(ccontainer));
	TestOne(1, mst::range(ccontainer.begin(), ccontainer.end()));
	TestOne(1, mst::range(std::make_pair(ccontainer.begin(), ccontainer.end())));
	TestOne(1, mst::range(arr));
	TestOne(1, mst::range(carr));
}

TEST_CASE("crange: one", "[ranges]")
{
	std::vector<int> container = { 1 };
	int arr[1] = { 1 };
	const int carr[1] = { 1 };

	const auto& ccontainer = container;

	TestOne(1, mst::crange(container));
	TestOne(1, mst::crange(ccontainer));
	TestOne(1, mst::crange(arr));
	TestOne(1, mst::crange(carr));
}

template<typename T>
void TestValues(T range)
{
	REQUIRE(range.begin() != range.end());

	int count = 0;
	int value = 1;

	for(auto item : range)
	{
		if(++count > 10)
			FAIL("Container should contain 10 elements");

		int expectedValue = value++;

		REQUIRE(expectedValue == item);
	}

	REQUIRE(count == 10);
}

TEST_CASE("range: values", "[ranges]")
{
	std::vector<int> container = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const int carr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	const auto& ccontainer = container;

	TestValues(mst::range(container));
	TestValues(mst::range(container.begin(), container.end()));
	TestValues(mst::range(std::make_pair(container.begin(), container.end())));
	TestValues(mst::range(ccontainer));
	TestValues(mst::range(ccontainer.begin(), ccontainer.end()));
	TestValues(mst::range(std::make_pair(ccontainer.begin(), ccontainer.end())));
	TestValues(mst::range(arr));
	TestValues(mst::range(carr));
}

TEST_CASE("crange: values", "[ranges]")
{
	std::vector<int> container = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const int carr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	const auto& ccontainer = container;

	TestValues(mst::crange(container));
	TestValues(mst::crange(ccontainer));
	TestValues(mst::crange(arr));
	TestValues(mst::crange(carr));
}