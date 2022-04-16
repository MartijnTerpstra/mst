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

#ifndef MCOMMON_H
#define MCOMMON_H

#include <mcore.h>
#include <mdebug.h>
#include <mmemory_view.h>
#include <string>
#include <mx_common.h>
#include <mx_printf2.h>
#include <mx_hash.h>

namespace mst {

class non_copyable
{
protected:
	non_copyable() = default;
	~non_copyable() = default;

private:
	non_copyable(const non_copyable&) = delete;
	non_copyable& operator=(const non_copyable&) = delete;

}; // class non_copyable

// zeroes objects and arrays
template<typename T>
inline void zeromem(T& val)
{
	static_assert(!std::is_pointer<T>::value, "T = pointer");
	static_assert(std::is_pod<T>::value, "T = not pod");

	memset(&val, 0, sizeof(T));
}

// copies objects and arrays
template<typename T>
inline void copymem(T& dst, const T& src)
{
	static_assert(!std::is_pointer<T>::value, "T = pointer");
	static_assert(std::is_pod<T>::value, "T = not pod");

	memcpy(&dst, &src, sizeof(T));
}

// returns the hash of the class
template<typename T>
_MST_CONSTEXPR17 uint32_t hash_of()
{
	return ::mst::_Details::_Hash_of32<T>();
}

template<typename T>
_MST_CONSTEXPR17 uint64_t hash_of64()
{
	return ::mst::_Details::_Hash_of64<T>();
}

inline uint32_t hash32(const memory_view& memory)
{
	return ::mst::_Details::_Hash32(reinterpret_cast<const char*>(memory.data()), memory.size());
}

_MST_CONSTEXPR17 uint32_t hash32(const char* cString)
{
	return ::mst::_Details::_Hash32(cString, std::char_traits<char>::length(cString));
}

inline uint32_t hash32(const ::std::string& stdString)
{
	return ::mst::_Details::_Hash32(stdString.data(), stdString.length());
}

inline uint32_t hash32(const ::std::wstring& stdString)
{
	return ::mst::_Details::_Hash32(stdString.data(), stdString.length());
}

#ifdef _MDLL_STRING_H
template<typename T>
inline uint32_t hash32(const ::mst::dll_safe::string<T>& dllString)
{
	return ::mst::_Details::_Hash32(dllString.data(), dllString.length());
}
#endif

inline uint64_t hash64(const memory_view& memory)
{
	return ::mst::_Details::_Hash64(reinterpret_cast<const char*>(memory.data()), memory.size());
}

_MST_CONSTEXPR17 uint64_t hash64(const char* cString)
{
	return ::mst::_Details::_Hash64(cString, std::char_traits<char>::length(cString));
}

inline uint64_t hash64(const ::std::string& stdString)
{
	return ::mst::_Details::_Hash64(stdString.data(), stdString.length());
}

inline uint64_t hash64(const ::std::wstring& stdString)
{
	return ::mst::_Details::_Hash64(
		reinterpret_cast<const char*>(stdString.data()), stdString.length());
}

#ifdef _MDLL_STRING_H
template<typename T>
inline uint64_t hash64(const ::mst::dll_safe::string<T>& dllString)
{
	return ::mst::_Details::_Hash64(dllString.data(), dllString.length());
}
#endif

template<typename T>
inline const char* typename_of()
{
	return ::mst::_Details::_Typename_of<T>();
}

template<typename Traits, typename Alloc>
struct is_string_type<::std::basic_string<char, Traits, Alloc>>
	: public ::std::integral_constant<bool, true>
{ };

template<typename Traits, typename Alloc>
struct is_wstring_type<::std::basic_string<wchar_t, Traits, Alloc>>
	: public ::std::integral_constant<bool, true>
{ };

template<typename _Elem, typename... Args>
inline int printf(const _Elem* format, Args&&... args)
{
	const auto result =
		::mst::_Details::_Printf<_Elem, ::std::char_traits<_Elem>, ::std::allocator<_Elem>>(
			format, ::std::forward<Args>(args)...);

	::mst::_Details::_Print_to_cout(result);

	return static_cast<int>(result.length());
}

template<typename _Elem, typename... Args>
inline int sprintf(_Elem* buffer, const _Elem* format, Args&&... args)
{
	const auto result =
		::mst::_Details::_Printf<_Elem, ::std::char_traits<_Elem>, ::std::allocator<_Elem>>(
			format, ::std::forward<Args>(args)...);

	::std::char_traits<_Elem>::copy(buffer, result.c_str(), result.length() + 1);

	return static_cast<int>(result.length());
}

template<typename _Elem, typename... Args>
inline ::std::basic_string<_Elem, ::std::char_traits<_Elem>, ::std::allocator<_Elem>>
to_printf_string(const _Elem* format, Args&&... args)
{
	return ::mst::_Details::_Printf<_Elem, ::std::char_traits<_Elem>, ::std::allocator<_Elem>>(
		format, ::std::forward<Args>(args)...);
}

} // namespace mst

#endif /* MCOMMON_H */