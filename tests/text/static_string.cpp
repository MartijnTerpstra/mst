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
	using stack_string = ::mst::basic_static_string<TestType, 1024>;

	stack_string s;
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
	using stack_string = ::mst::basic_static_string<TestType, 1024>;

	stack_string s{ STR("Raw Literal") };
	const stack_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("Raw Literal"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("Raw Literal"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("Raw Literal"));
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(std::string_view)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using stack_string = ::mst::basic_static_string<TestType, 1024>;

	stack_string s{ std::basic_string_view(STR("String view")) };
	const stack_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("String view"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("String view"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("String view"));
}

TEMPLATE_TEST_CASE(
	"static_string::static_string(std::string)", "[text]", char, wchar_t, char16_t, char32_t)
{
	using stack_string = ::mst::basic_static_string<TestType, 1024>;

	stack_string s{ std::basic_string<TestType>(STR("StdString")) };
	const stack_string& cs = s;

	REQUIRE(!s.empty());
	REQUIRE(s.length() == strlen("StdString"));
	REQUIRE(s.begin() != s.end());
	REQUIRE(s.end() - s.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.end() - cs.begin() == strlen("StdString"));
	REQUIRE(cs.begin() != cs.end());
	REQUIRE(cs.cend() - cs.cbegin() == strlen("StdString"));
}