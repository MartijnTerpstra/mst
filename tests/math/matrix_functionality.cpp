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

#include "helpers.h"

//#define MST_MATH_ALL_VECTORS_MATRICES_SIMD 1
#include <mmath2.h>
#include <mcommon.h>

using mst::math::vector;
using mst::math::matrix;
using mst::math::quaternion;

TEST_CASE("matrix<V,C,R>: type support", "[matrix][simd]")
{
	REQUIRE(matrix<float, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<float, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<int32_t, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int32_t, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<double, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<double, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<long double, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<long double, 4, 4>(0)[0][0] == 0);

	REQUIRE(matrix<int64_t, 1, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 1, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 2, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 3, 4>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 1>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 2>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 3>(0)[0][0] == 0);
	REQUIRE(matrix<int64_t, 4, 4>(0)[0][0] == 0);
}

TEST_CASE("matrix<V,C,R>: constructor", "[matrix][simd]")
{
	vector<float, 3> fposition;
	quaternion<float> fquaternion;

	vector<double, 3> dposition;
	quaternion<double> dquaternion;

	vector<long double, 3> ldposition;
	quaternion<long double> ldquaternion;

	matrix<float, 3, 3>{ fquaternion };
	matrix<float, 4, 3>{ fposition, fquaternion };
	matrix<float, 4, 4>{ fposition, fquaternion };

	matrix<double, 3, 3>{ dquaternion };
	matrix<double, 4, 3>{ dposition, dquaternion };
	matrix<double, 4, 4>{ dposition, dquaternion };

	matrix<long double, 3, 3>{ ldquaternion };
	matrix<long double, 4, 3>{ ldposition, ldquaternion };
	matrix<long double, 4, 4>{ ldposition, ldquaternion };
}