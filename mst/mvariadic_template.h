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

// note no include guard

#ifdef MST_MAX_NARGS

#if MST_MAX_NARGS > 10
#error "cannot do more than 10 variadic arguments in the VC compiler"
#endif // MST_MAX_NARGS > 10

#else // !defined(MST_MAX_NARGS)

#define MST_MAX_NARGS 7

#endif // defined(MST_MAX_NARGS)

#if !defined(MST_NARGS) && defined(MST_DEFAULT_NARGS)
#define MST_NARGS MST_DEFAULT_NARGS
#endif

#ifdef MST_NARGS

#if MST_NARGS < 1 || MST_NARGS > MST_MAX_NARGS
#error "mvariadic_template_begin.h: MST_NARGS has an invalid value"
#endif // MST_NARGS <= 1 || MST_NARGS > MST_MAX_NARGS

#else // !defined(MST_NARGS)

#error "mvariadic_template2.h: MST_NARGS is not defined"

#endif // defined(MST_NARGS)



#ifdef MST_VARIADIC_INCLUDE

#else // !defined(MST_NARGS)

#error "mvariadic_template2.h: MST_VARIADIC_INCLUDE is not defined"

#endif // defined(MST_NARGS)

#if _MST_HAS_VARIADIC_TEMPLATES

#undef MST_NARGS

#ifndef MST_VARIADIC_TEMPLATE_DECL
#define MST_VARIADIC_TEMPLATE_DECL			 template<typename... Args>
#define MST_VARIADIC_TEMPLATE				 , typename... Args
#define MST_VARIADIC_TEMPLATE_TYPES			 Args...
#define MST_VARIADIC_TEMPLATE_TYPES_COMMA	 , Args...
#define MST_VARIADIC_TEMPLATE_REFFS			 Args&&... args
#define MST_VARIADIC_TEMPLATE_REFFS_COMMA	 , Args&&... args
#define MST_VARIADIC_TEMPLATE_COPIES		 Args... args
#define MST_VARIADIC_TEMPLATE_COPIES_COMMA	 , Args... args
#define MST_VARIADIC_TEMPLATE_FORWARDS		 ::std::forward<Args>(args)...
#define MST_VARIADIC_TEMPLATE_FORWARDS_COMMA , ::std::forward<Args>(args)...
#define MST_VARIADIC_TEMPLATE_COUNT			 sizeof...(Args)
#endif

#include MST_VARIADIC_INCLUDE

#else // !_MST_HAS_VARIADIC_TEMPLATES

// fake it

#include <mx_variadic_template_begin.h>

#if MST_NARGS == 1
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 2
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 3
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 4
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 5
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 6
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 7
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 8
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 9
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#elif MST_NARGS == 10
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE
#include <mx_variadic_template.h>
#include MST_VARIADIC_INCLUDE

#else // MST_NARGS > 10

#error "invalid MST_NARGS"

#endif // MST_NARGS

#include <mx_variadic_template_end.h>

#endif // _MST_HAS_VARIADIC_TEMPLATES

#undef MST_VARIADIC_INCLUDE