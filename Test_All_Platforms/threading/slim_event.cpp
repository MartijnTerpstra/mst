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

#include <mthreading_slim.h>

using mst::threading::slim::event;

TEST_CASE("threading::slim::event: creation", "[thread][slim][event]")
{
	event e{ false, false };
}

TEST_CASE("threading::slim::event: single thread (manual)", "[thread][slim][event]")
{
	event e{ false, true };

	REQUIRE(!e.try_wait());

	e.set();

	REQUIRE(e.try_wait());

	e.reset();

	REQUIRE(!e.try_wait());
}

TEST_CASE("threading::slim::event: single thread (auto)", "[thread][slim][event]")
{
	event e{ false, false };

	REQUIRE(!e.try_wait());

	e.set();

	REQUIRE(e.try_wait());

	REQUIRE(!e.try_wait());

	e.set();
	e.reset();

	REQUIRE(!e.try_wait());
}