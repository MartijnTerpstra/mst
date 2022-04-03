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


template<typename T, size_t targetIndex, size_t currentIndex>
struct _Func_argument_type
{ };

template<typename T, size_t targetIndex, size_t currentIndex>
struct _Func_argument_type<T (*)(), targetIndex, currentIndex>
{
	static_assert(targetIndex == -1, "func_argument_type<T,index>: index is out of range");
};

template<typename T, size_t targetIndex, size_t currentIndex>
struct _Mem_func_argument_type
{ };

template<typename T, typename Class, size_t targetIndex, size_t currentIndex>
struct _Mem_func_argument_type<T (Class::*)(), targetIndex, currentIndex>
{
	static_assert(targetIndex == -1, "mem_func_argument_type<T,index>: index is out of range");
};

template<typename T, size_t targetIndex, size_t currentIndex, typename ArgFirst, typename... Args>
struct _Func_argument_type<T (*)(ArgFirst, Args...), targetIndex, currentIndex>
{
	typedef typename _Func_argument_type<T (*)(Args...), targetIndex, currentIndex + 1>::type type;
};

template<typename T, size_t targetIndex, typename ArgFirst, typename... Args>
struct _Func_argument_type<T (*)(ArgFirst, Args...), targetIndex, targetIndex>
{
	typedef ArgFirst type;
};

template<typename T, typename Class, size_t targetIndex, size_t currentIndex, typename ArgFirst, typename... Args>
struct _Mem_func_argument_type<T (Class::*)(ArgFirst, Args...), targetIndex, currentIndex>
{
	typedef typename _Mem_func_argument_type<T (Class::*)(Args...), targetIndex, currentIndex + 1>::type type;
};

template<typename T, typename Class, size_t targetIndex, typename ArgFirst, typename... Args>
struct _Mem_func_argument_type<T (Class::*)(ArgFirst, Args...), targetIndex, targetIndex>
{
	typedef ArgFirst type;
};

} // namespace _Details

template<typename T, size_t index>
struct func_argument_type
{
	typedef typename ::mst::_Details::_Func_argument_type<typename ::std::remove_pointer<T>::type*, index, 0>::type type;
};

template<typename T, size_t index>
struct mem_func_argument_type
{
	typedef typename ::mst::_Details::_Mem_func_argument_type<typename ::std::remove_pointer<T>::type, index, 0>::type type;
};

template<typename T>
struct return_type
{
	typedef T type;
};

template<typename T>
struct return_type<T*>
{
	typedef T type;
};

} // namespace mst