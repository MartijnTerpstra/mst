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

#if _MSC_VER

#include <yvals.h>

#define _MST_USING_VC_COMPILER	  1
#define _MST_USING_GCC_COMPILER	  0
#define _MST_USING_CLANG_COMPILER 0

#define _IS_VC14_COMPILER 0
#define _IS_VC13_COMPILER 0
#define _IS_VC12_COMPILER 0
#define _IS_VC11_COMPILER 0
#define _IS_VC10_COMPILER 0
#define _IS_VC9_COMPILER  0

#if _HAS_CONSTEXPR14 || defined(_CONSTEXPR17)

#undef _IS_VC14_COMPILER
#define _IS_VC14_COMPILER 1
#define _MST_COMPILER_VER 14

#elif _HAS_CPP17 || _CPPLIB_VER >= 650

#undef _IS_VC13_COMPILER
#define _IS_VC13_COMPILER 1
#define _MST_COMPILER_VER 13

#elif _HAS_VARIADIC_TEMPLATES || _CPPLIB_VER >= 610

#undef _IS_VC12_COMPILER
#define _IS_VC12_COMPILER 1
#define _MST_COMPILER_VER 12

#elif _CPPLIB_VER >= 540

#undef _IS_VC11_COMPILER
#define _IS_VC11_COMPILER 1
#define _MST_COMPILER_VER 11

#elif(_HAS_CPP0X && _HAS_TR1) || _CPPLIB_VER >= 520

#undef _IS_VC10_COMPILER
#define _IS_VC10_COMPILER 1
#define _MST_COMPILER_VER 10

#else

#undef _IS_VC9_COMPILER
#define _IS_VC9_COMPILER  1
#define _MST_COMPILER_VER 9

#endif

#define _MST_FUNCTION_SIGNATURE __FUNCSIG__

#elif __clang__

#define _MST_USING_VC_COMPILER	  0
#define _MST_USING_GCC_COMPILER	  0
#define _MST_USING_CLANG_COMPILER 1

#define _MST_CLANG_VER (__clang_major__ * 100 + __clang_minor__)

#define _MST_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

#elif __GNUC__ // GCC

#define _MST_USING_VC_COMPILER	  0
#define _MST_USING_GCC_COMPILER	  1
#define _MST_USING_CLANG_COMPILER 0

#define _MST_GCC_VER (__GNUC__ * 100 + __GNUC_MINOR__)

#define _MST_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

#else

#error "compiler not supported"

#endif // MSC_VER

// General values

#if __cpp_inline_variables >= 201606L
#define _MST_HAS_INLINE_VARIABLES 1
#define _MST_INLINE_VAR			  inline
#else
#define _MST_INLINE_VAR
#endif

#if __cpp_constexpr >= 201603L
#define _MST_HAS_CONSTEXPR17 1
#define _MST_CONSTEXPR17	 constexpr
#else
#define _MST_CONSTEXPR17 inline
#endif

#if __cpp_alias_templates >= 200704L
#define _MST_HAS_TEMPLATE_ALIASES 1
#endif

#if __cpp_lambdas >= 200907L
#define _MST_HAS_LAMBDAS 1
#endif

#if __cpp_decltype >= 200707L
#define _MST_HAS_DECLTYPE 1
#endif

#if __cpp_conditional_explicit >= 201806L
#define _MST_HAS_EXPLICIT_OPERATORS 1
#endif

#if __cpp_exceptions
#define _MST_HAS_EXCEPTIONS 1
#endif

#if __cpp_lib_is_invocable >= 201703L
#define _MST_NOEXCEPT_OP_INVOCABLE(...) noexcept(std::is_nothrow_invocable<__VA_ARGS__>::value)
#else
#define _MST_NOEXCEPT_OP_INVOCABLE(...)
#endif

#if __cpp_nontype_template_parameter_auto >= 201606L
#define _MST_HAS_TEMPLATE_AUTO 1
#endif

#if __cpp_lib_invoke >= 201411L
#define _MST_HAS_INVOKE 1
#endif

#if __cpp_concepts >= 201907L
#if __cpp_lib_concepts >= 202002L
#define _MST_HAS_CONCEPTS 1
#endif
#endif

// Attibutes
#ifdef __has_cpp_attribute

#if __has_cpp_attribute(nodiscard) >= 201907L
#define _MST_NODISCARD_CONSTRUCTOR [[nodiscard]]
#define _MST_NODISCARD			   [[nodiscard]]
#elif __has_cpp_attribute(nodiscard) >= 201603L
#define _MST_NODISCARD_CONSTRUCTOR
#define _MST_NODISCARD [[nodiscard]]
#else
#define _MST_NODISCARD
#endif

#if __has_cpp_attribute(deprecated) >= 201309L
#define _MST_HAS_ATTRIBUTE_DEPRECATED 1
#endif

#if __has_cpp_attribute(noreturn) >= 200809L
#define _MST_NORETURN [[noreturn]]
#else
#define _MST_NORETURN
#endif

#if __has_cpp_attribute(likely) >= 201803L
#define _MST_LIKELY [[likely]]
#else
#define _MST_LIKELY
#endif

#if __has_cpp_attribute(unlikely) >= 201803L
#define _MST_UNLIKELY [[unlikely]]
#else
#define _MST_UNLIKELY
#endif

#else

#define _MST_NODISCARD_CONSTRUCTOR
#define _MST_NODISCARD

#define _MST_NORETURN

#define _MST_LIKELY
#define _MST_UNLIKELY

#endif

#if __cpp_lib_is_invocable >= 201703L
#define _MST_HAS_IS_INVOCABLE 1
#endif

#if defined(__AVX512F__)
#define _MST_HAS_AVX512 1
#endif
#if defined(__AVX2__)
#define _MST_HAS_AVX2 1
#endif
#if defined(__AVX__)
#define _MST_HAS_AVX		  1
#define _MST_PERMUTE_PS(v, c) _mm_permute_ps((v), c)
#else
#define _MST_PERMUTE_PS(v, c) _mm_shuffle_ps((v), (v), c)
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define _MST_HAS_X64   1
#define _MST_HAS_64BIT 1
#elif defined(__i386__) || defined(_M_IX86)
#define _MST_HAS_X86 1
#elif(defined(__arm__) || defined(_M_ARM)) || defined(__aarch64__)
#define _MST_HAS_ARM 1
#define _MST_HAS_64BIT 1
#endif