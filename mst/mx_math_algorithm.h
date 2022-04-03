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

#ifndef _MST_MDET
#define _MST_MDET ::mst::math::_Details::
#endif

namespace mst {
namespace math {

template<typename _Value_type, size_t _Elems>
class vector_iterator;

template<typename _Value_type, size_t _Elems>
class const_vector_iterator;

namespace _Details {

struct _Math_type
{ };
struct _Scalar_type
{ };
struct _Array_type
{ };

template<typename T, typename IsScalar = typename ::std::is_arithmetic<T>::type>
struct _Math_traits_impl
{
	static const size_t dimensions = 1;
	typedef _MST_VC_TYPENAME T value_type;
	typedef _Scalar_type math_type;
	typedef ::std::is_unsigned<T> is_unsigned;
};

template<typename T>
struct _Math_traits_impl<T, ::std::false_type>
{
	static const size_t dimensions = T::dimensions;
	typedef typename T::value_type value_type;
	typedef _Math_type math_type;
	typedef typename T::is_unsigned is_unsigned;
};

template<typename T>
struct _Math_traits : public _Math_traits_impl<T>
{ };

template<typename T, size_t N>
struct _Math_traits<T[N]>
{
	static const size_t dimensions = N;
	typedef typename ::std::remove_const<T>::type value_type;
	typedef _Array_type math_type;
	typedef typename _Math_traits_impl<T>::is_unsigned is_unsigned;
};

template<typename T>
struct _Cref_value
{
	typedef const typename _Math_traits<T>::value_type& type;
};

template<typename T>
struct _Cvalue
{
	constexpr static typename _Math_traits<T>::value_type _Create(
		const double initValue, _Math_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const double initValue, _Scalar_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const double initValue, _Array_type) noexcept
	{
		return typename _Math_traits<T>::value_type(
			_Cvalue<typename _Math_traits<T>::value_type>::create(initValue));
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const size_t initValue, _Math_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const size_t initValue, _Scalar_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const size_t initValue, _Array_type) noexcept
	{
		return typename _Math_traits<T>::value_type(
			_Cvalue<typename _Math_traits<T>::value_type>::create(initValue));
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const int32_t initValue, _Math_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const int32_t initValue, _Scalar_type) noexcept
	{
		return typename _Math_traits<T>::value_type(initValue);
	}

	constexpr static typename _Math_traits<T>::value_type _Create(
		const int32_t initValue, _Array_type) noexcept
	{
		return typename _Math_traits<T>::value_type(
			_Cvalue<typename _Math_traits<T>::value_type>::create(initValue));
	}

	constexpr static typename _Math_traits<T>::value_type create(const double initValue) noexcept
	{
		return _Create(initValue, typename _Math_traits<T>::math_type());
	}

	constexpr static typename _Math_traits<T>::value_type create(const int32_t initValue) noexcept
	{
		return _Create(initValue, typename _Math_traits<T>::math_type());
	}

	constexpr static typename _Math_traits<T>::value_type create(const size_t initValue) noexcept
	{
		return _Create(initValue, typename _Math_traits<T>::math_type());
	}

	constexpr static T create_object(const double initValue) noexcept
	{
		return T(_Create(initValue, typename _Math_traits<T>::math_type()));
	}

	constexpr static T create_object(const int32_t initValue) noexcept
	{
		return T(_Create(initValue, typename _Math_traits<T>::math_type()));
	}

	constexpr static T create_object(const size_t initValue) noexcept
	{
		return T(_Create(initValue, typename _Math_traits<T>::math_type()));
	}
};

template<typename ValueType, size_t Columns, size_t Rows>
vector<ValueType, Columns>* begin(matrix<ValueType, Columns, Rows>& m)
{
	return m.begin();
}

template<typename ValueType, size_t Columns, size_t Rows>
vector<ValueType, Columns>* end(matrix<ValueType, Columns, Rows>& m)
{
	return m.end();
}

template<typename ValueType, size_t Columns, size_t Rows>
const vector<ValueType, Columns>* begin(const matrix<ValueType, Columns, Rows>& m)
{
	return m.begin();
}

template<typename ValueType, size_t Columns, size_t Rows>
const vector<ValueType, Columns>* end(const matrix<ValueType, Columns, Rows>& m)
{
	return m.end();
}

template<typename ValueType, size_t Columns, size_t Rows>
const vector<ValueType, Columns>* cbegin(const matrix<ValueType, Columns, Rows>& m)
{
	return m.begin();
}

template<typename ValueType, size_t Columns, size_t Rows>
const vector<ValueType, Columns>* cend(const matrix<ValueType, Columns, Rows>& m)
{
	return m.end();
}

template<typename ValueType, size_t Elems>
vector_iterator<ValueType, Elems> begin(mst::math::vector<ValueType, Elems>& v)
{
	return v.begin();
}

template<typename ValueType, size_t Elems>
vector_iterator<ValueType, Elems> end(mst::math::vector<ValueType, Elems>& v)
{
	return v.end();
}

template<typename ValueType, size_t Elems>
const_vector_iterator<ValueType, Elems> begin(const mst::math::vector<ValueType, Elems>& v)
{
	return v.begin();
}

template<typename ValueType, size_t Elems>
const_vector_iterator<ValueType, Elems> end(const mst::math::vector<ValueType, Elems>& v)
{
	return v.end();
}

template<typename ValueType, size_t Elems>
const_vector_iterator<ValueType, Elems> cbegin(const mst::math::vector<ValueType, Elems>& v)
{
	return v.cbegin();
}

template<typename ValueType, size_t Elems>
const_vector_iterator<ValueType, Elems> cend(const mst::math::vector<ValueType, Elems>& v)
{
	return v.cend();
}

template<typename T>
struct DefaultTag
{ };

template<typename ValueType, size_t Elems>
struct VectorTag
{ };

template<typename ValueType, size_t Columns, size_t Rows>
struct MatrixTag
{ };

template<typename T>
struct ConvertToTag
{
	typedef DefaultTag<T> type;
};

template<typename ValueType, size_t Elems>
struct ConvertToTag<mst::math::vector<ValueType, Elems>>
{
	typedef VectorTag<ValueType, Elems> type;
};

template<typename ValueType, size_t Columns, size_t Rows>
struct ConvertToTag<mst::math::matrix<ValueType, Columns, Rows>>
{
	typedef MatrixTag<ValueType, Columns, Rows> type;
};

template<typename T, typename V>
inline T ConvertToImpl(V scalar, DefaultTag<T>)
{
	return static_cast<T>(scalar);
}

template<typename ValueType, size_t Elems, typename V>
inline mst::math::vector<ValueType, Elems> ConvertToImpl(V scalar, VectorTag<ValueType, Elems>)
{
	return mst::math::vector<ValueType, Elems>{ (ValueType)scalar };
}

template<typename ValueType, size_t Columns, size_t Rows, typename V>
inline mst::math::matrix<ValueType, Columns, Rows> ConvertToImpl(
	V scalar, MatrixTag<ValueType, Columns, Rows>)
{
	return mst::math::matrix<ValueType, Columns, Rows>{ (ValueType)scalar };
}

template<typename T, typename V>
inline std::decay_t<T> ConvertTo(V scalar)
{
	return ConvertToImpl(scalar, typename ConvertToTag<std::decay_t<T>>::type{});
}

} // namespace _Details
} // namespace math
} // namespace mst