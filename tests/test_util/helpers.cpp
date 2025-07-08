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
#include <helpers.h>
#include <mmath2.h>

using namespace mst::test_util;

using vec3 = mst::math::vector<float, 3>;

TEST_CASE("mst::test_util::helpers::approx_equal: epsilon", "[test_util]")
{
	const vec3 value = { 1.0f, 1.0f, 1.0f };
	REQUIRE_THAT(value, approx_equal(vec3::zero, 1.0001f));
	REQUIRE_THAT(value, approx_equal(vec3{1.0f}, 0.0001f));
	REQUIRE_THAT(value, approx_equal(vec3{0.5f}, 0.50001f));
	REQUIRE_THAT(value, approx_equal(vec3{-0.5f}, 1.50001f));
}

TEST_CASE("mst::test_util::helpers::approx_equal: difference bigger than epsilon", "[test_util][!shouldfail]")
{
	const vec3 value = { 1.0f, 1.0f, 1.0f };
	REQUIRE_THAT(value, approx_equal(vec3{-0.5f}, 1.499999f));
}