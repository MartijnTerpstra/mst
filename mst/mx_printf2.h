//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <cstring>
#include <mx_is_string_type.h>

namespace mst {

namespace _Details {

#define _MST_FORMAT_STRING_SIZE (512)

template<typename T>
inline const char* _To_char_pointer_impl(const T& _Val, ::std::false_type)
{
	_MST_UNUSED(_Val);
	MST_FATAL_ERROR("Value does not convert to a string pointer");
	return nullptr;
}

template<typename T>
inline const char* _To_char_pointer_impl(const T& _Val, ::std::true_type)
{
	return _Val.c_str();
}

template<typename T>
inline const char* _To_char_pointer(const T& _Val)
{
	return _To_char_pointer_impl(_Val, typename ::mst::is_string_type<T>::type());
}

inline const char* _To_char_pointer(const char* const& _Val)
{
	return _Val;
}

template<size_t _Elemcount>
inline const char* _To_char_pointer(const char (&_Val)[_Elemcount])
{
	return _Val;
}

template<typename T>
inline const wchar_t* _To_wchar_t_pointer_impl(const T& _Val, std::true_type)
{
	return _Val.c_str();
}

template<typename T>
inline const wchar_t* _To_wchar_t_pointer_impl(const T& _Val, std::false_type)
{
	_MST_UNUSED(_Val);
	MST_FATAL_ERROR("Value does not convert to a string pointer");
	return nullptr;
}

template<typename T>
inline const wchar_t* _To_wchar_t_pointer(const T& _Val)
{
	return _To_wchar_t_pointer_impl(_Val, typename ::mst::is_wstring_type<T>::type());
}

inline const wchar_t* _To_wchar_t_pointer(const wchar_t* const& _Val)
{
	return _Val;
}

template<size_t _Elemcount>
inline const wchar_t* _To_wchar_t_pointer(const wchar_t (&_Val)[_Elemcount])
{
	return _Val;
}

inline bool _Is_format_char(char _Val)
{
	return _Val == '%';
}

inline bool _Is_format_char(wchar_t _Val)
{
	return _Val == L'%';
}

inline bool _Is_last_format_char(char _Val)
{
	switch(_Val)
	{
	case 'd':
	case 'i':
	case 'u':
	case 'o':
	case 'x':
	case 'X':
	case 'f':
	case 'F':
	case 'e':
	case 'E':
	case 'g':
	case 'G':
	case 'a':
	case 'A':
	case 'c':
	case 's':
	case 'p':
	case 'n':
		return true;
	default:
		return false;
	}
}

inline bool _Is_last_format_char(wchar_t _Val)
{
	switch(_Val)
	{
	case L'd':
	case L'i':
	case L'u':
	case L'o':
	case L'x':
	case L'X':
	case L'f':
	case L'F':
	case L'e':
	case L'E':
	case L'g':
	case L'G':
	case L'a':
	case L'A':
	case L'c':
	case L's':
	case L'p':
	case L'n':
		return true;
	default:
		return false;
	}
}

template<typename _Elem, char charValue>
struct _Char_type
{
	static const _Elem value = static_cast<_Elem>(charValue);
};

#if _MST_USING_VC_COMPILER
#define _MST_SPRINTF   ::sprintf_s
#define _MST_SWPRINTF  ::swprintf_s
#define _MST_SNPRINTF  ::snprintf
#define _MST_SNWPRINTF ::_snwprintf
#elif _MST_USING_GCC_COMPILER
#define _MST_SPRINTF(_Buf, _N, _For, ...) ::sprintf(_Buf, _For, __VA_ARGS__)
#define _MST_SWPRINTF					  ::swprintf
#define _MST_SNPRINTF					  ::snprintf
#elif _MST_USING_CLANG_COMPILER
#define _MST_SPRINTF(_Buf, _N, _For, ...) ::sprintf(_Buf, _For, __VA_ARGS__)
#define _MST_SWPRINTF					  ::swprintf
#define _MST_SNPRINTF					  ::snprintf
#else
#error "No sprintf support defined"
#endif

template<typename Arg, size_t StrSize>
inline void _To_string(char (&buffer)[StrSize], const char* formatString, Arg&& arg)
{
	_MST_SPRINTF(buffer, StrSize, formatString, std::forward<Arg>(arg));
}

template<typename Arg, size_t StrSize>
inline void _To_string(wchar_t (&buffer)[StrSize], const wchar_t* formatString, Arg&& arg)
{
	_MST_SWPRINTF(buffer, StrSize, formatString, std::forward<Arg>(arg));
}

inline size_t _Strlen(const char* arg)
{
	return ::strlen(arg);
}

inline size_t _Strlen(const wchar_t* arg)
{
	return ::wcslen(arg);
}

template<typename Arg>
inline void _String_to_string(char* buffer, size_t length, const char* formatString, Arg&& arg)
{
	_MST_SPRINTF(buffer, length, formatString, std::forward<Arg>(arg));
}

template<typename Arg>
inline void _String_to_string(
	wchar_t* buffer, size_t length, const wchar_t* formatString, Arg&& arg)
{
	_MST_SWPRINTF(buffer, length, formatString, std::forward<Arg>(arg));
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_signed_integer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::true_type)
{
	_Elem fmt[32] = {};

	if(stringLength >= 3)
	{
		switch(formatString[stringLength - 3])
		{
		case _Char_type<_Elem, 'h'>::value: // '%hhd' or '%hhi' => signed char type
			if(formatString[stringLength - 2] != 'h')
			{
				MST_FATAL_ERROR("Invalid format specifier");
			}
			if(arg >= SCHAR_MIN && arg <= SCHAR_MAX)
			{
				_To_string(fmt, formatString, static_cast<signed char>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'l': // '%lld' or '%lli' => long long int type
			if(formatString[stringLength - 2] != 'l')
			{
				MST_FATAL_ERROR("Invalid format specifier");
			}
			if(arg >= LLONG_MIN && arg <= LLONG_MAX)
			{
				_To_string(fmt, formatString, static_cast<long long int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		default:
			break;
		}
	}
	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case 'h': // '%hd' or '%hi' => short int type
			if(arg >= SHRT_MIN && arg <= SHRT_MAX)
			{
				_To_string(fmt, formatString, static_cast<short int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'l': // '%ld' or '%li' => long int type
			if(arg >= LONG_MIN && arg <= LONG_MAX)
			{
				_To_string(fmt, formatString, static_cast<long int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'j': // '%jd' or '%ji' => intmax_t type
			if(arg >= INTMAX_MIN && arg <= INTMAX_MAX)
			{
				_To_string(fmt, formatString, static_cast<intmax_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'z': // '%zd' or '%zi' => size_t type
			if(arg >= 0 && arg <= SIZE_MAX)
			{
				_To_string(fmt, formatString, static_cast<size_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 't': // '%td' or '%ti' => ptrdiff_t type
			if(arg >= PTRDIFF_MIN && arg <= PTRDIFF_MAX)
			{
				_To_string(fmt, formatString, static_cast<ptrdiff_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		default:
			break;
		}
	}

	// '%d' or '%i' => int type
	if(arg >= INT_MIN && arg <= INT_MAX)
	{
		_To_string(fmt, formatString, static_cast<int>(arg));
		buffer.append(fmt);
		return;
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_signed_integer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::false_type)
{
	_MST_UNUSED(buffer);
	_MST_UNUSED(formatString);
	_MST_UNUSED(stringLength);
	_MST_UNUSED(arg);
	MST_FATAL_ERROR("Argument could not be converted to this format specifier");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_unsigned_integer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::true_type)
{
	_Elem fmt[32] = {};

	if(stringLength >= 3)
	{
		switch(formatString[stringLength - 3])
		{
		case 'h': // '%hhu' or '%hho' or '%hhx' or '%hhX' => unsigned char type
			if(formatString[stringLength - 2] != 'h')
			{
				MST_FATAL_ERROR("Invalid format specifier");
			}
			if(arg >= 0 && arg <= UCHAR_MAX)
			{
				_To_string(fmt, formatString, static_cast<unsigned char>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'l': // '%llu' or '%llo' or '%llx' or '%llX' => unsigned long long int type
			if(formatString[stringLength - 2] != 'l')
			{
				MST_FATAL_ERROR("Invalid format specifier");
			}
			if(arg >= 0 && arg <= ULLONG_MAX)
			{
				_To_string(fmt, formatString, static_cast<unsigned long long int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		default:
			break;
		}
	}
	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case 'h': // '%hu' or '%ho' or '%hx' or '%hX' => unsigned short int type
			if(arg >= 0 && arg <= USHRT_MAX)
			{
				_To_string(fmt, formatString, static_cast<unsigned short int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'l': // '%lu' or '%lo' or '%lx' or '%lX' => unsigned long int type
			if(arg >= 0 && arg <= ULONG_MAX)
			{
				_To_string(fmt, formatString, static_cast<unsigned long int>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'j': // '%ju' or '%jo' or '%jx' or '%jX' => uintmax_t type
			if(arg >= 0 && arg <= UINTMAX_MAX)
			{
				_To_string(fmt, formatString, static_cast<uintmax_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 'z': // '%zu' or '%zo' or '%zx' or '%zX' => size_t type
			if(arg >= 0 && arg <= SIZE_MAX)
			{
				_To_string(fmt, formatString, static_cast<size_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		case 't': // '%tu' or '%to' or '%tx' or '%tX' => ptrdiff_t type
			if(arg >= PTRDIFF_MIN && arg <= PTRDIFF_MAX)
			{
				_To_string(fmt, formatString, static_cast<ptrdiff_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		default:
			break;
		}
	}

	// '%u' or '%o' or '%x' or '%X' => unsigned int type
	if(arg >= 0 && arg <= UINT_MAX)
	{
		_To_string(fmt, formatString, static_cast<unsigned int>(arg));
		buffer.append(fmt);
		return;
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_unsigned_integer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::false_type)
{
	_MST_UNUSED(buffer);
	_MST_UNUSED(formatString);
	_MST_UNUSED(stringLength);
	_MST_UNUSED(arg);
	MST_FATAL_ERROR("Argument could not be converted to this format specifier");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_floating_point_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::true_type)
{
	// Double can get quite big
	_Elem fmt[512] = {};

	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case 'L': // '%Lf' or '%LF' or '%Lg' or '%LG' or '%La' or '%LA' => long double type
			MST_FATAL_ERROR("Long double not supported");
			return;
		default:
			break;
		}
	}

	// '%f' or '%F' or '%g' or '%G' or '%a' or '%A' => double type
	if(arg >= -DBL_MAX && arg <= DBL_MAX)
	{
		_To_string(fmt, formatString, static_cast<double>(arg));
		buffer.append(fmt);
		return;
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_floating_point_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::false_type)
{
	_MST_UNUSED(buffer);
	_MST_UNUSED(formatString);
	_MST_UNUSED(stringLength);
	_MST_UNUSED(arg);
	MST_FATAL_ERROR("Argument could not be converted to this format specifier");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_pointer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::true_type)
{
	_MST_UNUSED(stringLength);
	_Elem fmt[32] = {};

	// '%p' => pointer type
	if(reinterpret_cast<size_t>(arg) >= 0 && reinterpret_cast<size_t>(arg) <= SIZE_MAX)
	{
		_To_string(fmt, formatString, static_cast<const void*>(arg));
		buffer.append(fmt);
		return;
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_pointer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::false_type)
{
	_MST_UNUSED(buffer);
	_MST_UNUSED(formatString);
	_MST_UNUSED(stringLength);
	_MST_UNUSED(arg);
	MST_FATAL_ERROR("Argument could not be converted to this format specifier");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_character_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::true_type)
{
	_Elem fmt[32] = {};

	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case(_Elem)'l': // '%lc' => wint_t type
			if(arg >= WINT_MIN && arg <= WINT_MAX)
			{
				_To_string(fmt, formatString, static_cast<wint_t>(arg));
				buffer.append(fmt);
				return;
			}
			else
			{
				MST_FATAL_ERROR("Argument could not be contained by this format specifier");
			}
			return;
		default:
			break;
		}
	}

	// '%c' => int type
	if(arg >= INT_MIN && arg <= INT_MAX)
	{
		_To_string(fmt, formatString, static_cast<int>(arg));
		buffer.append(fmt);
		return;
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_character_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg, ::std::false_type)
{
	_MST_UNUSED(buffer);
	_MST_UNUSED(formatString);
	_MST_UNUSED(stringLength);
	_MST_UNUSED(arg);
	MST_FATAL_ERROR("Argument could not be converted to this format specifier");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_string_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	if(stringLength > 3 && stringLength <= 1)
	{
		MST_FATAL_ERROR("Argument could not be converted to this format specifier");
	}
	else if(stringLength == 3 && (formatString[1] != (_Elem)'l' && formatString[2] != (_Elem)'s'))
	{
		MST_FATAL_ERROR("'%ls' or '%s' argument expected");
	}
	else if(stringLength == 2 && formatString[1] != (_Elem)'s')
	{
		MST_FATAL_ERROR("'%ls' or '%s' argument expected");
	}

	size_t _Startpos = buffer.length();
	size_t _Len;

	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case L'l': // '%ls' => const w_char_t* type

			_Len = _Strlen(_To_wchar_t_pointer(arg)) + 1;

			buffer.resize(_Startpos + _Len);

			_String_to_string(&buffer[_Startpos], _Len, formatString,
				static_cast<const wchar_t*>(_To_wchar_t_pointer(arg)));

			// remove trailing '\0'
			buffer.pop_back();
			return;
		default:
			break;
		}
	}

	_Len = _Strlen(_To_char_pointer(arg)) + 1;

	buffer.resize(_Startpos + _Len);

	// '%s' => const char* type
	_String_to_string(
		&buffer[_Startpos], _Len, formatString, static_cast<const char*>(_To_char_pointer(arg)));
	// remove trailing '\0'
	buffer.pop_back();
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_argument_impl(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	switch(formatString[stringLength - 1])
	{
	case L'd':
	case L'i':
		_Append_signed_integer_argument(buffer, formatString, stringLength,
			::std::forward<Arg>(arg),
			typename ::std::is_integral<typename std::remove_cv<
				typename ::std::remove_reference<Arg>::type>::type>::type());
		return;
	case L'u':
	case L'o':
	case L'x':
	case L'X':
		_Append_unsigned_integer_argument(buffer, formatString, stringLength,
			::std::forward<Arg>(arg),
			typename ::std::is_integral<typename std::remove_cv<
				typename ::std::remove_reference<Arg>::type>::type>::type());
		return;
	case L'f':
	case L'F':
	case L'e':
	case L'E':
	case L'g':
	case L'G':
	case L'a':
	case L'A':
		_Append_floating_point_argument(buffer, formatString, stringLength,
			::std::forward<Arg>(arg),
			typename ::std::is_arithmetic<typename std::remove_cv<
				typename ::std::remove_reference<Arg>::type>::type>::type());
		return;
	case L'c':
		_Append_character_argument(buffer, formatString, stringLength, ::std::forward<Arg>(arg),
			typename ::std::is_integral<typename std::remove_cv<
				typename ::std::remove_reference<Arg>::type>::type>::type());
		return;
	case L's':
		_Append_string_argument(buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		return;
	case L'p':
		_Append_pointer_argument(buffer, formatString, stringLength, ::std::forward<Arg>(arg),
			typename ::std::is_pointer<typename std::remove_cv<
				typename ::std::remove_reference<Arg>::type>::type>::type());
		return;
	case L'n':
		MST_BREAKPOINT;
	default:
		MST_FATAL_ERROR("Invalid format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_argument(
	::std::basic_string<_Elem, _Traits, _Alloc>& buffer, const _Elem*& format, Arg&& arg)
{
	_Elem formatString[_MST_FORMAT_STRING_SIZE] = { '%' };
	size_t index = 1;

	do
	{
		formatString[index] = *format;

		if(*format == (_Elem)'*')
		{
			MST_FATAL_ERROR("'*' not supported");
			return;
		}

		++format;

	} while(!_Is_last_format_char(formatString[index++]));

	_Append_argument_impl(buffer, formatString, index, ::std::forward<Arg>(arg));
}

template<typename _Elem, typename _Traits, typename _Alloc>
inline void _Printf_impl(::std::basic_string<_Elem, _Traits, _Alloc>& buffer, const _Elem* format)
{ // should not encounter any more arguments

	while(*format)
	{
		if(_Is_format_char(*format) && !_Is_format_char(*(++format)))
		{
			MST_FATAL_ERROR("Argument encountered where non where provided");
		}

		buffer.push_back(*format);

		++format;
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg, typename... Args>
inline void _Printf_impl(::std::basic_string<_Elem, _Traits, _Alloc>& buffer, const _Elem* format,
	Arg&& arg, Args&&... args)
{
	while(*format)
	{
		if(_Is_format_char(*format) && !_Is_format_char(*(++format)))
		{
			_Append_argument(buffer, format, arg);
			_Printf_impl(buffer, format, ::std::forward<Args>(args)...);
			return;
		}
		else
		{
			buffer.push_back(*format);
		}

		++format;
	}

	MST_FATAL_ERROR("Provided argument not encountered");
}

template<typename _Elem, typename _Traits, typename _Alloc, typename... Args>
inline ::std::basic_string<_Elem, _Traits, _Alloc> _Printf(const _Elem* format, Args&&... args)
{
	::std::basic_string<_Elem, _Traits, _Alloc> buffer;
	_Printf_impl(buffer, format, ::std::forward<Args>(args)...);
	return buffer;
}

template<typename _Traits, typename _Alloc>
inline void _Print_to_cout(const ::std::basic_string<char, _Traits, _Alloc>& buffer)
{
	std::cout << buffer.c_str() << std::flush;
}

template<typename _Traits, typename _Alloc>
inline void _Print_to_cout(const ::std::basic_string<wchar_t, _Traits, _Alloc>& buffer)
{
	std::wcout << buffer.c_str() << std::flush;
}

} // namespace _Details
} // namespace mst