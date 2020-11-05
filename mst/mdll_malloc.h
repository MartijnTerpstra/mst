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

namespace mst {
namespace dll_safe {

void* aligned_malloc(size_t size, size_t alignment);

void* aligned_realloc(void* memory, size_t newSize, size_t newAlignment);

void aligned_free(void* memory);

template<typename T>
T* malloc_object(size_t elemCount)
{
	return reinterpret_cast<T*>(
		::mst::dll_safe::aligned_malloc(sizeof(T) * elemCount, std::alignment_of<T>::value));
}

template<typename T>
T* realloc_object(T* memory, size_t elemCount)
{
	return reinterpret_cast<T*>(::mst::dll_safe::aligned_realloc(
		memory, sizeof(T) * elemCount, std::alignment_of<T>::value));
}

template<typename T>
void free_object(T* memory)
{
	::mst::dll_safe::aligned_free(memory);
}

template<typename T>
class allocator
{
public:
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T value_type;

	template<typename Y>
	struct rebind
	{
		typedef allocator<Y> other;
	};

	allocator() noexcept
	{ }

	allocator(const allocator<T>&) noexcept
	{ }

	template<class T2>
	allocator(const allocator<T2>&) noexcept
	{ }

	template<class T2>
	allocator& operator=(const allocator<T2>&)
	{
		return *this;
	}

	template<class T2>
	bool operator==(const allocator<T2>&) const noexcept
	{
		return true;
	}

	template<class T2>
	bool operator!=(const allocator<T2>&) const noexcept
	{
		return false;
	}

	pointer allocate(size_t n, const_pointer hint = (const_pointer)0)
	{
		_MST_UNUSED(hint);
		return malloc_object<T>(n);
	}

	void deallocate(pointer p, size_t)
	{
		free_object<T>(p);
	}

	void construct(pointer p, const T& t)
	{
		new(p) T(t);
	}

	void destroy(pointer p)
	{
		::mst::_Details::_Destroy_object(p);
	}

}; // class allocator<T>

}; // namespace dll_safe
}; // namespace mst