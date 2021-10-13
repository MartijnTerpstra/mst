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
#include <mcommon.h>

namespace mst {

namespace _Details {

template<typename _Fun>
class _scope_guard
{
public:
	inline _scope_guard(_Fun _Func)
		: _MyFunc(std::move(_Func))
		, _Dismissed(false)
	{ }

	inline ~_scope_guard()
	{
		if(!_Dismissed)
		{
			_MyFunc();
		}
	}

	inline bool is_dismissed()
	{
		return _Dismissed;
	}

	inline void dismiss()
	{
		_Dismissed = true;
	}

private:
	_Fun _MyFunc;
	bool _Dismissed;

}; // class _scope_guard


class _on_exit_scope_guard
{
public:
	template<typename _Fun>
	inline friend _scope_guard<_Fun> operator+(_on_exit_scope_guard left, _Fun _Func)
	{
		return _scope_guard<_Fun>(std::move(_Func));
	}

}; // class _on_exit_scope_guard

} // namespace _Details

template<typename _Fun>
::mst::_Details::_scope_guard<_Fun> scope_guard(_Fun _Func)
{
	return ::mst::_Details::_scope_guard<_Fun>(std::move(_Func));
}

#define mst_on_exit_scope                                                                          \
	auto _MST_ANONYMOUS_VAR(__ONEXITSTATE) = ::mst::_Details::_on_exit_scope_guard() + [&]()

} // namespace mst
