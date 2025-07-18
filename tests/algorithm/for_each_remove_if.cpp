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

#include <malgorithm.h>
#include <vector>

TEST_CASE("for_each_remove_if: removing odd only", "[algorithm][iterate_remove]")
{
	std::vector<int> input;
	std::vector<int> iterated;

	for(int i = 0; i < 100; ++i)
	{
		input.push_back(i);
	}

	auto temp = input;

	// remove even
	mst::for_each_remove_if(temp, [&](int elem) {
		iterated.push_back(elem);
		return (elem & 1) != 0;
	});

	REQUIRE(temp.size() == input.size() / 2);
	REQUIRE(input == iterated);
}

TEST_CASE("for_each_remove_if: all false", "[algorithm][iterate_remove]")
{
	std::vector<int> input;
	std::vector<int> iterated;

	for(int i = 0; i < 100; ++i)
	{
		input.push_back(i);
	}

	auto temp = input;

	// remove even
	mst::for_each_remove_if(temp, [&](int elem) {
		iterated.push_back(elem);
		return false;
	});

	REQUIRE(temp.size() == input.size());
	REQUIRE(input == iterated);
}

TEST_CASE("for_each_remove_if: all true", "[algorithm][iterate_remove]")
{
	std::vector<int> input;
	std::vector<int> iterated;

	for(int i = 0; i < 100; ++i)
	{
		input.push_back(i);
	}

	auto temp = input;

	// remove even
	mst::for_each_remove_if(temp, [&](int elem) {
		iterated.push_back(elem);
		return true;
	});

	REQUIRE(temp.empty());
	REQUIRE(input == iterated);
}