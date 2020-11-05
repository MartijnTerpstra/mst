//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mcore.h>
#include <utility>
#include <iostream>
#include <mlogger.h>
#include <mx_debug.h>

#define MST_BREAKPOINT _MST_BREAK

#if MST_FUNCTION_TRACING // for function tracing

#define ENTER_FUNC ::std::logger::info("entering: " __FUNCTION__ "()");
#define LEAVE_FUNC ::std::logger::info("leaving : " __FUNCTION__ "()");

#else

#define ENTER_FUNC
#define LEAVE_FUNC

#endif

#define _TO_ERROR_MESG(_Mesg) (::std::string(__FUNCTION__) + "() " + _Mesg)

#if MST_DEBUGMODE || defined(MST_DEBUG_CHECKS) // for assertions

#define ERROR_MESG(_Mesg)                                                                          \
	{                                                                                              \
		::mst::logger::error(_TO_ERROR_MESG(_Mesg));                                               \
		_MST_BREAK;                                                                                \
	}
#define ERROR_MESG_ARG(_Mesg, arg)                                                                 \
	{                                                                                              \
		::mst::logger::error(_TO_ERROR_MESG(_Mesg), arg);                                          \
		_MST_BREAK;                                                                                \
	}

#define CHECK_IF(_Value, _Mesg)                                                                    \
	if(_Value)                                                                                     \
	{                                                                                              \
		ERROR_MESG(_Mesg);                                                                         \
	}
#define CHECK_IF_ARG(_Value, _Mesg, _Arg)                                                          \
	if(_Value)                                                                                     \
	{                                                                                              \
		ERROR_MESG_ARG(_Mesg, _Arg)                                                                \
	}
#define CHECK_IFNOT(_Value, _Mesg)			 CHECK_IF(!(_Value), _Mesg)
#define CHECK_IFNOT_ARG(_Value, _Mesg, _Arg) CHECK_IF_ARG(!(_Value), _Mesg, _Arg)

#define CHECK_ITER_RANGE(_Value, _Begin, _End, _Mesg)                                              \
	if((_Value) < (_Begin) || (_Value) > (_End))                                                   \
	{                                                                                              \
		ERROR_MESG(_Mesg);                                                                         \
	}
#define CHECK_ARR_RANGE(_Value, _Size, _Mesg)                                                      \
	if((_Value) < 0 || (_Value) >= (_Size))                                                        \
	{                                                                                              \
		ERROR_MESG_ARG(_Mesg, _Value);                                                             \
	}
#define CHECK_VALUE_RANGE(_Value, _Begin, _End, _Mesg)                                             \
	if((_Value) < (_Begin) || (_Value) > (_End))                                                   \
	{                                                                                              \
		ERROR_MESG(_Mesg);                                                                         \
	}

// just use normal debugging
#define CHECK_IF_GPU(_Value, _Mesg)				 CHECK_IF(_Value, _Mesg)
#define CHECK_IF_ARG_GPU(_Value, _Mesg, _Arg)	 CHECK_IF_ARG(_Value, _Mesg, _Arg)
#define CHECK_IFNOT_GPU(_Value, _Mesg)			 CHECK_IFNOT(_Value, _Mesg)
#define CHECK_IFNOT_ARG_GPU(_Value, _Mesg, _Arg) CHECK_IFNOT_ARG(_Value, _Mesg, _Arg)

#define CHECK_ITER_RANGE_GPU(_Value, _Begin, _End, _Mesg)                                          \
	CHECK_ITER_RANGE(_Value, _Begin, _End, _Mesg)
#define CHECK_ARR_RANGE_GPU(_Value, _Size, _Mesg) CHECK_ARR_RANGE(_Value, _Size, _Mesg)
#define CHECK_VALUE_RANGE_GPU(_Value, _Begin, _End, _Mesg)                                         \
	CHECK_VALUE_RANGE(_Value, _Begin, _End, _Mesg);

// warnings
#if MST_BREAK_ON_WARNINGS

#define WARNING_MESG(_Mesg)                                                                        \
	{                                                                                              \
		::std::logger::warning(_TO_ERROR_MESG(_Mesg));                                             \
		_MST_BREAK;                                                                                \
	}
#define WARNING_MESG_ARG(_Mesg, _Arg)                                                              \
	{                                                                                              \
		::std::logger::warning(_TO_ERROR_MESG(_Mesg), _Arg);                                       \
		_MST_BREAK;                                                                                \
	}

#else

#define WARNING_MESG(_Mesg)			  ::std::logger::warning(_TO_ERROR_MESG(_Mesg))
#define WARNING_MESG_ARG(_Mesg, _Arg) ::std::logger::warning(_TO_ERROR_MESG(_Mesg), _Arg)

#endif

#define WARN_IF(_Value, _Mesg)                                                                     \
	if(_Value)                                                                                     \
	{                                                                                              \
		ERROR_MESG(_Mesg);                                                                         \
	}
#define WARN_IF_ARG(_Value, _Mesg, _Arg)                                                           \
	if(_Value)                                                                                     \
	{                                                                                              \
		ERROR_MESG_ARG(_Mesg, _Arg)                                                                \
	}
#define WARN_IFNOT(_Value, _Mesg)			WARN_IF(!(_Value), _Mesg)
#define WARN_IFNOT_ARG(_Value, _Mesg, _Arg) WARN_IF_ARG(!(_Value), _Mesg, _Arg)

#if _HAS_AMP
#define WARN_IF_GPU(_Value, _Mesg)				_Mst_warn_if_gpu(_Value)
#define WARN_IF_ARG_GPU(_Value, _Mesg, _Arg)	_Mst_warn_if_arg_gpu(_Value, _Arg)
#define WARN_IFNOT_GPU(_Value, _Mesg)			WARN_IF_GPU(!(_Value), _Mesg)
#define WARN_IFNOT_ARG_GPU(_Value, _Mesg, _Arg) WARN_IF_ARG_GPU(!(_Value), _Mesg, _Arg)
#else /* !_HAS_AMP */
#define WARN_IF_GPU(_Value, _Mesg)				WARN_IF(_Value, _Mesg)
#define WARN_IF_ARG_GPU(_Value, _Mesg, _Arg)	WARN_IF_ARG(_Value, _Mesg, _Arg)
#define WARN_IFNOT_GPU(_Value, _Mesg)			WARN_IFNOT(_Value, _Mesg)
#define WARN_IFNOT_ARG_GPU(_Value, _Mesg, _Arg) WARN_IFNOT_ARG(_Value, _Mesg, _Arg)
#endif /* _HAS_AMP */

#else /* !MST_DEBUGMODE && !DEBUG_CHECKS */

#define ERROR_MESG(_Mesg)
#define ERROR_MESG_ARG(_Mesg, _Arg)
#define CHECK_IF(_Value, _Mesg)
#define CHECK_IFNOT(_Value, _Mesg)
#define CHECK_ITER_RANGE(_Value, _Begin, _End, _Mesg)
#define CHECK_ARR_RANGE(_Value, _Size, _Mesg)
#define CHECK_IF_ARG(_Value, _Mesg, _Arg)
#define CHECK_IFNOT_ARG(_Value, _Mesg, _Arg)
#define CHECK_VALUE_RANGE(_Value, _Begin, _End, _Mesg)

#define CHECK_IF_GPU(_Value, _Mesg)
#define CHECK_IF_ARG_GPU(_Value, _Mesg, _Arg)
#define CHECK_IFNOT_GPU(_Value, _Mesg)
#define CHECK_IFNOT_ARG_GPU(_Value, _Mesg, _Arg)

#define CHECK_ITER_RANGE_GPU(_Value, _Begin, _End, _Mesg)
#define CHECK_ARR_RANGE_GPU(_Value, _Size, _Mesg)
#define CHECK_VALUE_RANGE_GPU(_Value, _Begin, _End, _Mesg)

#define WARNING_MESG(_Mesg)
#define WARNING_MESG_ARG(_Mesg, _Arg)
#define WARN_IF(_Value, _Mesg)
#define WARN_IF_ARG(_Value, _Mesg, _Arg)
#define WARN_IFNOT(_Value, _Mesg)
#define WARN_IFNOT_ARG(_Value, _Mesg, _Arg)

#define WARN_IF_GPU(_Value, _Mesg)
#define WARN_IF_ARG_GPU(_Value, _Mesg, _Arg)
#define WARN_IFNOT_GPU(_Value, _Mesg)
#define WARN_IFNOT_ARG_GPU(_Value, _Mesg, _Arg)

#endif /* MST_DEBUGMODE */


// Always working assertions

#define ALWAYS_ERROR_MESG(_Mesg)                                                                   \
	{                                                                                              \
		::mst::logger::error(__FUNCTION__ "() " _Mesg);                                            \
		_MST_BREAK;                                                                                \
	}
#define ALWAYS_ERROR_MESG_ARG(_Mesg, _Arg)                                                         \
	{                                                                                              \
		::mst::logger::error(__FUNCTION__ "() " _Mesg, _Arg);                                      \
		_MST_BREAK;                                                                                \
	}

#define ALWAYS_CHECK_IF(_Value, _Mesg)                                                             \
	if(_Value)                                                                                     \
	{                                                                                              \
		ALWAYS_ERROR_MESG(_Mesg);                                                                  \
	}
#define ALWAYS_CHECK_IF_ARG(_Value, _Mesg, _Arg)                                                   \
	if(_Value)                                                                                     \
	{                                                                                              \
		ALWAYS_ERROR_MESG_ARG(_Mesg, _Arg);                                                        \
	}
#define ALWAYS_CHECK_IFNOT(_Value, _Mesg)			ALWAYS_CHECK_IF(!(_Value), _Mesg)
#define ALWAYS_CHECK_IFNOT_ARG(_Value, _Mesg, _Arg) ALWAYS_CHECK_IF_ARG(!(_Value), _Mesg, _Arg)
#define ALWAYS_CHECK_ITER_RANGE(_Value, _Begin, _End, _Mesg)                                       \
	if((_Value) < (_Begin) || (_Value) > (_End))                                                   \
	{                                                                                              \
		ALWAYS_ERROR_MESG(_Mesg);                                                                  \
	}
#define ALWAYS_CHECK_ARR_RANGE(_Value, _Size, _Mesg)                                               \
	if((_Value) < 0 || (_Value) >= (_Size))                                                        \
	{                                                                                              \
		ALWAYS_ERROR_MESG(_Mesg);                                                                  \
	}
#define ALWAYS_CHECK_VALUE_RANGE(_Value, _Begin, _End, _Mesg)                                      \
	if((_Value) < (_Begin) || (_Value) > (_End))                                                   \
	{                                                                                              \
		ALWAYS_ERROR_MESG(_Mesg);                                                                  \
	}

// warnings
#if MST_BREAK_ON_WARNINGS

#define ALWAYS_WARNING_MESG(_Mesg)                                                                 \
	{                                                                                              \
		::mst::logger::error(_TO_ERROR_MESG(_Mesg));                                               \
		_MST_BREAK;                                                                                \
	}
#define ALWAYS_WARNING_MESG_ARG(_Mesg, _Arg)                                                       \
	{                                                                                              \
		::mst::logger::error(_TO_ERROR_MESG(_Mesg), _Arg);                                         \
		_MST_BREAK;                                                                                \
	}

#else

#define ALWAYS_WARNING_MESG(_Mesg)			 ::mst::logger::error(_TO_ERROR_MESG(_Mesg))
#define ALWAYS_WARNING_MESG_ARG(_Mesg, _Arg) ::mst::logger::error(_TO_ERROR_MESG(_Mesg), _Arg)

#endif

#define ALWAYS_WARN_IF(_Value, _Mesg)                                                              \
	if(_Value)                                                                                     \
	{                                                                                              \
		ALWAYS_WARNING_MESG(_Mesg);                                                                \
	}
#define ALWAYS_WARN_IF_ARG(_Value, _Mesg, _Arg)                                                    \
	if(_Value)                                                                                     \
	{                                                                                              \
		ALWAYS_WARNING_MESG_ARG(_Mesg, _Arg);                                                      \
	}
#define ALWAYS_WARN_IFNOT(_Value, _Mesg)		   ALWAYS_WARN_IF(!(_Value), _Mesg)
#define ALWAYS_WARN_IFNOT_ARG(_Value, _Mesg, _Arg) ALWAYS_WARN_IF_ARG(!(_Value), _Mesg, _Arg)