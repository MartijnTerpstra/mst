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

//
// --- cross-compiler, typedef-transparent type-name canonicalization ---
//
// _MST_FUNCTION_SIGNATURE (__FUNCSIG__ / __PRETTY_FUNCTION__) already resolves typedefs/aliases
// to their underlying type for us - two typedefs of the same type always produce the same raw
// signature text. What differs is spelling: MSVC prepends elaborated-type keywords
// ("struct "/"class "/"enum "/"union ") in front of every tag-type reference (including nested
// ones inside template arguments) and spells `long long`/`unsigned long long` as
// `__int64`/`unsigned __int64`; GCC prints compound integer keywords in its internal canonical
// order (e.g. `unsigned long` as "long unsigned int"); whitespace conventions around `*`, `&`,
// `,` etc. differ between all three. None of that carries type identity, so it is normalized
// away below, leaving a canonical string that is stable within a compiler (typedef-transparent,
// as required by std::is_same_v) and - for fundamental types, enums, pointers/references/cv
// qualifiers/arrays, and user-defined class/struct/union types, including templates thereof -
// identical across MSVC/Clang/GCC on the same platform ABI.
//
// Known, deliberately out-of-scope cases (documented rather than "fixed"):
//  - anonymous-namespace types: each compiler spells the anonymous namespace differently, and
//    since such types have internal linkage they are legitimately distinct per translation unit
//    anyway, so unifying the spelling would be both impractical and misleading.
//  - std:: container/string types: their spelling embeds standard-library-implementation detail
//    (inline namespaces, allocator/traits spelling) that differs by stdlib, not just by compiler;
//    a text canonicalizer operating on one compiler's pretty-printed string can't see through
//    that. These remain stable within one compiler+stdlib build, but are not guaranteed to match
//    across e.g. MSVC STL vs. libstdc++ vs. libc++.
//  - platform ABI differences (e.g. `size_t` is `unsigned long` under Linux LP64 but a 64-bit
//    type distinct from `unsigned long` under Windows LLP64) are genuine type differences, so
//    differing hashes there are correct, not a bug.
//

_MST_CONSTEXPR17 bool _Is_word_char(char _C) noexcept
{
	return (_C >= 'a' && _C <= 'z') || (_C >= 'A' && _C <= 'Z') || (_C >= '0' && _C <= '9')
		   || _C == '_';
}

// whole-word compare of a (non null-terminated) token against a null-terminated literal
_MST_CONSTEXPR17 bool _Word_equals(const char* _Word, size_t _Length, const char* _Literal) noexcept
{
	size_t _Index = 0;
	for(; _Index < _Length; ++_Index)
	{
		if(_Literal[_Index] == '\0' || _Word[_Index] != _Literal[_Index])
			return false;
	}

	return _Literal[_Index] == '\0';
}

// keywords that carry no type-identity information and are always dropped, wherever they occur
_MST_CONSTEXPR17 bool _Is_dropped_keyword(const char* _Word, size_t _Length) noexcept
{
	return _Word_equals(_Word, _Length, "struct") || _Word_equals(_Word, _Length, "class")
		   || _Word_equals(_Word, _Length, "enum") || _Word_equals(_Word, _Length, "union")
		   || _Word_equals(_Word, _Length, "__cdecl") || _Word_equals(_Word, _Length, "__stdcall")
		   || _Word_equals(_Word, _Length, "__fastcall")
		   || _Word_equals(_Word, _Length, "__thiscall")
		   || _Word_equals(_Word, _Length, "__vectorcall");
}

template<typename Sink>
_MST_CONSTEXPR17 void _Emit_word(Sink& _Out, bool& _NeedSeparator, const char* _Word, size_t _Length) noexcept
{
	if(_NeedSeparator)
		_Out(' ');

	for(size_t _Index = 0; _Index < _Length; ++_Index)
		_Out(_Word[_Index]);

	_NeedSeparator = true;
}

#if _MST_USING_VC_COMPILER

_MST_CONSTEXPR17 size_t _Type_name_begin(const char* _First) noexcept
{
	size_t _Begin = 0;
	while(_First[_Begin++] != '<')
	{
	}

	return _Begin;
}

_MST_CONSTEXPR17 size_t _Type_name_end(const char* _First, size_t) noexcept
{
	size_t _End = std::char_traits<char>::length(_First);
	while(_First[--_End] != '>')
	{
	}

	return _End;
}

// MSVC spells `long long`/`unsigned long long` as `__int64`/`unsigned __int64`; everything else
// (including `unsigned`, `long`, `short` on their own) is already spelled naturally.
struct _Word_translator
{
	template<typename Sink>
	_MST_CONSTEXPR17 void feed(const char* _Word, size_t _Length, Sink& _Out, bool& _NeedSeparator) noexcept
	{
		if(_Word_equals(_Word, _Length, "__int64"))
		{
			_Emit_word(_Out, _NeedSeparator, "long", 4);
			_Emit_word(_Out, _NeedSeparator, "long", 4);
		}
		else
		{
			_Emit_word(_Out, _NeedSeparator, _Word, _Length);
		}
	}

	template<typename Sink>
	_MST_CONSTEXPR17 void flush(Sink&, bool&) noexcept
	{ // stateless: nothing pending
	}
};

#elif _MST_USING_GCC_COMPILER || _MST_USING_CLANG_COMPILER

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

#if _MST_USING_GCC_COMPILER

_MST_CONSTEXPR17 size_t _Type_name_begin(const char* _First) noexcept
{
	const size_t _N = std::char_traits<char>::length(_First);
	return _Findstr(_First, _N, "[with T = ") + 10;
}

// GCC prints compound integer keywords in its internal canonical order, always suffixed with
// "int" (e.g. `unsigned long` -> "long unsigned int", `unsigned short` -> "short unsigned int",
// `long long` -> "long long int"). These four words are reserved keywords - they can never be
// part of an identifier - so accumulating a run of them and re-emitting in natural order/spelling
// is unambiguous.
struct _Word_translator
{
	size_t _LongCount = 0;
	bool _SawShort = false;
	bool _SawUnsigned = false;
	bool _SawInt = false;

	template<typename Sink>
	_MST_CONSTEXPR17 void feed(const char* _Word, size_t _Length, Sink& _Out, bool& _NeedSeparator) noexcept
	{
		if(_Word_equals(_Word, _Length, "long"))
		{
			++_LongCount;
			return;
		}
		if(_Word_equals(_Word, _Length, "short"))
		{
			_SawShort = true;
			return;
		}
		if(_Word_equals(_Word, _Length, "unsigned"))
		{
			_SawUnsigned = true;
			return;
		}
		if(_Word_equals(_Word, _Length, "int"))
		{
			_SawInt = true;
			return;
		}

		flush(_Out, _NeedSeparator);
		_Emit_word(_Out, _NeedSeparator, _Word, _Length);
	}

	template<typename Sink>
	_MST_CONSTEXPR17 void flush(Sink& _Out, bool& _NeedSeparator) noexcept
	{
		if(_LongCount == 0 && !_SawShort && !_SawUnsigned && !_SawInt)
			return;

		if(_SawUnsigned)
			_Emit_word(_Out, _NeedSeparator, "unsigned", 8);

		if(_LongCount == 1)
			_Emit_word(_Out, _NeedSeparator, "long", 4);
		else if(_LongCount >= 2)
		{
			_Emit_word(_Out, _NeedSeparator, "long", 4);
			_Emit_word(_Out, _NeedSeparator, "long", 4);
		}
		else if(_SawShort)
			_Emit_word(_Out, _NeedSeparator, "short", 5);
		else if(_SawInt)
			_Emit_word(_Out, _NeedSeparator, "int", 3);

		_LongCount = 0;
		_SawShort = _SawUnsigned = _SawInt = false;
	}
};

#else // _MST_USING_CLANG_COMPILER

_MST_CONSTEXPR17 size_t _Type_name_begin(const char* _First) noexcept
{
	const size_t _N = std::char_traits<char>::length(_First);
	return _Findstr(_First, _N, "[T = ") + 5;
}

// Clang prints fundamental integer keywords in natural source order already.
struct _Word_translator
{
	template<typename Sink>
	_MST_CONSTEXPR17 void feed(const char* _Word, size_t _Length, Sink& _Out, bool& _NeedSeparator) noexcept
	{
		_Emit_word(_Out, _NeedSeparator, _Word, _Length);
	}

	template<typename Sink>
	_MST_CONSTEXPR17 void flush(Sink&, bool&) noexcept
	{ // stateless: nothing pending
	}
};

#endif

// scans to the end of the "T = ..." clause: a top-level ';' (separating another template
// parameter) or ']' (closing the whole with-clause) ends it, but a ']' belonging to an array
// type spelled inside T (e.g. "int [3]") must not be mistaken for that - so '['/']' are
// depth-tracked instead of matched on the first occurrence.
_MST_CONSTEXPR17 size_t _Type_name_end(const char* _First, size_t _Begin) noexcept
{
	size_t _End = _Begin;
	size_t _Depth = 0;

	while(true)
	{
		const char _C = _First[_End];

		if(_C == ';' && _Depth == 0)
			return _End;
		if(_C == '[')
			++_Depth;
		else if(_C == ']')
		{
			if(_Depth == 0)
				return _End;
			--_Depth;
		}

		++_End;
	}
}

#else

#error "Requires pretty function signature macro"

#endif

// tokenizes [_Begin, _End) into words and punctuation, drops keywords that carry no type
// identity, canonicalizes fundamental-type spelling, and re-emits with a single canonical
// spacing rule (one space between adjacent words, none around punctuation) via _Sink(char)
template<typename Sink>
_MST_CONSTEXPR17 void _Emit_canonical_type_name(
	const char* _First, size_t _Begin, size_t _End, Sink& _Out) noexcept(noexcept(_Out(' ')))
{
	bool _NeedSeparator = false;
	_Word_translator _Translator{};

	size_t _Index = _Begin;
	while(_Index < _End)
	{
		const char _C = _First[_Index];

		if(_Is_word_char(_C))
		{
			const size_t _Start = _Index;
			while(_Index < _End && _Is_word_char(_First[_Index]))
				++_Index;

			const size_t _Length = _Index - _Start;

			if(!_Is_dropped_keyword(_First + _Start, _Length))
				_Translator.feed(_First + _Start, _Length, _Out, _NeedSeparator);
		}
		else if(_C == ' ' || _C == '\t')
		{
			++_Index;
		}
		else
		{
			_Translator.flush(_Out, _NeedSeparator);
			_Out(_C);
			_NeedSeparator = false;
			++_Index;
		}
	}

	_Translator.flush(_Out, _NeedSeparator);
}

struct _Hash32_sink
{
	uint32_t _Hash = 2166136261UL;

	_MST_CONSTEXPR17 void operator()(char _C) noexcept
	{
		_Hash ^= static_cast<uint32_t>(static_cast<unsigned char>(_C));
		_Hash *= 16777619UL;
	}
};

struct _Hash64_sink
{
	uint64_t _Hash = 14695981039346656037ULL;

	_MST_CONSTEXPR17 void operator()(char _C) noexcept
	{
		_Hash ^= static_cast<uint64_t>(static_cast<unsigned char>(_C));
		_Hash *= 1099511628211ULL;
	}
};

struct _Typename_sink
{
	::std::string _Result;

	void operator()(char _C)
	{
		_Result.push_back(_C);
	}
};

_MST_CONSTEXPR17 uint32_t _Hash_of32_impl(const char* _First) noexcept
{
	const size_t _Begin = _Type_name_begin(_First);

	_Hash32_sink _Sink;
	_Emit_canonical_type_name(_First, _Begin, _Type_name_end(_First, _Begin), _Sink);

	return _Sink._Hash;
}

_MST_CONSTEXPR17 uint64_t _Hash_of64_impl(const char* _First) noexcept
{
	const size_t _Begin = _Type_name_begin(_First);

	_Hash64_sink _Sink;
	_Emit_canonical_type_name(_First, _Begin, _Type_name_end(_First, _Begin), _Sink);

	_Sink._Hash ^= _Sink._Hash >> 32;

	return _Sink._Hash;
}

inline ::std::string _Typename_of_impl(const char* _First)
{
	const size_t _Begin = _Type_name_begin(_First);

	_Typename_sink _Sink;
	_Emit_canonical_type_name(_First, _Begin, _Type_name_end(_First, _Begin), _Sink);

	return ::std::move(_Sink._Result);
}

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
