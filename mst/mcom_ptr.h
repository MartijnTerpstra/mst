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

#ifndef MCOM_PTR_H
#define MCOM_PTR_H

#include <mcore.h>
#include <mdebug.h>
#include <Unknwn.h>
#include <mx_com_ptr.h>

namespace mst {

template<typename T>
class com_ptr
{

public:
	typedef T element_type;

public:
	explicit com_ptr() noexcept
		: _MyPtr((T*)0)
	{ }

	explicit com_ptr(T* _Ptr) noexcept
	{
		this->_MyPtr = _Ptr;
		// do not add reference
	}

	com_ptr(const com_ptr& _Other) noexcept
	{
		this->_MyPtr = _Other._MyPtr;
		_Add_ref();
	}

	com_ptr(::std::nullptr_t) noexcept
		: _MyPtr((T*)0)
	{ }

	template<typename T2>
	com_ptr(const com_ptr<T2>& _Other, T* _Dummy = typename ::mst::pointer<T2>::type()) noexcept
	{
		_MST_UNUSED(_Dummy);
		this->_MyPtr = _Other._MyPtr;
		_Add_ref();
	}

	// no reinterpret cast, not supported for com ptrs

	inline com_ptr& operator=(const com_ptr& _Right) noexcept
	{
		com_ptr(_Right).swap(*this);
		return (*this);
	}

	template<class T2>
	inline com_ptr& operator=(const com_ptr<T2>& _Right) noexcept
	{
		com_ptr(_Right).swap(*this);
		return (*this);
	}

	inline com_ptr& operator=(::std::nullptr_t) noexcept
	{
		_Release();
		_MyPtr = (T*)0;
		return (*this);
	}

	inline bool operator==(const com_ptr& _Other) const noexcept
	{
		return get() == _Other.get();
	}

	inline bool operator!=(const com_ptr& _Other) const noexcept
	{
		return !(*this == _Other);
	}

	inline bool operator==(::std::nullptr_t) const noexcept
	{
		return this->get() == (void*)0;
	}

	inline bool operator!=(::std::nullptr_t) const noexcept
	{
		return this->get() != (void*)0;
	}

	inline explicit operator bool() const noexcept
	{
		return (_MyPtr != 0);
	}

	inline ULONG reset() noexcept
	{
		T* _Released = release();
		if(_Released)
		{
			return _Released->Release();
		}
		return (ULONG)-1;
	}

	inline T* release() noexcept
	{
		T* retval = this->_MyPtr;
		this->_MyPtr = (T*)0;
		return retval;
	}

	void swap(com_ptr& _Right) noexcept
	{
		::std::swap(this->_MyPtr, _Right._MyPtr);
	}

	template<typename T2>
	com_ptr<T2> as() const
	{
		if(_MyPtr == nullptr)
		{
			ERROR_MESG("com_ptr<T>::as(): invalid call: pointer does not point to a valid object");
		}

		static_assert(::std::is_base_of<IUnknown, T2>::value, "T2 must inherit from IUnknown");
		return _As<T2>(::std::is_convertible<T, T2>::type());
	}

	template<typename T2>
	bool is() const
	{
		if(_MyPtr == nullptr)
		{
			ERROR_MESG("com_ptr<T>::is(): invalid call: pointer does not point to a valid object");
		}

		com_ptr<T2> _Right;
		return ::std::is_base_of<T2, T>::value || ::std::is_same<T2, T>::value ||
			   SUCCEEDED(_MyPtr->QueryInterface<T2>(::mst::initialize(_Right)));
	}

	~com_ptr() noexcept
	{
		_Release();
	}

	inline T* const* operator&() const noexcept
	{
		return &this->_MyPtr;
	}

	inline T* get() const noexcept
	{
		return this->_MyPtr;
	}

	inline T& operator*() const noexcept
	{
		return *this->get();
	}

	inline T* operator->() const noexcept
	{
		CHECK_IF(this->get() == nullptr, "cannot dereference a nullptr-pointer");
		return this->get();
	}

	com_ptr(com_ptr&& _Other) noexcept
	{
		this->_MyPtr = _Other._MyPtr;
		_Other._MyPtr = (T*)0;
	}

	template<typename T2>
	com_ptr(com_ptr<T2>&& _Other, T* _Dummy = typename ::mst::pointer<T2>::type()) noexcept
	{
		_MST_UNUSED(_Dummy);
		this->_MyPtr = _Other._MyPtr;
		_Other._MyPtr = (T2*)0;
	}

private:
	template<typename T2>
	com_ptr<T2> _As(::std::true_type) const noexcept
	{
		return *this;
	}

	template<typename T2>
	com_ptr<T2> _As(::std::false_type) const
	{

		com_ptr<T2> _Right;
		if(FAILED(_MyPtr->QueryInterface<T2>(::mst::initialize(_Right))))
		{
			return nullptr;
		}
		return _Right;
	}

	inline void _Add_ref()
	{
		if(this->_MyPtr)
		{
			_MyPtr->AddRef();
		}
	}

	inline void _Release()
	{
		if(this->_MyPtr)
		{
			_MyPtr->Release();
		}
	}


	T* _MyPtr;

	template<typename T2>
	friend class com_ptr;

}; // class com_ptr

template<typename T>
com_ptr<T> make_com_ptr(T* _Ptr) noexcept
{
	return com_ptr<T>(_Ptr);
}

template<typename T>
::mst::_Details::_Com_ptr_combiner<T> MST_DEPRECATED(mst::initialize(com_ptr<T>))
	com_ptr_constructor(::mst::com_ptr<T>& comObject)
{
	return ::mst::_Details::_Com_ptr_combiner<T>(comObject);
}

template<typename T>
::mst::_Details::_Com_ptr_combiner<T> initialize(::mst::com_ptr<T>& comObject)
{
	return ::mst::_Details::_Com_ptr_combiner<T>(comObject);
}

#define MST_IID_PPV_ARGS(comPtr)                                                                   \
	__uuidof(::std::remove_reference<decltype(comPtr)>::type::element_type),                       \
		::mst::_Details::_Initialize_ppv_args(comPtr)

}; // namespace mst

#endif /* MCOM_PTR_H */