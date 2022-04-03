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
#include <maligned_malloc.h>

namespace mst {

template<typename _Ty, size_t _Alignment>
class aligned_allocator
{
	static_assert(std::alignment_of<_Ty>::value <= _Alignment,
		"aligned_allocator<T, Alignment>: Alignment should have a bigger alignment than the "
		"alignment of T");

public:
	typedef _Ty value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;

	aligned_allocator() = default;

	template<typename T2>
	aligned_allocator(const aligned_allocator<T2, _Alignment>&)
	{
		// do nothing
	}

	void deallocate(pointer ptr, size_t count)
	{
		_MST_UNUSED(count);
		_free(ptr);
	}

	pointer allocate(size_t count)
	{
		return _alloc(count);
	}

	template<typename... Args>
	inline void construct(_Ty* ptr, Args&&... args)
	{
		new((void*)ptr) _Ty(std::forward<Args>(args)...);
	}

	void destroy(pointer ptr)
	{
		::mst::_Details::_Destroy_object(ptr);
	}

	template<typename T2>
	struct rebind
	{
		typedef aligned_allocator<T2, _Alignment> other;
	};

private:
	inline void _free_impl(void* memory, std::true_type)
	{
		aligned_free(memory);
	}

	inline void _free_impl(void* memory, std::false_type)
	{
		free(memory);
	}

	inline void _free(void* memory)
	{
		_free_impl(
			memory, std::integral_constant<bool, (_Alignment > alignof(std::max_align_t))>{});
	}

	inline pointer _alloc_impl(size_t count, std::true_type)
	{
		return reinterpret_cast<pointer>(aligned_malloc(sizeof(_Ty) * count, _Alignment));
	}

	inline pointer _alloc_impl(size_t count, std::false_type)
	{
		return reinterpret_cast<pointer>(malloc(sizeof(_Ty) * count));
	}

	inline pointer _alloc(size_t count)
	{
		return _alloc_impl(
			count, std::integral_constant<bool, (_Alignment > alignof(std::max_align_t))>{});
	}
};


template<size_t _Alignment>
class aligned_allocator<void, _Alignment>
{
public:
	typedef void value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef void* pointer;
	typedef const void* const_pointer;

	aligned_allocator() = default;

	template<typename T2>
	aligned_allocator(const aligned_allocator<T2, _Alignment>&)
	{
		// do nothing
	}

	template<typename T2>
	struct rebind
	{
		typedef aligned_allocator<T2, _Alignment> other;
	};
};

} // namespace mst