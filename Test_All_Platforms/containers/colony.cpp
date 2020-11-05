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
#include <catch.hpp>

#include <vector>
#include <mcolony.h>
#include <random>

using mst::colony;

TEST_CASE("colony<T>_creation", "[colony]")
{
	struct data
	{
		int data[4];
	};

	colony<data> container;
}

TEST_CASE("colony<T>_expand_and_clear", "[colony]")
{
	struct data
	{
		int data[4];
	};

	colony<data> intint;

	std::vector<colony<data>::iterator> iters;
	iters.reserve(8192 * 2);

	for(int i = 0; i < 8192 * 2; ++i)
	{
		iters.push_back(intint.emplace());

		intint.check_freelist();
	}

	std::mt19937 rand;

	int32_t loopIndex = 0;

	while(iters.size() > 8192)
	{
		std::uniform_int_distribution<> range(0, (int)iters.size() - 1);

		auto iterIndex = range(rand);

		intint.erase(iters[(size_t)iterIndex]);

		iters.erase(iters.begin() + iterIndex);

		intint.check_freelist();

		++loopIndex;
	}

	for(int i = 0; i < 8192; ++i)
	{
		intint.emplace();

		intint.check_freelist();
	}
}

TEST_CASE("colony<T>_element_construction_destruction", "[colony]")
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

			intint.check_freelist();
		}

		std::mt19937 rand;

		int32_t loopIndex = 0;

		while(iters.size() > 8192)
		{
			std::uniform_int_distribution<> range(0, (int)iters.size() - 1);

			auto iterIndex = range(rand);

			intint.erase(iters[(size_t)iterIndex]);

			iters.erase(iters.begin() + iterIndex);

			intint.check_freelist();

			++loopIndex;
		}

		for(int i = 0; i < 8192; ++i)
		{
			intint.emplace();

			intint.check_freelist();
		}
	}

	REQUIRE(createdCount == 0);
}

TEST_CASE("colony<T>_clear", "[colony]")
{
	struct data
	{
		int data[4];
	};

	colony<data> intint;

	std::vector<colony<data>::iterator> iters;
	iters.reserve(8192 * 2);

	for(int i = 0; i < 8192 * 2; ++i)
	{
		iters.push_back(intint.emplace());

		intint.check_freelist();
	}

	intint.clear();
	intint.check_freelist();

	for(int i = 0; i < 8192; ++i)
	{
		intint.emplace();

		intint.check_freelist();
	}
}