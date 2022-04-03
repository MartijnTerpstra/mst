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
class degrees;

template<typename _Value_type>
class radians;

template<typename _Value_type>
class radians
{
	template<typename _xValue_type>
	friend class degrees;

	template<typename _xValue_type>
	friend class radians;

public:
	typedef _Value_type value_type;

	constexpr radians() = default;

	constexpr radians(const _Value_type& _V) noexcept
		: _Angle(_V < max_angle() && _V >= 0 ? _V : fmod(_V, max_angle()))
	{ }

	constexpr radians(const radians& _Other) noexcept
		: _Angle(_Other._Angle)
	{ }

	template<typename _xValue_type>
	constexpr explicit radians(const radians<_xValue_type>& _Other) noexcept
		: _Angle((_Value_type)_Other._Angle)
	{ }

	constexpr radians(const degrees<_Value_type>& _Other) noexcept
		: _Angle(_Other._Angle * (max_angle() / _Other.max_angle()))
	{ }

	template<typename _xValue_type>
	explicit constexpr radians(const degrees<_xValue_type>& _Other) noexcept
		: _Angle((_Value_type)(_Other._Angle * (max_angle() / _Other.max_angle())))
	{ }

	_MST_NODISCARD constexpr bool is_zero() const noexcept
	{
		return _Angle == 0;
	}

	_MST_CONSTEXPR17 radians& operator-=(const radians& _Other) noexcept
	{
		_Angle -= _Other._Angle;
		if(_Angle < 0)
			_Angle += max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 radians& operator-=(const degrees<_Value_type>& _Other) noexcept
	{
		_Angle -= _Other._Angle * (max_angle() / _Other.max_angle());
		if(_Angle < 0)
			_Angle += max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 radians& operator+=(const radians& _Other) noexcept
	{
		_Angle += _Other._Angle;
		if(_Angle >= max_angle())
			_Angle -= max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 radians& operator+=(const degrees<_Value_type>& _Other) noexcept
	{
		_Angle += _Other._Angle * max_angle() / _Other.max_angle();
		if(_Angle >= max_angle())
			_Angle -= max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 radians& operator*=(const _Value_type& _V) noexcept
	{
		_Angle = fmod(_Angle * _V, max_angle());

		return *this;
	}

	_MST_CONSTEXPR17 radians& operator/=(const _Value_type& _V) noexcept
	{
		_Angle = fmod(_Angle / _V, max_angle());

		return *this;
	}

	_MST_NODISCARD _MST_CONSTEXPR17 friend radians operator-(
		const radians& _L, const radians& _R) noexcept
	{
		_Value_type tmp = _L._Angle - _R._Angle;
		if(tmp < 0)
			tmp += _L.max_angle();
		return radians(tmp);
	}

	_MST_NODISCARD _MST_CONSTEXPR17 friend radians operator+(
		const radians& _L, const radians& _R) noexcept
	{
		_Value_type tmp = _L._Angle + _R._Angle;
		if(tmp >= _L.max_angle())
			tmp -= _L.max_angle();
		return radians(tmp);
	}

	_MST_NODISCARD constexpr friend radians operator*(
		const radians& _L, const _Value_type& _R) noexcept
	{
		return radians(fmod(_L._Angle * _R, _L.max_angle()));
	}

	_MST_NODISCARD constexpr friend radians operator/(
		const radians& _L, const _Value_type& _R) noexcept
	{
		return radians(fmod(_L._Angle / _R, _L.max_angle()));
	}

	_MST_NODISCARD constexpr _Value_type count() const noexcept
	{
		return _Angle;
	}

	_MST_NODISCARD constexpr _Value_type max_angle() const noexcept
	{
		return pi<_Value_type>() * 2;
	}

	static const radians zero;

private:
	_Value_type _Angle;

}; // class radians

template<typename _Value_type>
class degrees
{
	template<typename _xValue_type>
	friend class degrees;

	template<typename _xValue_type>
	friend class radians;

public:
	typedef _Value_type value_type;

	constexpr degrees() = default;

	constexpr degrees(const _Value_type& _V) noexcept
		: _Angle(_V < max_angle() && _V >= 0 ? _V : fmod(_V, max_angle()))
	{ }

	constexpr degrees(const degrees& _Other) noexcept
		: _Angle(_Other._Angle)
	{ }

	template<typename _xValue_type>
	constexpr explicit degrees(const degrees<_xValue_type>& _Other) noexcept
		: _Angle((_Value_type)_Other._Angle)
	{ }

	constexpr degrees(const radians<_Value_type>& _Other) noexcept
		: _Angle(_Other._Angle * (max_angle() / _Other.max_angle()))
	{ }

	template<typename _xValue_type>
	constexpr explicit degrees(const radians<_xValue_type>& _Other) noexcept
		: _Angle((_Value_type)(_Other._Angle * (max_angle() / _Other.max_angle())))
	{ }

	_MST_NODISCARD constexpr bool is_zero() const noexcept
	{
		return _Angle == 0;
	}

	_MST_CONSTEXPR17 degrees& operator-=(const degrees& _Other) noexcept
	{
		_Angle -= _Other._Angle;
		if(_Angle < 0)
			_Angle += max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 degrees& operator-=(const radians<_Value_type>& _Other) noexcept
	{
		_Angle -= _Other._Angle * (max_angle() / _Other.max_angle());
		if(_Angle < 0)
			_Angle += max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 degrees& operator+=(const degrees& _Other) noexcept
	{
		_Angle += _Other._Angle;
		if(_Angle >= max_angle())
			_Angle -= max_angle();
		return *this;
	}

	_MST_CONSTEXPR17 degrees& operator+=(const radians<_Value_type>& _Other) noexcept
	{
		_Angle += _Other._Angle * max_angle() / _Other.max_angle();
		if(_Angle >= max_angle())
			_Angle -= max_angle();
		return *this;
	}

	inline degrees& operator*=(const _Value_type& _V) noexcept
	{
		_Angle = fmod(_Angle * _V, max_angle());

		return *this;
	}

	inline degrees& operator/=(const _Value_type& _V) noexcept
	{
		_Angle = fmod(_Angle / _V, max_angle());

		return *this;
	}

	_MST_NODISCARD _MST_CONSTEXPR17 friend degrees operator-(
		const degrees& _L, const degrees& _R) noexcept
	{
		_Value_type tmp = _L._Angle - _R._Angle;
		if(tmp < 0)
			tmp += _L.max_angle();
		return degrees(tmp);
	}

	_MST_NODISCARD _MST_CONSTEXPR17 friend degrees operator+(
		const degrees& _L, const degrees& _R) noexcept
	{
		_Value_type tmp = _L._Angle + _R._Angle;
		if(tmp >= _L.max_angle())
			tmp -= _L.max_angle();
		return degrees(tmp);
	}

	_MST_NODISCARD inline friend degrees operator*(
		const degrees& _L, const _Value_type& _R) noexcept
	{
		return degrees(fmod(_L._Angle * _R, _L.max_angle()));
	}

	_MST_NODISCARD inline friend degrees operator/(
		const degrees& _L, const _Value_type& _R) noexcept
	{
		return degrees(fmod(_L._Angle / _R, _L.max_angle()));
	}

	_MST_NODISCARD constexpr _Value_type count() const noexcept
	{
		return _Angle;
	}

	_MST_NODISCARD constexpr _Value_type max_angle() const noexcept
	{
		return (_Value_type)360;
	}

	static const degrees zero;

private:
	_Value_type _Angle;

}; // class degrees

template<typename _Value_type>
_MST_NODISCARD inline constexpr _Value_type sin(radians<_Value_type> radians) noexcept
{
	using ::std::sin;

	return sin(radians.count());
}

template<typename _Value_type>
_MST_NODISCARD inline _Value_type cos(radians<_Value_type> radians) noexcept
{
	using ::std::cos;

	return cos(radians.count());
}

template<typename _Value_type>
_MST_NODISCARD inline _Value_type tan(radians<_Value_type> radians) noexcept
{
	using ::std::tan;

	return tan(radians.count());
}

template<typename _Value_type>
_MST_NODISCARD inline radians<_Value_type> asin(_Value_type x) noexcept
{
	using ::std::asin;

	return radians<_Value_type>(asin(x));
}

template<typename _Value_type>
_MST_NODISCARD inline radians<_Value_type> acos(_Value_type x) noexcept
{
	using ::std::acos;

	return radians<_Value_type>(acos(x));
}

template<typename _Value_type>
_MST_NODISCARD inline radians<_Value_type> atan(_Value_type x) noexcept
{
	using ::std::atan;

	return radians<_Value_type>(atan(x));
}


namespace _Details {

template<typename _Value_type>
_MST_NODISCARD constexpr mst::math::degrees<_Value_type> _Make_zero_degrees()
{
	return mst::math::degrees<_Value_type>((_Value_type)0);
}

template<typename _Value_type>
_MST_NODISCARD constexpr mst::math::radians<_Value_type> _Make_zero_radians()
{
	return mst::math::radians<_Value_type>((_Value_type)0);
}

} // namespace _Details

template<typename _Value_type>
const degrees<_Value_type> degrees<_Value_type>::zero = _Details::_Make_zero_degrees<_Value_type>();

template<typename _Value_type>
const radians<_Value_type> radians<_Value_type>::zero = _Details::_Make_zero_radians<_Value_type>();

} // namespace math
} // namespace mst