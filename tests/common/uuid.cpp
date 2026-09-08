//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
//                                                                                          //
//      Permission is hereby granted, free of charge, to any person obtaining a copy        //
//      of this software and associated documentation files (the "Software"), to deal       //
//      in the Software without restriction, including without limitation the rights        //
//      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell           //
//      copies of the Software, and to permit persons to whom the Software is               //
//      furnished to do so, subject to the following conditions:                            //
//                                                                                          //
//      The above copyright notice and this permission notice shall be included in          //
//      all copies or substantial portions of the Software.                                 //
//                                                                                          //
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          //
//      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            //
//      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE         //
//      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              //
//      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       //
//      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           //
//      THE SOFTWARE.                                                                       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#include <catch2/catch_test_macros.hpp>

#include <set_assertions.h>

#include <set_assertions.h>
#include <muuid.h>

#include <array>

TEST_CASE("uuid: creation", "[uuid]")
{
	auto zero = mst::create_zero_uuid();
	auto random = mst::uuid::create();
	REQUIRE(random != zero);
}

TEST_CASE("uuid: zero constant matches create_zero_uuid", "[uuid]")
{
#if _MST_HAS_INLINE_VARIABLES
	REQUIRE(mst::uuid::zero == mst::create_zero_uuid());
#endif
}

TEST_CASE("uuid: construct from byte array matches byte-list constructor", "[uuid]")
{
	std::array<uint8_t, 16> bytes{ 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23,
		0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

	mst::uuid fromArray{ bytes };
	mst::uuid fromList{ 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67,
		0x89, 0xAB, 0xCD, 0xEF };

	REQUIRE(fromArray == fromList);
}

TEST_CASE("uuid: to_string produces the expected canonical form", "[uuid]")
{
	mst::uuid value{ 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67,
		0x89, 0xAB, 0xCD, 0xEF };

	REQUIRE(value.to_string() == "01234567-89AB-CDEF-0123-456789ABCDEF");
}

TEST_CASE("uuid: parse and try_parse decode a known string to the expected value", "[uuid]")
{
	mst::uuid expected{ 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67,
		0x89, 0xAB, 0xCD, 0xEF };

	REQUIRE(mst::uuid::parse("01234567-89AB-CDEF-0123-456789ABCDEF") == expected);
	REQUIRE(mst::uuid::try_parse("01234567-89AB-CDEF-0123-456789ABCDEF") == expected);
}

TEST_CASE("uuid: equality", "[uuid]")
{
	auto zero = mst::create_zero_uuid();
	auto random = mst::uuid::create();
	auto random2 = random;

	REQUIRE(zero != random);
	REQUIRE(random2 == random);
}

TEST_CASE("uuid: string conversions", "[uuid]")
{
	auto random = mst::uuid::create();
	auto random2 = mst::create_zero_uuid();

	REQUIRE(mst::uuid::try_parse(random.to_string()) == random);
	REQUIRE(mst::uuid::parse(random.to_string()) == random);
	REQUIRE(mst::uuid::try_parse(random2.to_string()) == random2);
	REQUIRE(mst::uuid::parse(random2.to_string()) == random2);

	REQUIRE(random.to_string() != mst::create_zero_uuid().to_string());
	REQUIRE(random != random2);
	REQUIRE(random.to_string() != random2.to_string());
}

TEST_CASE("uuid: try_parse returns nullopt on invalid string", "[uuid]")
{
	REQUIRE(!mst::uuid::try_parse("12BCDEF;~{dwgrgaRE df;f- 1403- /*-+").has_value());
}

TEST_CASE("uuid: try_parse returns nullopt missing -", "[uuid]")
{
	REQUIRE(!mst::uuid::try_parse("679A889B 4689-4E66-B0FA-71DA5D5C7C04").has_value());
}

TEST_CASE("uuid: try_parse returns nullopt on wrong length", "[uuid]")
{
	REQUIRE(!mst::uuid::try_parse("").has_value());
	REQUIRE(!mst::uuid::try_parse("679A889B-4689-4E66-B0FA-71DA5D5C7C0").has_value()); // too short
	REQUIRE(!mst::uuid::try_parse("679A889B-4689-4E66-B0FA-71DA5D5C7C044").has_value()); // too long
}

TEST_CASE("uuid: try_parse returns nullopt on out-of-range hex digit", "[uuid]")
{
	// each of these is otherwise a well-formed uuid string, but with a single character
	// (position 0) outside both the '0'-'9' and 'A'-'F' ranges - regression coverage for a
	// scalar-fallback validation bug that never rejected any invalid character.
	REQUIRE(!mst::uuid::try_parse("/79A889B-4689-4E66-B0FA-71DA5D5C7C04").has_value());
	REQUIRE(!mst::uuid::try_parse(":79A889B-4689-4E66-B0FA-71DA5D5C7C04").has_value());
	REQUIRE(!mst::uuid::try_parse("G79A889B-4689-4E66-B0FA-71DA5D5C7C04").has_value());
	REQUIRE(!mst::uuid::try_parse("a79A889B-4689-4E66-B0FA-71DA5D5C7C04").has_value());
}