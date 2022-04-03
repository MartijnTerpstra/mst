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

#include <cstddef>
#include <cstdint>
#include <utility>
#include <mx_compiler.h>

#if _MST_USING_VC_COMPILER
#define _MST_ALIGNED_ALLOC(_Size, _Alignment) _aligned_malloc(_Size, _Alignment);
#elif _MST_USING_GCC_COMPILER
#define _MST_ALIGNED_ALLOC(_Size, _Alignment) aligned_alloc(_Alignment, _Size);
#elif _MST_USING_CLANG_COMPILER
#define _MST_ALIGNED_ALLOC(_Size, _Alignment) aligned_alloc(_Alignment, _Size);
#else
#error "No alignment possible for this compiler"
#endif

#define MST_ALIGN(x) alignas(x)

#if _MST_USING_VC_COMPILER
#define MST_DECLARE_DLL_FUNCTION_GENERATE __declspec(dllexport)
#define MST_DECLARE_DLL_TYPE_GENERATE __declspec(dllexport)
#elif _MST_USING_GCC_COMPILER
#define MST_DECLARE_DLL_FUNCTION_GENERATE __attribute__(dllexport)
#define MST_DECLARE_DLL_TYPE_GENERATE __attribute__(dllexport)
#elif _MST_USING_CLANG_COMPILER
#define MST_DECLARE_DLL_FUNCTION_GENERATE __attribute__(dllexport)
#define MST_DECLARE_DLL_TYPE_GENERATE __attribute__(dllexport)
#else
#error "No exporting to DLL possible for this compiler"
#endif

#ifndef MST_DECLARE_DLL_FUNCTION
#define MST_DECLARE_DLL_FUNCTION
#endif // !MST_DECLARE_DLL_FUNCTION

#ifndef MST_DECLARE_DLL_TYPE
#define MST_DECLARE_DLL_TYPE
#endif // !MST_DECLARE_DLL_TYPE

#define MST_NON_COPYABLE(Typename)                 \
private:                                           \
	Typename(const Typename&) = delete;            \
	Typename(Typename&&) = delete;                 \
	Typename& operator=(const Typename&) = delete; \
	Typename& operator=(Typename&&) = delete

#if _MST_USING_VC_COMPILER
#define _MST_VC_TYPENAME typename
#else
#define _MST_VC_TYPENAME
#endif

#if _MST_HAS_ATTRIBUTE_DEPRECATED
#define MST_DEPRECATED(newFunction) [[deprecated("function is deprecated, use " #newFunction " instead")]]
#elif _MST_USING_VC_COMPILER
#define MST_DEPRECATED(newFunction) __declspec(deprecated("function is deprecated, use " #newFunction " instead"))
#elif _MST_USING_GCC_COMPILER
#define MST_DEPRECATED __attribute__(deprecated("function is deprecated, use " #newFunction " instead"))
#elif _MST_USING_CLANG_COMPILER
#define MST_DEPRECATED __attribute__(deprecated("function is deprecated, use " #newFunction " instead"))
#else
#error "MST_DEPRECATED not supported"
#endif

#define _MST_ANONYMOUS_VAR_IMPL2(str1, str2) str1##str2
#define _MST_ANONYMOUS_VAR_IMPL(str1, str2) _MST_ANONYMOUS_VAR_IMPL2(str1, str2)

#ifdef __COUNTER__
#define _MST_ANONYMOUS_VAR(str) _MST_ANONYMOUS_VAR_IMPL(str, __COUNTER__)
#else
#define _MST_ANONYMOUS_VAR(str) _MST_ANONYMOUS_VAR_IMPL(str, __LINE__)
#endif

#define _MST_UNUSED(x) ((void)x)


#include <mtype_traits.h>

namespace mst {
namespace _Details {

template<typename _Ty>
void _Destroy_object(_Ty*, ::std::true_type)
{
	/* do nothing, destructor is trivial */
}

template<typename _Ty>
void _Destroy_object(_Ty* _Ptr, ::std::false_type)
{
	_Ptr->~_Ty();
}

template<typename _Ty>
void _Destroy_object(_Ty* _Ptr)
{
	_Destroy_object(_Ptr, typename ::std::is_trivially_destructible<_Ty>::type());
}


template<typename _Ty>
void _Destroy_object_range(_Ty*, _Ty*, ::std::true_type)
{
	/* do nothing, destructor is trivial */
}

template<typename _Ty>
void _Destroy_object_range(_Ty* _Begin_ptr, _Ty* _Endptr, ::std::false_type)
{
	while(_Begin_ptr != _Endptr)
	{
		_Begin_ptr->~_Ty();
		++_Begin_ptr;
	}
}

template<typename _Ty>
void _Destroy_object_range(_Ty* _Begin_ptr, _Ty* _Endptr)
{
	_Destroy_object_range(_Begin_ptr, _Endptr, typename ::std::is_trivially_destructible<_Ty>::type());
}

} // namespace _Details
} // namespace mst