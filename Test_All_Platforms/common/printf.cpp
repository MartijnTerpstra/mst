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
#include <numeric>

using namespace Catch::Generators;
using namespace Catch::Matchers;

template<typename IntType>
std::string ToOctStr(IntType value);

template<typename IntType>
std::string ToHexStr(IntType value, bool upper);

std::string ToPtrStr(size_t value);

TEST_CASE("printf: no args", "[common]")
{
	REQUIRE(mst::to_printf_string("This string contains no arguments so it should not fail") ==
			"This string contains no arguments so it should not fail");
}

TEST_CASE("printf: fail on missing argument", "[!shouldfail][common]")
{
	mst::to_printf_string("%c");
}

TEST_CASE("printf: fail on too much arguments", "[!shouldfail][common]")
{
	mst::to_printf_string("%c", 'c', 111);
}

TEST_CASE("printf: char", "[common]")
{
	for(char c = 32; c <= 126; ++c)
	{
		REQUIRE(mst::to_printf_string("%c", c) == std::string(1, c));
	}
}

TEST_CASE("printf: wchar_t", "[common]")
{
	char c = 32;
	for(wchar_t w = 32; w <= 126; ++w, ++c)
	{
		REQUIRE(mst::to_printf_string("%lc", w) == std::string(1, c));
	}
}

TEST_CASE("printf: std::string", "[common]")
{
	REQUIRE(mst::to_printf_string("%s", std::string("Test")) == "Test");
}

TEST_CASE("printf: char string", "[common]")
{
	REQUIRE(mst::to_printf_string("%s", "Test") == "Test");
}

TEST_CASE("printf: std::wstring", "[common]")
{
	REQUIRE(mst::to_printf_string("%ls", std::wstring(L"Test")) == "Test");
}

TEST_CASE("printf: wchar_t string", "[common]")
{
	REQUIRE(mst::to_printf_string("%ls", L"Test") == "Test");
}

TEST_CASE("printf: pointer", "[common]")
{
	const void* pNull = nullptr;

	const auto ptrStr = mst::to_printf_string("%p", pNull);
	REQUIRE((ptrStr == ToPtrStr(0) || ptrStr == "(nil)"));
}

TEST_CASE("printf: int8_t", "[common]")
{
	const auto minValue = std::numeric_limits<int8_t>::min();
	const auto maxValue = std::numeric_limits<int8_t>::max();

	auto value = minValue;
	do
	{
		REQUIRE(mst::to_printf_string("%hhd", value) == std::to_string(value));
		REQUIRE(mst::to_printf_string("%hhi", value) == std::to_string(value));
		++value;

	} while(value != minValue);
}

TEST_CASE("printf: uint8_t", "[common]")
{
	const auto minValue = std::numeric_limits<uint8_t>::min();
	const auto maxValue = std::numeric_limits<uint8_t>::max();

	auto value = minValue;
	do
	{
		REQUIRE(mst::to_printf_string("%hhu", value) == std::to_string(value));
		REQUIRE(mst::to_printf_string("%hho", value) == ToOctStr(value));
		REQUIRE(mst::to_printf_string("%hhx", value) == ToHexStr(value, false));
		REQUIRE(mst::to_printf_string("%hhX", value) == ToHexStr(value, true));

	} while(value != minValue);
}

TEST_CASE("printf: int16_t", "[common]")
{
	const auto minValue = std::numeric_limits<int16_t>::min();
	const auto maxValue = std::numeric_limits<int16_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%hd", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%hi", value) == std::to_string(value));
}

TEST_CASE("printf: uint16_t", "[common]")
{
	const auto minValue = std::numeric_limits<uint16_t>::min();
	const auto maxValue = std::numeric_limits<uint16_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%hu", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%ho", value) == ToOctStr(value));
	REQUIRE(mst::to_printf_string("%hx", value) == ToHexStr(value, false));
	REQUIRE(mst::to_printf_string("%hX", value) == ToHexStr(value, true));
}

TEST_CASE("printf: int32_t", "[common]")
{
	const auto minValue = std::numeric_limits<int32_t>::min();
	const auto maxValue = std::numeric_limits<int32_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%d", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%i", value) == std::to_string(value));
}

TEST_CASE("printf: uint32_t", "[common]")
{
	const auto minValue = std::numeric_limits<uint32_t>::min();
	const auto maxValue = std::numeric_limits<uint32_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%u", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%o", value) == ToOctStr(value));
	REQUIRE(mst::to_printf_string("%x", value) == ToHexStr(value, false));
	REQUIRE(mst::to_printf_string("%X", value) == ToHexStr(value, true));
}

TEST_CASE("printf: int64_t", "[common]")
{
	const auto minValue = std::numeric_limits<int64_t>::min();
	const auto maxValue = std::numeric_limits<int64_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%lld", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%lli", value) == std::to_string(value));
}

TEST_CASE("printf: uint64_t", "[common]")
{
	const auto minValue = std::numeric_limits<uint64_t>::min();
	const auto maxValue = std::numeric_limits<uint64_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));

	REQUIRE(mst::to_printf_string("%llu", value) == std::to_string(value));
	REQUIRE(mst::to_printf_string("%llo", value) == ToOctStr(value));
	REQUIRE(mst::to_printf_string("%llx", value) == ToHexStr(value, false));
	REQUIRE(mst::to_printf_string("%llX", value) == ToHexStr(value, true));
}

TEST_CASE("printf: float", "[common]")
{
	const auto value = GENERATE_COPY(take(10000, random(-1e16f, 1e16f)));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%f", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%F", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%e", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%E", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%g", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%G", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%a", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));

	REQUIRE_THAT(std::stof(mst::to_printf_string("%A", value)),
		WithinRel(value, 0.0001f) || Catch::Matchers::WithinAbs(value, 0.0001f));
}

TEST_CASE("printf: double", "[common]")
{
	const auto value = GENERATE_COPY(take(10000, random(-1e32, 1e32)));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%f", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%F", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%e", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%E", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%g", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%G", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%a", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));

	REQUIRE_THAT(std::stod(mst::to_printf_string("%A", value)),
		WithinRel(value, 0.00001) || Catch::Matchers::WithinAbs(value, 0.0000001));
}

template<typename IntType>
inline IntType Negate(IntType value, std::true_type)
{
	return -value;
}

template<typename IntType>
inline IntType Negate(IntType value, std::false_type)
{
	return value;
}

template<typename IntType>
std::string ToOctStr(IntType value)
{
	if(value == 0)
		return "0";

	std::string oct;

	bool isNegative = false;

	if(value < 0)
	{
		isNegative = true;
		value = Negate<IntType>(value, typename std::is_signed<IntType>::type());
	}

	while(value != 0)
	{
		oct.insert(0, 1, '0' + (value & 7));
		value >>= 3;
	}

	if(isNegative)
		oct.insert(0, 1, '-');

	return oct;
}

template<typename IntType>
std::string ToHexStr(IntType value, bool upper)
{
	if(value == 0)
		return "0";

	char hexCharLower[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
		'e', 'f' };

	char hexCharUpper[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D',
		'E', 'F' };

	std::string oct;

	bool isNegative = false;

	if(value < 0)
	{
		isNegative = true;
		value = Negate<IntType>(value, typename std::is_signed<IntType>::type());
	}

	while(value != 0)
	{
		const auto index = value & 15;

		oct.insert(0, 1, upper ? hexCharUpper[index] : hexCharLower[index]);
		value >>= 4;
	}

	if(isNegative)
		oct.insert(0, 1, '-');

	return oct;
}

std::string ToPtrStr(size_t value)
{
	char hexChar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
		'f' };

	std::string oct;

	for(size_t i = 0; i < sizeof(size_t) * 2; ++i)
	{
		const auto index = value & 15;

		oct.insert(0, 1, hexChar[index]);
		value >>= 4;
	}

	return oct;
}