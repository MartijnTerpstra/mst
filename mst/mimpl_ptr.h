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
#include <mx_impl_ptr.h>

namespace mst {

template<typename _Ty>
class impl_ptr
{
	impl_ptr(const impl_ptr&) = delete;
	impl_ptr& operator=(const impl_ptr&) = delete;

public:
	inline impl_ptr() noexcept
		: _Myptr((_Ty*)0)
	{ }

	inline impl_ptr(impl_ptr&& _other) noexcept
		: _Myptr(_other._Myptr)
	{
		_other._Myptr = (_Ty*)0;
	}

	inline ~impl_ptr() noexcept
	{
		_Tidy();
	}

	inline void reset() noexcept
	{
		_Tidy();
	}

	inline _Ty* get() const noexcept
	{
		return _Myptr;
	}

	inline _Ty* operator->() const noexcept
	{
		return _Myptr;
	}

	inline _Ty& operator*() const noexcept
	{
		return *_Myptr;
	}

	inline impl_ptr& operator=(impl_ptr&& _other) noexcept
	{
		_Release();
		_Myptr = _other._Myptr;
		_other._Myptr = (_Ty*)0;
		return *this;
	}

	template<typename _Ty, typename... _Args>
	inline friend impl_ptr<_Ty> make_impl_ptr(_Args... _args);

private:
	inline impl_ptr(_Ty* _ptr) noexcept
		: _Myptr(_ptr)
	{ }

	typedef void (*_DxFun)(_Ty*);

	inline void _Tidy() noexcept
	{
		_Release();
		_Myptr = (_Ty*)0;
	}

	inline void _Release() noexcept
	{
		if(_Myptr)
		{
			_DxFun* func = reinterpret_cast<_DxFun*>(_Myptr) - 1;
			(*func)(_Myptr);
		}
	}

private:
	_Ty* _Myptr;
};

template<typename _Ty, typename... _Args>
inline impl_ptr<_Ty> make_impl_ptr(_Args... _args)
{
	typedef void (*_DxFun)(_Ty*);

	byte* memory =
		reinterpret_cast<byte*>(malloc(sizeof(_Ty) + std::alignment_of<std::max_align_t>::value));

	_Ty* objectPtr = new(memory + std::alignment_of<std::max_align_t>::value)
		_Ty(::std::forward<_Args>(_args)...);

	_DxFun* func = reinterpret_cast<_DxFun*>(memory);

	*func = &::mst::_Details::_Impl_release<_Ty>;

	return impl_ptr<_Ty>(objectPtr);
}

}