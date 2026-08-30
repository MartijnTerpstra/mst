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

#define _MST_QUATERNION_EPSILON 1e-4

template<typename _Value_type>
constexpr quaternion<_Value_type>::quaternion() noexcept
	: w(1)
	, x(0)
	, y(0)
	, z(0)
{ }

template<typename _Value_type>
template<typename _xValue_type>
constexpr quaternion<_Value_type>::quaternion(const quaternion<_xValue_type>& _Other) noexcept
	: w(static_cast<_Value_type>(_Other.w))
	, x(static_cast<_Value_type>(_Other.x))
	, y(static_cast<_Value_type>(_Other.y))
	, z(static_cast<_Value_type>(_Other.z))
{ }

template<typename _Value_type>
/* constructs a quaternion with a rotation axis and an angle */
_MST_CONSTEXPR17 quaternion<_Value_type>::quaternion(
	radians<_Value_type> _Angle, const vector<_Value_type, 3>& _Normalized_axis) noexcept
	: w(cos(_Angle / (_Value_type)2.0))
	, x(0)
	, y(0)
	, z(0)
{
	MST_ASSERT(fabs(_Normalized_axis.length() - 1) <= (_Value_type)_MST_QUATERNION_EPSILON,
		"quaternion needs to be normalized again: ", _Normalized_axis.length());

	const _Value_type halfAngleSin = sin(_Angle / 2);
	x = _Normalized_axis.x * halfAngleSin;
	y = _Normalized_axis.y * halfAngleSin;
	z = _Normalized_axis.z * halfAngleSin;
}

template<typename _Value_type>
/* constructs a quaternion from all the loose components */
constexpr quaternion<_Value_type>::quaternion(
	_Value_type _W, _Value_type _X, _Value_type _Y, _Value_type _Z) noexcept
	: w(_W)
	, x(_X)
	, y(_Y)
	, z(_Z)
{ }

template<typename _Value_type>
/* constructs a quaternion from a single value */
constexpr quaternion<_Value_type>::quaternion(_Value_type initVal) noexcept
	: w(initVal)
	, x(initVal)
	, y(initVal)
	, z(initVal)
{ }

template<typename _Value_type>
template<size_t _Columns, size_t _Rows>
_MST_CONSTEXPR17 quaternion<_Value_type>::quaternion(
	const matrix<_Value_type, _Columns, _Rows>& _Matrix) noexcept
{
	static_assert(_Columns >= 3 && _Rows >= 3, "matrix too small");

	_Value_type trace = _Matrix[0][0] + _Matrix[1][1] + _Matrix[2][2];

	if(trace > 0)
	{
		_Value_type s = (_Value_type)0.5 / sqrt(trace + 1);
		this->w = (_Value_type)0.25 / s;
		this->x = (_Matrix[2][1] - _Matrix[1][2]) * s;
		this->y = (_Matrix[0][2] - _Matrix[2][0]) * s;
		this->z = (_Matrix[1][0] - _Matrix[0][1]) * s;
	}
	else
	{
		if(_Matrix[0][0] > _Matrix[1][1] && _Matrix[0][0] > _Matrix[2][2])
		{
			_Value_type s = 2 * sqrt(1 + _Matrix[0][0] - _Matrix[1][1] - _Matrix[2][2]);
			this->w = (_Matrix[2][1] - _Matrix[1][2]) / s;
			this->x = (_Value_type)0.25 * s;
			this->y = (_Matrix[1][0] + _Matrix[0][1]) / s;
			this->z = (_Matrix[2][0] + _Matrix[0][2]) / s;
		}
		else
		{
			if(_Matrix[1][1] > _Matrix[2][2])
			{
				_Value_type s = 2 * sqrt(1 + _Matrix[1][1] - _Matrix[0][0] - _Matrix[2][2]);
				this->w = (_Matrix[0][2] - _Matrix[2][0]) / s;
				this->x = (_Matrix[1][0] + _Matrix[0][1]) / s;
				this->y = (_Value_type)0.25 * s;
				this->z = (_Matrix[2][1] + _Matrix[1][2]) / s;
			}
			else
			{
				_Value_type s = 2 * sqrt(1 + _Matrix[2][2] - _Matrix[0][0] - _Matrix[1][1]);
				this->w = (_Matrix[1][0] - _Matrix[0][1]) / s;
				this->x = (_Matrix[2][0] + _Matrix[0][2]) / s;
				this->y = (_Matrix[2][1] + _Matrix[1][2]) / s;
				this->z = (_Value_type)0.25 * s;
			}
		}
	}
}

template<typename _Value_type>
/* constructs a quaternion's values from an array of values */
constexpr quaternion<_Value_type>::quaternion(const _Value_type (&_Values)[4]) noexcept
	: w(_Values[0])
	, x(_Values[1])
	, y(_Values[2])
	, z(_Values[3])
{ }

template<typename _Value_type>
/* returns the conjugate */
_MST_CONSTEXPR17 quaternion<_Value_type> quaternion<_Value_type>::get_conjugate() const noexcept
{
	return quaternion<_Value_type>(w, -x, -y, -z);
}

template<typename _Value_type>
/* returns the inverse */
_MST_CONSTEXPR17 quaternion<_Value_type> quaternion<_Value_type>::get_inverse() const noexcept
{
	quaternion<_Value_type> retval = get_conjugate();
	retval /= squared_length();
	return retval;
}

template<typename _Value_type>
/* returns the left vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_left_direction() const noexcept
{
	return vector<_Value_type, 3>(
		-1.0f + 2.0f * (y * y + z * z), -2.0f * (x * y + w * z), -2.0f * (x * z - w * y));
}

template<typename _Value_type>
/* returns the right vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_right_direction() const noexcept
{
	return vector<_Value_type, 3>(
		1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y + w * z), 2.0f * (x * z - w * y));
}

template<typename _Value_type>
/* returns the up vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_up_direction() const noexcept
{
	return vector<_Value_type, 3>(
		2.0f * (x * y - w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z + w * x));
}

template<typename _Value_type>
/* returns the down vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_down_direction() const noexcept
{
	return vector<_Value_type, 3>(
		-2.0f * (x * y - w * z), -1.0f + 2.0f * (x * x + z * z), -2.0f * (y * z + w * x));
}

template<typename _Value_type>
/* returns the forward vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_forward_direction() const noexcept
{
	return vector<_Value_type, 3>(
		2.0f * (x * z + w * y), 2.0f * (y * z - w * x), 1.0f - 2.0f * (x * x + y * y));
}

template<typename _Value_type>
/* returns the backward vector */
constexpr vector<_Value_type, 3> quaternion<_Value_type>::get_backward_direction() const noexcept
{
	return vector<_Value_type, 3>(
		-2.0f * (x * z + w * y), -2.0f * (y * z - w * x), -1.0f + 2.0f * (x * x + y * y));
}

template<typename _Value_type>
/* returns a vector with xyz as axis and w as angle */
_MST_CONSTEXPR17 vector<_Value_type, 4> quaternion<_Value_type>::to_axis_angle() const noexcept
{
	vector<_Value_type, 4> axisAngle;

	const radians<_Value_type> halfAngle = acos(this->w);

	if(halfAngle.count() < _MST_EPSILON)
	{
		axisAngle.x = 0;
		axisAngle.y = 0;
		axisAngle.z = 0;
		axisAngle.w = 0;
		return axisAngle;
	}

	const _Value_type sinf_theta_inv = 1 / sin(halfAngle);

	axisAngle.x = this->x * sinf_theta_inv;
	axisAngle.y = this->y * sinf_theta_inv;
	axisAngle.z = this->z * sinf_theta_inv;
	axisAngle.w = halfAngle.count() * 2;

	return axisAngle;
}

template<typename _Value_type>
/* normalizes itself and returns a reference to itself */
_MST_CONSTEXPR17 quaternion<_Value_type>& quaternion<_Value_type>::normalize() noexcept
{
	const _Value_type invLength = 1 / length();
	this->w *= invLength;
	this->x *= invLength;
	this->y *= invLength;
	this->z *= invLength;

	return *this;
}

template<typename _Value_type>
/* returns the normalized version of itself */
_MST_CONSTEXPR17 quaternion<_Value_type> quaternion<_Value_type>::normalized() const noexcept
{
	return quaternion<_Value_type>(*this).normalize();
}

template<typename _Value_type>
/* returns the length of the quaternion */
_MST_CONSTEXPR17 _Value_type quaternion<_Value_type>::length() const noexcept
{
	return sqrt(squared_length());
}

template<typename _Value_type>
/* returns the square length of the quaternion */
constexpr _Value_type quaternion<_Value_type>::squared_length() const noexcept
{
	return (&this->w)[0] * (&this->w)[0] + (&this->w)[1] * (&this->w)[1] +
		   (&this->w)[2] * (&this->w)[2] + (&this->w)[3] * (&this->w)[3];
}

template<typename _Value_type>
/* returns a copy of vec rotated by the orientation of the quaternion */
_MST_CONSTEXPR17 vector<_Value_type, 3> quaternion<_Value_type>::rotate_point(
	const vector<_Value_type, 3>& _Vec) const noexcept
{
	MST_ASSERT(fabs(length() - 1) <= (_Value_type)_MST_QUATERNION_EPSILON,
		"quaternion is not normalized, length: ", length());

	const _Value_type qx2 = (&this->w)[1] * (&this->w)[1];
	const _Value_type qy2 = (&this->w)[2] * (&this->w)[2];
	const _Value_type qz2 = (&this->w)[3] * (&this->w)[3];

	vector<_Value_type, 3> dx = { 1 - 2 * qy2 - 2 * qz2, 2 * x * y - 2 * z * w,
		2 * x * z + 2 * y * w },
						   dy = { 2 * x * y + 2 * z * w, 1 - 2 * qx2 - 2 * qz2,
							   2 * y * z - 2 * x * w },
						   dz = { 2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w,
							   1 - 2 * qx2 - 2 * qy2 };

	return vector<_Value_type, 3>(dx[0] * _Vec[0] + dx[1] * _Vec[1] + dx[2] * _Vec[2],
		dy[0] * _Vec[0] + dy[1] * _Vec[1] + dy[2] * _Vec[2],
		dz[0] * _Vec[0] + dz[1] * _Vec[1] + dz[2] * _Vec[2]);
}

template<typename _Value_type>
/* rotates the quaternion with quat's rotation */
_MST_CONSTEXPR17 void quaternion<_Value_type>::rotate(const quaternion<_Value_type>& _Quat) noexcept
{
	(*this) = _Quat * (*this);
}

template<typename _Value_type>
/* returns a copy of the quaternion rotated with quat's rotation */
constexpr quaternion<_Value_type> quaternion<_Value_type>::rotated(
	const quaternion<_Value_type>& _Quat) const noexcept
{
	return _Quat * (*this);
}

template<typename _Value_type>
/* makes the quaternion look at the position, assuming its own position is the origin */
_MST_CONSTEXPR17 void quaternion<_Value_type>::look_at(
	const vector<_Value_type, 3>& _Direction, const vector<_Value_type, 3>& _Up) noexcept
{
	vector<_Value_type, 3> forward = _Direction.normalized();
	vector<_Value_type, 3> right = _Up.cross(forward).normalize();
	vector<_Value_type, 3> up = forward.cross(right).normalize();

	/* extracts the quaternion directly from the (right, up, forward) basis, following the same
		trace-based method as the matrix constructor above -- right/up/forward take the place of
		the matrix's column 0/1/2 */
	_Value_type trace = right.x + up.y + forward.z;

	if(trace > 0)
	{
		_Value_type s = (_Value_type)0.5 / sqrt(trace + 1);
		this->w = (_Value_type)0.25 / s;
		this->x = (up.z - forward.y) * s;
		this->y = (forward.x - right.z) * s;
		this->z = (right.y - up.x) * s;
	}
	else if(right.x > up.y && right.x > forward.z)
	{
		_Value_type s = 2 * sqrt(1 + right.x - up.y - forward.z);
		this->w = (up.z - forward.y) / s;
		this->x = (_Value_type)0.25 * s;
		this->y = (right.y + up.x) / s;
		this->z = (right.z + forward.x) / s;
	}
	else if(up.y > forward.z)
	{
		_Value_type s = 2 * sqrt(1 + up.y - right.x - forward.z);
		this->w = (forward.x - right.z) / s;
		this->x = (right.y + up.x) / s;
		this->y = (_Value_type)0.25 * s;
		this->z = (up.z + forward.y) / s;
	}
	else
	{
		_Value_type s = 2 * sqrt(1 + forward.z - right.x - up.y);
		this->w = (right.y - up.x) / s;
		this->x = (right.z + forward.x) / s;
		this->y = (up.z + forward.y) / s;
		this->z = (_Value_type)0.25 * s;
	}
}

template<typename _Value_type>
/* returns the dot product of this and quat */
constexpr _Value_type quaternion<_Value_type>::dot(const quaternion& _Quat) const noexcept
{
	return ((&this->w)[0] * _Quat[0] + (&this->w)[1] * _Quat[1] + (&this->w)[2] * _Quat[2] +
			(&this->w)[3] * _Quat[3]);
}

template<typename _Value_type>
/* returns a quaternion which is the spherical linear interpolation result between this and quat */
_MST_CONSTEXPR17 quaternion<_Value_type> quaternion<_Value_type>::slerp(
	const quaternion<_Value_type>& _To, _Value_type _Interp) const noexcept
{
	MST_ASSERT(fabs(length() - 1.0f) <= (_Value_type)_MST_QUATERNION_EPSILON,
		"quaternion is not normalized, length: ", length());

	quaternion q3;
	_Value_type dot12 = dot(_To);

	/*	dot = cos(theta)
	if (dot < 0), q1 and q2 are more than 90 degrees apart,
	so we can invert one to reduce spinning	*/
	if(dot12 < 0)
	{
		dot12 = -dot12;
		q3 = -_To;
	}
	else
	{
		q3 = _To;
	}

	if(dot12 < (_Value_type)0.95)
	{
		const radians<_Value_type> angle = acos(dot12);
		return ((*this) * sin(angle * (1 - _Interp)) + q3 * sin(angle * _Interp)) *
			   (1 / sin(angle));
	}
	else // if the angle is small, use linear interpolation
		return (*this) * (1 - _Interp) + q3 * _Interp;
}

// operator overloads

template<typename _Value_type>
constexpr quaternion<_Value_type> operator*(
	const quaternion<_Value_type>& _Left, const quaternion<_Value_type>& _Right) noexcept
{
	return quaternion<_Value_type>(
		_Left[0] * _Right[0] - vector<_Value_type, 3>(_Left.x, _Left.y, _Left.z)
								   .dot(vector<_Value_type, 3>(_Right.x, _Right.y, _Right.z)),
		_Left[2] * _Right[3] - _Left[3] * _Right[2] + _Left[0] * _Right[1] + _Left[1] * _Right[0],
		_Left[3] * _Right[1] - _Left[1] * _Right[3] + _Left[0] * _Right[2] + _Left[2] * _Right[0],
		_Left[1] * _Right[2] - _Left[2] * _Right[1] + _Left[0] * _Right[3] + _Left[3] * _Right[0]);
}

template<typename _Value_type>
constexpr quaternion<_Value_type> operator*(
	const quaternion<_Value_type>& _Quat, _Value_type _Val) noexcept
{
	return quaternion<_Value_type>(
		_Quat[0] * _Val, _Quat[1] * _Val, _Quat[2] * _Val, _Quat[3] * _Val);
}

template<typename _Value_type>
constexpr quaternion<_Value_type> operator+(
	const quaternion<_Value_type>& _Left, const quaternion<_Value_type>& _Right) noexcept
{
	return quaternion<_Value_type>(
		_Left[0] + _Right[0], _Left[1] + _Right[1], _Left[2] + _Right[2], _Left[3] + _Right[3]);
}

template<typename _Value_type>
_MST_CONSTEXPR17 quaternion<_Value_type>& quaternion<_Value_type>::operator*=(
	const quaternion<_Value_type>& _Quat) noexcept
{
	const _Value_type rw = (&this->w)[0] * _Quat[0] - (&this->w)[1] * _Quat[1] -
						   (&this->w)[2] * _Quat[2] - (&this->w)[3] * _Quat[3];
	const _Value_type rx = (&this->w)[0] * _Quat[1] + (&this->w)[1] * _Quat[0] +
						   (&this->w)[2] * _Quat[3] - (&this->w)[3] * _Quat[2];
	const _Value_type ry = (&this->w)[0] * _Quat[2] - (&this->w)[1] * _Quat[3] +
						   (&this->w)[2] * _Quat[0] + (&this->w)[3] * _Quat[1];
	const _Value_type rz = (&this->w)[0] * _Quat[3] + (&this->w)[1] * _Quat[2] -
						   (&this->w)[2] * _Quat[1] + (&this->w)[3] * _Quat[0];

	this->w = rw;
	this->x = rx;
	this->y = ry;
	this->z = rz;

	return *this;
}

template<typename _Value_type>
_MST_CONSTEXPR17 quaternion<_Value_type>& quaternion<_Value_type>::operator*=(
	_Value_type _Val) noexcept
{
	this->w *= _Val;
	this->x *= _Val;
	this->y *= _Val;
	this->z *= _Val;

	return *this;
}

template<typename _Value_type>
_MST_CONSTEXPR17 quaternion<_Value_type>& quaternion<_Value_type>::operator/=(
	_Value_type _Val) noexcept
{
	const _Value_type invVal = 1 / _Val;

	this->w *= invVal;
	this->x *= invVal;
	this->y *= invVal;
	this->z *= invVal;

	return *this;
}

template<typename _Value_type>
constexpr quaternion<_Value_type> quaternion<_Value_type>::operator-() const noexcept
{
	return quaternion<_Value_type>(-w, -x, -y, -z);
}

template<typename _Value_type>
inline ::std::ostream& operator<<(
	std::ostream& stream, const quaternion<_Value_type>& _Quat) noexcept
{
	return stream << "<" << _Quat[0] << ", " << _Quat[1] << ", " << _Quat[2] << ", " << _Quat[3]
				  << ">";
}

template<typename _Value_type>
_MST_CONSTEXPR17 _Value_type& quaternion<_Value_type>::operator[](size_t _Index) noexcept
{
	return (&w)[_Index];
}

template<typename _Value_type>
constexpr const _Value_type& quaternion<_Value_type>::operator[](size_t _Index) const noexcept
{
	return (&w)[_Index];
}

} // namespace math
} // namespace mst