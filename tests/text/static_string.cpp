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

#include <string>
#include <string_view>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <set_assertions.h>

#include <mstatic_string.h>

#include <variant>

#define STR(x)                                                                                     \
	std::get<const TestType*>([]() {                                                               \
		std::variant<const char*, const wchar_t*, const char16_t*, const char32_t*> strs;          \
		if constexpr(std::is_same_v<TestType, char>)                                               \
			strs = x;                                                                              \
		else if constexpr(std::is_same_v<TestType, wchar_t>)                                       \
			strs = L##x;                                                                           \
		else if constexpr(std::is_same_v<TestType, char16_t>)                                      \
			strs = u##x;                                                                           \
		else if constexpr(std::is_same_v<TestType, char32_t>)                                      \
			strs = U##x;                                                                           \
		return strs;                                                                               \
	}())

#define CHR(x)                                                                                     \
	std::get<TestType>([]() {                                                                      \
		std::variant<char, wchar_t, char16_t, char32_t> chrs;                                      \
		if constexpr(std::is_same_v<TestType, char>)                                               \
			chrs = x;                                                                              \
		else if constexpr(std::is_same_v<TestType, wchar_t>)                                       \
			chrs = L##x;                                                                           \
		else if constexpr(std::is_same_v<TestType, char16_t>)                                      \
			chrs = u##x;                                                                           \
		else if constexpr(std::is_same_v<TestType, char32_t>)                                      \
			chrs = U##x;                                                                           \
		return chrs;                                                                               \
	}())


#define OVERFLOW_STRING STR("012345678901234567890123")

TEMPLATE_TEST_CASE("static_string: OVERFLOW_STRING should be length 24", "[text]", char, wchar_t,
	char16_t, char32_t)
{
	REQUIRE(std::char_traits<TestType>::length(OVERFLOW_STRING) == 24);
}

TEMPLATE_TEST_CASE(
	"static_string: capacity() and max_size()", "[text]", char, wchar_t, char16_t, char32_t)
{
	::mst::basic_static_string<TestType, 11> s11;
	REQUIRE(s11.capacity() == 10);
	REQUIRE(s11.max_size() == 10);

	::mst::basic_static_string<TestType, 1024> s1024;
	REQUIRE(s1024.capacity() == 1023);
	REQUIRE(s1024.max_size() == 1023);
}

TEMPLATE_TEST_CASE("static_string::static_string()", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;
	const auto& cs = s;

	REQUIRE(s.empty());
	REQUIRE(s.length() == 0);
	REQUIRE(s.begin() == s.end());
	REQUIRE(cs.begin() == cs.end());
	REQUIRE(cs.cbegin() == cs.cend());
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(const CharT*)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s{ STR("Raw Literal") };
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("Raw Literal"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("Raw Literal"));
}

TEMPLATE_TEST_CASE("static_string::static_string(const CharT*): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	static_string s{ STR("012345678901234567890123") };
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(std::string_view)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s{ std::basic_string_view(STR("String view")) };
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("String view"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("String view"));
}

TEMPLATE_TEST_CASE("static_string::static_string(std::string_view): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	static_string s{ std::basic_string_view(OVERFLOW_STRING) };
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(std::string)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s{ std::basic_string<TestType>(STR("StdString")) };
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("StdString"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("StdString"));
}

TEMPLATE_TEST_CASE("static_string::static_string(std::string): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	static_string s{ std::basic_string(OVERFLOW_STRING) };
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(beginIter, endIter)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	std::basic_string<TestType> range{ STR("Range") };

	static_string s{ range.begin(), range.end() };
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("Range"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("Range"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("Range"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("Range"));
}

TEMPLATE_TEST_CASE("static_string::static_string(beginIter, endIter): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	std::basic_string<TestType> range{ OVERFLOW_STRING };

	static_string s{ range.begin(), range.end() };
}

TEMPLATE_TEST_CASE(
	"static_string::operator = (const CharT*)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;
	s = STR("Raw Literal");
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("Raw Literal"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("Raw Literal"));
}

TEMPLATE_TEST_CASE("static_string::operator = (const CharT*): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	static_string s;
	s = OVERFLOW_STRING;
}

TEMPLATE_TEST_CASE(
	"static_string::operator = (std::string_view)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;
	s = std::basic_string_view(STR("String view"));
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("String view"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("String view"));
}

TEMPLATE_TEST_CASE("static_string::operator = (std::string_view): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;


	static_string s;
	s = std::basic_string_view(OVERFLOW_STRING);
}

TEMPLATE_TEST_CASE(
	"static_string::operator = (std::string)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;
	s = std::basic_string<TestType>(STR("StdString"));
	const static_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("StdString"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("StdString"));
}

TEMPLATE_TEST_CASE("static_string::operator = (std::string): length overflow",
	"[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 24>;

	static_string s;
	s = std::basic_string(OVERFLOW_STRING);
}

TEMPLATE_TEST_CASE("static_string::str()", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	const auto stdStr = std::basic_string<TestType>(STR("StdString"));
	static_string s{ stdStr };
	const static_string& cs = s;

	REQUIRE(s.str() == stdStr);
	REQUIRE(cs.str() == stdStr);
}

TEMPLATE_TEST_CASE("static_string::operator[](size_t)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	const auto helloStr = STR("Hello");
	static_string s = helloStr;
	const static_string& cs = s;

	REQUIRE(s[0] == helloStr[0]);
	REQUIRE(s[1] == helloStr[1]);
	REQUIRE(s[2] == helloStr[2]);
	REQUIRE(s[3] == helloStr[3]);
	REQUIRE(s[4] == helloStr[4]);

	REQUIRE(cs[0] == helloStr[0]);
	REQUIRE(cs[1] == helloStr[1]);
	REQUIRE(cs[2] == helloStr[2]);
	REQUIRE(cs[3] == helloStr[3]);
	REQUIRE(cs[4] == helloStr[4]);
}

TEMPLATE_TEST_CASE("static_string::operator[](size_t): out of bounds", "[!shouldfail][text]", char,
	wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s = STR("Hello");

	auto& value = s[5];
	_MST_UNUSED(value);
}

TEMPLATE_TEST_CASE("static_string::operator[](size_t) const: out of bounds", "[!shouldfail][text]",
	char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s = STR("Hello");

	auto& value = s[5];
	_MST_UNUSED(value);
}

TEMPLATE_TEST_CASE("static_string::clear()", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s{ STR("Raw Literal") };

	REQUIRE(!s.empty());

	s.clear();

	const static_string& cs = s;

	REQUIRE(s.empty());
	REQUIRE(s.length() == 0);
	REQUIRE(s.begin() == s.end());
	REQUIRE(s.end() - s.begin() == 0);
	REQUIRE(cs.begin() == cs.end());
	REQUIRE(cs.end() - cs.begin() == 0);
	REQUIRE(cs.begin() == cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == 0);
}

TEMPLATE_TEST_CASE("static_string::push_back()", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;

	for(size_t i = 0; i < 100; ++i)
	{
		s.push_back('X');

		const static_string& cs = s;

		auto const elemCount = i + 1;

		REQUIRE(s.str() == std::basic_string<TestType>(elemCount, CHR('X')));
		REQUIRE(!s.empty());
		REQUIRE(s.length() == elemCount);
		REQUIRE(s.begin() != s.end());
		REQUIRE(s.end() - s.begin() == elemCount);
		REQUIRE(cs.begin() != cs.end());
		REQUIRE(cs.end() - cs.begin() == elemCount);
		REQUIRE(cs.begin() != cs.end());
		REQUIRE(cs.cend() - cs.cbegin() == elemCount);
	}
}

TEMPLATE_TEST_CASE(
	"static_string::push_back() overflow double check", "[text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;

	while(s.size() < s.max_size())
	{
		s.push_back('X');
	}
}

TEMPLATE_TEST_CASE(
	"static_string::push_back() overflow", "[!shouldfail][text]", char, wchar_t, char16_t, char32_t)
{
	using static_string = ::mst::basic_static_string<TestType, 1024>;

	static_string s;

	while(s.size() < s.max_size())
	{
		s.push_back('X');
	}

	s.push_back('E');
}