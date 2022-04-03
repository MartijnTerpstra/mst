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

namespace _Details {

template<typename IntegerType>
_MST_CONSTEXPR17 uint32_t _Hash32(const IntegerType* _First, size_t _Count) noexcept
{
	const uint32_t _FNV_offset_basis = 2166136261UL;
	const uint32_t _FNV_prime = 16777619UL;

	uint32_t _Hash = _FNV_offset_basis;
	for(size_t _Next = 0; _Next < _Count; ++_Next)
	{ // fold in another byte
		_Hash ^= static_cast<uint32_t>(_First[_Next]);
		_Hash *= _FNV_prime;
	}

	return _Hash;
}

template<typename IntegerType>
_MST_CONSTEXPR17 uint64_t _Hash64(const IntegerType* _First, size_t _Count) noexcept
{
	const uint64_t _FNV_offset_basis = 14695981039346656037ULL;
	const uint64_t _FNV_prime = 1099511628211ULL;

	uint64_t _Hash = _FNV_offset_basis;
	for(uint64_t _Next = 0; _Next < _Count; ++_Next)
	{ // fold in another byte
		_Hash ^= static_cast<uint64_t>(_First[_Next]);
		_Hash *= _FNV_prime;
	}

	_Hash ^= _Hash >> 32;

	return _Hash;
}

#if _MST_USING_VC_COMPILER

_MST_CONSTEXPR17 uint32_t _Hash_of32_impl(const char* _First) noexcept
{

	size_t _N = std::char_traits<char>::length(_First);

	size_t _Begin = 0;
	while(_First[_Begin++] != '<')
	{
	}

	size_t _Begin2 = _Begin;
	while(_First[_Begin2++] != ' ')
	{
		if(_First[_Begin2] == '>')
		{
			_Begin2 = _Begin;
			break;
		}
	}

	size_t _End = _N;
	while(_First[--_End] != '>')
	{
	}

	return _Hash32(_First + _Begin2, _End - _Begin2);
}

_MST_CONSTEXPR17 uint64_t _Hash_of64_impl(const char* _First) noexcept
{
	size_t _N = std::char_traits<char>::length(_First);

	size_t _Begin = 0;
	while(_First[_Begin++] != '<')
	{
	}

	size_t _Begin2 = _Begin;
	while(_First[_Begin2++] != ' ')
	{
		if(_First[_Begin2] == '>')
		{
			_Begin2 = _Begin;
			break;
		}
	}

	size_t _End = _N;
	while(_First[--_End] != '>')
	{
	}

	return _Hash64(_First + _Begin2, _End - _Begin2);
}

inline ::std::string _Typename_of_impl(const char* _First)
{

	size_t _N = std::char_traits<char>::length(_First);

	size_t _Begin = 0;
	while(_First[_Begin++] != '<')
	{
	}

	size_t _End = _N;
	while(_First[--_End] != '>')
	{
	}

	return ::std::string(_First + _Begin, _End - _Begin);
}

#elif _MST_USING_GCC_COMPILER

_MST_CONSTEXPR17 size_t _Findstr(
	const char* _First, size_t _First_length, const char* _Needle) noexcept
{
	const size_t _Needle_length = std::char_traits<char>::length(_Needle);

	for(size_t _Index = 0; _Index <= _First_length - _Needle_length; ++_Index)
	{
		for(size_t _Offset = 0; _Offset < _Needle_length; ++_Offset)
		{
			if(_First[_Index + _Offset] != _Needle[_Offset])
				break;

			if(_Offset == _Needle_length - 1)
				return _Index;
		}
	}

	return SIZE_MAX;
}

_MST_CONSTEXPR17 uint32_t _Hash_of32_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[with T = ") + 10;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return _Hash32(_First + _Begin, _End - _Begin);
}

_MST_CONSTEXPR17 uint64_t _Hash_of64_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[with T = ") + 10;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return _Hash64(_First + _Begin, _End - _Begin);
}

inline ::std::string _Typename_of_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[with T = ") + 10;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return ::std::string(_First + _Begin, _End - _Begin);
}

#elif _MST_USING_CLANG_COMPILER

_MST_CONSTEXPR17 size_t _Findstr(
	const char* _First, size_t _First_length, const char* _Needle) noexcept
{
	const size_t _Needle_length = std::char_traits<char>::length(_Needle);

	for(size_t _Index = 0; _Index <= _First_length - _Needle_length; ++_Index)
	{
		for(size_t _Offset = 0; _Offset < _Needle_length; ++_Offset)
		{
			if(_First[_Index + _Offset] != _Needle[_Offset])
				break;

			if(_Offset == _Needle_length - 1)
				return _Index;
		}
	}

	return SIZE_MAX;
}

_MST_CONSTEXPR17 uint32_t _Hash_of32_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[T = ") + 5;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return _Hash32(_First + _Begin, _End - _Begin);
}

_MST_CONSTEXPR17 uint64_t _Hash_of64_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[T = ") + 5;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return _Hash64(_First + _Begin, _End - _Begin);
}

inline ::std::string _Typename_of_impl(const char* _First)
{
	const size_t _N = std::char_traits<char>::length(_First);

	const size_t _Begin = _Findstr(_First, _N, "[T = ") + 5;

	size_t _End = _Begin;
	while(_First[++_End] != ';' && _First[_End] != ']')
	{
	}

	return ::std::string(_First + _Begin, _End - _Begin);
}

#else

#error "Requires pretty function signature macro"

#endif

template<typename T>
_MST_CONSTEXPR17 uint32_t _Hash_of32()
{
	return ::mst::_Details::_Hash_of32_impl(_MST_FUNCTION_SIGNATURE);
}

template<typename T>
_MST_CONSTEXPR17 uint64_t _Hash_of64()
{
	return ::mst::_Details::_Hash_of64_impl(_MST_FUNCTION_SIGNATURE);
}

template<typename T>
inline const char* _Typename_of()
{
	static ::std::string retval = ::mst::_Details::_Typename_of_impl(_MST_FUNCTION_SIGNATURE);

	return retval.c_str();
}

} // namespace _Details
} // namespace mst