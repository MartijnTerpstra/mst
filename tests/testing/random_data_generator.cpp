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

using namespace ::Catch::Generators;
using namespace ::mst::tests;

TEST_CASE("random_data_generator::scalar_int: range should be [min,max]", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is " << rdg.seed());

	for(int i = 0; i < 100000; ++i)
	{
		const auto rngNumber = rdg.scalar_int(-10000, 100000);

		INFO("Number is " << rngNumber);
		REQUIRE(rngNumber >= -10000);
		REQUIRE(rngNumber <= 100000);
	}
}

TEST_CASE("random_data_generator.scalar_int: returns the same value when min == max", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is " << rdg.seed());

	for(int i = -100000; i < 100000; ++i)
	{
		REQUIRE(rdg.scalar_int(i, i) == i);
	}
}

TEST_CASE("random_data_generator::scalar_int: fails on min > max", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is " << rdg.seed());

	REQUIRE_THROWS_AS(rdg.scalar_int(1, -1), std::invalid_argument);
}

TEST_CASE("random_data_generator::vector_int: range should be [min,max]", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is " << rdg.seed());

	const auto size = GENERATE(range<size_t>(1, 1000));

	INFO("Size is " << size);

	const auto rngVector = rdg.vector_int(size, -10000, 100000);

	REQUIRE(rngVector.size() == size);

	for(const auto rngNumber : rngVector)
	{
		INFO("Number is" << rngNumber);
		REQUIRE(rngNumber >= -10000);
		REQUIRE(rngNumber <= 100000);
	}

	const auto rngVectorRandom = rdg.vector_int(1, size, -10000, 100000);

	REQUIRE(rngVectorRandom.size() >= 1);
	REQUIRE(rngVectorRandom.size() <= size);

	for(const auto rngNumber : rngVectorRandom)
	{
		INFO("Number is" << rngNumber);
		REQUIRE(rngNumber >= -10000);
		REQUIRE(rngNumber <= 100000);
	}
}

TEST_CASE("random_data_generator.vector_int: returns the same value when min == max", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is " << rdg.seed());

	const auto size = GENERATE(range<size_t>(1, 1000));

	const auto rngVector = rdg.vector_int(size, (int)size, (int)size);

	REQUIRE(rngVector.size() == size);

	for(const auto rngNumber : rngVector)
	{
		INFO("Number is " << rngNumber);
		REQUIRE(rngNumber == (int)size);
	}

	const auto rngVectorRandom = rdg.vector_int(1, size, (int)size, (int)size);

	REQUIRE(rngVectorRandom.size() >= 1);
	REQUIRE(rngVectorRandom.size() <= size);

	for(const auto rngNumber : rngVectorRandom)
	{
		INFO("Number is" << rngNumber);
		REQUIRE(rngNumber == (int)size);
	}

	const auto rngVectorRandomOnlySize = rdg.vector_int(1, size, (int)size);

	REQUIRE(rngVectorRandomOnlySize.size() >= 1);
	REQUIRE(rngVectorRandomOnlySize.size() <= size);

	for(const auto rngNumber : rngVectorRandom)
	{
		INFO("Number is" << rngNumber);
		REQUIRE(rngNumber <= (int)size);
	}
}

TEST_CASE("random_data_generator::vector_int: fails on min > max", "[testing]")
{
	random_data_generator rdg{ false };
	INFO("Seed is" << rdg.seed());

	REQUIRE_THROWS_AS(rdg.vector_int(1, 1, -1), std::invalid_argument);
	REQUIRE_THROWS_AS(rdg.vector_int(1, 1, 1, -1), std::invalid_argument);
	REQUIRE_THROWS_AS(rdg.vector_int(1, 0, 1, 1), std::invalid_argument);
	REQUIRE_THROWS_AS(rdg.vector_int((size_t)1, (size_t)0, 1), std::invalid_argument);
}