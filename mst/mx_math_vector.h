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

template<typename _Value_type, size_t _Elems>
class vector_iterator;

template<typename _Value_type, size_t _Elems>
class const_vector_iterator;

#define _mst_declare_swizzle2(_Elemcount, _Component1, _Component2)                                \
	_mst_declare_get_swizzle2(_Elemcount, _Component1, _Component2)

#define _mst_declare_get_swizzle2(_Elemcount, _Component1, _Component2)                            \
	namespace swizzle {                                                                            \
	template<typename _Value_type>                                                                 \
	inline vector<_Value_type, 2> _Component1##_Component2(                                        \
		const vector<_Value_type, _Elemcount>& vec)                                                \
	{                                                                                              \
		return vector<_Value_type, 2>(vec._Component1, vec._Component2);                           \
	}                                                                                              \
	}

#define _mst_declare_swizzle3(_Elemcount, _Component1, _Component2, _Component3)                   \
	_mst_declare_get_swizzle3(_Elemcount, _Component1, _Component2, _Component3)

#define _mst_declare_get_swizzle3(_Elemcount, _Component1, _Component2, _Component3)               \
	namespace swizzle {                                                                            \
	template<typename _Value_type>                                                                 \
	inline vector<_Value_type, 3> _Component1##_Component2##_Component3(                           \
		const vector<_Value_type, _Elemcount>& vec)                                                \
	{                                                                                              \
		return vector<_Value_type, 3>(vec._Component1, vec._Component2, vec._Component3);          \
	}                                                                                              \
	}

#define _mst_declare_swizzle4(_Component1, _Component2, _Component3, _Component4)                  \
	_mst_declare_get_swizzle4(4, _Component1, _Component2, _Component3, _Component4)

#define _mst_declare_get_swizzle4(_Elemcount, _Component1, _Component2, _Component3, _Component4)  \
	namespace swizzle {                                                                            \
	template<typename _Value_type>                                                                 \
	inline vector<_Value_type, 4> _Component1##_Component2##_Component3##_Component4(              \
		const vector<_Value_type, _Elemcount>& vec)                                                \
	{                                                                                              \
		return vector<_Value_type, 4>(                                                             \
			vec._Component1, vec._Component2, vec._Component3, vec._Component4);                   \
	}                                                                                              \
	}

namespace _Details {

template<typename _Value_type, size_t _Elems>
class _Math_vector_base_data
{
public:
};

template<typename _Value_type>
class _Math_vector_base_data<_Value_type, 1>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(_Value_type _X) noexcept
		: x(_X)
	{ }

	_Value_type x;
};

template<typename _Value_type>
class _Math_vector_base_data<_Value_type, 2>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(_Value_type _X, _Value_type _Y) noexcept
		: x(_X)
		, y(_Y)
	{ }

	_Value_type x, y;
};

template<typename _Value_type>
class _Math_vector_base_data<_Value_type, 3>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(_Value_type _X, _Value_type _Y, _Value_type _Z) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
	{ }

	_Value_type x, y, z;
};

template<typename _Value_type>
class _Math_vector_base_data<_Value_type, 4>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(
		_Value_type _X, _Value_type _Y, _Value_type _Z, _Value_type _W) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
		, w(_W)
	{ }

	_Value_type x, y, z, w;
};

#if _MST_MATH_SIMD_ENABLED

template<>
class MST_ALIGN(16) _Math_vector_base_data<double, 2>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(double _X, double _Y) noexcept
		: x(_X)
		, y(_Y)
	{ }

	union
	{
		struct
		{
			double x, y;
		};
		__m128d _Simd;
	};
};

template<>
class MST_ALIGN(16) _Math_vector_base_data<int64_t, 2>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(int64_t _X, int64_t _Y) noexcept
		: x(_X)
		, y(_Y)
	{ }

	union
	{
		struct
		{
			int64_t x, y;
		};
		__m128i _Simd;
	};
};

template<>
class MST_ALIGN(16) _Math_vector_base_data<float, 4>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(float _X, float _Y, float _Z, float _W) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
		, w(_W)
	{ }

	union
	{
		struct
		{
			float x, y, z, w;
		};
		__m128 _Simd;
	};
};

template<>
class MST_ALIGN(16) _Math_vector_base_data<int32_t, 4>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(int32_t _X, int32_t _Y, int32_t _Z, int32_t _W) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
		, w(_W)
	{ }

	union
	{
		struct
		{
			int32_t x, y, z, w;
		};
		__m128i _Simd;
	};
};

template<>
class MST_ALIGN(32) _Math_vector_base_data<double, 4>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(double _X, double _Y, double _Z, double _W) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
		, w(_W)
	{ }

	union
	{
		struct
		{
			double x, y, z, w;
		};
		__m256d _Simd;
	};
};

template<>
class MST_ALIGN(32) _Math_vector_base_data<int64_t, 4>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(int64_t _X, int64_t _Y, int64_t _Z, int64_t _W) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
		, w(_W)
	{ }

	union
	{
		struct
		{
			int64_t x, y, z, w;
		};
		__m256i _Simd;
	};
};

#if _MST_ALL_VECTORS_SIMD

template<>
class MST_ALIGN(16) _Math_vector_base_data<float, 2>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(float _X, float _Y) noexcept
		: x(_X)
		, y(_Y)
	{ }

	union
	{
		struct
		{
			float x, y;
		};
		__m128 _Simd;
	};
};

template<>
class MST_ALIGN(16) _Math_vector_base_data<float, 3>
{
public:
	_Math_vector_base_data() = default;
	constexpr _Math_vector_base_data(float _X, float _Y, float _Z) noexcept
		: x(_X)
		, y(_Y)
		, z(_Z)
	{ }

	union
	{
		struct
		{
			float x, y, z;
		};
		__m128 _Simd;
	};
};

#endif // MST_MATH

#endif // !MST_MATH_NO_SIMD

template<typename _Value_type, size_t _Elems,
	bool _IsFP = ::std::is_floating_point<_Value_type>::value,
	bool _IsUnsigned = ::std::is_unsigned<_Value_type>::value>
class _Math_vector_base : public _Math_vector_base_data<_Value_type, _Elems>
{
	static_assert(_Elems <= 4 && _Elems != 0, "_Elems value not supported");

	template<typename _xValue_type, size_t _xElems>
	friend class ::mst::math::vector;

	typedef ::mst::math::vector<_Value_type, _Elems> _Vector_t;

public:
	using _Math_vector_base_data<_Value_type, _Elems>::_Math_vector_base_data;

	typedef _Value_type value_type;
	static const size_t dimensions = _Elems;
	typedef typename ::std::is_unsigned<_Value_type> is_unsigned;

	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type* data() noexcept;
	_MST_NODISCARD constexpr const _Value_type* data() const noexcept;

	[[nodiscard]] constexpr size_t size() const noexcept;

	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type& operator[](size_t _Idx) noexcept;
	_MST_NODISCARD constexpr const _Value_type& operator[](size_t _Idx) const noexcept;

	[[nodiscard]] inline vector_iterator<_Value_type, _Elems> begin() noexcept;
	[[nodiscard]] inline const_vector_iterator<_Value_type, _Elems> begin() const noexcept;
	[[nodiscard]] inline const_vector_iterator<_Value_type, _Elems> cbegin() const noexcept;

	[[nodiscard]] inline vector_iterator<_Value_type, _Elems> end() noexcept;
	[[nodiscard]] inline const_vector_iterator<_Value_type, _Elems> end() const noexcept;
	[[nodiscard]] inline const_vector_iterator<_Value_type, _Elems> cend() const noexcept;

	/* returns the squared length */
	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type squared_length() const noexcept;

	/* returns the squared distance towards _Vec */
	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type squared_distance(
		const _Vector_t& _Vec) const noexcept;
};

template<typename _Value_type, size_t _Elems>
class _Math_vector_base<_Value_type, _Elems, false, false>
	: public _Math_vector_base<_Value_type, _Elems, false, true>
{
	// signed types only functions (should also exclude unsigned floating point)

	typedef ::mst::math::vector<_Value_type, _Elems> _Vector_t;

public:
	using _Math_vector_base<_Value_type, _Elems, false, true>::_Math_vector_base;

	/* returns negative version of itself */
	_MST_NODISCARD _MST_CONSTEXPR17 _Vector_t operator-() const noexcept;
};


template<typename _Value_type, size_t _Elems, bool _IsUnsigned>
class _Math_vector_base<_Value_type, _Elems, true, _IsUnsigned>
	: public _Math_vector_base<_Value_type, _Elems, false, _IsUnsigned>
{
	// floating point only functions

	typedef ::mst::math::vector<_Value_type, _Elems> _Vector_t;

public:
	using _Math_vector_base<_Value_type, _Elems, false, _IsUnsigned>::_Math_vector_base;

	/* returns the length */
	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type length() const noexcept;

	/* returns the squared distance towards _Vec */
	_MST_NODISCARD _MST_CONSTEXPR17 _Value_type distance(const _Vector_t& _Vec) const noexcept;

	/* normalizes and returns itself */
	_MST_CONSTEXPR17 vector<_Value_type, _Elems>& normalize() noexcept;

	/* returns a normalized version of itself */
	_MST_NODISCARD _MST_CONSTEXPR17 vector<_Value_type, _Elems> normalized() const noexcept;
};

} // namespace _Details

template<typename _Value_type>
class vector<_Value_type, 1> : public _Details::_Math_vector_base<_Value_type, 1>
{
	typedef _Details::_Math_vector_base<_Value_type, 1> _Mybase;

public:
	vector() = default;

	template<typename _xValue_type>
	constexpr explicit vector(const vector<_xValue_type, 1>& _Other) noexcept;
	constexpr explicit vector(_Value_type _X) noexcept;

	static const vector zero;

	_MST_NODISCARD constexpr operator _Value_type() const noexcept
	{
		return this->x;
	}
};

template<typename _Value_type>
const vector<_Value_type, 1> vector<_Value_type, 1>::zero{ 0 };

_mst_declare_get_swizzle2(1, x, x)
_mst_declare_get_swizzle3(1, x, x, x)
_mst_declare_get_swizzle4(1, x, x, x, x)

template<typename _Value_type>
class vector<_Value_type, 2> : public _Details::_Math_vector_base<_Value_type, 2>
{
	typedef _Details::_Math_vector_base<_Value_type, 2> _Mybase;

public:
	inline vector() = default;

	template<typename _xValue_type>
	constexpr explicit vector(const vector<_xValue_type, 2>& _Other) noexcept;
	constexpr vector(_Value_type _X, _Value_type _Y) noexcept;
	constexpr explicit vector(_Value_type initVal) noexcept;

	static const vector zero;
};

template<typename _Value_type>
const vector<_Value_type, 2> vector<_Value_type, 2>::zero{ 0 };

_mst_declare_swizzle2(2, x, y)
_mst_declare_swizzle2(2, y, x)

_mst_declare_get_swizzle2(2, x, x)
_mst_declare_get_swizzle2(2, y, y)

_mst_declare_get_swizzle3(2, x, x, x)
_mst_declare_get_swizzle3(2, y, y, y)

_mst_declare_get_swizzle3(2, y, x, x)
_mst_declare_get_swizzle3(2, x, y, x)
_mst_declare_get_swizzle3(2, x, x, y)

_mst_declare_get_swizzle3(2, x, y, y)
_mst_declare_get_swizzle3(2, y, x, y)
_mst_declare_get_swizzle3(2, y, y, x)

_mst_declare_get_swizzle4(2, x, x, x, x)
_mst_declare_get_swizzle4(2, y, y, y, y)

_mst_declare_get_swizzle4(2, y, x, x, x)
_mst_declare_get_swizzle4(2, x, y, y, y)

_mst_declare_get_swizzle4(2, x, y, x, x)
_mst_declare_get_swizzle4(2, y, x, y, y)

_mst_declare_get_swizzle4(2, x, x, y, x)
_mst_declare_get_swizzle4(2, y, y, x, y)

_mst_declare_get_swizzle4(2, x, x, x, y)
_mst_declare_get_swizzle4(2, y, y, y, x)

_mst_declare_get_swizzle4(2, y, y, x, x)
_mst_declare_get_swizzle4(2, y, x, y, x)
_mst_declare_get_swizzle4(2, y, x, x, y)
_mst_declare_get_swizzle4(2, x, y, y, x)
_mst_declare_get_swizzle4(2, x, y, x, y)
_mst_declare_get_swizzle4(2, x, x, y, y)

template<typename _Value_type>
class vector<_Value_type, 3> : public _Details::_Math_vector_base<_Value_type, 3>
{
	typedef _Details::_Math_vector_base<_Value_type, 3> _Mybase;

public:
	inline vector() = default;

	template<typename _xValue_type>
	constexpr explicit vector(const vector<_xValue_type, 3>& _Other) noexcept;
	constexpr vector(_Value_type _X, _Value_type _Y, _Value_type _Z) noexcept;
	constexpr vector(const vector<_Value_type, 2>& _XY, _Value_type _Z) noexcept;
	constexpr vector(_Value_type _X, const vector<_Value_type, 2>& _YZ) noexcept;
	constexpr explicit vector(_Value_type initVal) noexcept;

	/* returns the dot product with _Vec */
	_MST_NODISCARD constexpr _Value_type dot(const vector& _Vec) const noexcept;

	/* returns the cross product with _Vec */
	_MST_NODISCARD constexpr vector cross(const vector& _Vec) const noexcept;

	static const vector zero;
};

template<typename _Value_type>
const vector<_Value_type, 3> vector<_Value_type, 3>::zero{ 0 };

_mst_declare_swizzle2(3, x, y)
_mst_declare_swizzle2(3, y, x)
_mst_declare_swizzle2(3, y, z)
_mst_declare_swizzle2(3, z, y)
_mst_declare_swizzle2(3, x, z)
_mst_declare_swizzle2(3, z, x)

_mst_declare_swizzle2(3, x, x)
_mst_declare_swizzle2(3, y, y)
_mst_declare_swizzle2(3, z, z)

_mst_declare_swizzle3(3, x, y, z)
_mst_declare_swizzle3(3, y, x, z)
_mst_declare_swizzle3(3, y, z, x)
_mst_declare_swizzle3(3, x, z, y)
_mst_declare_swizzle3(3, z, x, y)
_mst_declare_swizzle3(3, z, y, x)

_mst_declare_get_swizzle3(3, x, x, x)
_mst_declare_get_swizzle3(3, y, y, y)
_mst_declare_get_swizzle3(3, z, z, z)

_mst_declare_get_swizzle3(3, z, x, x)
_mst_declare_get_swizzle3(3, x, z, x)
_mst_declare_get_swizzle3(3, x, x, z)
_mst_declare_get_swizzle3(3, y, x, x)
_mst_declare_get_swizzle3(3, x, y, x)
_mst_declare_get_swizzle3(3, x, x, y)

_mst_declare_get_swizzle3(3, x, y, y)
_mst_declare_get_swizzle3(3, y, x, y)
_mst_declare_get_swizzle3(3, y, y, x)
_mst_declare_get_swizzle3(3, z, y, y)
_mst_declare_get_swizzle3(3, y, z, y)
_mst_declare_get_swizzle3(3, y, y, z)

_mst_declare_get_swizzle3(3, x, z, z)
_mst_declare_get_swizzle3(3, z, x, z)
_mst_declare_get_swizzle3(3, z, z, x)
_mst_declare_get_swizzle3(3, y, z, z)
_mst_declare_get_swizzle3(3, z, y, z)
_mst_declare_get_swizzle3(3, z, z, y)

_mst_declare_get_swizzle4(3, x, x, y, z)
_mst_declare_get_swizzle4(3, x, x, z, y)
_mst_declare_get_swizzle4(3, x, y, x, z)
_mst_declare_get_swizzle4(3, x, y, z, x)
_mst_declare_get_swizzle4(3, x, z, x, y)
_mst_declare_get_swizzle4(3, x, z, y, x)
_mst_declare_get_swizzle4(3, y, x, x, z)
_mst_declare_get_swizzle4(3, y, x, z, x)
_mst_declare_get_swizzle4(3, y, z, x, x)
_mst_declare_get_swizzle4(3, z, x, x, y)
_mst_declare_get_swizzle4(3, z, x, y, x)
_mst_declare_get_swizzle4(3, z, y, x, x)

_mst_declare_get_swizzle4(3, x, y, y, z)
_mst_declare_get_swizzle4(3, x, y, z, y)
_mst_declare_get_swizzle4(3, x, z, y, y)
_mst_declare_get_swizzle4(3, y, x, y, z)
_mst_declare_get_swizzle4(3, y, x, z, y)
_mst_declare_get_swizzle4(3, y, y, x, z)
_mst_declare_get_swizzle4(3, y, y, z, x)
_mst_declare_get_swizzle4(3, y, z, x, y)
_mst_declare_get_swizzle4(3, y, z, y, x)
_mst_declare_get_swizzle4(3, z, x, y, y)
_mst_declare_get_swizzle4(3, z, y, x, y)
_mst_declare_get_swizzle4(3, z, y, y, x)

_mst_declare_get_swizzle4(3, x, y, z, z)
_mst_declare_get_swizzle4(3, x, z, y, z)
_mst_declare_get_swizzle4(3, x, z, z, y)
_mst_declare_get_swizzle4(3, y, x, z, z)
_mst_declare_get_swizzle4(3, y, z, x, z)
_mst_declare_get_swizzle4(3, y, z, z, x)
_mst_declare_get_swizzle4(3, z, x, y, z)
_mst_declare_get_swizzle4(3, z, x, z, y)
_mst_declare_get_swizzle4(3, z, y, x, z)
_mst_declare_get_swizzle4(3, z, y, z, x)
_mst_declare_get_swizzle4(3, z, z, x, y)
_mst_declare_get_swizzle4(3, z, z, y, x)

_mst_declare_get_swizzle4(3, x, y, y, y)
_mst_declare_get_swizzle4(3, y, x, y, y)
_mst_declare_get_swizzle4(3, y, y, x, y)
_mst_declare_get_swizzle4(3, y, y, y, x)

_mst_declare_get_swizzle4(3, x, z, z, z)
_mst_declare_get_swizzle4(3, z, x, z, z)
_mst_declare_get_swizzle4(3, z, z, x, z)
_mst_declare_get_swizzle4(3, z, z, z, x)

_mst_declare_get_swizzle4(3, x, x, x, y)
_mst_declare_get_swizzle4(3, x, x, y, x)
_mst_declare_get_swizzle4(3, x, y, x, x)
_mst_declare_get_swizzle4(3, y, x, x, x)

_mst_declare_get_swizzle4(3, y, z, z, z)
_mst_declare_get_swizzle4(3, z, y, z, z)
_mst_declare_get_swizzle4(3, z, z, y, z)
_mst_declare_get_swizzle4(3, z, z, z, y)

_mst_declare_get_swizzle4(3, x, x, x, z)
_mst_declare_get_swizzle4(3, x, x, z, x)
_mst_declare_get_swizzle4(3, x, z, x, x)
_mst_declare_get_swizzle4(3, z, x, x, x)

_mst_declare_get_swizzle4(3, y, y, y, z)
_mst_declare_get_swizzle4(3, y, y, z, y)
_mst_declare_get_swizzle4(3, y, z, y, y)
_mst_declare_get_swizzle4(3, z, y, y, y)

_mst_declare_get_swizzle4(3, x, x, y, y)
_mst_declare_get_swizzle4(3, x, y, x, y)
_mst_declare_get_swizzle4(3, x, y, y, x)
_mst_declare_get_swizzle4(3, y, x, x, y)
_mst_declare_get_swizzle4(3, y, x, y, x)
_mst_declare_get_swizzle4(3, y, y, x, x)

_mst_declare_get_swizzle4(3, x, x, z, z)
_mst_declare_get_swizzle4(3, x, z, x, z)
_mst_declare_get_swizzle4(3, x, z, z, x)
_mst_declare_get_swizzle4(3, z, x, x, z)
_mst_declare_get_swizzle4(3, z, x, z, x)
_mst_declare_get_swizzle4(3, z, z, x, x)

_mst_declare_get_swizzle4(3, y, y, z, z)
_mst_declare_get_swizzle4(3, y, z, y, z)
_mst_declare_get_swizzle4(3, y, z, z, y)
_mst_declare_get_swizzle4(3, z, y, y, z)
_mst_declare_get_swizzle4(3, z, y, z, y)
_mst_declare_get_swizzle4(3, z, z, y, y)

template<typename _Value_type>
class vector<_Value_type, 4> : public _Details::_Math_vector_base<_Value_type, 4>
{
	typedef _Details::_Math_vector_base<_Value_type, 4> _Mybase;

public:
	inline vector() = default;

	template<typename _xValue_type>
	constexpr explicit vector(const vector<_xValue_type, 4>& _Other) noexcept;
	constexpr vector(_Value_type _X, _Value_type _Y, _Value_type _Z, _Value_type _W) noexcept;
	constexpr vector(const vector<_Value_type, 2>& _XY, const vector<_Value_type, 2>& _ZW) noexcept;
	constexpr vector(const vector<_Value_type, 2>& _XY, _Value_type _Z, _Value_type _W) noexcept;
	constexpr vector(_Value_type _X, const vector<_Value_type, 2>& _YZ, _Value_type _W) noexcept;
	constexpr vector(_Value_type _X, _Value_type _Y, const vector<_Value_type, 2>& _ZW) noexcept;
	constexpr vector(const vector<_Value_type, 3>& _XYZ, _Value_type _W) noexcept;
	constexpr vector(_Value_type _X, const vector<_Value_type, 3>& _YZW) noexcept;
	constexpr explicit vector(_Value_type initVal) noexcept;

	static const vector zero;
};

template<typename _Value_type>
const vector<_Value_type, 4> vector<_Value_type, 4>::zero{ 0 };

_mst_declare_swizzle2(4, x, y)
_mst_declare_swizzle2(4, y, x)

_mst_declare_swizzle2(4, y, z)
_mst_declare_swizzle2(4, z, y)
_mst_declare_swizzle2(4, x, z)
_mst_declare_swizzle2(4, z, x)

_mst_declare_swizzle2(4, x, w)
_mst_declare_swizzle2(4, w, x)
_mst_declare_swizzle2(4, y, w)
_mst_declare_swizzle2(4, w, y)
_mst_declare_swizzle2(4, z, w)
_mst_declare_swizzle2(4, w, z)

_mst_declare_swizzle2(4, x, x)
_mst_declare_swizzle2(4, y, y)
_mst_declare_swizzle2(4, z, z)
_mst_declare_swizzle2(4, w, w)

_mst_declare_swizzle3(4, x, y, z)
_mst_declare_swizzle3(4, y, x, z)
_mst_declare_swizzle3(4, y, z, x)
_mst_declare_swizzle3(4, x, z, y)
_mst_declare_swizzle3(4, z, x, y)
_mst_declare_swizzle3(4, z, y, x)

_mst_declare_swizzle3(4, w, y, z)
_mst_declare_swizzle3(4, w, x, z)
_mst_declare_swizzle3(4, w, z, x)
_mst_declare_swizzle3(4, w, z, y)
_mst_declare_swizzle3(4, w, x, y)
_mst_declare_swizzle3(4, w, y, x)

_mst_declare_swizzle3(4, x, w, z)
_mst_declare_swizzle3(4, y, w, z)
_mst_declare_swizzle3(4, y, w, x)
_mst_declare_swizzle3(4, x, w, y)
_mst_declare_swizzle3(4, z, w, y)
_mst_declare_swizzle3(4, z, w, x)

_mst_declare_swizzle3(4, x, y, w)
_mst_declare_swizzle3(4, y, x, w)
_mst_declare_swizzle3(4, y, z, w)
_mst_declare_swizzle3(4, x, z, w)
_mst_declare_swizzle3(4, z, x, w)
_mst_declare_swizzle3(4, z, y, w)

_mst_declare_get_swizzle3(4, x, x, x)
_mst_declare_get_swizzle3(4, y, y, y)
_mst_declare_get_swizzle3(4, z, z, z)
_mst_declare_get_swizzle3(4, w, w, w)

_mst_declare_get_swizzle3(4, y, x, x)
_mst_declare_get_swizzle3(4, x, y, x)
_mst_declare_get_swizzle3(4, x, x, y)
_mst_declare_get_swizzle3(4, z, x, x)
_mst_declare_get_swizzle3(4, x, z, x)
_mst_declare_get_swizzle3(4, x, x, z)
_mst_declare_get_swizzle3(4, w, x, x)
_mst_declare_get_swizzle3(4, x, w, x)
_mst_declare_get_swizzle3(4, x, x, w)

_mst_declare_get_swizzle3(4, x, y, y)
_mst_declare_get_swizzle3(4, y, x, y)
_mst_declare_get_swizzle3(4, y, y, x)
_mst_declare_get_swizzle3(4, z, y, y)
_mst_declare_get_swizzle3(4, y, z, y)
_mst_declare_get_swizzle3(4, y, y, z)
_mst_declare_get_swizzle3(4, w, y, y)
_mst_declare_get_swizzle3(4, y, w, y)
_mst_declare_get_swizzle3(4, y, y, w)

_mst_declare_get_swizzle3(4, x, z, z)
_mst_declare_get_swizzle3(4, z, x, z)
_mst_declare_get_swizzle3(4, z, z, x)
_mst_declare_get_swizzle3(4, y, z, z)
_mst_declare_get_swizzle3(4, z, y, z)
_mst_declare_get_swizzle3(4, z, z, y)
_mst_declare_get_swizzle3(4, w, z, z)
_mst_declare_get_swizzle3(4, z, w, z)
_mst_declare_get_swizzle3(4, z, z, w)

_mst_declare_get_swizzle3(4, x, w, w)
_mst_declare_get_swizzle3(4, w, x, w)
_mst_declare_get_swizzle3(4, w, w, x)
_mst_declare_get_swizzle3(4, y, w, w)
_mst_declare_get_swizzle3(4, w, y, w)
_mst_declare_get_swizzle3(4, w, w, y)
_mst_declare_get_swizzle3(4, z, w, w)
_mst_declare_get_swizzle3(4, w, z, w)
_mst_declare_get_swizzle3(4, w, w, z)

_mst_declare_get_swizzle4(4, x, x, x, x)
_mst_declare_get_swizzle4(4, y, y, y, y)
_mst_declare_get_swizzle4(4, z, z, z, z)
_mst_declare_get_swizzle4(4, w, w, w, w)

_mst_declare_swizzle4(x, y, z, w)
_mst_declare_swizzle4(x, z, w, y)
_mst_declare_swizzle4(x, z, y, w)
_mst_declare_swizzle4(y, w, x, z)
_mst_declare_swizzle4(y, w, z, x)
_mst_declare_swizzle4(y, x, w, z)
_mst_declare_swizzle4(y, x, z, w)
_mst_declare_swizzle4(y, z, w, x)
_mst_declare_swizzle4(y, z, x, w)
_mst_declare_swizzle4(z, w, x, y)
_mst_declare_swizzle4(z, w, y, x)
_mst_declare_swizzle4(z, x, w, y)
_mst_declare_swizzle4(z, x, y, w)
_mst_declare_swizzle4(z, y, w, x)
_mst_declare_swizzle4(z, y, x, w)

_mst_declare_get_swizzle4(4, x, x, y, z)
_mst_declare_get_swizzle4(4, x, x, z, y)
_mst_declare_get_swizzle4(4, x, y, x, z)
_mst_declare_get_swizzle4(4, x, y, z, x)
_mst_declare_get_swizzle4(4, x, z, x, y)
_mst_declare_get_swizzle4(4, x, z, y, x)
_mst_declare_get_swizzle4(4, y, x, x, z)
_mst_declare_get_swizzle4(4, y, x, z, x)
_mst_declare_get_swizzle4(4, y, z, x, x)
_mst_declare_get_swizzle4(4, z, x, x, y)
_mst_declare_get_swizzle4(4, z, x, y, x)
_mst_declare_get_swizzle4(4, z, y, x, x)

_mst_declare_get_swizzle4(4, x, y, y, z)
_mst_declare_get_swizzle4(4, x, y, z, y)
_mst_declare_get_swizzle4(4, x, z, y, y)
_mst_declare_get_swizzle4(4, y, x, y, z)
_mst_declare_get_swizzle4(4, y, x, z, y)
_mst_declare_get_swizzle4(4, y, y, x, z)
_mst_declare_get_swizzle4(4, y, y, z, x)
_mst_declare_get_swizzle4(4, y, z, x, y)
_mst_declare_get_swizzle4(4, y, z, y, x)
_mst_declare_get_swizzle4(4, z, x, y, y)
_mst_declare_get_swizzle4(4, z, y, x, y)
_mst_declare_get_swizzle4(4, z, y, y, x)

_mst_declare_get_swizzle4(4, x, y, z, z)
_mst_declare_get_swizzle4(4, x, z, y, z)
_mst_declare_get_swizzle4(4, x, z, z, y)
_mst_declare_get_swizzle4(4, y, x, z, z)
_mst_declare_get_swizzle4(4, y, z, x, z)
_mst_declare_get_swizzle4(4, y, z, z, x)
_mst_declare_get_swizzle4(4, z, x, y, z)
_mst_declare_get_swizzle4(4, z, x, z, y)
_mst_declare_get_swizzle4(4, z, y, x, z)
_mst_declare_get_swizzle4(4, z, y, z, x)
_mst_declare_get_swizzle4(4, z, z, x, y)
_mst_declare_get_swizzle4(4, z, z, y, x)

_mst_declare_get_swizzle4(4, w, y, y, z)
_mst_declare_get_swizzle4(4, w, y, z, y)
_mst_declare_get_swizzle4(4, w, z, y, y)
_mst_declare_get_swizzle4(4, y, w, y, z)
_mst_declare_get_swizzle4(4, y, w, z, y)
_mst_declare_get_swizzle4(4, y, y, w, z)
_mst_declare_get_swizzle4(4, y, y, z, w)
_mst_declare_get_swizzle4(4, y, z, w, y)
_mst_declare_get_swizzle4(4, y, z, y, w)
_mst_declare_get_swizzle4(4, z, w, y, y)
_mst_declare_get_swizzle4(4, z, y, w, y)
_mst_declare_get_swizzle4(4, z, y, y, w)

_mst_declare_get_swizzle4(4, w, y, z, z)
_mst_declare_get_swizzle4(4, w, z, y, z)
_mst_declare_get_swizzle4(4, w, z, z, y)
_mst_declare_get_swizzle4(4, y, w, z, z)
_mst_declare_get_swizzle4(4, y, z, w, z)
_mst_declare_get_swizzle4(4, y, z, z, w)
_mst_declare_get_swizzle4(4, z, w, y, z)
_mst_declare_get_swizzle4(4, z, w, z, y)
_mst_declare_get_swizzle4(4, z, y, w, z)
_mst_declare_get_swizzle4(4, z, y, z, w)
_mst_declare_get_swizzle4(4, z, z, w, y)
_mst_declare_get_swizzle4(4, z, z, y, w)

_mst_declare_get_swizzle4(4, w, w, y, z)
_mst_declare_get_swizzle4(4, w, w, z, y)
_mst_declare_get_swizzle4(4, w, y, w, z)
_mst_declare_get_swizzle4(4, w, y, z, w)
_mst_declare_get_swizzle4(4, w, z, w, y)
_mst_declare_get_swizzle4(4, w, z, y, w)
_mst_declare_get_swizzle4(4, y, w, w, z)
_mst_declare_get_swizzle4(4, y, w, z, w)
_mst_declare_get_swizzle4(4, y, z, w, w)
_mst_declare_get_swizzle4(4, z, w, w, y)
_mst_declare_get_swizzle4(4, z, w, y, w)
_mst_declare_get_swizzle4(4, z, y, w, w)

_mst_declare_get_swizzle4(4, w, x, x, z)
_mst_declare_get_swizzle4(4, w, x, z, x)
_mst_declare_get_swizzle4(4, w, z, x, x)
_mst_declare_get_swizzle4(4, x, w, x, z)
_mst_declare_get_swizzle4(4, x, w, z, x)
_mst_declare_get_swizzle4(4, x, x, w, z)
_mst_declare_get_swizzle4(4, x, x, z, w)
_mst_declare_get_swizzle4(4, x, z, w, x)
_mst_declare_get_swizzle4(4, x, z, x, w)
_mst_declare_get_swizzle4(4, z, w, x, x)
_mst_declare_get_swizzle4(4, z, x, w, x)
_mst_declare_get_swizzle4(4, z, x, x, w)

_mst_declare_get_swizzle4(4, w, x, z, z)
_mst_declare_get_swizzle4(4, w, z, x, z)
_mst_declare_get_swizzle4(4, w, z, z, x)
_mst_declare_get_swizzle4(4, x, w, z, z)
_mst_declare_get_swizzle4(4, x, z, w, z)
_mst_declare_get_swizzle4(4, x, z, z, w)
_mst_declare_get_swizzle4(4, z, w, x, z)
_mst_declare_get_swizzle4(4, z, w, z, x)
_mst_declare_get_swizzle4(4, z, x, w, z)
_mst_declare_get_swizzle4(4, z, x, z, w)
_mst_declare_get_swizzle4(4, z, z, w, x)
_mst_declare_get_swizzle4(4, z, z, x, w)

_mst_declare_get_swizzle4(4, w, w, x, z)
_mst_declare_get_swizzle4(4, w, w, z, x)
_mst_declare_get_swizzle4(4, w, x, w, z)
_mst_declare_get_swizzle4(4, w, x, z, w)
_mst_declare_get_swizzle4(4, w, z, w, x)
_mst_declare_get_swizzle4(4, w, z, x, w)
_mst_declare_get_swizzle4(4, x, w, w, z)
_mst_declare_get_swizzle4(4, x, w, z, w)
_mst_declare_get_swizzle4(4, x, z, w, w)
_mst_declare_get_swizzle4(4, z, w, w, x)
_mst_declare_get_swizzle4(4, z, w, x, w)
_mst_declare_get_swizzle4(4, z, x, w, w)

_mst_declare_get_swizzle4(4, w, x, x, y)
_mst_declare_get_swizzle4(4, w, x, y, x)
_mst_declare_get_swizzle4(4, w, y, x, x)
_mst_declare_get_swizzle4(4, x, w, x, y)
_mst_declare_get_swizzle4(4, x, w, y, x)
_mst_declare_get_swizzle4(4, x, x, w, y)
_mst_declare_get_swizzle4(4, x, x, y, w)
_mst_declare_get_swizzle4(4, x, y, w, x)
_mst_declare_get_swizzle4(4, x, y, x, w)
_mst_declare_get_swizzle4(4, y, w, x, x)
_mst_declare_get_swizzle4(4, y, x, w, x)
_mst_declare_get_swizzle4(4, y, x, x, w)

_mst_declare_get_swizzle4(4, w, x, y, y)
_mst_declare_get_swizzle4(4, w, y, x, y)
_mst_declare_get_swizzle4(4, w, y, y, x)
_mst_declare_get_swizzle4(4, x, w, y, y)
_mst_declare_get_swizzle4(4, x, y, w, y)
_mst_declare_get_swizzle4(4, x, y, y, w)
_mst_declare_get_swizzle4(4, y, w, x, y)
_mst_declare_get_swizzle4(4, y, w, y, x)
_mst_declare_get_swizzle4(4, y, x, w, y)
_mst_declare_get_swizzle4(4, y, x, y, w)
_mst_declare_get_swizzle4(4, y, y, w, x)
_mst_declare_get_swizzle4(4, y, y, x, w)

_mst_declare_get_swizzle4(4, w, w, x, y)
_mst_declare_get_swizzle4(4, w, w, y, x)
_mst_declare_get_swizzle4(4, w, x, w, y)
_mst_declare_get_swizzle4(4, w, x, y, w)
_mst_declare_get_swizzle4(4, w, y, w, x)
_mst_declare_get_swizzle4(4, w, y, x, w)
_mst_declare_get_swizzle4(4, x, w, w, y)
_mst_declare_get_swizzle4(4, x, w, y, w)
_mst_declare_get_swizzle4(4, x, y, w, w)
_mst_declare_get_swizzle4(4, y, w, w, x)
_mst_declare_get_swizzle4(4, y, w, x, w)
_mst_declare_get_swizzle4(4, y, x, w, w)

_mst_declare_get_swizzle4(4, x, y, y, y)
_mst_declare_get_swizzle4(4, y, x, y, y)
_mst_declare_get_swizzle4(4, y, y, x, y)
_mst_declare_get_swizzle4(4, y, y, y, x)

_mst_declare_get_swizzle4(4, x, z, z, z)
_mst_declare_get_swizzle4(4, z, x, z, z)
_mst_declare_get_swizzle4(4, z, z, x, z)
_mst_declare_get_swizzle4(4, z, z, z, x)

_mst_declare_get_swizzle4(4, w, w, w, x)
_mst_declare_get_swizzle4(4, w, w, x, w)
_mst_declare_get_swizzle4(4, w, x, w, w)
_mst_declare_get_swizzle4(4, x, w, w, w)

_mst_declare_get_swizzle4(4, x, x, x, y)
_mst_declare_get_swizzle4(4, x, x, y, x)
_mst_declare_get_swizzle4(4, x, y, x, x)
_mst_declare_get_swizzle4(4, y, x, x, x)

_mst_declare_get_swizzle4(4, y, z, z, z)
_mst_declare_get_swizzle4(4, z, y, z, z)
_mst_declare_get_swizzle4(4, z, z, y, z)
_mst_declare_get_swizzle4(4, z, z, z, y)

_mst_declare_get_swizzle4(4, w, w, w, y)
_mst_declare_get_swizzle4(4, w, w, y, w)
_mst_declare_get_swizzle4(4, w, y, w, w)
_mst_declare_get_swizzle4(4, y, w, w, w)

_mst_declare_get_swizzle4(4, x, x, x, z)
_mst_declare_get_swizzle4(4, x, x, z, x)
_mst_declare_get_swizzle4(4, x, z, x, x)
_mst_declare_get_swizzle4(4, z, x, x, x)

_mst_declare_get_swizzle4(4, y, y, y, z)
_mst_declare_get_swizzle4(4, y, y, z, y)
_mst_declare_get_swizzle4(4, y, z, y, y)
_mst_declare_get_swizzle4(4, z, y, y, y)

_mst_declare_get_swizzle4(4, w, w, w, z)
_mst_declare_get_swizzle4(4, w, w, z, w)
_mst_declare_get_swizzle4(4, w, z, w, w)
_mst_declare_get_swizzle4(4, z, w, w, w)

_mst_declare_get_swizzle4(4, w, x, x, x)
_mst_declare_get_swizzle4(4, x, w, x, x)
_mst_declare_get_swizzle4(4, x, x, w, x)
_mst_declare_get_swizzle4(4, x, x, x, w)

_mst_declare_get_swizzle4(4, w, y, y, y)
_mst_declare_get_swizzle4(4, y, w, y, y)
_mst_declare_get_swizzle4(4, y, y, w, y)
_mst_declare_get_swizzle4(4, y, y, y, w)

_mst_declare_get_swizzle4(4, w, z, z, z)
_mst_declare_get_swizzle4(4, z, w, z, z)
_mst_declare_get_swizzle4(4, z, z, w, z)
_mst_declare_get_swizzle4(4, z, z, z, w)

_mst_declare_get_swizzle4(4, x, x, y, y)
_mst_declare_get_swizzle4(4, x, y, x, y)
_mst_declare_get_swizzle4(4, x, y, y, x)
_mst_declare_get_swizzle4(4, y, x, x, y)
_mst_declare_get_swizzle4(4, y, x, y, x)
_mst_declare_get_swizzle4(4, y, y, x, x)

_mst_declare_get_swizzle4(4, x, x, z, z)
_mst_declare_get_swizzle4(4, x, z, x, z)
_mst_declare_get_swizzle4(4, x, z, z, x)
_mst_declare_get_swizzle4(4, z, x, x, z)
_mst_declare_get_swizzle4(4, z, x, z, x)
_mst_declare_get_swizzle4(4, z, z, x, x)

_mst_declare_get_swizzle4(4, w, w, x, x)
_mst_declare_get_swizzle4(4, w, x, w, x)
_mst_declare_get_swizzle4(4, w, x, x, w)
_mst_declare_get_swizzle4(4, x, w, w, x)
_mst_declare_get_swizzle4(4, x, w, x, w)
_mst_declare_get_swizzle4(4, x, x, w, w)

_mst_declare_get_swizzle4(4, y, y, z, z)
_mst_declare_get_swizzle4(4, y, z, y, z)
_mst_declare_get_swizzle4(4, y, z, z, y)
_mst_declare_get_swizzle4(4, z, y, y, z)
_mst_declare_get_swizzle4(4, z, y, z, y)
_mst_declare_get_swizzle4(4, z, z, y, y)

_mst_declare_get_swizzle4(4, w, w, y, y)
_mst_declare_get_swizzle4(4, w, y, w, y)
_mst_declare_get_swizzle4(4, w, y, y, w)
_mst_declare_get_swizzle4(4, y, w, w, y)
_mst_declare_get_swizzle4(4, y, w, y, w)
_mst_declare_get_swizzle4(4, y, y, w, w)

_mst_declare_get_swizzle4(4, w, w, z, z)
_mst_declare_get_swizzle4(4, w, z, w, z)
_mst_declare_get_swizzle4(4, w, z, z, w)
_mst_declare_get_swizzle4(4, z, w, w, z)
_mst_declare_get_swizzle4(4, z, w, z, w)
_mst_declare_get_swizzle4(4, z, z, w, w)

#undef _mst_declare_swizzle2
#undef _mst_declare_swizzle3
#undef _mst_declare_swizzle4

#undef _mst_declare_get_swizzle2
#undef _mst_declare_get_swizzle3
#undef _mst_declare_get_swizzle4

#undef _mst_declare_value

template<typename _Value_type, size_t _Elems>
class vector_iterator
{
	friend class const_vector_iterator<_Value_type, _Elems>;

	template<typename _Vt, size_t _El, bool _IsFP, bool _IsU>
	friend class _Details::_Math_vector_base;

public:
	vector_iterator() = default;
	vector_iterator(const vector_iterator&) = default;
	vector_iterator(vector_iterator&&) = default;

	vector_iterator& operator=(const vector_iterator&) = default;
	vector_iterator& operator=(vector_iterator&&) = default;

	inline vector_iterator& operator++() noexcept
	{
		++m_idx;
		return *this;
	}

	[[nodiscard]] inline vector_iterator operator++(int) noexcept
	{
		return vector_iterator(m_vec, m_idx++);
	}

	inline vector_iterator& operator--() noexcept
	{
		--m_idx;
		return *this;
	}

	[[nodiscard]] inline vector_iterator operator--(int) noexcept
	{
		return vector_iterator(m_vec, m_idx--);
	}

	inline vector_iterator& operator+=(ptrdiff_t offset) noexcept
	{
		m_idx += offset;
		return *this;
	}

	[[nodiscard]] inline vector_iterator operator+(ptrdiff_t offset) noexcept
	{
		return vector_iterator(m_vec, m_idx + offset);
	}

	inline vector_iterator& operator-=(ptrdiff_t offset) noexcept
	{
		m_idx -= offset;
		return *this;
	}

	[[nodiscard]] inline vector_iterator operator-(ptrdiff_t offset) noexcept
	{
		return vector_iterator(m_vec, m_idx - offset);
	}

	[[nodiscard]] inline bool operator==(const vector_iterator& other) const noexcept
	{
		return m_vec == other.m_vec && m_idx == other.m_idx;
	}

	[[nodiscard]] inline bool operator!=(const vector_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	[[nodiscard]] inline bool operator<(const vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx < other.m_idx;
	}

	[[nodiscard]] inline bool operator<=(const vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx <= other.m_idx;
	}

	[[nodiscard]] inline bool operator>(const vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx > other.m_idx;
	}

	[[nodiscard]] inline bool operator>=(const vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx >= other.m_idx;
	}

	[[nodiscard]] inline _Value_type& operator*() const noexcept
	{
		MST_ASSERT(m_vec, "Iterators to different vectors");
		MST_ASSERT(m_idx < _Elems, "Iterator out of range");

		return m_vec[m_idx];
	}

private:
	vector_iterator(_Value_type* vec, size_t idx)
		: m_vec(vec)
		, m_idx(idx)
	{ }

private:
	_Value_type* m_vec;
	size_t m_idx;
};

template<typename _Value_type, size_t _Elems>
[[nodiscard]] inline vector_iterator<_Value_type, _Elems> operator+(
	ptrdiff_t offset, vector_iterator<_Value_type, _Elems> iter) noexcept
{
	return iter += offset;
}

template<typename _Value_type, size_t _Elems>
class const_vector_iterator
{
	template<typename _Vt, size_t _El, bool _IsFP, bool _IsU>
	friend class _Details::_Math_vector_base;

public:
	const_vector_iterator() = default;
	const_vector_iterator(const const_vector_iterator&) = default;
	const_vector_iterator(const_vector_iterator&&) = default;

	const_vector_iterator(const vector_iterator<_Value_type, _Elems>& other)
		: m_vec(other.m_vec)
		, m_idx(other.m_idx)
	{ }

	const_vector_iterator(vector_iterator<_Value_type, _Elems>&& other)
		: m_vec(other.m_vec)
		, m_idx(other.m_idx)
	{ }

	const_vector_iterator& operator=(const const_vector_iterator&) = default;
	const_vector_iterator& operator=(const_vector_iterator&&) = default;

	const_vector_iterator& operator=(const vector_iterator<_Value_type, _Elems>& other)
	{
		m_vec = other.m_vec;
		m_idx = other.m_idx;
		return *this;
	}

	const_vector_iterator& operator=(vector_iterator<_Value_type, _Elems>&& other)
	{
		m_vec = std::move(other.m_vec);
		m_idx = std::move(other.m_idx);
		return *this;
	}

	inline const_vector_iterator& operator++() noexcept
	{
		++m_idx;
		return *this;
	}

	[[nodiscard]] inline const_vector_iterator operator++(int) noexcept
	{
		return const_vector_iterator(m_vec, m_idx++);
	}

	inline const_vector_iterator& operator--() noexcept
	{
		--m_idx;
		return *this;
	}

	[[nodiscard]] inline const_vector_iterator operator--(int) noexcept
	{
		return const_vector_iterator(m_vec, m_idx--);
	}

	inline const_vector_iterator& operator+=(ptrdiff_t offset) noexcept
	{
		m_idx += offset;
		return *this;
	}

	[[nodiscard]] inline const_vector_iterator operator+(ptrdiff_t offset) noexcept
	{
		return const_vector_iterator(m_idx + offset);
	}

	inline const_vector_iterator& operator-=(ptrdiff_t offset) noexcept
	{
		m_idx -= offset;
		return *this;
	}

	[[nodiscard]] inline const_vector_iterator operator-(ptrdiff_t offset) noexcept
	{
		return const_vector_iterator(m_idx - offset);
	}

	[[nodiscard]] inline bool operator==(const const_vector_iterator& other) const noexcept
	{
		return m_vec == other.m_vec && m_idx == other.m_idx;
	}

	[[nodiscard]] inline bool operator!=(const const_vector_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	[[nodiscard]] inline bool operator<(const const_vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx < other.m_idx;
	}

	[[nodiscard]] inline bool operator<=(const const_vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx <= other.m_idx;
	}

	[[nodiscard]] inline bool operator>(const const_vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx > other.m_idx;
	}

	[[nodiscard]] inline bool operator>=(const const_vector_iterator& other) const noexcept
	{
		MST_ASSERT(m_vec == other.m_vec, "Iterators to different vectors");

		return m_idx >= other.m_idx;
	}

	[[nodiscard]] inline const _Value_type& operator*() const noexcept
	{
		MST_ASSERT(m_vec, "Iterators to different vectors");
		MST_ASSERT(m_idx < _Elems, "Iterator out of range");

		return m_vec[m_idx];
	}

private:
	const_vector_iterator(const _Value_type* vec, size_t idx)
		: m_vec(vec)
		, m_idx(idx)
	{ }

private:
	const _Value_type* m_vec;
	size_t m_idx;
};

template<typename _Value_type, size_t _Elems>
[[nodiscard]] inline const_vector_iterator<_Value_type, _Elems> operator+(
	ptrdiff_t offset, const_vector_iterator<_Value_type, _Elems> iter) noexcept
{
	return iter += offset;
}


} // namespace math
} // namespace mst