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

template<typename _Value_type, size_t _Columns, size_t _RowsColumns, size_t _Rows>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, _Rows> ReferenceMatrixMultiply(
	const matrix<_Value_type, _Columns, _RowsColumns>& _Left,
	const matrix<_Value_type, _RowsColumns, _Rows>& _Right) noexcept
{
	matrix<_Value_type, _Columns, _Rows> retval = matrix<_Value_type, _Columns, _Rows>::zero;

	for(size_t x = 0; x < _Rows; ++x)
		for(size_t y = 0; y < _Columns; ++y)
			for(size_t i = 0; i < _RowsColumns; ++i)
			{
				retval[x][y] += _Left[x][i] * _Right[i][y];
			}

	return retval;
}

template<typename _Value_type, size_t _RowsColumns, size_t _Rows>
_MST_CONSTEXPR17 vector<_Value_type, _Rows> ReferenceVectorMatrixMultiply(
	const vector<_Value_type, _RowsColumns>& _Left,
	const matrix<_Value_type, _RowsColumns, _Rows>& _Right) noexcept
{
	vector<_Value_type, _Rows> retval = vector<_Value_type, _Rows>::zero;

	for(size_t x = 0; x < _Rows; ++x)
		for(size_t i = 0; i < _RowsColumns; ++i)
		{
			retval[x] += _Left[i] * _Right[i][x];
		}

	return retval;
}

template<typename V>
V TryNegate(V v, std::true_type)
{
	return -v;
}

template<typename V>
V TryNegate(V v, std::false_type)
{
	return v;
}

template<typename V, size_t CxR>
void TestMultiplication()
{
	CAPTURE(mst::typename_of<V>());
	CAPTURE(CxR);

	typedef matrix<V, CxR, CxR> mat;

	mat left = mat::zero;
	mat right = mat::zero;

	V value = (V)1;
	for(size_t i = 0; i < CxR; ++i)
		for(size_t j = 0; j < CxR; ++j)
		{
			left[i][j] = value;
			value += 1;
			right[i][j] = TryNegate(value, typename std::is_signed<V>::type{});
		}

	auto ref = ReferenceMatrixMultiply(left, right);

	auto simdresult = left * right;

	REQUIRE(memcmp(&ref, &simdresult, sizeof(ref)) == 0);

	typename mat::column_type vleft = { 1, 2, 3, 4 };

	auto vref = ReferenceVectorMatrixMultiply(vleft, right);

	auto vsimdresult = vleft * right;

	REQUIRE(memcmp(&vref, &vsimdresult, sizeof(vref)) == 0);
}

TEST_CASE("matrix<V,C,R>: matrix hierarchy", "[matrix][simd]")
{
	typedef matrix<float, 4, 4> mat;
	typedef mst::math::degrees<float> degrees;
	using mst::math::euler_rotation_order;

	auto parent = mat::identity;

	parent.set_position({ 2, 4, 6 });

	auto child =
		mat::identity.rotated(degrees(45), degrees(45), degrees(45), euler_rotation_order::xyz);

	auto result = parent * child;

	auto pos = result.get_position();

	INFO("position.x: " << pos.x);
	INFO("position.y: " << pos.y);
	INFO("position.z: " << pos.z);
}

TEST_CASE("matrix<V,C,R>: SIMDmultiplication", "[matrix][simd]")
{
	TestMultiplication<float, 4>();
	TestMultiplication<uint32_t, 4>();
	TestMultiplication<int32_t, 4>();
}

template<typename _Value_type, size_t _ColumnsAndRows>
inline matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows> ReferenceMatrixInverse(
	const matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows>& mat) noexcept
{
	// get the determinant
	const _Value_type det = 1 / mat.get_determinant();

	const size_t order = _ColumnsAndRows;

	matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows> retval;

	matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1> minor;

	for(size_t j = 0; j < order; j++)
	{
		for(size_t i = 0; i < order; i++)
		{
			// get the co-factor (matrix) of A(j,i)
			minor = mat.get_minor(i, j);
			retval[i][j] = det * minor.get_determinant();
			if((i + j) % 2 == 1)
				retval[i][j] = -retval[i][j];
		}
	}

	// release memory
	// delete [] minor[0];

	return retval;
}

template<typename ValueType>
void TestInverse()
{
	typedef matrix<ValueType, 4, 4> mat;
	typedef vector<ValueType, 3> float3;
	typedef mst::math::quaternion<ValueType> quat;
	typedef mst::math::degrees<ValueType> degrees;

	auto id = mat::identity;

	auto idinv = id.get_inverse();

	REQUIRE(id == idinv);

	mat left = { float3(1, 2, 3), quat(degrees(45), float3(1, 1, 1).normalized()) };

	mat right = { float3(100, 53, 727), quat(degrees(-45), float3(-1, 1, -1).normalized()) };

	auto rightinv = right.get_inverse();

	auto rightinvref = ReferenceMatrixInverse(right);

	REQUIRE_THAT(
		rightinv, mst::test::approx_equal(rightinvref, (ValueType)0.00001, (ValueType)0.00015));

	REQUIRE_THAT(left * right * rightinv,
		mst::test::approx_equal(left, (ValueType)0.00001, (ValueType)0.00015));
}

TEST_CASE("matrix<V,C,R>: SIMD inverse", "[matrix][simd]")
{
	TestInverse<float>();
	TestInverse<double>();
}