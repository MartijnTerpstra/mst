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

#include <mcompiletime.h>
#include <mcommon.h>

template<typename ValueType, ValueType CompiletimeHash>
class Checker
{
public:
	bool IsSame(ValueType runtimeValue) const
	{
		return CompiletimeHash == runtimeValue;
	}
};

TEST_CASE("compiletime_hash32", "[compiletime]")
{
	Checker<uint32_t, mst::compiletime::hash32("TestValue")> checker;

	REQUIRE(checker.IsSame(mst::hash32("TestValue")));
}

TEST_CASE("compiletime_hash64", "[compiletime]")
{
	Checker<uint64_t, mst::compiletime::hash64("TestValue")> checker;

	REQUIRE(checker.IsSame(mst::hash64("TestValue")));
}