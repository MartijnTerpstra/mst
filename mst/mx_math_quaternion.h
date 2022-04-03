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

namespace mst {
namespace math {

template<typename _Value_type>
class quaternion : private ::mst::math::_Details::_Math_base<_Value_type>
{
public:
	/*static_assert(::std::is_floating_point<_Value_type>::value &&
		::std::is_signed<_Value_type>::value, "quaternion<_Value_type>: _Value_type must be of a
	   signed floating point type");*/

	static const size_t dimensions = 4;
	typedef _Value_type value_type;
	typedef typename ::std::is_unsigned<_Value_type> is_unsigned;

	constexpr quaternion() noexcept;

	template<typename _Value_type2>
	constexpr explicit quaternion(const quaternion<_Value_type2>& _Other) noexcept;

	/* constructs a quaternion with a rotation axis and an angle */
	_MST_CONSTEXPR17 quaternion(
		radians<_Value_type> _Angle, const vector<_Value_type, 3>& _Normalized_axis) noexcept;

	/* constructs a quaternion from all the loose components */
	constexpr quaternion(_Value_type _W, _Value_type _X, _Value_type _Y, _Value_type _Z) noexcept;

	/* constructs a quaternion from a single value */
	constexpr explicit quaternion(_Value_type initValue) noexcept;

	template<size_t _Columns, size_t _Rows>
	/* constructs a quaternion from a 4x4 matrix */
	_MST_CONSTEXPR17 explicit quaternion(
		const matrix<_Value_type, _Columns, _Rows>& _Matrix) noexcept;

	/* constructs a quaternion's values from an array of values */
	constexpr quaternion(const _Value_type (&_Values)[4]) noexcept;

	/* returns the conjugate */
	_MST_CONSTEXPR17 quaternion get_conjugate() const noexcept;

	/* returns the inverse */
	_MST_CONSTEXPR17 quaternion get_inverse() const noexcept;

	/* returns the left vector */
	constexpr vector<_Value_type, 3> get_left_direction() const noexcept;

	/* returns the right vector */
	constexpr vector<_Value_type, 3> get_right_direction() const noexcept;

	/* returns the up vector */
	constexpr vector<_Value_type, 3> get_up_direction() const noexcept;

	/* returns the down vector */
	constexpr vector<_Value_type, 3> get_down_direction() const noexcept;

	/* returns the forward vector */
	constexpr vector<_Value_type, 3> get_forward_direction() const noexcept;

	/* returns the backward vector */
	constexpr vector<_Value_type, 3> get_backward_direction() const noexcept;

	/* returns a vector with xyz as axis and w as angle */
	_MST_CONSTEXPR17 vector<_Value_type, 4> to_axis_angle() const noexcept;

	/* normalizes itself and returns a reference to itself */
	_MST_CONSTEXPR17 quaternion& normalize() noexcept;

	/* returns the normalized version of itself */
	_MST_CONSTEXPR17 quaternion normalized() const noexcept;

	/* returns the length of the quaternion */
	_MST_CONSTEXPR17 _Value_type length() const noexcept;

	/* returns the square length of the quaternion */
	constexpr _Value_type squared_length() const noexcept;

	/* returns a copy of vec rotated by the orientation of the quaternion */
	_MST_CONSTEXPR17 vector<_Value_type, 3> rotate_point(
		const vector<_Value_type, 3>& _Vec) const noexcept;

	/* rotates the quaternion with quat's rotation */
	_MST_CONSTEXPR17 void rotate(const quaternion& _Quat) noexcept;

	/* returns a copy of the quaternion rotated with quat's rotation */
	constexpr quaternion rotated(const quaternion& _Quat) const noexcept;

	/* makes the quaternion lookat the position */
	_MST_CONSTEXPR17 void look_at(const vector<_Value_type, 3>& _Position,
		const vector<_Value_type, 3>& _Up = vector<_Value_type, 3>(0, 1, 0)) noexcept;

	/* returns the dot product of this and quat */
	constexpr _Value_type dot(const quaternion& _Quat) const noexcept;

	/* returns a quaternion which is the spherical linear interpolation result between this and quat
	 */
	_MST_CONSTEXPR17 quaternion slerp(const quaternion& _To, _Value_type _Interp) const noexcept;

	// operator overloads

	_MST_CONSTEXPR17 quaternion& operator*=(const quaternion& _Quat) noexcept;
	_MST_CONSTEXPR17 quaternion& operator*=(_Value_type _Val) noexcept;

	_MST_CONSTEXPR17 quaternion& operator/=(_Value_type _Val) noexcept;

	constexpr quaternion operator-() const noexcept;

	_MST_CONSTEXPR17 _Value_type& operator[](size_t _Index) noexcept;
	constexpr const _Value_type& operator[](size_t _Index) const noexcept;

	_Value_type w, x, y, z;

	static const quaternion identity;

private:
	template<typename _xValue_type>
	friend class quaternion;
};

template<typename _Value_type>
const quaternion<_Value_type> quaternion<_Value_type>::identity{ 1, 0, 0, 0 };

} // namespace math
} // namespace mst