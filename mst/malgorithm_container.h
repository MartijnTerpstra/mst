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

#include <mcore.h>
#include <malgorithm.h>

namespace mst {

template<typename _Container>
void sort(_Container& _Cont)
{
	::std::sort(::std::begin(_Cont), ::std::end(_Cont));
}

template<typename _Container, typename _Predicate>
void sort(_Container& _Cont, _Predicate _Pred)
{
	::std::sort(::std::begin(_Cont), ::std::end(_Cont), ::std::move(_Pred));
}

template<typename _Container, typename _Type>
auto find(_Container& _Cont, const _Type& _Ty) -> decltype(::std::begin(_Cont))
{
	return ::std::find(::std::begin(_Cont), ::std::end(_Cont), _Ty);
}

template<typename _Container, typename _Predicate>
auto find_if(_Container& _Cont, _Predicate _Pred) -> decltype(::std::begin(_Cont))
{
	return ::std::find_if(::std::begin(_Cont), ::std::end(_Cont), ::std::move(_Pred));
}

template<typename _Container, typename _Type>
bool contains(_Container& _Cont, const _Type& _Ty)
{
	return ::std::find(::std::begin(_Cont), ::std::end(_Cont), _Ty) != ::std::end(_Cont);
}

} // namespace mst