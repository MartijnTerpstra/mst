//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
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
#include <iterator>
#include <mdebug.h>
#include <mranges.h>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

namespace mst {

class stridemap;

template<typename T>
class stridemap_iterator
{
public:
	typedef ::std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef ptrdiff_t difference_type;

	typedef stridemap_iterator& _ThisRef;
	typedef stridemap_iterator _ThisCpy;

	friend class ::mst::stridemap;

	stridemap_iterator(char* _Ptr, size_t _Stride)
		: _MyPtr(_Ptr)
		, _MyStride(_Stride)
	{
		MST_ASSERT(sizeof(T) <= _Stride, "invalid size!");
	}

	inline pointer operator->()
	{
		return (pointer)_MyPtr;
	}

	inline reference operator*() const
	{
		return *(pointer)_MyPtr;
	}

	inline _ThisRef operator++()
	{
		_MyPtr += _MyStride;
		return *this;
	}

	inline _ThisCpy operator++(int)
	{
		_ThisCpy retval(*this);
		_MyPtr += _MyStride;
		return retval;
	}

	inline _ThisRef operator--()
	{
		_MyPtr -= _MyStride;
		return *this;
	}

	inline _ThisCpy operator--(int)
	{
		_ThisCpy retval(*this);
		_MyPtr -= _MyStride;
		return retval;
	}

	inline _ThisRef operator+=(difference_type _Count)
	{
		_MyPtr += _Count * (difference_type)_MyStride;
		return *this;
	}

	inline _ThisRef operator-=(difference_type _Count)
	{
		_MyPtr -= _Count * (difference_type)_MyStride;
		return *this;
	}

	inline _ThisCpy operator+(difference_type _Count) const
	{
		return _ThisCpy(*this) += _Count;
	}

	inline _ThisCpy operator-(difference_type _Count) const
	{
		return _ThisCpy(*this) -= _Count;
	}

	inline difference_type operator-(const stridemap_iterator& _Other) const
	{
		return (_MyPtr - _Other._MyPtr) / (difference_type)_MyStride;
	}

	inline bool operator==(const stridemap_iterator& _Other) const
	{
		return _MyPtr == _Other._MyPtr;
	}

	inline bool operator!=(const stridemap_iterator& _Other) const
	{
		return _MyPtr != _Other._MyPtr;
	}

	inline bool operator<(const stridemap_iterator& _Other) const
	{
		return _MyPtr < _Other._MyPtr;
	}

	inline bool operator<=(const stridemap_iterator& _Other) const
	{
		return _MyPtr <= _Other._MyPtr;
	}

	inline bool operator>(const stridemap_iterator& _Other) const
	{
		return _MyPtr > _Other._MyPtr;
	}

	inline bool operator>=(const stridemap_iterator& _Other) const
	{
		return _MyPtr >= _Other._MyPtr;
	}

private:
	char* _MyPtr;
	const size_t _MyStride;
};

class stridemap
{
public:
	explicit stridemap(size_t _Stride)
		: _MyBegin((char*)malloc(_Stride))
		, _MyEnd(_MyBegin)
		, _MyLast(_MyBegin + _Stride)
		, _MyStride(_Stride)
	{ }
	explicit stridemap(size_t _Stride, size_t _InitSize)
		: _MyBegin((char*)malloc(_InitSize * _Stride))
		, _MyEnd(_MyBegin + (_InitSize * _Stride))
		, _MyLast(_MyEnd)
		, _MyStride(_Stride)
	{ }
	stridemap(const stridemap& _OtherMap)
		: _MyBegin((char*)malloc(_OtherMap.size() * _OtherMap._MyStride))
		, _MyEnd(_MyBegin + (_OtherMap.size() * _OtherMap._MyStride))
		, _MyLast(_MyBegin + (_OtherMap.size() * _OtherMap._MyStride))
		, _MyStride(_OtherMap._MyStride)
	{
#if MST_STRIDEMAP_COPY_WARNING
		WARNING_MESG("copying std::stridemap. this is slow, try moving it");
#endif
		_copydata(_OtherMap);
	}

	template<typename T>
	stridemap(::std::initializer_list<T> initList)
		: _MyBegin((char*)malloc(initList.size() * sizeof(T)))
		, _MyEnd(_MyBegin + (initList.size() * sizeof(T)))
		, _MyLast(_MyBegin + (initList.size() * sizeof(T)))
		, _MyStride(sizeof(T))
	{
		_copydata(initList);
	}

	// rvalue reference constructor
	stridemap(stridemap&& _OtherMap)
		: _MyBegin(_OtherMap._MyBegin)
		, _MyEnd(_OtherMap._MyEnd)
		, _MyLast(_OtherMap._MyLast)
		, _MyStride(_OtherMap._MyStride)
	{
		_OtherMap._MyBegin = _OtherMap._MyEnd = _OtherMap._MyLast = nullptr;
	}

	~stridemap()
	{
		_orphan();
	}

	template<typename T>
	inline void erase(stridemap_iterator<T> _It)
	{
		MST_ASSERT(_It._MyStride == _MyStride, "incompatable strides between iterator and stridemap");
		MST_ASSERT(_It._MyPtr >= _MyBegin && _It._MyPtr < _MyEnd, "erase iterator out of range");

		memmove(_It._MyPtr, _It._MyPtr + _MyStride, (_MyEnd - _It._MyPtr) - _MyStride);
		_MyEnd -= _MyStride;
		_checkcollapse();
	}

	template<typename T>
	inline void erase(stridemap_iterator<T> _ItFirst, stridemap_iterator<T> _ItEnd)
	{
		MST_ASSERT(_ItFirst._MyStride == _MyStride && _ItEnd._MyStride == _MyStride,
			"incompatable strides between iterators and stridemap");
		MST_ASSERT(
			_ItFirst._MyPtr >= _MyBegin && _ItFirst._MyPtr <= _MyEnd, "erase iterator out of range");
		MST_ASSERT(_ItEnd._MyPtr >= _MyBegin && _ItEnd._MyPtr <= _MyEnd, "erase iterator out of range");
		MST_ASSERT(_ItFirst._MyPtr <= _ItEnd._MyPtr, "erase range is invalid");

		memmove(_ItFirst._MyPtr, _ItEnd._MyPtr, _MyEnd - _ItEnd._MyPtr);
		_MyEnd -= _ItEnd._MyPtr - _ItFirst._MyPtr;
		_checkcollapse();
	}

	template<typename T>
	inline void push_back(const T& _Value)
	{
		MST_ASSERT(sizeof(T) == _MyStride, "size of T is not the same as the stride");
		static_assert(::std::is_pod<T>::value, "T is not pod");

		_checkexpansion();
		new(_MyEnd) T(_Value);

		_MyEnd += _MyStride;
	}

	inline void push_back()
	{
		_checkexpansion();
		memset(_MyEnd, 0, _MyStride);
		_push();
	}

	inline void push_back_from_index(size_t _Idx)
	{
		MST_ASSERT(_Idx < size(), "index out of range");

		_checkexpansion();

		memcpy(_MyEnd, _MyBegin + _Idx * _MyStride, _MyStride);

		_MyEnd += _MyStride;
	}

	template<typename T>
	inline size_t find(const T& _Value) const
	{
		for(auto it = begin<T>(); it != end<T>(); ++it)
			if(*it == _Value)
				return (size_t)(it - begin<T>());
		return (size_t)-1;
	};

	template<typename T, typename _Func>
	inline size_t find_if(_Func _Pred) const
	{
		for(auto it = begin<T>(); it != end<T>(); ++it)
			if(_Pred(*it))
				return (size_t)(it - begin<T>());
		return (size_t)-1;
	};

	inline void pop_back()
	{
		MST_ASSERT(!empty(), "cannot pop back when stridemap is empty");
		_MyEnd -= _MyStride;
		_checkcollapse();
	}

	inline void pop_front()
	{
		MST_ASSERT(!empty(), "cannot pop front when stridemap is empty");

		memmove(_MyBegin, _MyBegin + _MyStride, (_MyEnd - _MyBegin) - _MyStride);
		_MyEnd -= _MyStride;
		_checkcollapse();
	}

	template<typename T>
	inline const T& back() const
	{
		MST_ASSERT(!empty(), "the stridemap is empty");
		return *(const T*)(_MyEnd - _MyStride);
	}

	template<typename T>
	inline const T& front() const
	{
		MST_ASSERT(!empty(), "the stridemap is empty");
		return *(const T*)_MyBegin;
	}

	template<typename T>
	inline T& back()
	{
		MST_ASSERT(!empty(), "the stridemap is empty");
		return *(T*)(_MyEnd - _MyStride);
	}

	template<typename T>
	inline T& front()
	{
		MST_ASSERT(!empty(), "the stridemap is empty");
		return *(T*)_MyBegin;
	}

	inline void* data()
	{
		return _MyBegin;
	}

	inline const void* data() const
	{
		return _MyBegin;
	}

	inline size_t data_size() const
	{
		return size() * stride();
	}

	inline void set_stride(size_t _Stride)
	{
		MST_ASSERT(empty(), "cannot set stride when not empty");

		_MyStride = _Stride;

		_MyBegin = _realloc(1);
		_MyEnd = _MyBegin;
		_MyLast = _MyBegin + _Stride;
	}

	template<typename T>
	inline T& index(size_t _Idx)
	{
		MST_ASSERT(_Idx < size(), "index out of range");
		return *(T*)(_MyBegin + _Idx * _MyStride);
	}

	template<typename T>
	inline const T& index(size_t _Idx) const
	{
		MST_ASSERT(_Idx < size(), "index out of range");
		return *(const T*)(_MyBegin + _Idx * _MyStride);
	}

	inline size_t stride() const
	{
		return _MyStride;
	}

	template<typename T>
	inline stridemap_iterator<T> begin()
	{
		return stridemap_iterator<T>(_MyBegin, _MyStride);
	}

	template<typename T>
	inline stridemap_iterator<T> end()
	{
		return stridemap_iterator<T>(_MyEnd, _MyStride);
	}

	template<typename T>
	inline stridemap_iterator<const T> begin() const
	{
		return stridemap_iterator<const T>(_MyBegin, _MyStride);
	}

	template<typename T>
	inline stridemap_iterator<const T> end() const
	{
		return stridemap_iterator<const T>(_MyEnd, _MyStride);
	}

	template<typename T>
	inline stridemap_iterator<const T> cbegin() const
	{
		return stridemap_iterator<const T>(_MyBegin, _MyStride);
	}

	template<typename T>
	inline stridemap_iterator<const T> cend() const
	{
		return stridemap_iterator<const T>(_MyEnd, _MyStride);
	}

	inline void pre_allocate(size_t _NumObjects)
	{
		size_t _Size = size();
		size_t _AllocSize = _myallocsize();
		if(_AllocSize - _Size < _NumObjects)
		{
			_MyBegin = _realloc(_Size + _NumObjects);
			_MyEnd = _MyBegin + (_Size * _MyStride);
			_MyLast = _MyEnd + (_NumObjects * _MyStride);
		}
	}

	inline void clear()
	{
		_tidy();
	}

	inline void shrink_to_fit()
	{
		size_t _Oldsize = size();
		if(_Oldsize == 0)
		{
			_tidy();
			return;
		}
		_MyBegin = _realloc(_Oldsize);
		_MyLast = _MyEnd = _MyBegin + (_Oldsize * _MyStride);
	}

	inline void resize(size_t _NewSize)
	{
		if(_NewSize == 0)
		{
			_tidy();
			return;
		}
		_MyBegin = _realloc(_NewSize);
		_MyLast = _MyEnd = _MyBegin + (_NewSize * _MyStride);
	}

	inline size_t size() const
	{
		return (_MyEnd - _MyBegin) / _MyStride;
	}
	inline bool empty() const
	{
		return (_MyBegin == _MyEnd);
	}


	inline stridemap& operator+=(const stridemap& _OtherMap)
	{
		MST_ASSERT(_MyStride == _OtherMap._MyStride, "incompatible strides");

		size_t _Size = size();
		resize(_Size + _OtherMap.size());
		memcpy(_MyBegin + (_Size * _MyStride), _OtherMap._MyBegin, _OtherMap.size() * _MyStride);

		return *this;
	}

	inline stridemap& operator=(const stridemap& _OtherMap)
	{
#if MST_STRIDEMAP_COPY_WARNING
		WARNING_MESG("copying std::stridemap. this is slow, try moving it");
#endif
		MST_ASSERT(_OtherMap._MyStride == _MyStride,
			"incompatible strides, use set_stride() to make them match");

		_MyBegin = _realloc(_OtherMap._myallocsize());
		_MyEnd = _MyBegin + (_OtherMap.size() * _MyStride);
		_MyLast = _MyBegin + (_OtherMap._myallocsize() * _MyStride);

		_copydata(_OtherMap);

		return *this;
	}

	inline stridemap& operator=(stridemap&& _OtherMap)
	{
		MST_ASSERT(_OtherMap._MyStride == _MyStride, "incompatible strides");

		_orphan();

		_MyBegin = _OtherMap._MyBegin;
		_MyEnd = _OtherMap._MyEnd;
		_MyLast = _OtherMap._MyLast;

		_OtherMap._MyBegin = _OtherMap._MyEnd = _OtherMap._MyLast = nullptr;

		return *this;
	}

private:
	inline size_t _myallocsize() const
	{
		return (_MyLast - _MyBegin) / _MyStride;
	}

	inline void _release()
	{
		_MyBegin = _MyEnd = _MyLast = nullptr;
	}

	inline void _push()
	{
		_MyEnd += _MyStride;
	}

	inline void _checkexpansion()
	{
		size_t _Mysize = size();
		size_t _Myallocsize = _myallocsize();
		if(_Mysize >= _Myallocsize)
		{
			// grow geometrically; when the current capacity is 0 (e.g. right after construction
			// with an initial size of 0, or after clear()) doubling would stay stuck at 0
			size_t _Newallocsize = _Myallocsize == 0 ? 1 : _Myallocsize * 2;
			_MyBegin = _realloc(_Newallocsize);
			_MyEnd = _MyBegin + (_Mysize * _MyStride);
			_MyLast = _MyBegin + (_Newallocsize * _MyStride);
		}
	}

	inline void _checkcollapse()
	{
		size_t _Mysize = size();
		if(_Mysize == 0)
		{
			_tidy();
		}
		else if(_Mysize < _myallocsize() / 4)
		{
			_MyBegin = _realloc(_Mysize);
			_MyLast = _MyEnd = _MyBegin + (_Mysize * _MyStride);
		}
	}

	inline void _tidy()
	{
		_MyLast = _MyEnd = _MyBegin = _realloc(1);
		_MyLast += _MyStride;
	}

	inline void _orphan()
	{
		if(_MyBegin)
		{
			free(_MyBegin);
			_release();
		}
	}

	inline char* _realloc(size_t _Amount)
	{
		return (char*)realloc(_MyBegin, _Amount * _MyStride);
	}

	// is plain old data, copy memory
	inline void _copydata(const stridemap& _OtherMap)
	{
		memcpy(_MyBegin, _OtherMap._MyBegin, _OtherMap.size() * _MyStride);
	}

	// is plain old data, copy memory
	template<typename T>
	inline void _copydata(const ::std::initializer_list<T>& initList)
	{
		memcpy(_MyBegin, initList.begin(), initList.size() * _MyStride);
	}

private:
	char* _MyBegin;
	char* _MyEnd;
	char* _MyLast;
	size_t _MyStride;

}; // struct stridemap

template<typename T>
iterator_range<stridemap_iterator<T>> range(stridemap& strideMap)
{
	return iterator_range<stridemap_iterator<T>>(strideMap.begin<T>(), strideMap.end<T>());
}

template<typename T>
iterator_range<stridemap_iterator<const T>> range(const stridemap& strideMap)
{
	return iterator_range<stridemap_iterator<const T>>(strideMap.begin<T>(), strideMap.end<T>());
}

template<typename T>
iterator_range<stridemap_iterator<const T>> crange(stridemap& strideMap)
{
	return iterator_range<stridemap_iterator<const T>>(strideMap.cbegin<T>(), strideMap.cend<T>());
}

template<typename T>
iterator_range<stridemap_iterator<const T>> crange(const stridemap& strideMap)
{
	return iterator_range<stridemap_iterator<const T>>(strideMap.cbegin<T>(), strideMap.cend<T>());
}

} // namespace mst