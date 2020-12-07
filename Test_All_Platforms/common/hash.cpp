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

#include <mcommon.h>
#include <string>

TEST_CASE("hash32: literal & std::string are equal", "[common]")
{
	REQUIRE(mst::hash32("") == mst::hash32(std::string("")));
	REQUIRE(mst::hash32("test") == mst::hash32(std::string("test")));
}

TEST_CASE("hash64: literal std::string are equal", "[common]")
{
	REQUIRE(mst::hash64("") == mst::hash64(std::string("")));
	REQUIRE(mst::hash64("test") == mst::hash64(std::string("test")));
}

TEST_CASE("hash32: different values give different hashes", "[common]")
{
	REQUIRE(mst::hash32("1") != mst::hash32("2"));
	REQUIRE(mst::hash32("test1") != mst::hash32(std::string("test2")));
}

TEST_CASE("hash64: different values give different hashes", "[common]")
{
	REQUIRE(mst::hash64("1") != mst::hash64("2"));
	REQUIRE(mst::hash64("test1") != mst::hash64(std::string("test2")));
}