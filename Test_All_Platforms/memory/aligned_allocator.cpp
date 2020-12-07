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

#include <mallocator.h>
#include <vector>
#include <map>
#include <memory>

struct alignas(16) packed16
{
	float floats[4];
};

struct alignas(32) packed32
{
	float floats[4];
};

struct alignas(64) packed64
{
	float floats[4];
};

template<typename _Ty>
void CheckType()
{
	constexpr const auto alignment = alignof(_Ty);

	mst::aligned_allocator<_Ty, alignment> alloc;

	const auto ptrValue = alloc.allocate(1);

	const auto intValue = reinterpret_cast<uintptr_t>(ptrValue);

	REQUIRE((intValue & (alignment - 1)) == 0);

	alloc.deallocate(ptrValue, 1);
}

TEST_CASE("aligned_allocator<T>: sizes", "[memory][allocator]")
{
	CheckType<short>();

	CheckType<int>();

	CheckType<double>();

	CheckType<packed16>();

	CheckType<packed32>();

	CheckType<packed64>();
}

TEST_CASE("aligned_allocator<T>: vector", "[memory][allocator]")
{
	const std::vector<packed16, mst::aligned_allocator<packed16, 64>> packed(1);

	const auto intValue = reinterpret_cast<uintptr_t>(&packed[0]);

	const auto alignment = 64;

	REQUIRE((intValue & (alignment - 1)) == 0);
}