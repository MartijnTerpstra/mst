//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2022 Martinus Terpstra                                                 //
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

#include <cstdint>

namespace mst {
namespace math {

enum class euler_rotation_order : uint8_t;

namespace _Details {

template<typename _Value_type, size_t _Columns, size_t _Rows>
class _Math_matrix_data : public _Math_base<_Value_type>
{
protected:
	vector<_Value_type, _Columns> _Data[_Rows];
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
class _Math_matrix_base : public _Math_matrix_data<_Value_type, _Columns, _Rows>
{
public:
	typedef _Value_type value_type;
	typedef vector<_Value_type, _Columns> row_type;
	typedef vector<_Value_type, _Rows> column_type;
	typedef matrix<_Value_type, _Rows, _Columns> transpose_type;

	static const constexpr size_t rowCount = _Rows;
	static const constexpr size_t columnCount = _Columns;

protected:
	static_assert(std::is_arithmetic<_Value_type>::value,
		"vector<_Value_type,_Columns,_Rows>: _Value_type must be of an arithmetic type");

	template<typename _xValue_type, size_t _xColumns, size_t _xRows>
	friend class matrix;

	typedef mst::math::matrix<_Value_type, _Columns, _Rows> _Matrix_t;

public:
	/* 0 initialized matrix */
	static const matrix<_Value_type, _Columns, _Rows> zero;

	static const constexpr size_t dimensions = _Columns * _Rows;
	typedef typename ::std::is_unsigned<_Value_type> is_unsigned;

	_MST_NODISCARD inline transpose_type get_transpose() const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 row_type get_row(size_t index) const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 column_type get_column(size_t index) const noexcept;

	_MST_NODISCARD constexpr const _Value_type* data() const noexcept;
	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type* data() noexcept;

	[[nodiscard]] constexpr size_t size() const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 mst::math::vector<_Value_type, _Columns>& operator[](
		size_t _Idx) noexcept;
	_MST_NODISCARD constexpr const mst::math::vector<_Value_type, _Columns>& operator[](
		size_t _Idx) const noexcept;

	[[nodiscard]] inline mst::math::vector<_Value_type, _Columns>* begin() noexcept;
	[[nodiscard]] inline const mst::math::vector<_Value_type, _Columns>* begin() const noexcept;
	[[nodiscard]] inline const mst::math::vector<_Value_type, _Columns>* cbegin() const noexcept;

	[[nodiscard]] inline mst::math::vector<_Value_type, _Columns>* end() noexcept;
	[[nodiscard]] inline const mst::math::vector<_Value_type, _Columns>* end() const noexcept;
	[[nodiscard]] inline const mst::math::vector<_Value_type, _Columns>* cend() const noexcept;

	/* sets the values of the matrix */
	_MST_CONSTEXPR17 void set(const _Value_type (&_Values)[_Columns * _Rows]) noexcept;

	/* sets the values of the matrix */
	_MST_CONSTEXPR17 void set(const _Value_type (&_Values)[_Columns][_Rows]) noexcept;

	/* sets only a part of the matrix from _Matrix */
	template<size_t _xColumns, size_t _xRows>
	_MST_CONSTEXPR17 void set_part(const _Matrix_t& _Matrix, size_t _Column_count,
		size_t _Row_count, size_t _Column_offset = 0, size_t _Row_offset = 0) noexcept;

	/* sets only a part of the matrix from _Matrix */
	template<size_t _xColumns, size_t _xRows>
	_MST_CONSTEXPR17 void set_part(const matrix<_Value_type, _xColumns, _xRows>& _Matrix,
		size_t _Column_offset = 0, size_t _Row_offset = 0) noexcept;

	template<size_t _xColumns, size_t _xRows>
	/* sets only a part of the matrix from _Values */
	_MST_CONSTEXPR17 void set_part(const _Value_type (&_Values)[_xColumns][_xRows],
		size_t _Column_offset = 0, size_t _Row_offset = 0) noexcept;
};

template<typename _Value_type, size_t _ColumnsAndRows>
class _Math_matrix_square : public _Math_matrix_base<_Value_type, _ColumnsAndRows, _ColumnsAndRows>
{
public:
	/* identity matrix */
	static const mst::math::matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows> identity;

	_MST_NODISCARD _MST_CONSTEXPR17 matrix<_Value_type, _ColumnsAndRows - 1, _ColumnsAndRows - 1>
	get_minor(size_t _Column, size_t _Row) const noexcept;

	_MST_NODISCARD inline matrix<_Value_type, _ColumnsAndRows, _ColumnsAndRows>
	get_inverse() const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type get_determinant() const noexcept;
};

template<typename _Value_type>
class _Math_matrix_square<_Value_type, 1> : public _Math_matrix_base<_Value_type, 1, 1>
{
private:
	typedef matrix<_Value_type, 1, 1> _Matrix_t;

public:
	/* identity matrix */
	static const mst::math::matrix<_Value_type, 1, 1> identity;

	_MST_NODISCARD constexpr _Matrix_t get_inverse() const noexcept;

	_MST_NODISCARD constexpr _Value_type get_determinant() const noexcept;
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
struct _GetSquareBaseType
{
	typedef typename std::conditional<_Columns == _Rows, _Math_matrix_square<_Value_type, _Columns>,
		_Math_matrix_base<_Value_type, _Columns, _Rows>>::type type;
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
class _Math_matrix_orientational : public _GetSquareBaseType<_Value_type, _Columns, _Rows>::type
{

	typedef mst::math::matrix<_Value_type, _Columns, _Rows> _Matrix_t;

public:
	_MST_CONSTEXPR17 void set_orientation(const quaternion<_Value_type>& _Orientation) noexcept;

	/* returns the left vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_left_direction() const noexcept;

	/* returns the right vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_right_direction() const noexcept;

	/* returns the up vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_up_direction() const noexcept;

	/* returns the down vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_down_direction() const noexcept;

	/* returns the forward vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_forward_direction() const noexcept;

	/* returns the backward vector */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_backward_direction() const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 _Matrix_t rotated(radians<_Value_type> _X_angle,
		radians<_Value_type> _Y_angle, radians<_Value_type> _Z_angle,
		euler_rotation_order order) const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 _Matrix_t rotated_x(radians<_Value_type> _Angle) const noexcept;
	_MST_NODISCARD _MST_CONSTEXPR17 _Matrix_t rotated_y(radians<_Value_type> _Angle) const noexcept;
	_MST_NODISCARD _MST_CONSTEXPR17 _Matrix_t rotated_z(radians<_Value_type> _Angle) const noexcept;
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
class _Math_matrix_positional : public _Math_matrix_orientational<_Value_type, _Columns, _Rows>
{
public:
	/* the position of the matrix */
	_MST_NODISCARD constexpr vector<_Value_type, 3> get_position() const noexcept;

	/* the position of the matrix */
	_MST_CONSTEXPR17 void set_position(const vector<_Value_type, 3>& position) noexcept;

	/* translates the position */
	_MST_CONSTEXPR17 void translate(const vector<_Value_type, 3>& translation) noexcept;

	_MST_CONSTEXPR17 void lookat(
		const vector<_Value_type, 3>& _Direction, const vector<_Value_type, 3>& _Upvector) noexcept;
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
struct _GetMatrixBase
{
	/*

		if(!std::is_signed_v<_Value_type>)
			return _Math_matrix_base<_Value_type, _Columns, _Rows>;

		if(_Columns == 1 && _Rows == 1)
			return _Math_matrix_square<_Value_type, 1,1>;

		if (std::is_floating_point_v<_Value_type> && _Rows >= 3 && _Columns >= 4)
			return _Math_matrix_positional<_Value_type, _Columns, _Rows>;

		if (std::is_floating_point_v<_Value_type> && _Rows == 3 && _Columns == 3)
			return _Math_matrix_orientatational<_Value_type, _Columns, _Rows>;

		if (_Rows == _Columns)
			return _Math_matrix_square<_Value_type, _Columns, _Rows>;

		return _Math_matrix_base<_Value_type, _Columns, _Rows>;

	*/

	typedef typename ::std::conditional<!std::is_signed<_Value_type>::value ||
											!std::is_floating_point<_Value_type>::value,
		_Math_matrix_base<_Value_type, _Columns, _Rows>,
		typename ::std::conditional<_Rows >= 3 && _Columns >= 4,
			_Math_matrix_positional<_Value_type, _Columns, _Rows>,
			typename ::std::conditional<_Rows == 3 && _Columns == 3,
				_Math_matrix_orientational<_Value_type, _Columns, _Rows>,
				typename ::std::conditional<_Rows == _Columns,
					_Math_matrix_square<_Value_type, _Columns>,
					_Math_matrix_base<_Value_type, _Columns, _Rows>>::type>::type>::type>::type
		type;
};

} // namespace _Details

enum class euler_rotation_order : uint8_t
{
	xyz = (0 << 0) | (1 << 2) | (2 << 4),
	xzy = (0 << 0) | (2 << 2) | (1 << 4),
	yxz = (1 << 0) | (0 << 2) | (2 << 4),
	yzx = (1 << 0) | (2 << 2) | (0 << 4),
	zxy = (2 << 0) | (0 << 2) | (1 << 4),
	zyx = (2 << 0) | (1 << 2) | (0 << 4),
};

template<typename _Value_type, size_t _Columns, size_t _Rows>
class matrix : public _Details::_GetMatrixBase<_Value_type, _Columns, _Rows>::type
{ };

template<typename _Value_type, size_t _Columns>
class matrix<_Value_type, _Columns, 1>
	: public _Details::_GetMatrixBase<_Value_type, _Columns, 1>::type
{

public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, _Columns, 1>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, _Columns>& _Row0) noexcept;
};

template<typename _Value_type, size_t _Columns>
class matrix<_Value_type, _Columns, 2>
	: public _Details::_GetMatrixBase<_Value_type, _Columns, 2>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, _Columns, 2>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, _Columns>& _Row0,
		const ::mst::math::vector<_Value_type, _Columns>& _Row1) noexcept;
};

template<typename _Value_type, size_t _Columns>
class matrix<_Value_type, _Columns, 3>
	: public _Details::_GetMatrixBase<_Value_type, _Columns, 3>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, _Columns, 3>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, _Columns>& _Row0,
		const ::mst::math::vector<_Value_type, _Columns>& _Row1,
		const ::mst::math::vector<_Value_type, _Columns>& _Row2) noexcept;
};

template<typename _Value_type>
class matrix<_Value_type, 3, 3> : public _Details::_GetMatrixBase<_Value_type, 3, 3>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, 3, 3>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, 3>& _Row0,
		const ::mst::math::vector<_Value_type, 3>& _Row1,
		const ::mst::math::vector<_Value_type, 3>& _Row2) noexcept;

	template<typename _Ty = _Value_type>
	_MST_CONSTEXPR17 explicit matrix(const quaternion<_Value_type>& _Orientation,
		typename std::enable_if<std::is_signed<_Ty>::value &&
								std::is_floating_point<_Ty>::value>::type* = 0) noexcept;
};

template<typename _Value_type>
class matrix<_Value_type, 4, 3> : public _Details::_GetMatrixBase<_Value_type, 4, 3>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, 4, 3>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, 4>& _Row0,
		const ::mst::math::vector<_Value_type, 4>& _Row1,
		const ::mst::math::vector<_Value_type, 4>& _Row2) noexcept;

	template<typename _Ty = _Value_type>
	_MST_CONSTEXPR17 matrix(const vector<_Value_type, 3>& _Position,
		typename std::enable_if<std::is_signed<_Ty>::value &&
								std::is_floating_point<_Ty>::value>::type* = 0) noexcept;

	template<typename _Ty = _Value_type>
	_MST_CONSTEXPR17 matrix(const vector<_Value_type, 3>& _Position,
		const quaternion<_Value_type>& _Orientation,
		typename std::enable_if<std::is_signed<_Ty>::value &&
								std::is_floating_point<_Ty>::value>::type* = 0) noexcept;

	static const matrix<_Value_type, 4, 3> identity;
};

template<typename _Value_type, size_t _Columns>
class matrix<_Value_type, _Columns, 4>
	: public _Details::_GetMatrixBase<_Value_type, _Columns, 4>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, _Columns, 4>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 matrix(const ::mst::math::vector<_Value_type, _Columns>& _Row0,
		const ::mst::math::vector<_Value_type, _Columns>& _Row1,
		const ::mst::math::vector<_Value_type, _Columns>& _Row2,
		const ::mst::math::vector<_Value_type, _Columns>& _Row3) noexcept;
};

template<typename _Value_type>
class matrix<_Value_type, 4, 4> : public _Details::_GetMatrixBase<_Value_type, 4, 4>::type
{
public:
	constexpr matrix() = default;

	template<typename _xValue_type>
	_MST_CONSTEXPR17 explicit matrix(const matrix<_xValue_type, 4, 4>& _Other) noexcept;
	_MST_CONSTEXPR17 explicit matrix(_Value_type initVal) noexcept;

	_MST_CONSTEXPR17 explicit matrix(
		const matrix<_Value_type, 4, 3>& _Row012, vector<_Value_type, 4>& _Row3) noexcept;

	_MST_CONSTEXPR17 matrix(const vector<_Value_type, 4>& _Row0,
		const vector<_Value_type, 4>& _Row1, const vector<_Value_type, 4>& _Row2,
		const vector<_Value_type, 4>& _Row3) noexcept;

	template<typename _Ty = _Value_type>
	_MST_CONSTEXPR17 matrix(const vector<_Value_type, 3>& _Position,
		typename std::enable_if<std::is_signed<_Ty>::value &&
								std::is_floating_point<_Ty>::value>::type* = 0) noexcept;

	template<typename _Ty = _Value_type>
	_MST_CONSTEXPR17 matrix(const vector<_Value_type, 3>& _Position,
		const quaternion<_Value_type>& _Orientation,
		typename std::enable_if<std::is_signed<_Ty>::value &&
								std::is_floating_point<_Ty>::value>::type* = 0) noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 static matrix create_perspective(
		const radians<_Value_type>& _Fov, _Value_type width, _Value_type height,
		_Value_type _NearDepth, _Value_type _FarDepth) noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 static matrix create_perspective(
		const radians<_Value_type>& _Fov, _Value_type _Aspect, _Value_type _NearDepth,
		_Value_type _FarDepth) noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 static matrix create_orthographic(
		const aabb<_Value_type, 2>& _Aabb, _Value_type _NearDepth, _Value_type _FarDepth) noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 static matrix create_orthographic(
		const aabb<_Value_type, 3>& _Aabb) noexcept;

	_MST_CONSTEXPR17 void scale(_Value_type _Scale) noexcept;

	_MST_CONSTEXPR17 void scale(const vector<_Value_type, 3>& _Scale) noexcept;
};

} // namespace math
} // namespace mst