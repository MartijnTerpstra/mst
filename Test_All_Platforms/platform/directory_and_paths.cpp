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

//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <set_assertions.h>

#include <mplatform.h>

int main(int argc, char** argv)
{
	Catch::Session().run(argc, argv);
}

TEST_CASE("platform::newline", "[platform]")
{
	REQUIRE(mst::platform::newline().length() > 0);
}

TEST_CASE("platform::directory_separator", "[platform]")
{
	REQUIRE(mst::platform::directory_separator() != 0);
}

TEST_CASE("platform::downloads_path", "[platform]")
{
	REQUIRE(mst::platform::downloads_path().length() > 0);
}

TEST_CASE("platform::desktop_path", "[platform]")
{
	REQUIRE(mst::platform::desktop_path().length() > 0);
}

TEST_CASE("platform::my_documents_path", "[platform]")
{
	REQUIRE(mst::platform::my_documents_path().length() > 0);
}

TEST_CASE("platform::temp_path", "[platform]")
{
	REQUIRE(mst::platform::temp_path().length() > 0);
}

TEST_CASE("platform::recycle_bin_path", "[platform]")
{
	REQUIRE(mst::platform::recycle_bin_path().length() > 0);
}

TEST_CASE("platform::current_directory", "[platform]")
{
	REQUIRE(mst::platform::current_directory().length() > 0);
}

TEST_CASE("platform::set_current_directory", "[platform]")
{
	auto current = mst::platform::current_directory();

	current.push_back(mst::platform::directory_separator());
	current.append("set_current_directory_test_dir");

	REQUIRE(mst::platform::current_directory() != current);

	REQUIRE(mst::platform::create_directory(current.c_str()));

	mst::platform::set_current_directory(current);

	REQUIRE(mst::platform::current_directory() == current);
}