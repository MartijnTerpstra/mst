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

#include <catch2/catch_test_macros.hpp>

#include <set_assertions.h>

#include <mstridemap.h>

namespace {

struct stridemap_test_point
{
	int x;
	int y;
};

} // namespace

TEST_CASE("mst::stridemap: default single-stride construction", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	REQUIRE(sm.size() == 0);
	REQUIRE(sm.empty());
	REQUIRE(sm.stride() == sizeof(int));
	REQUIRE(sm.data_size() == 0);
}

TEST_CASE("mst::stridemap: construction with initial size", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 4);
	REQUIRE(sm.size() == 4);
	REQUIRE(!sm.empty());
	REQUIRE(sm.data_size() == 4 * sizeof(int));

	for(size_t i = 0; i < 4; ++i)
		sm.index<int>(i) = (int)i;

	for(size_t i = 0; i < 4; ++i)
		REQUIRE(sm.index<int>(i) == (int)i);

	const mst::stridemap& csm = sm;
	for(size_t i = 0; i < 4; ++i)
		REQUIRE(csm.index<int>(i) == (int)i);
}

TEST_CASE("mst::stridemap: construction with an initial size of zero", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 0);
	REQUIRE(sm.size() == 0);
	REQUIRE(sm.empty());

	// growing an empty, zero-capacity stridemap must not get stuck doubling zero
	sm.push_back<int>(42);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 42);
}

TEST_CASE("mst::stridemap: initializer_list construction", "[stridemap]")
{
	mst::stridemap sm = { 1, 2, 3, 4 };
	REQUIRE(sm.size() == 4);
	REQUIRE(sm.stride() == sizeof(int));

	for(int i = 0; i < 4; ++i)
		REQUIRE(sm.index<int>((size_t)i) == i + 1);
}

TEST_CASE("mst::stridemap: push_back(value) grows across several reallocations", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));

	for(int i = 0; i < 16; ++i)
		sm.push_back<int>(i);

	REQUIRE(sm.size() == 16);
	for(int i = 0; i < 16; ++i)
		REQUIRE(sm.index<int>((size_t)i) == i);
}

TEST_CASE("mst::stridemap: push_back() zero-initializes the new element", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));

	sm.push_back();
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 0);

	sm.push_back<int>(5);
	sm.push_back();
	REQUIRE(sm.index<int>(1) == 5);
	REQUIRE(sm.index<int>(2) == 0);
}

TEST_CASE("mst::stridemap: push_back_from_index duplicates an existing element", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));

	sm.push_back<int>(10);
	sm.push_back<int>(20);

	sm.push_back_from_index(0);

	REQUIRE(sm.size() == 3);
	REQUIRE(sm.index<int>(0) == 10);
	REQUIRE(sm.index<int>(1) == 20);
	REQUIRE(sm.index<int>(2) == 10);
}

TEST_CASE("mst::stridemap: find locates an element or reports failure", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	sm.push_back<int>(10);
	sm.push_back<int>(20);
	sm.push_back<int>(30);

	REQUIRE(sm.find<int>(20) == 1);
	REQUIRE(sm.find<int>(10) == 0);
	REQUIRE(sm.find<int>(99) == (size_t)-1);
}

TEST_CASE("mst::stridemap: find_if locates an element or reports failure", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	sm.push_back<int>(10);
	sm.push_back<int>(20);
	sm.push_back<int>(30);

	REQUIRE(sm.find_if<int>([](int v) { return v > 15; }) == 1);
	REQUIRE(sm.find_if<int>([](int v) { return v > 100; }) == (size_t)-1);
}

TEST_CASE("mst::stridemap: pop_back shrinks down to empty and back", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));

	for(int i = 0; i < 16; ++i)
		sm.push_back<int>(i);

	while(!sm.empty())
		sm.pop_back();

	REQUIRE(sm.size() == 0);
	REQUIRE(sm.empty());

	// still usable after collapsing all the way down
	sm.push_back<int>(7);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 7);
}

TEST_CASE("mst::stridemap: pop_front shifts the remaining elements down", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 5; ++i)
		sm.push_back<int>(i);

	sm.pop_front();
	REQUIRE(sm.size() == 4);
	REQUIRE(sm.index<int>(0) == 1);
	REQUIRE(sm.index<int>(3) == 4);

	sm.pop_front();
	REQUIRE(sm.size() == 3);
	REQUIRE(sm.index<int>(0) == 2);
}

TEST_CASE("mst::stridemap: erase a single element", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 5; ++i)
		sm.push_back<int>(i); // 0,1,2,3,4

	auto it = sm.begin<int>();
	++it;
	++it; // points at element 2

	sm.erase(it);
	REQUIRE(sm.size() == 4);
	REQUIRE(sm.index<int>(0) == 0);
	REQUIRE(sm.index<int>(1) == 1);
	REQUIRE(sm.index<int>(2) == 3);
	REQUIRE(sm.index<int>(3) == 4);

	sm.erase(sm.begin<int>()); // erase the front
	REQUIRE(sm.size() == 3);
	REQUIRE(sm.index<int>(0) == 1);

	auto lastIt = sm.begin<int>() + (int)(sm.size() - 1);
	sm.erase(lastIt); // erase the back
	REQUIRE(sm.size() == 2);
	REQUIRE(sm.index<int>(0) == 1);
	REQUIRE(sm.index<int>(1) == 3);
}

TEST_CASE("mst::stridemap: erase a range of elements", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 6; ++i)
		sm.push_back<int>(i); // 0..5

	auto first = sm.begin<int>() + 1;
	auto last = sm.begin<int>() + 3;
	sm.erase(first, last); // erase indices 1 and 2

	REQUIRE(sm.size() == 4);
	REQUIRE(sm.index<int>(0) == 0);
	REQUIRE(sm.index<int>(1) == 3);
	REQUIRE(sm.index<int>(2) == 4);
	REQUIRE(sm.index<int>(3) == 5);

	// an empty range is a no-op
	auto mid = sm.begin<int>();
	sm.erase(mid, mid);
	REQUIRE(sm.size() == 4);

	// erasing everything collapses back to empty
	sm.erase(sm.begin<int>(), sm.end<int>());
	REQUIRE(sm.empty());

	// still usable after collapsing to empty via erase()
	sm.push_back<int>(42);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 42);
}

TEST_CASE("mst::stridemap: front and back, const and non-const", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	sm.push_back<int>(1);
	sm.push_back<int>(2);
	sm.push_back<int>(3);

	REQUIRE(sm.front<int>() == 1);
	REQUIRE(sm.back<int>() == 3);

	const mst::stridemap& csm = sm;
	REQUIRE(csm.front<int>() == 1);
	REQUIRE(csm.back<int>() == 3);

	sm.front<int>() = 100;
	sm.back<int>() = 300;
	REQUIRE(sm.index<int>(0) == 100);
	REQUIRE(sm.index<int>(2) == 300);
}

TEST_CASE("mst::stridemap: data() and data_size()", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 3);
	sm.index<int>(0) = 1;
	sm.index<int>(1) = 2;
	sm.index<int>(2) = 3;

	REQUIRE(sm.data() != nullptr);
	REQUIRE(sm.data_size() == 3 * sizeof(int));

	const mst::stridemap& csm = sm;
	REQUIRE(csm.data() != nullptr);
	REQUIRE(csm.data() == sm.data());

	REQUIRE(*(const int*)csm.data() == 1);
}

TEST_CASE("mst::stridemap: set_stride while empty", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	REQUIRE(sm.stride() == sizeof(int));

	sm.set_stride(sizeof(double));
	REQUIRE(sm.stride() == sizeof(double));
	REQUIRE(sm.empty());

	sm.push_back<double>(3.5);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<double>(0) == 3.5);
}

TEST_CASE("mst::stridemap: pre_allocate", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 5);
	for(size_t i = 0; i < 5; ++i)
		sm.index<int>(i) = (int)i;

	// already has exactly enough room: no-op branch
	sm.pre_allocate(0);
	REQUIRE(sm.size() == 5);

	// needs more room: reallocation branch
	sm.pre_allocate(10);
	REQUIRE(sm.size() == 5);
	for(size_t i = 0; i < 5; ++i)
		REQUIRE(sm.index<int>(i) == (int)i);

	for(int i = 5; i < 15; ++i)
		sm.push_back<int>(i);
	REQUIRE(sm.size() == 15);
}

TEST_CASE("mst::stridemap: clear empties the container and it stays usable", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 4; ++i)
		sm.push_back<int>(i);

	sm.clear();
	REQUIRE(sm.size() == 0);
	REQUIRE(sm.empty());

	// regression: growing again after clear() must not get stuck doubling zero
	sm.push_back<int>(99);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 99);
}

TEST_CASE("mst::stridemap: shrink_to_fit", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	sm.push_back<int>(1);
	sm.push_back<int>(2);
	sm.push_back<int>(3);

	sm.shrink_to_fit();
	REQUIRE(sm.size() == 3);
	REQUIRE(sm.index<int>(0) == 1);
	REQUIRE(sm.index<int>(1) == 2);
	REQUIRE(sm.index<int>(2) == 3);

	sm.clear();
	sm.shrink_to_fit(); // shrinking an already-empty stridemap
	REQUIRE(sm.empty());

	sm.push_back<int>(9);
	REQUIRE(sm.index<int>(0) == 9);
}

TEST_CASE("mst::stridemap: resize grows, shrinks and clears", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 3);
	sm.index<int>(0) = 1;
	sm.index<int>(1) = 2;
	sm.index<int>(2) = 3;

	sm.resize(5);
	REQUIRE(sm.size() == 5);
	REQUIRE(sm.index<int>(0) == 1);
	REQUIRE(sm.index<int>(1) == 2);
	REQUIRE(sm.index<int>(2) == 3);

	sm.resize(2);
	REQUIRE(sm.size() == 2);
	REQUIRE(sm.index<int>(0) == 1);
	REQUIRE(sm.index<int>(1) == 2);

	sm.resize(0);
	REQUIRE(sm.empty());

	sm.push_back<int>(42);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm.index<int>(0) == 42);
}

TEST_CASE("mst::stridemap: operator+= appends another stridemap", "[stridemap]")
{
	mst::stridemap a(sizeof(int), 2);
	a.index<int>(0) = 1;
	a.index<int>(1) = 2;

	mst::stridemap b(sizeof(int), 3);
	b.index<int>(0) = 3;
	b.index<int>(1) = 4;
	b.index<int>(2) = 5;

	a += b;

	REQUIRE(a.size() == 5);
	for(int i = 0; i < 5; ++i)
		REQUIRE(a.index<int>((size_t)i) == i + 1);

	// the appended-from map is untouched
	REQUIRE(b.size() == 3);
}

TEST_CASE("mst::stridemap: copy construction and copy assignment", "[stridemap]")
{
	mst::stridemap src(sizeof(int), 3);
	src.index<int>(0) = 1;
	src.index<int>(1) = 2;
	src.index<int>(2) = 3;

	mst::stridemap copyCtor(src);
	REQUIRE(copyCtor.size() == 3);
	for(size_t i = 0; i < 3; ++i)
		REQUIRE(copyCtor.index<int>(i) == src.index<int>(i));

	// the copy is independent from the source
	copyCtor.index<int>(0) = 100;
	REQUIRE(src.index<int>(0) == 1);

	mst::stridemap assignee(sizeof(int));
	assignee = src;
	REQUIRE(assignee.size() == 3);
	for(size_t i = 0; i < 3; ++i)
		REQUIRE(assignee.index<int>(i) == src.index<int>(i));

	assignee.index<int>(0) = 200;
	REQUIRE(src.index<int>(0) == 1);
}

TEST_CASE("mst::stridemap: move construction and move assignment", "[stridemap]")
{
	mst::stridemap src(sizeof(int), 3);
	src.index<int>(0) = 1;
	src.index<int>(1) = 2;
	src.index<int>(2) = 3;

	mst::stridemap moved(std::move(src));
	REQUIRE(moved.size() == 3);
	REQUIRE(moved.index<int>(0) == 1);
	REQUIRE(moved.index<int>(2) == 3);
	REQUIRE(src.empty()); // NOLINT: intentionally inspecting the moved-from state

	mst::stridemap moveAssignee(sizeof(int));
	moveAssignee = std::move(moved);
	REQUIRE(moveAssignee.size() == 3);
	REQUIRE(moveAssignee.index<int>(0) == 1);
	REQUIRE(moved.empty()); // NOLINT: intentionally inspecting the moved-from state
}

TEST_CASE("mst::stridemap: begin/end/cbegin/cend, const and non-const", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	sm.push_back<int>(1);
	sm.push_back<int>(2);
	sm.push_back<int>(3);

	int sum = 0;
	for(auto it = sm.begin<int>(); it != sm.end<int>(); ++it)
		sum += *it;
	REQUIRE(sum == 6);

	const mst::stridemap& csm = sm;

	sum = 0;
	for(auto it = csm.begin<int>(); it != csm.end<int>(); ++it)
		sum += *it;
	REQUIRE(sum == 6);

	sum = 0;
	for(auto it = sm.cbegin<int>(); it != sm.cend<int>(); ++it)
		sum += *it;
	REQUIRE(sum == 6);
}

TEST_CASE("mst::stridemap: range() and crange() free functions", "[stridemap]")
{
	mst::stridemap sm(sizeof(int), 3);
	sm.index<int>(0) = 1;
	sm.index<int>(1) = 2;
	sm.index<int>(2) = 3;

	int sum = 0;
	for(int v : mst::range<int>(sm))
		sum += v;
	REQUIRE(sum == 6);

	const mst::stridemap& csm = sm;

	sum = 0;
	for(int v : mst::range<int>(csm))
		sum += v;
	REQUIRE(sum == 6);

	sum = 0;
	for(int v : mst::crange<int>(sm))
		sum += v;
	REQUIRE(sum == 6);

	sum = 0;
	for(int v : mst::crange<int>(csm))
		sum += v;
	REQUIRE(sum == 6);
}

TEST_CASE("mst::stridemap_iterator<T>: increment, decrement and dereference", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 5; ++i)
		sm.push_back<int>(i); // 0,1,2,3,4

	auto it = sm.begin<int>();
	REQUIRE(*it == 0);

	++it;
	REQUIRE(*it == 1);

	auto postIncResult = it++;
	REQUIRE(*postIncResult == 1);
	REQUIRE(*it == 2);

	--it;
	REQUIRE(*it == 1);

	auto postDecResult = it--;
	REQUIRE(*postDecResult == 1);
	REQUIRE(*it == 0);
}

TEST_CASE("mst::stridemap_iterator<T>: random access arithmetic and distance", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 5; ++i)
		sm.push_back<int>(i); // 0,1,2,3,4

	auto it = sm.begin<int>();
	it += 3;
	REQUIRE(*it == 3);

	it -= 2;
	REQUIRE(*it == 1);

	auto it2 = it + 2;
	REQUIRE(*it2 == 3);

	auto it3 = it2 - 1;
	REQUIRE(*it3 == 2);

	REQUIRE((sm.end<int>() - sm.begin<int>()) == 5);
	REQUIRE((it2 - it) == 2);
}

TEST_CASE("mst::stridemap_iterator<T>: comparison operators", "[stridemap]")
{
	mst::stridemap sm(sizeof(int));
	for(int i = 0; i < 3; ++i)
		sm.push_back<int>(i);

	auto first = sm.begin<int>();
	auto second = first + 1;
	auto firstCopy = sm.begin<int>();

	REQUIRE(first == firstCopy);
	REQUIRE(first != second);
	REQUIRE(first < second);
	REQUIRE(first <= second);
	REQUIRE(first <= firstCopy);
	REQUIRE(second > first);
	REQUIRE(second >= first);
	REQUIRE(first >= firstCopy);
}

TEST_CASE("mst::stridemap_iterator<T>: operator-> gives member access", "[stridemap]")
{
	mst::stridemap sm(sizeof(stridemap_test_point));
	sm.push_back<stridemap_test_point>({ 1, 2 });
	sm.push_back<stridemap_test_point>({ 3, 4 });

	auto it = sm.begin<stridemap_test_point>();
	REQUIRE(it->x == 1);
	REQUIRE(it->y == 2);

	++it;
	REQUIRE(it->x == 3);
	REQUIRE(it->y == 4);
}
