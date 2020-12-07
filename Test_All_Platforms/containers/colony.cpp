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

#include <vector>
#include <mcolony.h>

using mst::colony;
using namespace mst::tests;

TEST_CASE("colony<T>: creation", "[colony]")
{
	struct data
	{
		int data[4];
	};

	colony<data> container;
}

TEST_CASE("colony<T>: expand and clear", "[colony]")
{
	random_data_generator rdg{ true };
	INFO("Seed" << rdg.seed());

	struct data
	{
		int data[4];
	};

	colony<data> datas;
	std::vector<colony<data>::iterator> iters;

	SECTION("Fill the colony with data and store iterators")
	{
		const auto elemCount = 8192 * 2;

		iters.reserve(elemCount);

		for(int i = 0; i < elemCount; ++i)
		{
			iters.push_back(datas.emplace());
		}

		REQUIRE(datas.size() == elemCount);

		SECTION("Clear the colony by erasing random items")
		{
			while(!iters.empty())
			{
				const auto iterIndex = rdg.scalar_int<size_t>(0, iters.size() - 1);

				datas.erase(iters[(size_t)iterIndex]);

				iters.erase(iters.begin() + iterIndex);
			}

			REQUIRE(datas.empty());
			REQUIRE(datas.size() == 0);

			for(auto& item : datas)
			{
				FAIL("Should be empty: ");
			}

			SECTION("Fill the colony with data again")
			{
				const size_t reserveSize = 8192 * 4 + 1;

				for(int i = 0; i < reserveSize; ++i)
				{
					datas.emplace();
				}

				REQUIRE(datas.size() == reserveSize);
			}
		}
	}
}

TEST_CASE("colony<T>: should call T constructor & destructor", "[colony]")
{
	static int32_t createdCount = 0;

	struct data
	{
	public:
		data()
		{
			++createdCount;
		}
		data(data&&) noexcept
		{
			++createdCount;
		}
		~data()
		{
			--createdCount;
		}
	};

	{
		colony<data> intint;

		std::vector<colony<data>::iterator> iters;
		iters.reserve(8192 * 2);

		for(int i = 0; i < 8192 * 2; ++i)
		{
			iters.push_back(intint.emplace());
		}

		std::mt19937 rand;

		int32_t loopIndex = 0;

		while(iters.size() > 8192)
		{
			std::uniform_int_distribution<> range(0, (int)iters.size() - 1);

			auto iterIndex = range(rand);

			intint.erase(iters[(size_t)iterIndex]);

			iters.erase(iters.begin() + iterIndex);

			++loopIndex;
		}

		for(int i = 0; i < 8192; ++i)
		{
			intint.emplace();
		}
	}

	REQUIRE(createdCount == 0);
}

TEST_CASE("colony<T>: clear and expand", "[colony]")
{
	struct data
	{
		int data[4];
	};

	colony<data> intint;

	SECTION("Fill the colony with data")
	{
		for(int i = 0; i < 8192 * 2; ++i)
		{
			intint.emplace();
		}

		size_t elemsEncountered = 0;
		for(auto& item : intint)
		{
			++elemsEncountered;
		}

		REQUIRE(elemsEncountered == intint.size());

		SECTION("Clear should leave behind an empty container")
		{
			intint.clear();

			REQUIRE(intint.empty());
			REQUIRE(intint.size() == 0);

			size_t elemsEncountered = 0;
			for(auto& item : intint)
			{
				++elemsEncountered;
			}

			REQUIRE(elemsEncountered == intint.size());

			SECTION("Fill the colony with data after clear()")
			{
				for(int i = 0; i < 8192; ++i)
				{
					intint.emplace();
				}

				size_t elemsEncountered = 0;
				for(auto item : intint)
				{
					++elemsEncountered;
				}

				REQUIRE(elemsEncountered == intint.size());
			}
		}
	}
}