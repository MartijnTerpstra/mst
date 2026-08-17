//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
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

#pragma once

namespace mst {
namespace math {

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 1>::matrix(
	const matrix<_xValue_type, _Columns, 1>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other[0]);
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
	this->_Data[0] = vector<_Value_type, _Columns>(_Other[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other[1]);
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
	this->_Data[0] = vector<_Value_type, _Columns>(_Other[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other[1]);
	this->_Data[2] = vector<_Value_type, _Columns>(_Other[2]);
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
	this->_Data[0] = vector<_Value_type, 3>(_Other[0]);
	this->_Data[1] = vector<_Value_type, 3>(_Other[1]);
	this->_Data[2] = vector<_Value_type, 3>(_Other[2]);
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
_MST_CONSTEXPR17 matrix<_Value_type, 3, 4>::matrix(
	const matrix<_xValue_type, 3, 4>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, 3>(_Other[0]);
	this->_Data[1] = vector<_Value_type, 3>(_Other[1]);
	this->_Data[2] = vector<_Value_type, 3>(_Other[2]);
	this->_Data[3] = vector<_Value_type, 3>(_Other[3]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 4>::matrix(_Value_type initVal) noexcept
{
	this->_Data[0] = vector<_Value_type, 3>(initVal);
	this->_Data[1] = vector<_Value_type, 3>(initVal);
	this->_Data[2] = vector<_Value_type, 3>(initVal);
	this->_Data[3] = vector<_Value_type, 3>(initVal);
}

template<typename _Value_type>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 4>::matrix(const vector<_Value_type, 3>& _Row0,
	const vector<_Value_type, 3>& _Row1, const vector<_Value_type, 3>& _Row2,
	const vector<_Value_type, 3>& _Row3) noexcept
{
	this->_Data[0] = _Row0;
	this->_Data[1] = _Row1;
	this->_Data[2] = _Row2;
	this->_Data[3] = _Row3;
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 4>::matrix(const vector<_Value_type, 3>& _Position,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	*this = this->identity;
	this->set_position(_Position);
}

template<typename _Value_type>
template<typename _Ty>
_MST_CONSTEXPR17 matrix<_Value_type, 3, 4>::matrix(const vector<_Value_type, 3>& _Position,
	const quaternion<_Value_type>& _Orientation,
	typename std::enable_if<std::is_signed<_Ty>::value &&
							std::is_floating_point<_Ty>::value>::type*) noexcept
{
	/* rows 0-2 (orientation) and row 3 (position) between them cover every element -- no
		zero-padding needed, unlike matrix<V,4,4> below, since this type has no 4th column */
	this->set_position(_Position);
	this->set_orientation(_Orientation);
}

template<typename _Value_type, size_t _Columns>
template<typename _xValue_type>
_MST_CONSTEXPR17 matrix<_Value_type, _Columns, 4>::matrix(
	const matrix<_xValue_type, _Columns, 4>& _Other) noexcept
{
	this->_Data[0] = vector<_Value_type, _Columns>(_Other[0]);
	this->_Data[1] = vector<_Value_type, _Columns>(_Other[1]);
	this->_Data[2] = vector<_Value_type, _Columns>(_Other[2]);
	this->_Data[3] = vector<_Value_type, _Columns>(_Other[3]);
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
	this->_Data[0] = vector<_Value_type, 4>(_Other[0]);
	this->_Data[1] = vector<_Value_type, 4>(_Other[1]);
	this->_Data[2] = vector<_Value_type, 4>(_Other[2]);
	this->_Data[3] = vector<_Value_type, 4>(_Other[3]);
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
_MST_CONSTEXPR17 matrix<_Value_type, 4, 4>::matrix(const matrix<_Value_type, 3, 4>& _Columns012,
	const vector<_Value_type, 4>& _Column3) noexcept
{
	this->_Data[0] = vector<_Value_type, 4>(_Columns012[0], _Column3[0]);
	this->_Data[1] = vector<_Value_type, 4>(_Columns012[1], _Column3[1]);
	this->_Data[2] = vector<_Value_type, 4>(_Columns012[2], _Column3[2]);
	this->_Data[3] = vector<_Value_type, 4>(_Columns012[3], _Column3[3]);
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
	/* start from identity so rows 0-2's 4th column (unused by set_orientation) reads 0 and
		row 3's 4th column (unused by set_position) reads 1 */
	*this = this->identity;
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

/* create_perspective()/create_orthographic() build the matrix using this library's row-vector
	convention directly (see set_position()): row 3 carries the terms that only make sense as a
	constant offset (the ones a plain column-vector reference formula puts in the "translation"
	slot), rows 0-2 hold the pure scale/orientation terms. The result is usable as-is via plain
	`point * result` -- no `.get_transpose()` needed. This is the opposite of what FDOIT's
	Camera.cpp/Particles.cpp do (they predate this fix and compensate with an explicit transpose,
	or rely on the GPU's implicit transpose from uploading row-major data to a column-major,
	non-`row_major`, HLSL constant buffer); a consumer picking up this version needs to either
	add `row_major` to its HLSL matrix declarations or transpose before upload instead. */
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
	retval[2].w = _Value_type(1);

	/* Fourth row */
	retval[3].z = _Value_type(-_NearDepth * _FarDepth / deltaZ);

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
	/* maps [_NearDepth, _FarDepth] to [0, 1], matching the aabb<V,3> overload below */
	retval[2][2] = _Value_type(1) / (_FarDepth - _NearDepth);
	retval[3][0] = (-aabb_pos.x) / (aabb_extent.x);
	retval[3][1] = (-aabb_pos.y) / (aabb_extent.y);
	retval[3][2] = (-_NearDepth) / (_FarDepth - _NearDepth);
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
	retval[3][0] = (-aabb_pos.x) / (aabb_extent.x);
	retval[3][1] = (-aabb_pos.y) / (aabb_extent.y);
	retval[3][2] = -_Aabb.min.z / (_Aabb.max.z - _Aabb.min.z);
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
vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::begin() noexcept
{
	return this->_Data;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
const vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::begin() const noexcept
{
	return this->_Data;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
const vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::cbegin() const noexcept
{
	return this->_Data;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::end() noexcept
{
	return this->_Data + _Rows;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
const vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::end() const noexcept
{
	return this->_Data + _Rows;
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
const vector<_Value_type, _Columns>*
_Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::cend() const noexcept
{
	return this->_Data + _Rows;
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
		retval[i] = this->_Data[_Index][i];
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

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::set(
	const _Value_type (&_Values)[_Columns * _Rows]) noexcept
{
	/* _Values is laid out row-major, matching the layout returned by data() */
	for(size_t r = 0; r < _Rows; ++r)
		for(size_t c = 0; c < _Columns; ++c)
		{
			this->_Data[r][c] = _Values[r * _Columns + c];
		}
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::set(
	const _Value_type (&_Values)[_Columns][_Rows]) noexcept
{
	/* _Values is laid out column-major: _Values[column][row] */
	for(size_t c = 0; c < _Columns; ++c)
		for(size_t r = 0; r < _Rows; ++r)
		{
			this->_Data[r][c] = _Values[c][r];
		}
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
template<size_t _xColumns, size_t _xRows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::set_part(
	const matrix<_Value_type, _xColumns, _xRows>& _Matrix, size_t _Column_count, size_t _Row_count,
	size_t _Column_offset, size_t _Row_offset) noexcept
{
	MST_ASSERT(_Column_count <= _xColumns, "set_part: column count out of range of source matrix");
	MST_ASSERT(_Row_count <= _xRows, "set_part: row count out of range of source matrix");
	MST_ASSERT(_Column_offset + _Column_count <= _Columns,
		"set_part: column range out of range of destination matrix");
	MST_ASSERT(_Row_offset + _Row_count <= _Rows,
		"set_part: row range out of range of destination matrix");

	for(size_t r = 0; r < _Row_count; ++r)
		for(size_t c = 0; c < _Column_count; ++c)
		{
			this->_Data[_Row_offset + r][_Column_offset + c] = _Matrix[r][c];
		}
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
template<size_t _xColumns, size_t _xRows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::set_part(
	const matrix<_Value_type, _xColumns, _xRows>& _Matrix, size_t _Column_offset,
	size_t _Row_offset) noexcept
{
	MST_ASSERT(_Column_offset + _xColumns <= _Columns,
		"set_part: column range out of range of destination matrix");
	MST_ASSERT(
		_Row_offset + _xRows <= _Rows, "set_part: row range out of range of destination matrix");

	for(size_t r = 0; r < _xRows; ++r)
		for(size_t c = 0; c < _xColumns; ++c)
		{
			this->_Data[_Row_offset + r][_Column_offset + c] = _Matrix[r][c];
		}
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
template<size_t _xColumns, size_t _xRows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_base<_Value_type, _Columns, _Rows>::set_part(
	const _Value_type (&_Values)[_xColumns][_xRows], size_t _Column_offset,
	size_t _Row_offset) noexcept
{
	/* _Values is laid out column-major: _Values[column][row], matching set() */
	MST_ASSERT(_Column_offset + _xColumns <= _Columns,
		"set_part: column range out of range of destination matrix");
	MST_ASSERT(
		_Row_offset + _xRows <= _Rows, "set_part: row range out of range of destination matrix");

	for(size_t c = 0; c < _xColumns; ++c)
		for(size_t r = 0; r < _xRows; ++r)
		{
			this->_Data[_Row_offset + r][_Column_offset + c] = _Values[c][r];
		}
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
	const matrix<_Value_type, _Rows, _RowsColumns>& _Right) noexcept
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
	const matrix<_Value_type, _RowsColumns, _Rows>& _Left,
	const matrix<_Value_type, _Columns, _RowsColumns>& _Right) noexcept
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

/* fast path for composing two compact affine transforms without promoting either to a full 4x4:
	rows 0-2 (the orientation) compose as a plain 3x3 product; row 3 (the position) is _Left's
	position transformed by _Right's orientation, plus _Right's position -- i.e. this is
	equivalent to promoting both operands to matrix<V,4,4> (appending (0,0,0,1)), multiplying,
	and dropping the resulting (always (0,0,0,1)) 4th column. */
template<typename _Value_type>
constexpr matrix<_Value_type, 3, 4> operator*(
	const matrix<_Value_type, 3, 4>& _Left, const matrix<_Value_type, 3, 4>& _Right) noexcept
{
	matrix<_Value_type, 3, 4> retval = matrix<_Value_type, 3, 4>::zero;

	for(int x = 0; x < 3; ++x)
		for(int y = 0; y < 3; ++y)
			for(int i = 0; i < 3; ++i)
			{
				retval[x][y] += _Left[x][i] * _Right[i][y];
			}

	for(int y = 0; y < 3; ++y)
	{
		for(int i = 0; i < 3; ++i)
		{
			retval[3][y] += _Left[3][i] * _Right[i][y];
		}
		retval[3][y] += _Right[3][y];
	}

	return retval;
}

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
_MST_CONSTEXPR17 const matrix<_Value_type, 3, 4> _Make_identity3x4_matrix() noexcept
{
	/* rows 0-2 form a 3x3 identity (the orientation), row 3 (the position) stays zero */
	matrix<_Value_type, 3, 4> m{ (_Value_type)0 };
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
const matrix<_Value_type, 3, 4> matrix<_Value_type, 3, 4>::identity =
	_Details::_Make_identity3x4_matrix<_Value_type>();

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
			if(!_X_angle.is_zero())
				m = m.rotated_x(_X_angle);
			break;
		case 1:
			if(!_Y_angle.is_zero())
				m = m.rotated_y(_Y_angle);
			break;
		case 2:
			if(!_Z_angle.is_zero())
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

/* the translation lives in row 3 (columns 0-2), not column 3 of rows 0-2: with this library's
	row-vector `point * M` convention, only a dedicated row can add a constant offset to the
	result without also scaling it by the point's own x/y/z (see the class comment above
	create_perspective()). Rows 0-2 hold the pure (unscaled) basis vectors. */
template<typename _Value_type, size_t _Columns, size_t _Rows>
constexpr vector<_Value_type, 3>
_Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::get_position() const noexcept
{
	return vector<_Value_type, 3>(this->_Data[3][0], this->_Data[3][1], this->_Data[3][2]);
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::set_position(
	const vector<_Value_type, 3>& position) noexcept
{
	this->_Data[3][0] = position[0];
	this->_Data[3][1] = position[1];
	this->_Data[3][2] = position[2];
}

template<typename _Value_type, size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 void _Details::_Math_matrix_positional<_Value_type, _Columns, _Rows>::translate(
	const vector<_Value_type, 3>& translation) noexcept
{
	this->_Data[3][0] += translation[0];
	this->_Data[3][1] += translation[1];
	this->_Data[3][2] += translation[2];
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
	scale(vector<_Value_type, 3>(_Scale, _Scale, _Scale));
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