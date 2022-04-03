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
template<typename _xValue_type>
constexpr vector<_Value_type, 1>::vector(const vector<_xValue_type, 1>& _Other) noexcept
	: _Mybase(static_cast<_Value_type>(_Other.x))
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 1>::vector(_Value_type _X) noexcept
	: _Mybase(_X)
{ }

template<typename _Value_type>
template<typename _xValue_type>
constexpr vector<_Value_type, 2>::vector(const vector<_xValue_type, 2>& _Other) noexcept
	: _Mybase(static_cast<_Value_type>(_Other.x), static_cast<_Value_type>(_Other.y))
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 2>::vector(_Value_type _X, _Value_type _Y) noexcept
	: _Mybase(_X, _Y)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 2>::vector(_Value_type initVal) noexcept
	: _Mybase(initVal, initVal)
{ }

template<typename _Value_type>
template<typename _xValue_type>
constexpr vector<_Value_type, 3>::vector(const vector<_xValue_type, 3>& _Other) noexcept
	: _Mybase(static_cast<_Value_type>(_Other.x), static_cast<_Value_type>(_Other.y),
		  static_cast<_Value_type>(_Other.z))
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 3>::vector(_Value_type _X, _Value_type _Y, _Value_type _Z) noexcept
	: _Mybase(_X, _Y, _Z)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 3>::vector(const vector<_Value_type, 2>& _XY, _Value_type _Z) noexcept
	: _Mybase(_XY[0], _XY[1], _Z)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 3>::vector(_Value_type _X, const vector<_Value_type, 2>& _YZ) noexcept
	: _Mybase(_X, _YZ[0], _YZ[1])
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 3>::vector(_Value_type initVal) noexcept
	: _Mybase(initVal, initVal, initVal)
{ }

template<typename _Value_type>
template<typename _xValue_type>
constexpr vector<_Value_type, 4>::vector(const vector<_xValue_type, 4>& _Other) noexcept
	: _Mybase(static_cast<_Value_type>(_Other._Data[0]), static_cast<_Value_type>(_Other._Data[1]),
		  static_cast<_Value_type>(_Other._Data[2]), static_cast<_Value_type>(_Other._Data[3]))
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	_Value_type _X, _Value_type _Y, _Value_type _Z, _Value_type _W) noexcept
	: _Mybase(_X, _Y, _Z, _W)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	const vector<_Value_type, 2>& _XY, const vector<_Value_type, 2>& _ZW) noexcept
	: _Mybase(_XY[0], _XY[1], _ZW[0], _ZW[1])
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	const vector<_Value_type, 2>& _XY, _Value_type _Z, _Value_type _W) noexcept
	: _Mybase(_XY[0], _XY[1], _Z, _W)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	_Value_type _X, const vector<_Value_type, 2>& _YZ, _Value_type _W) noexcept
	: _Mybase(_X, _YZ[0], _YZ[1], _W)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	_Value_type _X, _Value_type _Y, const vector<_Value_type, 2>& _ZW) noexcept
	: _Mybase(_X, _Y, _ZW[0], _ZW[1])
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	const vector<_Value_type, 3>& _XYZ, _Value_type _W) noexcept
	: _Mybase(_XYZ[0], _XYZ[1], _XYZ[2], _W)
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(
	_Value_type _X, const vector<_Value_type, 3>& _YZW) noexcept
	: _Mybase(_X, _YZW[0], _YZW[1], _YZW[2])
{ }

template<typename _Value_type>
constexpr vector<_Value_type, 4>::vector(_Value_type initVal) noexcept
	: _Mybase(initVal, initVal, initVal, initVal)
{ }

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type*
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::data() noexcept
{
	return (&this->x);
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
constexpr const _Value_type*
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::data() const noexcept
{
	return (&this->x);
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
constexpr size_t
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::size() const noexcept
{
	return _Elems;
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type&
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::operator[](
	size_t _Idx) noexcept
{
	MST_ASSERT(_Idx < _Elems, "index out of range");
	return (&this->x)[_Idx];
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
constexpr const _Value_type&
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::operator[](
	size_t _Idx) const noexcept
{
	MST_ASSERT(_Idx < _Elems, "index out of range");
	return (&this->x)[_Idx];
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::begin() noexcept
{
	return { &this->x, 0 };
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
const_vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::begin() const noexcept
{
	return { &this->x, 0 };
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
const_vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::cbegin() const noexcept
{
	return { &this->x, 0 };
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::end() noexcept
{
	return { &this->x, _Elems };
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
const_vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::end() const noexcept
{
	return { &this->x, _Elems };
}

template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
const_vector_iterator<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::cend() const noexcept
{
	return { &this->x, _Elems };
}

/* returns the squared length */
template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type(
	_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::squared_length)()
	const noexcept
{
	_Value_type retval = 0;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval += (&this->x)[i] * (&this->x)[i];
	}
	return retval;
}

/* returns the squared distance towards _Vec */
template<typename _Value_type, size_t _Elems, bool _IsFP, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type
_Details::_Math_vector_base<_Value_type, _Elems, _IsFP, _IsUnsigned>::squared_distance(
	const _Vector_t& _Vec) const noexcept
{
	_Value_type retval = 0;
	for(size_t i = 0; i < _Elems; ++i)
	{
		const _Value_type difference = _Vec[i] - (&this->x)[i];
		retval += difference * difference;
	}
	return retval;
}

/* returns the length */
template<typename _Value_type, size_t _Elems, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type
_Details::_Math_vector_base<_Value_type, _Elems, true, _IsUnsigned>::length() const noexcept
{
	return sqrt(this->squared_length());
}

/* returns the squared towards _Vec */
template<typename _Value_type, size_t _Elems, bool _IsUnsigned>
_MST_CONSTEXPR17 _Value_type
_Details::_Math_vector_base<_Value_type, _Elems, true, _IsUnsigned>::distance(
	const _Vector_t& _Vec) const noexcept
{
	return sqrt(squared_distance(_Vec));
}

/* normalized and returns itself */
template<typename _Value_type, size_t _Elems, bool _IsUnsigned>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>&
_Details::_Math_vector_base<_Value_type, _Elems, true, _IsUnsigned>::normalize() noexcept
{
	const _Value_type invLength = 1 / length();
	for(size_t i = 0; i < _Elems; ++i)
	{
		(&this->x)[i] *= invLength;
	}
	return static_cast<vector<_Value_type, _Elems>&>(*this);
}

template<typename _Value_type, size_t _Elems, bool _IsUnsigned>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, true, _IsUnsigned>::normalized() const noexcept
{
	return static_cast<const vector<_Value_type, _Elems>&>(*this) / length();
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>
_Details::_Math_vector_base<_Value_type, _Elems, false, false>::operator-() const noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = -(&this->x)[i];
	}

	return retval;
}

/* returns the dot product of the  */
template<typename _Value_type>
constexpr _Value_type vector<_Value_type, 3>::dot(
	const vector<_Value_type, 3>& _Other) const noexcept
{
	return (&this->x)[0] * _Other[0] + (&this->x)[1] * _Other[1] + (&this->x)[2] * _Other[2];
}

template<typename _Value_type>
constexpr vector<_Value_type, 3> vector<_Value_type, 3>::cross(
	const vector<_Value_type, 3>& _Other) const noexcept
{
	return vector<_Value_type, 3>((&this->x)[1] * _Other[2] - (&this->x)[2] * _Other[1],
		(&this->x)[2] * _Other[0] - (&this->x)[0] * _Other[2],
		(&this->x)[0] * _Other[1] - (&this->x)[1] * _Other[0]);
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator+=(
	vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] += _Right[i];
	}
	return _Left;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator-=(
	vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] -= _Right[i];
	}
	return _Left;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator*=(
	vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] *= _Right[i];
	}
	return _Left;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator/=(
	vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] /= _Right[i];
	}
	return _Left;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator*=(
	vector<_Value_type, _Elems>& _Left, _Value_type _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] *= _Right;
	}
	return _Left;
}

namespace _Details {

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& _Diff_assign(
	vector<_Value_type, _Elems>& _Left, _Value_type _Right, ::std::true_type) noexcept
{
	const _Value_type invRight = 1 / _Right;
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] *= invRight;
	}
	return _Left;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& _Diff_assign(
	vector<_Value_type, _Elems>& _Left, _Value_type _Right, ::std::false_type) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		_Left[i] /= _Right;
	}
	return _Left;
}

} // namespace _Details

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems>& operator/=(
	vector<_Value_type, _Elems>& _Left, _Value_type _Right) noexcept
{
	return ::mst::math::_Details::_Diff_assign(_Left, _Right,
		typename ::std::integral_constant<bool,
			(_Elems > 1 && std::is_floating_point<_Value_type>::value)>::type());
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator+(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] + _Right[i];
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator-(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] - _Right[i];
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator*(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] * _Right[i];
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator/(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] / _Right[i];
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator*(
	const vector<_Value_type, _Elems>& _Left, _Value_type _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] * _Right;
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 vector<_Value_type, _Elems> operator/(
	const vector<_Value_type, _Elems>& _Left, _Value_type _Right) noexcept
{
	vector<_Value_type, _Elems> retval;
	for(size_t i = 0; i < _Elems; ++i)
	{
		retval[i] = _Left[i] / _Right;
	}
	return retval;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 bool operator==(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	for(size_t i = 0; i < _Elems; ++i)
	{
		if(_Left[i] != _Right[i])
		{
			return false;
		}
	}

	return true;
}

template<typename _Value_type, size_t _Elems>
_MST_CONSTEXPR17 bool operator!=(
	const vector<_Value_type, _Elems>& _Left, const vector<_Value_type, _Elems>& _Right) noexcept
{
	return !(_Left == _Right);
}

namespace _Details {

template<typename _Value_type, size_t _Elems>
constexpr const vector<_Value_type, _Elems> _Make_zero_vector() noexcept
{
	return vector<_Value_type, _Elems>{ (_Value_type)0 };
}

} // namespace _Details

} // namespace math
} // namespace mst