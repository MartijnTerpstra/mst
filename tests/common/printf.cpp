//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2022 Martinus Terpstra                                                 //
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

TEST_CASE("printf: wstring support", "[common]")
{
	REQUIRE(mst::to_printf_string(L"pre%d%s%zupost", 123, "hello", 644) == L"pre123hello644post");
}

TEST_CASE("printf: forward non-format characters", "[common]")
{
	REQUIRE(mst::to_printf_string("Prefix%dPostFix--", 111) == "Prefix111PostFix--");
}

TEST_CASE("printf: prints excaped % once", "[common]")
{
	REQUIRE(mst::to_printf_string("pre%%post") == "pre%post");
}

TEST_CASE("printf: char", "[common]")
{
	for(char c = 32; c <= 126; ++c)
	{
		REQUIRE(mst::to_printf_string("%c", c) == std::string(1, c));
	}
}

TEST_CASE("printf: char safe downcast", "[common]")
{
	REQUIRE(mst::to_printf_string("%c", std::intmax_t(32)) == std::string(1, 32));
}

TEST_CASE("printf: fail char on downcast outside int_t range", "[!shouldfail][common]")
{
	static_assert(std::numeric_limits<std::uintmax_t>::max() > INT_MAX);
	mst::to_printf_string("%c", std::uintmax_t(INT_MAX) + 1);
}

TEST_CASE("printf: fail on char type", "[!shouldfail][common]")
{
	mst::to_printf_string("%c", 1.0f);
}

TEST_CASE("printf: wide char", "[common]")
{
	for(wchar_t c = 32; c <= 126; ++c)
	{
		REQUIRE(mst::to_printf_string("%lc", c) == std::string(1, c));
	}
}

TEST_CASE("printf: wide char safe downcast", "[common]")
{
	REQUIRE(mst::to_printf_string("%lc", std::intmax_t(32)) == std::string(1, 32));
}

TEST_CASE("printf: fail wide char on downcast outside wint_t range", "[!shouldfail][common]")
{
	static_assert(std::numeric_limits<std::uintmax_t>::max() > WINT_MAX);
	mst::to_printf_string("%lc", std::uintmax_t(WINT_MAX) + 1);
}

TEST_CASE("printf: std::string", "[common]")
{
	REQUIRE(mst::to_printf_string("%s", std::string("Test")) == "Test");
}

TEST_CASE("printf: std::string_view", "[common]")
{
	REQUIRE(mst::to_printf_string("%s", std::string_view("Test")) == "Test");
}

TEST_CASE("printf: char string", "[common]")
{
	REQUIRE(mst::to_printf_string("%s", "Test") == "Test");
}

TEST_CASE("printf: char string with explicit padding", "[common]")
{
	REQUIRE(mst::to_printf_string("%7s", "Test") == "   Test");
}

TEST_CASE("printf(wchar_t): char string with explicit padding", "[common]")
{
	REQUIRE(mst::to_printf_string(L"%7s", "Test") == L"   Test");
}

TEST_CASE("printf: char string with explict shorter length", "[common]")
{
	REQUIRE(mst::to_printf_string("%2.3s", "Test") == "Tes");
}

TEST_CASE("printf (wchar_t): char string with explict shorter length", "[common]")
{
	REQUIRE(mst::to_printf_string(L"%2.3s", "Test") == L"Tes");
}

TEST_CASE("printf: fail char string type mismatch", "[!shouldfail][common]")
{
	mst::to_printf_string("%s", 613);
}

TEST_CASE("printf: fail char string invalid padding", "[!shouldfail][common]")
{
	mst::to_printf_string("%B.s", "Test");
}

TEST_CASE("printf: fail char string invalid padding (no dot)", "[!shouldfail][common]")
{
	mst::to_printf_string("%Bs", "Test");
}

TEST_CASE("printf (wchar_t): fail char string invalid padding", "[!shouldfail][common]")
{
	mst::to_printf_string(L"%B.s", "Test");
}

TEST_CASE("printf (wchar_t): fail char string invalid padding (no dot)", "[!shouldfail][common]")
{
	mst::to_printf_string(L"%Bs", "Test");
}

TEST_CASE("printf: fail char string invalid length", "[!shouldfail][common]")
{
	mst::to_printf_string("%.Bs", "Test");
}

TEST_CASE("printf (wchar_t): fail char string invalid length", "[!shouldfail][common]")
{
	mst::to_printf_string(L"%.Bs", "Test");
}

TEST_CASE("printf: std::wstring", "[common]")
{
	REQUIRE(mst::to_printf_string("%ls", std::wstring(L"Test")) == "Test");
}

TEST_CASE("printf: wchar_t string", "[common]")
{
	REQUIRE(mst::to_printf_string("%ls", L"Test") == "Test");
}

TEST_CASE("printf: wchar_t string with explict padding", "[common]")
{
	REQUIRE(mst::to_printf_string("%7ls", L"Test") == "   Test");
}

TEST_CASE("printf (wchar_t): wchar_t string with explict padding", "[common]")
{
	REQUIRE(mst::to_printf_string(L"%7ls", L"Test") == L"   Test");
}

TEST_CASE("printf: wchar_t string with explict shorter length", "[common]")
{
	REQUIRE(mst::to_printf_string("%2.3ls", L"Test") == "Tes");
}

TEST_CASE("printf (wchar_t): wchar_t string with explict shorter length", "[common]")
{
	REQUIRE(mst::to_printf_string(L"%2.3ls", L"Test") == L"Tes");
}

TEST_CASE("printf: fail wchar_t string type mismatch", "[!shouldfail][common]")
{
	mst::to_printf_string("%ls", 613);
}

TEST_CASE("printf: fail wchar_t string invalid padding", "[!shouldfail][common]")
{
	mst::to_printf_string("%B.ls", L"Test");
}

TEST_CASE("printf: fail wchar_t string invalid length", "[!shouldfail][common]")
{
	mst::to_printf_string("%.Bls", L"Test");
}

TEST_CASE("printf: null pointer", "[common]")
{
	const void* pNull = nullptr;

	const auto ptrStr = mst::to_printf_string("%p", pNull);

	std::string expectedValue = ToPtrStr(0);

	REQUIRE(ptrStr == expectedValue);
}

TEST_CASE("printf: pointer", "[common]")
{
	const auto minValue = std::numeric_limits<size_t>::min();
	const auto maxValue = std::numeric_limits<size_t>::max();
	const auto value = GENERATE_COPY(take(10000, random(minValue, maxValue)));
	const void* ptr = reinterpret_cast<const void*>(value);

	const auto ptrStr = mst::to_printf_string("%p", ptr);

	const auto expectedValue = ToPtrStr(value);

	REQUIRE(ptrStr == expectedValue);
}

TEST_CASE("printf: int8_t", "[common]")
{
	const auto minValue = std::numeric_limits<int8_t>::min();

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

	auto value = minValue;
	do
	{
		REQUIRE(mst::to_printf_string("%hhu", value) == std::to_string(value));
		REQUIRE(mst::to_printf_string("%hho", value) == ToOctStr(value));
		REQUIRE(mst::to_printf_string("%hhx", value) == ToHexStr(value, false));
		REQUIRE(mst::to_printf_string("%hhX", value) == ToHexStr(value, true));

		++value;

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

TEST_CASE("printf: n receives current length", "[common]")
{
	size_t elem = 0;
	mst::to_printf_string("prefix%npostfix", &elem);
	REQUIRE(elem == strlen("prefix"));
}

TEST_CASE("printf: fail n received invalid type", "[!shouldfail][common]")
{
	mst::to_printf_string("prefix%npostfix", 1.0f);
}

TEST_CASE("printf: fail n does not fit in output type", "[!shouldfail][common]")
{
	int8_t s = 0;
	mst::to_printf_string("%266s%npostfix", &s);
}

template<typename IntType>
std::string ToOctStr(IntType value)
{
	if(value == 0)
		return "0";

	std::string oct;

	bool isNegative = false;

	if constexpr(std::is_signed_v<IntType>)
	{
		if(value < 0)
		{
			isNegative = true;
			value = -value;
		}
	}

	while(value != 0)
	{
		oct.insert(oct.begin(), 1, static_cast<char>('0' + (value & 7)));
		value >>= 3;
	}

	if constexpr(std::is_signed_v<IntType>)
	{
		if(isNegative)
			oct.insert(0, 1, '-');
	}

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

	if constexpr(std::is_signed_v<IntType>)
	{
		if(value < 0)
		{
			isNegative = true;
			value = -value;
		}
	}

	while(value != 0)
	{
		const auto index = value & 15;

		oct.insert(0, 1, upper ? hexCharUpper[index] : hexCharLower[index]);
		value >>= 4;
	}

	if constexpr(std::is_signed_v<IntType>)
	{
		if(isNegative)
			oct.insert(0, 1, '-');
	}

	return oct;
}

std::string ToPtrStr(size_t value)
{
	const auto expectedLength = ToHexStr(std::numeric_limits<size_t>::max(), false).length();
	auto expectedValue = ToHexStr(value, false);
	expectedValue.insert(expectedValue.begin(), expectedLength - expectedValue.length(), '0');

	return "0x" + expectedValue;
}