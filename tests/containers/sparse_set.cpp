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

#include <msparse_set.h>
#include <random>

using mst::sparse_set;

TEST_CASE("sparse_set<E,I>: creation", "[sparse_set]")
{
	sparse_set<int, int> intint;
	const auto& cintint = intint;

	intint.emplace(100, 1000);

	REQUIRE(intint.size() == 1);
	REQUIRE(intint.contains(100));
	REQUIRE(intint.at(100) == 1000);
	REQUIRE(cintint.at(100) == 1000);

	REQUIRE(intint.cbegin()->second == 1000);

	REQUIRE(intint.begin() + 1 == intint.end());

	intint.clear();

	REQUIRE(intint.size() == 0);
	REQUIRE(!intint.contains(100));
}

TEST_CASE("sparse_set<E,I>: erasion", "[sparse_set]")
{
	sparse_set<int, int> intint;
	const auto& cintint = intint;

	intint.emplace(100, 1000);

	REQUIRE(intint.size() == 1);
	REQUIRE(intint.contains(100));
	REQUIRE(intint.at(100) == 1000);
	REQUIRE(cintint.at(100) == 1000);

	REQUIRE(intint.cbegin()->second == 1000);

	REQUIRE(intint.begin() + 1 == intint.end());

	intint.emplace(1000, 10000);

	intint.erase(100);
	intint.erase(1000);

	REQUIRE(intint.size() == 0);
	REQUIRE(!intint.contains(100));
}