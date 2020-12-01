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

#pragma once

namespace mst {
namespace math {

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 1>::matrix(
	const matrix<_xValue_type, _Columns, 1>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other._Data[0]);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 1>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(initVal);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 1>::matrix(
	const vector<_Value_type, _Columns>& _Row0) noexcept
{
	this->_Data[0] = _Row0;
}

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 2>::matrix(
	const matrix<_xValue_type, _Columns, 2>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other._Data[1]);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 2>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(initVal);
	this->_Data[1] = vector<_Value_type, _Columns>(initVal);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 2>::matrix(
	const vector<_Value_type, _Columns>& _Row0, const vector<_Value_type, _Columns>& _Row1) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
}

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 3>::matrix(
	const matrix<_xValue_type, _Columns, 3>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other._Data[1]);
	this->_Data[2] = vector<_Value_type, _Columns>(_Other._Data[2]);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 3>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(initVal);
	this->_Data[1] = vector<_Value_type, _Columns>(initVal);
	this->_Data[2] = vector<_Value_type, _Columns>(initVal);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 3>::matrix(
	const vector<_Value_type, _Columns>& _Row0, const vector<_Value_type, _Columns>& _Row1,
	const vector<_Value_type, _Columns>& _Row2) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
}

template<typename _Value_type>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 3>::matrix(
	const matrix<_xValue_type, 3, 3>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, 3>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, 3>(_Other._Data[1]);
	this->_Data[2] = vector<_Value_type, 3>(_Other._Data[2]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 3>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, 3>(initVal);
	this->_Data[1] = vector<_Value_type, 3>(initVal);
	this->_Data[2] = vector<_Value_type, 3>(initVal);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 3>::matrix(const vector<_Value_type, 3>& _Row0,
	const vector<_Value_type, 3>& _Row1, const vector<_Value_type, 3>& _Row2) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 3>::matrix(const quaternion<_Value_type>& _Orientation,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	this->set_orientation(_Orientation);
}

template<typename _Value_type>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 3>::matrix(
	const matrix<_xValue_type, 4, 3>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, 4>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, 4>(_Other._Data[1]);
	this->_Data[2] = vector<_Value_type, 4>(_Other._Data[2]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 3>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, 4>(initVal);
	this->_Data[1] = vector<_Value_type, 4>(initVal);
	this->_Data[2] = vector<_Value_type, 4>(initVal);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 3>::matrix(const vector<_Value_type, 4>& _Row0,
	const vector<_Value_type, 4>& _Row1, const vector<_Value_type, 4>& _Row2) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 3>::matrix(const vector<_Value_type, 3>& _Position,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	*this = this->identity;
	this->set_position(_Position);
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 3>::matrix(const vector<_Value_type, 3>& _Position,
	const quaternion<_Value_type>& _Orientation,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	this->set_position(_Position);
	this->set_orientation(_Orientation);
}

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 4>::matrix(
	const matrix<_xValue_type, _Columns, 4>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other._Data[1]);
	this->_Data[2] = vector<_Value_type, _Columns>(_Other._Data[2]);
	this->_Data[3] = vector<_Value_type, _Columns>(_Other._Data[3]);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 4>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(initVal);
	this->_Data[1] = vector<_Value_type, _Columns>(initVal);
	this->_Data[2] = vector<_Value_type, _Columns>(initVal);
	this->_Data[3] = vector<_Value_type, _Columns>(initVal);
}

template<typename _Value_type, size_t _Columns>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 4>::matrix(
	const ::mst::math::vector<_Value_type, _Columns>& _Row0,
	const ::mst::math::vector<_Value_type, _Columns>& _Row1,
	const ::mst::math::vector<_Value_type, _Columns>& _Row2,
	const ::mst::math::vector<_Value_type, _Columns>& _Row3) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
	this->_Data[3] = _Row3;
}

template<typename _Value_type>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(
	const matrix<_xValue_type, 4, 4>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, 4>(_Other._Data[0]);
	this->_Data[1] = vector<_Value_type, 4>(_Other._Data[1]);
	this->_Data[2] = vector<_Value_type, 4>(_Other._Data[2]);
	this->_Data[3] = vector<_Value_type, 4>(_Other._Data[3]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, 4>(initVal);
	this->_Data[1] = vector<_Value_type, 4>(initVal);
	this->_Data[2] = vector<_Value_type, 4>(initVal);
	this->_Data[3] = vector<_Value_type, 4>(initVal);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(
	const matrix<_Value_type, 4, 3>& _Row012, vector<_Value_type, 4>& _Row3) noexcept
{
	this->_Data[0] = _Row012._Data[0];
	this->_Data[1] = _Row012._Data[1];
	this->_Data[2] = _Row012._Data[2];
	this->_Data[3] = _Row3;
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(const vector<_Value_type, 4>& _Row0,
	const vector<_Value_type, 4>& _Row1, const vector<_Value_type, 4>& _Row2,
	const vector<_Value_type, 4>& _Row3) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
	this->_Data[3] = _Row3;
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(const vector<_Value_type, 3>& _Position,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	*this = this->identity;
	this->set_position(_Position);
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(const vector<_Value_type, 3>& _Position,
	const quaternion<_Value_type>& _Orientation,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	this->_Data[3] = vector<_Value_type, 4>(0, 0, 0, 1);
	this->set_position(_Position);
	this->set_orientation(_Orientation);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::set_orientation(
	const quaternion<_Value_type>& _Orientation) noexcept
{
	this->_Data[0][0] = 1 - 2 * (_Orientation.y * _Orientation.y + _Orientation.z * _Orientation.z);
	this->_Data[0][1] = 2 * (_Orientation.x * _Orientation.y - _Orientation.w * _Orientation.z);
	this->_Data[0][2] = 2 * (_Orientation.x * _Orientation.z + _Orientation.w * _Orientation.y);
	this->_Data[1][0] = 2 * (_Orientation.x * _Orientation.y + _Orientation.w * _Orientation.z);
	this->_Data[1][1] = 1 - 2 * (_Orientation.x * _Orientation.x + _Orientation.z * _Orientation.z);
	this->_Data[1][2] = 2 * (_Orientation.y * _Orientation.z - _Orientation.w * _Orientation.x);
	this->_Data[2][0] = 2 * (_Orientation.x * _Orientation.z - _Orientation.w * _Orientation.y);
	this->_Data[2][1] = 2 * (_Orientation.y * _Orientation.z + _Orientation.w * _Orientation.x);
	this->_Data[2][2] = 1 - 2 * (_Orientation.x * _Orientation.x + _Orientation.y * _Orientation.y);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::lookat(
	const vector<_Value_type, 3>& _Position, const vector<_Value_type, 3>& _Upvector) noexcept
{
	vector<_Value_type, 3> normalized_z = (_Position - get_position()).normalize();

	vector<_Value_type, 3> normalized_x = _Upvector.cross(normalized_z).normalize();

	vector<_Value_type, 3> normalized_y = normalized_z.cross(normalized_x).normalize();

	this->_Data[0][0] = normalized_x[0], this->_Data[1][0] = normalized_x[1],
	this->_Data[2][0] = normalized_x[2];
	this->_Data[0][1] = normalized_y[0], this->_Data[1][1] = normalized_y[1],
	this->_Data[2][1] = normalized_y[2];
	this->_Data[0][2] = normalized_z[0], this->_Data[1][2] = normalized_z[1],
	this->_Data[2][2] = normalized_z[2];
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4> matrix<_Value_type, 4, 4>::create_perspective(
	const radians<_Value_type>& _Fov, _Value_type _Width, _Value_type _Height,
	_Value_type _NearDepth, _Value_type _FarDepth) noexcept
{
	return create_perspective(_Fov, _Width / _Height, _NearDepth, _FarDepth);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4> matrix<_Value_type, 4, 4>::create_perspective(
	const radians<_Value_type>& _Fov, _Value_type _Aspect, _Value_type _NearDepth,
	_Value_type _FarDepth) noexcept
{
	const radians<long double> halfRadians = radians<long double>(_Fov) / 2;
	const long double aspect = _Aspect;

	const long double deltaZ = _FarDepth - _NearDepth;
	const long double sine = sin(halfRadians);
	const long double cotangent = cos(halfRadians) / sine;

	matrix<_Value_type, 4, 4> retval = matrix<_Value_type, 4, 4>::zero;

	/* First row */
	retval[0].x = _Value_type(cotangent / aspect);

	/* Second row */
	retval[1].y = _Value_type(cotangent);

	/* Third row */
	retval[2].z = _Value_type((_FarDepth) / deltaZ);
	retval[2].w = _Value_type(-_NearDepth * _FarDepth / deltaZ);

	/* Fourth row */
	retval[3].z = _Value_type(1);

	return retval;
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4> matrix<_Value_type, 4, 4>::create_orthographic(
	const aabb<_Value_type, 2>& _Aabb, _Value_type _NearDepth, _Value_type _FarDepth) noexcept
{
	matrix<_Value_type, 4, 4> retval = matrix<_Value_type, 4, 4>::zero;

	const ::mst::math::vector<_Value_type, 2> aabb_pos = (_Aabb.max + _Aabb.min) / _Value_type(2);
	const ::mst::math::vector<_Value_type, 2> aabb_extent =
		(_Aabb.max - _Aabb.min) / _Value_type(2);


	retval[0][0] = _Value_type(1) / aabb_extent.x;
	retval[1][1] = _Value_type(1) / aabb_extent.y;
	retval[2][2] = _Value_type(-1) / (_FarDepth - _NearDepth);
	retval[0][3] = (-aabb_pos.x) / (aabb_extent.x);
	retval[1][3] = (-aabb_pos.y) / (aabb_extent.y);
	retval[2][3] = _NearDepth / (_FarDepth - _NearDepth);
	retval[3][3] = _Value_type(1);

	return retval;
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4> matrix<_Value_type, 4, 4>::create_orthographic(
	const aabb<_Value_type, 3>& _Aabb) noexcept
{
	matrix<_Value_type, 4, 4> retval = matrix<_Value_type, 4, 4>::zero;

	const ::mst::math::vector<_Value_type, 2> aabb_pos =
		(swizzle::xy(_Aabb.max) + swizzle::xy(_Aabb.min)) / _Value_type(2);
	const ::mst::math::vector<_Value_type, 2> aabb_extent =
		(swizzle::xy(_Aabb.max) - swizzle::xy(_Aabb.min)) / _Value_type(2);


	retval[0][0] = _Value_type(1) / aabb_extent.x;
	retval[1][1] = _Value_type(1) / aabb_extent.y;
	retval[2][2] = _Value_type(1) / (_Aabb.max.z - _Aabb.min.z);
	retval[0][3] = (-aabb_pos.x) / (aabb_extent.x);
	retval[1][3] = (-aabb_pos.y) / (aabb_extent.y);
	retval[2][3] = -_Aabb.min.z / (_Aabb.max.z - _Aabb.min.z);
	retval[3][3] = _Value_type(1);

	return retval;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 _Value_type*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::data() noexcept
{
	return this->_Data[0].data();
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr const _Value_type*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::data() const noexcept
{
	return this->_Data[0].data();
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr size_t _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::size() const noexcept
{
	return _Columns * _Rows;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 vector<_Value_type, _Columns>&
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::operator[](size_t _Idx) noexcept
{
	MST_ASSERT(_Idx < _Rows, "index out of range");
	return this->_Data[_Idx];
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr const vector<_Value_type, _Columns>&
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::operator[](size_t _Idx) const noexcept
{
	MST_ASSERT(_Idx < _Rows, "index out of range");
	return this->_Data[_Idx];
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
inline matrix<_Value_type, _Rows, _Columns>
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::get_transpose() const noexcept
{
	matrix<_Value_type, _Rows, _Columns> retval;
	for(size_t i = 0; i < _Rows; ++i)
		for(size_t j = 0; j < _Columns; ++j)
		{
			retval[j][i] = this->_Data[i][j];
		}
	return retval;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 vector<_Value_type, _Columns>
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::get_row(size_t _Index) const noexcept
{
	row_type retval;

	for(size_t i = 0; i < _Columns; ++i)
	{
		retval[i] = this->_Data[_Index][_Columns];
	}

	return retval;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 vector<_Value_type, _Rows>
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::get_column(size_t _Index) const noexcept
{
	column_type retval;

	for(size_t i = 0; i < _Rows; ++i)
	{
		retval[i] = this->_Data[i][_Index];
	}

	return retval;
}

template<typename _Value_type, size_t _ColumnsAndRows>
_MST_CONSTEXPR17 matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1>
_Details::_Math_matrix_square<_Value_type, _ColumnsAndRows>::get_minor(
	size_t _Column, size_t _Row) const noexcept
{
	// indicate which col and row is being copied to dest
	size_t cCount = 0, rCount = 0;

	size_t order = _ColumnsAndRows;

	matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1> retval =
		matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1>::zero;

	for(size_t i = 0; i < order; i++)
	{
		if(i != _Row)
		{
			cCount = 0;
			for(size_t j = 0; j < order; j++)
			{
				// when j is not the element
				if(j != _Column)
				{
					retval[rCount][cCount] = this->_Data[i][j];
					++cCount;
				}
			}

			++rCount;
		}
	}

	return retval;
}

template<typename _Value_type, size_t _ColumnsAndRows>
_MST_CONSTEXPR17 _Value_type
_Details::_Math_matrix_square<_Value_type, _ColumnsAndRows>::get_determinant() const noexcept
{
	// the determinant value
	_Value_type det = 0;

	size_t order = _ColumnsAndRows;

	// allocate the cofactor matrix
	matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1> minor;

	for(size_t i = 0; i < order; i++)
	{
		// get minor of element (0,i)
		// GetMinor( mat, minor, 0, i );
		minor = get_minor(i, 0);
		// the recusion is here!

		det += (i % 2 == 1 ? -1 : 1) * this->_Data[0][i] * minor.get_determinant();
		// det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
	}

	return det;
}

template<typename _Value_type>
constexpr _Value_type
_Details::_Math_matrix_square<_Value_type, 1>::get_determinant() const noexcept
{
	return this->_Data[0][0];
}

template<typename _Value_type, size_t _ColumnsAndRows>
inline matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows>
_Details::_Math_matrix_square<_Value_type, _ColumnsAndRows>::get_inverse() const noexcept
{
	// get the determinant
	const _Value_type det = 1 / get_determinant();

	const size_t order = _ColumnsAndRows;

	matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows> retval;

	matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1> minor;

	for(size_t j = 0; j < order; j++)
	{
		for(size_t i = 0; i < order; i++)
		{
			// get the co-factor (matrix) of A(j,i)
			minor = get_minor(i, j);
			retval[i][j] = det * minor.get_determinant();
			if((i + j) % 2 == 1)
				retval[i][j] = -retval[i][j];
		}
	}

	// release memory
	// delete [] minor[0];

	return retval;
}

template<typename _Value_type>
constexpr typename _Details::_Math_matrix_square<_Value_type, 1>::_Matrix_t
_Details::_Math_matrix_square<_Value_type, 1>::get_inverse() const noexcept
{
	return matrix<_Value_type, 1, 1>(1 / this->_Data[0][0]);
}

template<typename _Value_type, size_t _RowsColumns, size_t _Rows>
_MST_CONSTEXPR17 vector<_Value_type, _Rows> operator*(
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

template<typename _Value_type, size_t _Columns, size_t _RowsColumns, size_t _Rows>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, _Rows> operator*(
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

#if !_MST_MATH_SIMD_ENABLED

template<typename _Value_type>
constexpr matrix<_Value_type, 4, 3> operator*(
	const matrix<_Value_type, 4, 3>& _Left, const matrix<_Value_type, 4, 3>& _Right) noexcept
{
	matrix<_Value_type, 4, 3> retval = matrix<_Value_type, 4, 3>::zero;

	for(int x = 0; x < 3; ++x)
	{
		for(int y = 0; y < 4; ++y)
			for(int i = 0; i < 3; ++i)
			{
				retval[x][y] += _Left[x][i] * _Right[i][y];
			}
		retval[x][3] += _Left[x][3];
	}

	return retval;
}

#endif

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 bool operator==(const matrix<_Value_type, _Columns, _Rows>& _Left,
	const matrix<_Value_type, _Columns, _Rows>& _Right) noexcept
{
	for(size_t x = 0; x < _Rows; ++x)
	{
		if(_Left[x] != _Right[x])
			return false;
	}

	return true;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 bool operator!=(const matrix<_Value_type, _Columns, _Rows>& _Left,
	const matrix<_Value_type, _Columns, _Rows>& _Right) noexcept
{
	return !(_Left == _Right);
}

namespace _Details {

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 const matrix<_Value_type, _Columns, _Rows> _Make_zero_matrix() noexcept
{
	return matrix<_Value_type, _Columns, _Rows>{ (_Value_type)0 };
}

template<typename _Value_type, size_t _ColumnsAndRows>
_MST_CONSTEXPR17 const matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows>
_Make_identity_matrix() noexcept
{
	matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows> m{ (_Value_type)0 };
	for(size_t i = 0; i < _ColumnsAndRows; ++i)
	{
		m[i][i] = (_Value_type)1;
	}
	return m;
}

template<typename _Value_type>
_MST_CONSTEXPR17 const matrix<_Value_type, 4, 3> _Make_identity4x3_matrix() noexcept
{
	matrix<_Value_type, 4, 3> m{ (_Value_type)0 };
	for(size_t i = 0; i < 3; ++i)
	{
		m[i][i] = (_Value_type)1;
	}
	return m;
}

} // namespace _Details

template<typename _Value_type, size_t _Columns, size_t _Rows>
const matrix<_Value_type, _Columns, _Rows>
	_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::zero =
		_Details::_Make_zero_matrix<_Value_type, _Columns, _Rows>();

template<typename _Value_type, size_t _ColumnsAndRows>
const matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows>
	_Details::_Math_matrix_square<_Value_type, _ColumnsAndRows>::identity =
		_Details::_Make_identity_matrix<_Value_type, _ColumnsAndRows>();

template<typename _Value_type>
const matrix<_Value_type, 1, 1> _Details::_Math_matrix_square<_Value_type, 1>::identity =
	_Details::_Make_identity_matrix<_Value_type, 1>();

template<typename _Value_type>
const matrix<_Value_type, 4, 3>
	matrix<_Value_type, 4, 3>::identity = _Details::_Make_identity4x3_matrix<_Value_type>();

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17
	typename _Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::_Matrix_t
	_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::rotated(
		radians<_Value_type> _X_angle, radians<_Value_type> _Y_angle, radians<_Value_type> _Z_angle,
		euler_rotation_order order) const noexcept
{
	byte orderingByte = (byte)order;

	_Matrix_t m = static_cast<const _Matrix_t&>(*this);

	for(int i = 0; i < 3; ++i)
	{
		switch(orderingByte & 3)
		{
		case 0:
			if(_X_angle.is_zero())
				m = m.rotated_x(_X_angle);
			break;
		case 1:
			if(_Y_angle.is_zero())
				m = m.rotated_y(_Y_angle);
			break;
		case 2:
			if(_Z_angle.is_zero())
				m = m.rotated_z(_Z_angle);
			break;
		default:
			MST_FATAL_ERROR("invalid euler ordering value");
		}

		orderingByte >>= 2;
	}

	return m;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
/* returns a copy of the matrix, that is rotated around the global x axis */
_MST_CONSTEXPR17
	typename _Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::_Matrix_t
	_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::rotated_x(
		radians<_Value_type> _Angle) const noexcept
{
	const _Value_type sx = sin(_Angle);
	const _Value_type cx = cos(_Angle);
	_Value_type _Right[9] = { 1, 0, 0, 0, cx, -sx, 0, sx, cx };

	_Matrix_t m = static_cast<const _Matrix_t&>(*this);

	for(size_t x = 0; x < 3; ++x)
		for(size_t y = 0; y < 3; ++y)
		{
			m[x][y] = (_Value_type)0;
			for(size_t i = 0; i < 3; ++i)
			{
				m[x][y] += (*this)[i][y] * _Right[x * 3 + i];
			}
		}

	return m;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
/* returns a copy of the matrix, that is rotated around the global y axis */
_MST_CONSTEXPR17
	typename _Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::_Matrix_t
	_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::rotated_y(
		radians<_Value_type> _Angle) const noexcept
{
	const _Value_type sy = sin(_Angle);
	const _Value_type cy = cos(_Angle);

	_Value_type _Right[9] = { cy, 0, sy, 0, 1, 0, -sy, 0, cy };

	_Matrix_t m = static_cast<const _Matrix_t&>(*this);

	for(size_t x = 0; x < 3; ++x)
		for(size_t y = 0; y < 3; ++y)
		{
			m[x][y] = (_Value_type)0;
			for(size_t i = 0; i < 3; ++i)
			{
				m[x][y] += (*this)[i][y] * _Right[x * 3 + i];
			}
		}

	return m;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
/* returns a copy of the matrix, that is rotated around the global z axis */
_MST_CONSTEXPR17
	typename _Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::_Matrix_t
	_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::rotated_z(
		radians<_Value_type> _Angle) const noexcept
{
	const _Value_type sz = sin(_Angle);
	const _Value_type cz = cos(_Angle);

	_Value_type _Right[9] = { cz, -sz, 0, sz, cz, 0, 0, 0, 1 };

	_Matrix_t m = static_cast<const _Matrix_t&>(*this);

	for(size_t x = 0; x < 3; ++x)
		for(size_t y = 0; y < 3; ++y)
		{
			m[x][y] = (_Value_type)0;
			for(size_t i = 0; i < 3; ++i)
			{
				m[x][y] += (*this)[i][y] * _Right[x * 3 + i];
			}
		}

	return m;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::get_position() const noexcept
{
	return vector<_Value_type, 3>(this->_Data[0][3], this->_Data[1][3], this->_Data[2][3]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::set_position(
	const vector<_Value_type, 3>& position) noexcept
{
	this->_Data[0][3] = position[0];
	this->_Data[1][3] = position[1];
	this->_Data[2][3] = position[2];
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::translate(
	const vector<_Value_type, 3>& translation) noexcept
{
	this->_Data[0][3] += translation[0];
	this->_Data[1][3] += translation[1];
	this->_Data[2][3] += translation[2];
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_left_direction()
	const noexcept
{
	return vector<_Value_type, 3>(-this->_Data[0][0], -this->_Data[1][0], -this->_Data[2][0]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_right_direction()
	const noexcept
{
	return vector<_Value_type, 3>(this->_Data[0][0], this->_Data[1][0], this->_Data[2][0]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_up_direction()
	const noexcept
{
	return vector<_Value_type, 3>(this->_Data[0][1], this->_Data[1][1], this->_Data[2][1]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_down_direction()
	const noexcept
{
	return vector<_Value_type, 3>(-this->_Data[0][1], -this->_Data[1][1], -this->_Data[2][1]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_forward_direction()
	const noexcept
{
	return vector<_Value_type, 3>(this->_Data[0][2], this->_Data[1][2], this->_Data[2][2]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_orientational<_Value_type, _Columns, _Rows>::get_backward_direction()
	const noexcept
{
	return vector<_Value_type, 3>(-this->_Data[0][2], -this->_Data[1][2], -this->_Data[2][2]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 void matrix<_Value_type, 4, 4>::scale(_Value_type _Scale) noexcept
{
	scale(vector3_type(_Scale, _Scale, _Scale));
}

template<typename _Value_type>
_MST_CONSTEXPR17 void matrix<_Value_type, 4, 4>::scale(
	const vector<_Value_type, 3>& _Scale) noexcept
{
	*this = matrix<_Value_type, 4, 4>(vector<_Value_type, 4>(_Scale.x, 0, 0, 0),
				vector<_Value_type, 4>(0, _Scale.y, 0, 0),
				vector<_Value_type, 4>(0, 0, _Scale.z, 0), vector<_Value_type, 4>(0, 0, 0, 1)) *
			*this;
}

} // namespace math
} // namespace mst