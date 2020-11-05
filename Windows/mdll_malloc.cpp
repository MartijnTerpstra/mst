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

#include <mdll_malloc.h>
#include <exception>

#include <Windows.h>

inline HANDLE GetHeapHandle()
{
	static HANDLE retval = GetProcessHeap();

	return retval;
}

#define _MST_MINIMAL_ALIGNMENT (16)
#define _MST_BLOCK_SIZE		   (64)

static void* dll_malloc(size_t size)
{
	if(size == 0)
	{
		size = 1;
	}


	void* retval = HeapAlloc(GetHeapHandle(), 0, size);

	if(retval == nullptr)
	{
		throw std::bad_alloc();
	}
	return retval;
}

static void dll_free(void* memory)
{
	if(memory)
	{
		BOOL success = HeapFree(GetHeapHandle(), 0, memory);
		if(success == FALSE)
		{
			throw std::bad_alloc();
		}
	}
}

void* mst::dll_safe::aligned_malloc(size_t size, size_t alignment)
{
	if(size == 0)
	{
		size = 1;
	}

	alignment = (alignment < _MST_MINIMAL_ALIGNMENT) ? _MST_MINIMAL_ALIGNMENT : alignment;

	if(alignment == 0 || ((alignment - 1) & alignment))
	{
		// bad alignment
		throw std::bad_alloc();
	}

	size_t allocSize = (size + _MST_BLOCK_SIZE) & ~(_MST_BLOCK_SIZE - 1);

	void* memory = dll_malloc(allocSize + alignment + sizeof(void*) + sizeof(size_t));

	if(memory == nullptr)
	{
		throw std::bad_alloc();
	}

	void** aligned = reinterpret_cast<void**>(
		(reinterpret_cast<size_t>(memory) + alignment + sizeof(void*) + sizeof(size_t)) /
		alignment * alignment);

	*(aligned - 1) = memory;
	*(reinterpret_cast<size_t*>(aligned - 1) - 1) = size;

	return aligned;
}

void* mst::dll_safe::aligned_realloc(void* memory, size_t newSize, size_t newAlignment)
{
	if(newSize == 0)
	{
		::mst::dll_safe::aligned_free(memory);
		return nullptr;
	}

	if(memory == nullptr)
	{
		return ::mst::dll_safe::aligned_malloc(newSize, newAlignment);
	}

	size_t oldSize = *((size_t*)memory - 2);

	size_t oldAllocSize = (oldSize + _MST_BLOCK_SIZE) & ~(_MST_BLOCK_SIZE - 1);

	size_t newAllocSize = (newSize + _MST_BLOCK_SIZE) & ~(_MST_BLOCK_SIZE - 1);

	if(oldAllocSize == newAllocSize)
	{
		void** aligned = reinterpret_cast<void**>(memory);
		*(reinterpret_cast<size_t*>(aligned - 1) - 1) = newSize;
		return memory;
	}

	void* newMemory = ::mst::dll_safe::aligned_malloc(newSize, newAlignment);

	if(newSize > oldSize)
	{
		memcpy(newMemory, memory, oldSize);
	}
	else
	{
		memcpy(newMemory, memory, newSize);
	}

	::mst::dll_safe::aligned_free(memory);

	return newMemory;
}

void mst::dll_safe::aligned_free(void* memory)
{
	void** memptr = reinterpret_cast<void**>(memory) - 1;

	dll_free(*memptr);
}