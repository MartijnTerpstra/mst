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

#include <string>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <cstring>
#include <charconv>
#include <mx_is_string_type.h>

namespace mst {

namespace _Details {

#define _MST_FORMAT_STRING_SIZE (512)

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

template<typename _Elem>
inline void _To_ptr_str(_Elem* buffer, size_t value)
{
	const char hexChar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
		'e', 'f' };

	buffer[0] = '0';
	buffer[1] = 'x';
	size_t idx = sizeof(size_t) * 2;
	size_t charIdx = 2;
	do
	{
		--idx;
		buffer[charIdx++] = hexChar[(value >> (idx * 4)) & 0xF];
	} while(idx != 0);
}

template<bool IsWideChar>
inline void length_format_string(char* buffer, size_t length, size_t fmtLength)
{
	if constexpr(IsWideChar)
	{
		_MST_SPRINTF(buffer, length, "%%.%zuls", fmtLength);
	}
	else
	{
#if _MST_USING_VC_COMPILER
		_MST_SPRINTF(buffer, length, "%%.%zuhs", fmtLength);
#else
		_MST_SPRINTF(buffer, length, "%%.%zus", fmtLength);
#endif
	}
}

template<bool IsWideChar>
inline void length_format_string(wchar_t* buffer, size_t length, size_t fmtLength)
{
	if constexpr(IsWideChar)
	{
		_MST_SWPRINTF(buffer, length, L"%%.%zuls", fmtLength);
	}
	else
	{
#if _MST_USING_VC_COMPILER
		_MST_SWPRINTF(buffer, length, L"%%.%zuhs", fmtLength);
#else
		_MST_SWPRINTF(buffer, length, L"%%.%zus", fmtLength);
#endif
	}
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
	const _Elem* formatString, size_t stringLength, Arg&& arg)
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
			if(arg <= SCHAR_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= SCHAR_MIN))
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
			if(arg <= LLONG_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= LLONG_MIN))
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
			if(arg <= SHRT_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= SHRT_MIN))
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
			if(arg <= LONG_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= LONG_MIN))
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
			if(arg <= INTMAX_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= INTMAX_MIN))
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
			if(arg <= PTRDIFF_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= PTRDIFF_MIN))
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
	if(arg <= INT_MAX && (std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= INT_MIN))
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
inline void _Append_unsigned_integer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
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
inline void _Append_floating_point_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
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
inline void _Append_pointer_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	_MST_UNUSED(stringLength);
	_Elem fmt[32] = {};
	_To_ptr_str(fmt, reinterpret_cast<size_t>(arg));
	buffer.append(fmt);
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_character_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	_Elem fmt[32] = {};

	if(stringLength >= 2)
	{
		switch(formatString[stringLength - 2])
		{
		case(_Elem)'l': // '%lc' => wint_t type
			if(arg <= WINT_MAX &&
				(std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= WINT_MIN))
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
	if(arg <= INT_MAX && (std::is_unsigned_v<_Elem> || static_cast<intmax_t>(arg) >= INT_MIN))
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

template<typename _ArgElem, typename _FormatElem>
inline void get_string_format_arguments(std::basic_string_view<_ArgElem> arg,
	std::basic_string_view<_FormatElem> formatting, size_t& totalCount, size_t& charCount)
{
	const auto pointIndex = formatting.find((_FormatElem)'.');
	if(pointIndex == std::string_view::npos)
	{ // Example %255s
		size_t value = 0;
		if constexpr(std::is_same_v<_FormatElem, char>)
		{
			const auto [ptr, _] =
				std::from_chars(formatting.data(), formatting.data() + formatting.length(), value);
			if(ptr != formatting.data() + formatting.length())
			{
				MST_FATAL_ERROR("Invalid characters encountered during string argument parsing");
			}
		}
		else
		{
			size_t processed = 0;
			try
			{
				value = std::stoull(
					std::basic_string<_FormatElem>{ formatting.begin(), formatting.end() },
					&processed);
			}
			catch(...)
			{
				processed = 0;
			}
			if(processed != formatting.length())
			{
				MST_FATAL_ERROR("Invalid characters encountered during string argument parsing");
			}
		}
		totalCount = std::max(arg.length(), value);
	}
	else
	{
		charCount = 0;
		totalCount = 0;
		if(pointIndex < formatting.length() - 1)
		{
			if constexpr(std::is_same_v<_FormatElem, char>)
			{
				const auto [ptr, _] = std::from_chars(formatting.data() + pointIndex + 1,
					formatting.data() + formatting.length(), charCount);

				if(ptr != formatting.data() + formatting.length())
				{
					MST_FATAL_ERROR("Invalid characters encountered during string "
									"argument parsing");
				}
			}
			else
			{
				size_t processed = 0;
				try
				{
					charCount = std::stoull(
						std::basic_string<_FormatElem>{
							formatting.begin() + pointIndex + 1, formatting.end() },
						&processed);
				}
				catch(...)
				{
					processed = 0;
				}
				if(processed != (formatting.length() - pointIndex - 1))
				{
					MST_FATAL_ERROR(
						"Invalid characters encountered during string argument parsing");
				}
			}
			charCount = std::min(arg.length(), charCount);
		}
		if(pointIndex != 0)
		{ // Example %25.s or %25.32s
			if constexpr(std::is_same_v<_FormatElem, char>)
			{
				const auto [ptr, _] =
					std::from_chars(formatting.data(), formatting.data() + pointIndex, totalCount);

				if(ptr != formatting.data() + pointIndex)
				{
					MST_FATAL_ERROR("Invalid characters encountered during string "
									"argument parsing");
				}
			}
			else
			{
				size_t processed = 0;
				try
				{
					totalCount = std::stoull(std::basic_string<_FormatElem>{ formatting.begin(),
												 formatting.begin() + pointIndex },
						&processed);
				}
				catch(...)
				{
					processed = 0;
				}
				if(processed != pointIndex)
				{
					MST_FATAL_ERROR(
						"Invalid characters encountered during string argument parsing");
				}
			}
			totalCount = std::max(charCount, totalCount);
		}
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_string_argument(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	size_t const startpos = buffer.length();

	if(stringLength >= 3)
	{
		switch(formatString[stringLength - 2])
		{
		case(_Elem)'l': // '%ls' => const w_char_t* type
			if constexpr(std::is_constructible_v<std::wstring_view, decltype(arg)>)
			{
				const std::wstring_view argView{ arg };

				size_t totalCount = argView.length();
				size_t charCount = argView.length();
				if(stringLength > 3)
				{ // Example: %3213.42s
					get_string_format_arguments(argView,
						std::basic_string_view{ formatString + 1, stringLength - 3 }, totalCount,
						charCount);
				}

				_Elem tmpFmt[32];
				length_format_string<true>(tmpFmt, 32, charCount);

				buffer.resize(startpos + totalCount + 1);
				const auto paddingCount = totalCount - charCount;
				for(size_t spaceIdx = 0; spaceIdx < paddingCount; ++spaceIdx)
				{
					buffer[startpos + spaceIdx] = (_Elem)' ';
				}

				_String_to_string(
					&buffer[startpos + paddingCount], charCount + 1, tmpFmt, argView.data());
				// remove trailing '\0'
				buffer.pop_back();
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

	// '%s' => const char* type
	if constexpr(std::is_constructible_v<std::string_view, decltype(arg)>)
	{
		const std::string_view argView{ arg };

		size_t totalCount = argView.length();
		size_t charCount = argView.length();
		if(stringLength > 2)
		{ // Example: %3213s
			get_string_format_arguments(argView,
				std::basic_string_view{ formatString + 1, stringLength - 2 }, totalCount,
				charCount);
		}

		_Elem tmpFmt[32];
		length_format_string<false>(tmpFmt, 32, std::min(charCount, argView.length()));

		buffer.resize(startpos + totalCount + 1);
		const auto paddingCount = totalCount - charCount;
		for(size_t spaceIdx = 0; spaceIdx < paddingCount; ++spaceIdx)
		{
			buffer[startpos + spaceIdx] = (_Elem)' ';
		}

		_String_to_string(&buffer[startpos + paddingCount], charCount + 1, tmpFmt, argView.data());
		// remove trailing '\0'
		buffer.pop_back();
	}
	else
	{
		MST_FATAL_ERROR("Argument could not be contained by this format specifier");
	}
}

template<typename _Elem, typename _Traits, typename _Alloc, typename Arg>
inline void _Append_argument_impl(::std::basic_string<_Elem, _Traits, _Alloc>& buffer,
	const _Elem* formatString, size_t stringLength, Arg&& arg)
{
	switch(formatString[stringLength - 1])
	{
	case L'd':
	case L'i':
		if constexpr(std::is_integral_v<std::decay_t<Arg>>)
		{
			_Append_signed_integer_argument(
				buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
	case L'u':
	case L'o':
	case L'x':
	case L'X':
		if constexpr(std::is_integral_v<std::decay_t<Arg>>)
		{
			_Append_unsigned_integer_argument(
				buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
	case L'f':
	case L'F':
	case L'e':
	case L'E':
	case L'g':
	case L'G':
	case L'a':
	case L'A':
		if constexpr(std::is_arithmetic_v<std::decay_t<Arg>>)
		{
			_Append_floating_point_argument(
				buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
	case L'c':
		if constexpr(std::is_integral_v<std::decay_t<Arg>>)
		{
			_Append_character_argument(
				buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
	case L's':
		_Append_string_argument(buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		return;
	case L'p':
		if constexpr(std::is_pointer_v<std::decay_t<Arg>>)
		{
			_Append_pointer_argument(buffer, formatString, stringLength, ::std::forward<Arg>(arg));
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
	case L'n':
		if constexpr(std::is_pointer_v<std::decay_t<Arg>> &&
					 !std::is_const_v<std::remove_pointer_t<std::decay_t<Arg>>> &&
					 std::is_integral_v<std::remove_pointer_t<std::decay_t<Arg>>>)
		{
			using PointerIntType = std::remove_pointer_t<std::decay_t<Arg>>;
			if(buffer.length() <= std::numeric_limits<PointerIntType>::max())
			{
				*arg = static_cast<PointerIntType>(buffer.length());
			}
			else
			{
				MST_FATAL_ERROR("Buffer length does not fit in n parameter");
			}
		}
		else
		{
			MST_FATAL_ERROR("Argument could not be converted to this format specifier");
		}
		return;
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