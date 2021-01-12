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
#include <random_data_generator.h>

#include <malgorithm_container.h>
#include <vector>

using namespace ::Catch::Generators;
using namespace ::mst::tests;

void CheckIsSorted(const std::vector<int>& input)
{
	for(auto it = input.begin(); it != input.end() - 1; ++it)
	{
		REQUIRE(*it <= *(it + 1));
	}
}

void CheckIsReverseSorted(const std::vector<int>& input)
{
	for(auto it = input.begin(); it != input.end() - 1; ++it)
	{
		REQUIRE(*it >= *(it + 1));
	}
}

TEST_CASE("sort: container", "[algorithm][container]")
{
	random_data_generator rdg{ true };
	INFO("Seed" << rdg.seed());

	const auto elemCount = GENERATE(range<size_t>(2, 1000));

	auto input = rdg.vector_int(elemCount, INT_MIN, INT_MAX);

	mst::sort(input);

	CheckIsSorted(input);
}

TEST_CASE("sort: container & predictate", "[algorithm][container]")
{
	random_data_generator rdg{ true };
	INFO("Seed" << rdg.seed());

	const auto elemCount = GENERATE(range<size_t>(2, 1000));

	auto input = rdg.vector_int(elemCount, INT_MIN, INT_MAX);

	mst::sort(input, [](int l, int r) { return r < l; });

	CheckIsReverseSorted(input);
}

TEST_CASE("find: container", "[algorithm][container]")
{
	std::vector<int> input{ 3, 2, 5, 1, 4 };

	const auto foundIt = mst::find(input, 1);

	REQUIRE(foundIt != input.end());
	REQUIRE(foundIt - input.begin() == 3);
	REQUIRE(*foundIt == 1);
}

TEST_CASE("find_if: container", "[algorithm][container]")
{
	std::vector<int> input{ 3, 2, 5, 1, 4 };

	const auto foundIt = mst::find_if(input, [](int v) { return v > 4; });

	REQUIRE(foundIt != input.end());
	REQUIRE(foundIt - input.begin() == 2);
	REQUIRE(*foundIt == 5);
}