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
#include <stdlib.h>
#include <new>
#include <string.h>

#define _MST_MINIMAL_ALIGNMENT (16)
#define _MST_BLOCK_SIZE		   (64)

namespace mst {

inline void* aligned_malloc(size_t size, size_t alignment);
inline void* aligned_realloc(void* memory, size_t newSize, size_t newAlignment);
inline void aligned_free(void* memory);

inline void* aligned_malloc(size_t size, size_t alignment)
{
	if(size == 0)
	{
		size = 1;
	}

	alignment = (alignment < _MST_MINIMAL_ALIGNMENT) ? _MST_MINIMAL_ALIGNMENT : alignment;

	if(alignment == 0 || ((alignment - 1) & alignment))
	{
		MST_FATAL_ERROR("bad alignment in mst::dll_safe::aligned_malloc(): must be a power of 2");

		// bad alignment
#if _MST_HAS_EXCEPTIONS
		throw std::invalid_argument("alignment: must be a power of 2");
#else
		::std::abort();
#endif
	}

	size_t allocSize = (size + _MST_BLOCK_SIZE) & ~(_MST_BLOCK_SIZE - 1);

	void* memory = malloc(allocSize + alignment + sizeof(void*) + sizeof(size_t));

	if(memory == nullptr)
	{
		return nullptr;
	}

	void** aligned = reinterpret_cast<void**>(
		(reinterpret_cast<size_t>(memory) + alignment + sizeof(void*) + sizeof(size_t)) /
		alignment * alignment);

	*(aligned - 1) = memory;
	*(reinterpret_cast<size_t*>(aligned - 1) - 1) = size;

	return aligned;
}

inline void* aligned_realloc(void* memory, size_t newSize, size_t newAlignment)
{
	if(newSize == 0)
	{
		::mst::aligned_free(memory);
		return nullptr;
	}

	if(memory == nullptr)
	{
		return ::mst::aligned_malloc(newSize, newAlignment);
	}

	size_t oldSize = *((size_t*)memory - 2);

	size_t oldAllocSize = (oldSize + _MST_BLOCK_SIZE - 1) & ~(_MST_BLOCK_SIZE - 1);

	size_t newAllocSize = (newSize + _MST_BLOCK_SIZE - 1) & ~(_MST_BLOCK_SIZE - 1);

	if(oldAllocSize == newAllocSize)
	{
		void** aligned = reinterpret_cast<void**>(memory);
		*(reinterpret_cast<size_t*>(aligned - 1) - 1) = newSize;
		return memory;
	}

	void* newMemory = ::mst::aligned_malloc(newSize, newAlignment);

	if(newSize > oldSize)
	{
		memcpy(newMemory, memory, oldSize);
	}
	else
	{
		memcpy(newMemory, memory, newSize);
	}

	::mst::aligned_free(memory);

	return newMemory;
}

inline void aligned_free(void* memory)
{
	void** memptr = reinterpret_cast<void**>(memory) - 1;

	free(*memptr);
}

} // namespace mst