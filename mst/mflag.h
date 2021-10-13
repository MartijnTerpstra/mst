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

#include "mx_compiler.h"
#include <mcore.h>
#include <mflag_traits.h>
#include <initializer_list>

namespace mst {

template<typename T, typename FlagTraits = flag_traits<T>>
class flag
{
	static_assert(
		::std::is_integral<typename FlagTraits::storage_type>::value, "FlagType must be integral");

#if _MST_HAS_IS_INVOCABLE
	static_assert(::std::is_nothrow_invocable<decltype(FlagTraits::to_storage_type), T>::value,
		"FlagTraits::to_storage_type should be noexcept");
#endif

	static const bool _Preshifted = FlagTraits::preshifted;

public:
	constexpr flag() noexcept
		: _Myflags(FlagTraits::init_value)
	{ }

	constexpr flag(::std::nullptr_t) noexcept
		: _Myflags(FlagTraits::init_value)
	{ }

	constexpr flag(T arg0) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0))
	{ }

	constexpr flag(T arg0, T arg1) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1))
	{ }

	constexpr flag(T arg0, T arg1, T arg2) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3, T arg4) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4) | FlagTraits::to_storage_type(arg5))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4) | FlagTraits::to_storage_type(arg5) |
				   FlagTraits::to_storage_type(arg6))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4) | FlagTraits::to_storage_type(arg5) |
				   FlagTraits::to_storage_type(arg6) | FlagTraits::to_storage_type(arg7))
	{ }

	constexpr flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4) | FlagTraits::to_storage_type(arg5) |
				   FlagTraits::to_storage_type(arg6) | FlagTraits::to_storage_type(arg7) |
				   FlagTraits::to_storage_type(arg8))
	{ }

	constexpr flag(
		T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8, T arg9) noexcept
		: _Myflags(FlagTraits::to_storage_type(arg0) | FlagTraits::to_storage_type(arg1) |
				   FlagTraits::to_storage_type(arg2) | FlagTraits::to_storage_type(arg3) |
				   FlagTraits::to_storage_type(arg4) | FlagTraits::to_storage_type(arg5) |
				   FlagTraits::to_storage_type(arg6) | FlagTraits::to_storage_type(arg7) |
				   FlagTraits::to_storage_type(arg8) | FlagTraits::to_storage_type(arg9))
	{ }

	_MST_CONSTEXPR17 flag(std::initializer_list<T> initList) noexcept
		: _Myflags(0)
	{
		for(auto it : initList)
		{
			_Myflags |= FlagTraits::to_storage_type(it);
		}
	}

	[[nodiscard]] constexpr typename FlagTraits::storage_type get() const noexcept
	{
		return _Myflags;
	}

	[[nodiscard]] constexpr bool is_enabled(T enumFlag) const noexcept
	{
		return (_Myflags & FlagTraits::to_storage_type(enumFlag)) ==
			   FlagTraits::to_storage_type(enumFlag);
	}

	_MST_CONSTEXPR17 void enable(T enumFlag) noexcept
	{
		_Myflags |= FlagTraits::to_storage_type(enumFlag);
	}

	_MST_CONSTEXPR17 void disable(T enumFlag) noexcept
	{
		_Myflags &= ~FlagTraits::to_storage_type(enumFlag);
	}

	_MST_CONSTEXPR17 void toggle(T enumFlag) noexcept
	{
		_Myflags ^= FlagTraits::to_storage_type(enumFlag);
	}

	_MST_CONSTEXPR17 static flag from_storage(typename FlagTraits::storage_type storage) noexcept
	{
		flag r;
		r._Myflags = storage;
		return r;
	}

private:
	typename FlagTraits::storage_type _Myflags;

}; // class flag

template<typename T>
constexpr flag<T> make_flag(T arg0) noexcept
{
	return flag<T>(arg0);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1) noexcept
{
	return flag<T>(arg0, arg1);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2) noexcept
{
	return flag<T>(arg0, arg1, arg2);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2, T arg3) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2, T arg3, T arg4) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4, arg5);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

template<typename T>
constexpr flag<T> make_flag(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template<typename T>
constexpr flag<T> make_flag(
	T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template<typename T>
constexpr flag<T> make_flag(
	T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8, T arg9) noexcept
{
	return flag<T>(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

} // namespace mst