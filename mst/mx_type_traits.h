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

namespace std {

template<typename _Ty>
class shared_ptr;

template<typename _Ty>
class weak_ptr;

} // namespace std

namespace mst {

template<bool value, typename _If_true, typename _If_false>
struct if_else
{ };

template<typename _If_true, typename _If_false>
struct if_else<true, _If_true, _If_false>
{
	typedef _If_true type;
};

template<typename _If_true, typename _If_false>
struct if_else<false, _If_true, _If_false>
{
	typedef _If_false type;
};

namespace _Details {

template<typename T>
struct _To_pointer_to_const
{
	T type;
};


#define _MST_ADD_CV_TO_POINTER_TO_CONST(_Template_override, _Const_version)                        \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Template_override, _Const_version)                       \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Template_override const, _Const_version const)           \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Template_override volatile, _Const_version volatile)     \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(                                                          \
		_Template_override const volatile, _Const_version const volatile)                          \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Const_version, _Const_version)                           \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Const_version const, _Const_version const)               \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Const_version volatile, _Const_version volatile)         \
	_MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(                                                          \
		_Const_version const volatile, _Const_version const volatile)

#define _MST_ADD_CV_TO_POINTER_TO_CONST_IMPL(_Template_override, _Const_version)                   \
	template<typename T>                                                                           \
	struct _To_pointer_to_const<_Template_override>                                                \
	{                                                                                              \
		_MST_VC_TYPENAME typedef _Const_version type;                                              \
	};



_MST_ADD_CV_TO_POINTER_TO_CONST(T*, const T*)

_MST_ADD_CV_TO_POINTER_TO_CONST(::std::shared_ptr<T>, ::std::shared_ptr<const T>)
_MST_ADD_CV_TO_POINTER_TO_CONST(::std::weak_ptr<T>, ::std::weak_ptr<const T>)

} // namespace _Details


template<typename _Ty>
struct to_pointer_to_const : public ::mst::_Details::_To_pointer_to_const<_Ty>
{ };

#if _MST_HAS_TEMPLATE_ALIASES

template<typename _Ty>
using to_pointer_to_const_t = typename ::mst::to_pointer_to_const<_Ty>::type;

#endif // _MST_HAS_TEMPLATE_ALIASES

#endif

} // namespace mst