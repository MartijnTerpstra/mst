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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <set_assertions.h>

#include <maligned_malloc.h>

TEST_CASE("aligned_malloc: alignments", "[memory][allocator]")
{
	for(size_t alignment = 1; alignment <= 1024; alignment <<= 1)
	{
		const auto ptr = mst::aligned_malloc(alignment * 4, alignment);

		const auto ptrValue = reinterpret_cast<uintptr_t>(ptr);

		REQUIRE((ptrValue & (alignment - 1)) == 0);

		mst::aligned_free(ptr);
	}
}

TEST_CASE("aligned_realloc: alignments", "[memory][allocator]")
{
	for(size_t alignment = 1; alignment <= 1024; alignment <<= 1)
	{
		const auto malloced = mst::aligned_malloc(alignment * 4, alignment);

		const auto ptr = mst::aligned_realloc(malloced, alignment * 8, alignment);

		const auto ptrValue = reinterpret_cast<uintptr_t>(ptr);

		REQUIRE((ptrValue & (alignment - 1)) == 0);

		mst::aligned_free(ptr);
	}
}

// Realloc with nullptr should behave like malloc
TEST_CASE("aligned_realloc: malloc", "[memory][allocator]")
{
	for(size_t alignment = 1; alignment <= 1024; alignment <<= 1)
	{
		const auto ptr = mst::aligned_realloc(nullptr, alignment * 4, alignment);

		const auto ptrValue = reinterpret_cast<uintptr_t>(ptr);

		REQUIRE((ptrValue & (alignment - 1)) == 0);

		mst::aligned_free(ptr);
	}
}

// Realloc with nullptr should behave like malloc
TEST_CASE("aligned_realloc: free", "[memory][allocator]")
{
	for(size_t alignment = 1; alignment <= 1024; alignment <<= 1)
	{
		const auto ptr = mst::aligned_realloc(nullptr, alignment * 4, alignment);

		REQUIRE(mst::aligned_realloc(ptr, 0, alignment) == nullptr);
	}
}