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

// note: no include guards

template<typename T, size_t targetIndex, size_t currentIndex, typename ArgFirst MST_VARIADIC_TEMPLATE>
struct _Func_argument_type<T (*)(ArgFirst MST_VARIADIC_TEMPLATE_TYPES_COMMA), targetIndex, currentIndex>
{
	typedef typename _Func_argument_type<T (*)(MST_VARIADIC_TEMPLATE_TYPES), targetIndex, currentIndex + 1>::type type;
};

template<typename T, size_t targetIndex, typename ArgFirst MST_VARIADIC_TEMPLATE>
struct _Func_argument_type<T (*)(ArgFirst MST_VARIADIC_TEMPLATE_TYPES_COMMA), targetIndex, targetIndex>
{
	typedef ArgFirst type;
};

template<typename T, typename Class, size_t targetIndex, size_t currentIndex, typename ArgFirst MST_VARIADIC_TEMPLATE>
struct _Mem_func_argument_type<T (Class::*)(ArgFirst MST_VARIADIC_TEMPLATE_TYPES_COMMA), targetIndex, currentIndex>
{
	typedef typename _Mem_func_argument_type<T (Class::*)(MST_VARIADIC_TEMPLATE_TYPES), targetIndex, currentIndex + 1>::type type;
};

template<typename T, typename Class, size_t targetIndex, typename ArgFirst MST_VARIADIC_TEMPLATE>
struct _Mem_func_argument_type<T (Class::*)(ArgFirst MST_VARIADIC_TEMPLATE_TYPES_COMMA), targetIndex, targetIndex>
{
	typedef ArgFirst type;
};