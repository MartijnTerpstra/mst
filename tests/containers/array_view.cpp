//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2025 Martinus Terpstra                                                 //
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

#include <marray_view.h>

TEST_CASE("array_view<T>: empty", "[array_view]")
{
	const mst::array_view<int> arrview;
	REQUIRE(arrview.size() == 0);
	REQUIRE(arrview.empty());
	REQUIRE(arrview.begin() == arrview.end());
	REQUIRE(arrview.cbegin() == arrview.cend());
	REQUIRE(arrview.end() - arrview.begin() == 0);
	REQUIRE(arrview.cend() - arrview.cbegin() == 0);

	for(auto elem : arrview)
	{
		_MST_UNUSED(elem);
		FAIL("should be empty");
	}
}

TEST_CASE("array_view<T>: single value", "[array_view]")
{
	int value = 21;
	const mst::array_view<int> arrview(value);
	REQUIRE(arrview.size() == 1);
	REQUIRE(!arrview.empty());
	REQUIRE(arrview[0] == 21);
	REQUIRE(*arrview.begin() == 21);
	REQUIRE(*arrview.cbegin() == 21);
	REQUIRE(*(arrview.end() - 1) == 21);
	REQUIRE(*(arrview.cend() - 1) == 21);

	int count = 0;
	for(auto elem : arrview)
	{
		++count;
		REQUIRE(elem == 21);
	}
	REQUIRE(count == 1);
}

TEST_CASE("array_view<T>: container value", "[array_view]")
{
	std::vector<int> value = { 21 };
	const mst::array_view<int> arrview(value);
	REQUIRE(arrview.size() == 1);
	REQUIRE(!arrview.empty());
	REQUIRE(arrview[0] == 21);
	REQUIRE(*arrview.begin() == 21);
	REQUIRE(*arrview.cbegin() == 21);
	REQUIRE(*(arrview.end() - 1) == 21);
	REQUIRE(*(arrview.cend() - 1) == 21);

	int count = 0;
	for(auto elem : arrview)
	{
		++count;
		REQUIRE(elem == 21);
	}
	REQUIRE(count == 1);
}

TEST_CASE("array_view<T>: pointer to array", "[array_view]")
{
	int* value = new int[1];
	value[0] = 21;
	const mst::array_view<int> arrview(value, 1);
	REQUIRE(arrview.size() == 1);
	REQUIRE(!arrview.empty());
	REQUIRE(arrview[0] == 21);
	REQUIRE(*arrview.begin() == 21);
	REQUIRE(*arrview.cbegin() == 21);
	REQUIRE(*(arrview.end() - 1) == 21);
	REQUIRE(*(arrview.cend() - 1) == 21);

	int count = 0;
	for(auto elem : arrview)
	{
		++count;
		REQUIRE(elem == 21);
	}
	REQUIRE(count == 1);
}

TEST_CASE("array_view<T>: array", "[array_view]")
{
	int value[1]{ 21 };
	const mst::array_view<int> arrview(value);
	REQUIRE(arrview.size() == 1);
	REQUIRE(!arrview.empty());
	REQUIRE(arrview[0] == 21);
	REQUIRE(*arrview.begin() == 21);
	REQUIRE(*arrview.cbegin() == 21);
	REQUIRE(*(arrview.end() - 1) == 21);
	REQUIRE(*(arrview.cend() - 1) == 21);

	int count = 0;
	for(auto elem : arrview)
	{
		++count;
		REQUIRE(elem == 21);
	}
	REQUIRE(count == 1);
}

void ConsumeInitList(mst::array_view<int> arrview)
{
	REQUIRE(arrview.size() == 1);
	REQUIRE(!arrview.empty());
	REQUIRE(arrview[0] == 21);
	REQUIRE(*arrview.begin() == 21);
	REQUIRE(*arrview.cbegin() == 21);
	REQUIRE(*(arrview.end() - 1) == 21);
	REQUIRE(*(arrview.cend() - 1) == 21);

	int count = 0;
	for(auto elem : arrview)
	{
		++count;
		REQUIRE(elem == 21);
	}
	REQUIRE(count == 1);
}

TEST_CASE("array_view<T>: std::initializer_list", "[array_view]")
{
	ConsumeInitList({ 21 });
}