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
#include <mdebug.h>
#include <matomics.h>
#include <mx_packed_pair.h>

namespace mst {

typedef uint32_t refcount_t;

template<typename _Ty>
class smart_ptr;

template<typename _Ty>
class back_ptr;

namespace _Details {

class _Refblock_base
{ // base reference block object
public:
	_Refblock_base()
		: _MyRefs(1)
		, _MyBackRefs(0)
	{ }

protected:
	inline void _Inc()
	{
		atomic::increment(_MyRefs);
		//++_MyRefs;
	}

	inline bool _Lock()
	{
		for(;;)
		{
			volatile refcount_t refCount = static_cast<volatile refcount_t&>(_MyRefs);

			if(refCount == 0)
			{
				return false;
			}

			if(atomic::compare_and_swap(_MyRefs, refCount, refCount + 1) == refCount)
			{
				return true;
			}
		}
	}

	inline void _Dec()
	{
		CHECK_IF(_MyRefs == 0, "reference count == 0, are you trying to erase the same smart_ptr "
							   "in the destructor of the object?");

		if(atomic::decrement(_MyRefs) == 0)
		{
			_Dealloc();

			if(_MyBackRefs == 0)
			{
				_Delete_this();
			}
		}
	}

	inline void _Inc_back()
	{
		atomic::increment(_MyBackRefs);
	}

	inline void _Dec_back()
	{
		CHECK_IF(_MyBackRefs == 0, "reference count == 0, are you trying to erase the same "
								   "smart_ptr in the destructor of the object?");

		if(atomic::decrement(_MyBackRefs) == 0)
		{
			if(_MyRefs == 0)
			{
				_Delete_this();
			}
		}
	}

	inline refcount_t _Use_count() const
	{
		return _MyRefs;
	}

	inline refcount_t _Back_count() const
	{
		return _MyBackRefs;
	}

	virtual bool _Is_valid()
	{
		return true;
	}

	virtual void _Dealloc() = 0;
	virtual void _Delete_this() = 0;

	refcount_t _MyRefs;
	refcount_t _MyBackRefs;

	template<typename _Ty, bool _Is_void>
	friend class _Smart_ptr_base;

	template<typename _Ty>
	friend class smart_ptr;

	template<typename _Ty>
	friend class back_ptr;

	template<typename _Ty>
	void _Add_this_ptr_impl(_Ty* _Ptr, ::std::true_type)
	{
		::mst::enable_smart_this_ptr* tptr = static_cast<::mst::enable_smart_this_ptr*>(_Ptr);
		tptr->_Block = this;
	}

	template<typename _Ty>
	void _Add_this_ptr_impl(_Ty*, ::std::false_type)
	{
		/* do nothing*/
	}

	template<typename _Ty>
	void _Add_this_ptr(_Ty* _Ptr)
	{
		_Add_this_ptr_impl(_Ptr, typename ::std::is_base_of<enable_smart_this_ptr, _Ty>::type());
	}
};

class _Refblock_static : public _Refblock_base
{ // non dieing reference block for nullptr and uninitialized ptrs
	_Refblock_static()
	{
		_MyRefs = 0xFFFFFF;
		_MyBackRefs = 0xFFFFFF;
	}

	bool _Is_valid() final override
	{
		return false;
	}

	void _Dealloc() final override
	{
		// keep it alive
		_MyRefs = 0xFFFFFF;
		_MyBackRefs = 0xFFFFFF;
	}

	void _Delete_this() final override
	{
		/* do nothing */
	}

	inline static _Refblock_static* _Get_null_block()
	{
		static _Refblock_static _Block;
		return &_Block;
	}

	template<typename _Ty, bool _Is_void>
	friend class _Smart_ptr_base;
	template<typename _Ty>
	friend class smart_ptr;
	template<typename _Ty>
	friend class back_ptr;
};

template<typename _Ty>
class _Refblock_ptr : public _Refblock_base
{
public:
	_Refblock_ptr(_Ty* _Ptr)
		: _MyPtr(_Ptr)
	{
		_Add_this_ptr(_Ptr);
	}

protected:
	void _Delete_this() final override
	{
		delete this;
	}

	virtual void _Dealloc() override
	{
		delete _MyPtr;
	}

	_Ty* const _MyPtr;
};

template<typename _Ty, typename _Dx>
class _Refblock_ptr_del : public _Refblock_base
{
public:
	_Refblock_ptr_del(_Ty* _Ptr, _Dx _Custom_deleter)
		: _MyPtr(_Ptr)
		, _Deleter(std::move(_Custom_deleter))
	{
		_Add_this_ptr(_Ptr);
	}

private:
	void _Delete_this() final override
	{
		delete this;
	}

	void _Dealloc() override
	{
		_Deleter(_MyPtr);
	}

	_Ty* const _MyPtr;
	_Dx _Deleter;
};

template<typename _Ty, typename _Dx>
class _Refblock_object : public _Refblock_base
{
public:
	template<typename _Fn>
	_Refblock_object(_Fn _Func, _Dx _Deleter)
		: _Mypair(::std::move(_Deleter))
	{
		_Func(_Get_ptr());

		_Add_this_ptr(_Get_ptr());
	}

	inline _Ty* _Get_ptr()
	{
		return (_Ty*)(&_Mypair._Get_second());
	}

	void _Delete_this() final override
	{
		delete this;
	}

	virtual void _Dealloc() final override
	{
		_Mypair._Get_first()(_Get_ptr());
	}

public:
	/*
	inline void _Check_alignment()
	{
		CHECK_IF((((size_t)this->_Get_ptr()) & (__alignof(_Ty) - 1)) != 0, "alignment failed, object
	is not aligned");
	}
	*/

	/*
	union {
	typename ::std::aligned_storage<sizeof(_Ty), __alignof(_Ty)>::type _MyStorage;
	struct { _Dx _MyDeleter; };
	};
	*/

	_Packed_pair<_Dx, typename ::std::aligned_storage<sizeof(_Ty), __alignof(_Ty)>::type> _Mypair;
};

template<typename _Ty>
class _Refblock_array : public _Refblock_base
{ // reference block with an array attached to it
public:
	_Refblock_array(size_t _Count)
		: _MyObjects(new _Ty[_Count])
	{ }

	inline _Ty* _Get_ptr()
	{
		return _MyObjects;
	}

protected:
	void _Delete_this() final override
	{
		delete this;
	}

	void _Dealloc() override
	{
		delete[] _MyObjects;
	}

	_Ty* const _MyObjects;
};

class _Refblock_alloc : public _Refblock_base
{ // reference block with allocated memory attached to it
public:
	_Refblock_alloc(size_t _Size)
		: _MyMem(malloc(_Size))
	{ }

	void* _Get_ptr()
	{
		return _MyMem;
	}

private:
	void _Delete_this() final override
	{
		delete this;
	}

	void _Dealloc() override
	{
		free(_Get_ptr());
	}

	void* const _MyMem;
};

class _Refblock_alloc_align : public _Refblock_base
{ // reference block with allocated memory attached to it
public:
	_Refblock_alloc_align(size_t _Size, size_t _Alignment)
		: _MyMem(_aligned_malloc(_Size, _Alignment))
	{ }

	void* _Get_ptr()
	{
		return _MyMem;
	}

private:
	void _Delete_this() final override
	{
		delete this;
	}

	void _Dealloc() override
	{
		_aligned_free(_Get_ptr());
	}

	void* const _MyMem;
};

_MST_ADD_CV_TO_POINTER_TO_CONST(::mst::smart_ptr<T>, ::mst::smart_ptr<const T>)
_MST_ADD_CV_TO_POINTER_TO_CONST(::mst::back_ptr<T>, ::mst::back_ptr<const T>)

} // namespace _Details
} // namespace mst