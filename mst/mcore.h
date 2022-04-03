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

#ifndef MCORE_H
#define MCORE_H

#include <mx_core.h>

#include <mversion.h>

// typedefs of unsigned types
#ifndef _UNSIGNED_INTEGERS_DEFINED
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char ubyte;
typedef unsigned char byte;
#define _UNSIGNED_INTEGERS_DEFINED 1
#endif

#ifndef _VARIADIC_MAX
#define _VARIADIC_MAX 10
#endif

#ifndef _MST_EPSILON
#define _MST_EPSILON (1e-5L)
#endif

#ifndef NOMINMAX
#define NOMINMAX 1
#endif

#ifndef MST_DEBUGMODE
#if _DEBUG
#define MST_DEBUGMODE 1
#endif
#endif

#ifndef MST_DEFAULT_NARGS
#define MST_DEFAULT_NARGS 5
#endif

#endif // MCORE_H