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

#include <mflag.h>

enum class Flags : uint32_t
{
	Value0,
	Value1,
	Value2
};

enum class PreshiftedFlags : uint32_t
{
	Value0 = 1 << 0,
	Value1 = 1 << 1,
	Value2 = 1 << 2
};

using mst::flag;

TEST_CASE("flag: empty", "[common]")
{
	flag<Flags> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));
}

TEST_CASE("flag: enable disable", "[common]")
{
	flag<Flags> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	f.enable(Flags::Value1);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	f.enable(Flags::Value2);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(f.is_enabled(Flags::Value2));

	f.disable(Flags::Value1);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(f.is_enabled(Flags::Value2));

	f.disable(Flags::Value2);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));
}

TEST_CASE("flag: toggle", "[common]")
{
	flag<Flags> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	f.toggle(Flags::Value1);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	f.toggle(Flags::Value1);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(!f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	f.toggle(Flags::Value1);
	f.toggle(Flags::Value2);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(f.is_enabled(Flags::Value2));

	f.toggle(Flags::Value2);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));
}

TEST_CASE("flag: preshifted empty", "[common]")
{
	flag<PreshiftedFlags, mst::flag_traits_preshifted<PreshiftedFlags>> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));
}

TEST_CASE("flag: preshifted enable disable", "[common]")
{
	flag<PreshiftedFlags, mst::flag_traits_preshifted<PreshiftedFlags>> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));

	f.enable(PreshiftedFlags::Value1);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));

	f.enable(PreshiftedFlags::Value2);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value2));

	f.disable(PreshiftedFlags::Value1);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value2));

	f.disable(PreshiftedFlags::Value2);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));
}

TEST_CASE("flag: preshifted toggle", "[common]")
{
	flag<PreshiftedFlags, mst::flag_traits_preshifted<PreshiftedFlags>> f;

	REQUIRE(f.get() == 0);
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));

	f.toggle(PreshiftedFlags::Value1);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));

	f.toggle(PreshiftedFlags::Value1);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));

	f.toggle(PreshiftedFlags::Value1);
	f.toggle(PreshiftedFlags::Value2);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value2));

	f.toggle(PreshiftedFlags::Value2);

	REQUIRE(!f.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f.is_enabled(PreshiftedFlags::Value2));
}

TEST_CASE("make_flag: single value")
{
	const auto f = mst::make_flag(Flags::Value1);

	REQUIRE(!f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	const auto f2 = mst::make_flag(PreshiftedFlags::Value1);

	REQUIRE(!f2.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f2.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f2.is_enabled(PreshiftedFlags::Value2));
}

TEST_CASE("make_flag: multiple values")
{
	const auto f = mst::make_flag(Flags::Value1, Flags::Value0);

	REQUIRE(f.is_enabled(Flags::Value0));
	REQUIRE(f.is_enabled(Flags::Value1));
	REQUIRE(!f.is_enabled(Flags::Value2));

	const auto f2 = mst::make_flag(PreshiftedFlags::Value1, PreshiftedFlags::Value0);

	REQUIRE(f2.is_enabled(PreshiftedFlags::Value0));
	REQUIRE(f2.is_enabled(PreshiftedFlags::Value1));
	REQUIRE(!f2.is_enabled(PreshiftedFlags::Value2));
}