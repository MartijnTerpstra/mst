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

#include <mstatic_map.h>

namespace {

struct static_map_test_point
{
	int x;
	int y;
};

} // namespace

TEST_CASE("mst::static_map: default construction", "[static_map]")
{
	mst::static_map<int, 4> sm;

	REQUIRE(sm.empty());
	REQUIRE(!sm.full());
	REQUIRE(sm.size() == 0);
	REQUIRE(sm.capacity() == 4);
	REQUIRE(sm.max_size() == 4);
	REQUIRE(sm.begin() == sm.end());
}

TEST_CASE("mst::static_map: initializer_list construction", "[static_map]")
{
	mst::static_map<int, 8> sm = { 1, 2, 3, 4 };

	REQUIRE(sm.size() == 4);
	REQUIRE(!sm.empty());
	REQUIRE(!sm.full());

	for(int i = 0; i < 4; ++i)
		REQUIRE(sm[(size_t)i] == i + 1);
}

TEST_CASE("mst::static_map: initializer_list construction exceeding capacity", "[!shouldfail][static_map]")
{
	mst::static_map<int, 2> sm = { 1, 2, 3 };
}

TEST_CASE("mst::static_map: push_back grows the container", "[static_map]")
{
	mst::static_map<int, 4> sm;

	sm.push_back(10);
	REQUIRE(sm.size() == 1);
	REQUIRE(!sm.empty());

	sm.push_back(20);
	sm.push_back(30);
	REQUIRE(sm.size() == 3);

	REQUIRE(sm[0] == 10);
	REQUIRE(sm[1] == 20);
	REQUIRE(sm[2] == 30);
}

TEST_CASE("mst::static_map: push_back on a full container", "[!shouldfail][static_map]")
{
	mst::static_map<int, 2> sm;
	sm.push_back(1);
	sm.push_back(2);

	REQUIRE(sm.full());

	sm.push_back(3);
}

TEST_CASE("mst::static_map: pop_back shrinks the container down to empty and back", "[static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2, 3 };

	sm.pop_back();
	REQUIRE(sm.size() == 2);
	REQUIRE(sm[1] == 2);

	sm.pop_back();
	sm.pop_back();
	REQUIRE(sm.empty());

	// still usable after popping back to empty
	sm.push_back(42);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm[0] == 42);
}

TEST_CASE("mst::static_map: pop_back on an empty container", "[!shouldfail][static_map]")
{
	mst::static_map<int, 4> sm;
	sm.pop_back();
}

TEST_CASE("mst::static_map: operator[] out of range", "[!shouldfail][static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2 };
	(void)sm[2];
}

TEST_CASE("mst::static_map: operator[] const out of range", "[!shouldfail][static_map]")
{
	const mst::static_map<int, 4> sm = { 1, 2 };
	(void)sm[2];
}

TEST_CASE("mst::static_map: front and back, const and non-const", "[static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2, 3 };

	REQUIRE(sm.front() == 1);
	REQUIRE(sm.back() == 3);

	const mst::static_map<int, 4>& csm = sm;
	REQUIRE(csm.front() == 1);
	REQUIRE(csm.back() == 3);

	sm.front() = 100;
	sm.back() = 300;
	REQUIRE(sm[0] == 100);
	REQUIRE(sm[2] == 300);
}

TEST_CASE("mst::static_map: front on an empty container", "[!shouldfail][static_map]")
{
	mst::static_map<int, 4> sm;
	(void)sm.front();
}

TEST_CASE("mst::static_map: front const on an empty container", "[!shouldfail][static_map]")
{
	const mst::static_map<int, 4> sm;
	(void)sm.front();
}

TEST_CASE("mst::static_map: back on an empty container", "[!shouldfail][static_map]")
{
	mst::static_map<int, 4> sm;
	(void)sm.back();
}

TEST_CASE("mst::static_map: back const on an empty container", "[!shouldfail][static_map]")
{
	const mst::static_map<int, 4> sm;
	(void)sm.back();
}

TEST_CASE("mst::static_map: full() reflects whether capacity is reached", "[static_map]")
{
	mst::static_map<int, 2> sm;
	REQUIRE(!sm.full());

	sm.push_back(1);
	REQUIRE(!sm.full());

	sm.push_back(2);
	REQUIRE(sm.full());

	sm.pop_back();
	REQUIRE(!sm.full());
}

TEST_CASE("mst::static_map: clear empties the container and it stays usable", "[static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2, 3, 4 };

	sm.clear();
	REQUIRE(sm.size() == 0);
	REQUIRE(sm.empty());

	sm.push_back(99);
	REQUIRE(sm.size() == 1);
	REQUIRE(sm[0] == 99);
}

TEST_CASE("mst::static_map: data() pointer access, const and non-const", "[static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2, 3 };

	REQUIRE(sm.data() != nullptr);
	REQUIRE(sm.data()[0] == 1);

	sm.data()[1] = 200;
	REQUIRE(sm[1] == 200);

	const mst::static_map<int, 4>& csm = sm;
	REQUIRE(csm.data() == sm.data());
	REQUIRE(csm.data()[1] == 200);
}

TEST_CASE("mst::static_map: begin/end/cbegin/cend, const and non-const", "[static_map]")
{
	mst::static_map<int, 4> sm = { 1, 2, 3 };

	int sum = 0;
	for(auto it = sm.begin(); it != sm.end(); ++it)
		sum += *it;
	REQUIRE(sum == 6);
	REQUIRE(sm.end() - sm.begin() == 3);

	const mst::static_map<int, 4>& csm = sm;

	sum = 0;
	for(auto it = csm.begin(); it != csm.end(); ++it)
		sum += *it;
	REQUIRE(sum == 6);

	sum = 0;
	for(auto it = sm.cbegin(); it != sm.cend(); ++it)
		sum += *it;
	REQUIRE(sum == 6);

	sum = 0;
	for(int v : sm)
		sum += v;
	REQUIRE(sum == 6);
}

TEST_CASE("mst::static_map: copy construction and copy assignment are independent", "[static_map]")
{
	mst::static_map<int, 4> src = { 1, 2, 3 };

	mst::static_map<int, 4> copyCtor(src);
	REQUIRE(copyCtor.size() == 3);
	for(size_t i = 0; i < 3; ++i)
		REQUIRE(copyCtor[i] == src[i]);

	copyCtor[0] = 100;
	REQUIRE(src[0] == 1);

	mst::static_map<int, 4> assignee;
	assignee = src;
	REQUIRE(assignee.size() == 3);
	for(size_t i = 0; i < 3; ++i)
		REQUIRE(assignee[i] == src[i]);

	assignee[0] = 200;
	REQUIRE(src[0] == 1);
}

TEST_CASE("mst::static_map: move construction and move assignment", "[static_map]")
{
	mst::static_map<int, 4> src = { 1, 2, 3 };

	mst::static_map<int, 4> moved(std::move(src));
	REQUIRE(moved.size() == 3);
	REQUIRE(moved[0] == 1);
	REQUIRE(moved[2] == 3);

	mst::static_map<int, 4> moveAssignee;
	moveAssignee = std::move(moved);
	REQUIRE(moveAssignee.size() == 3);
	REQUIRE(moveAssignee[0] == 1);
	REQUIRE(moveAssignee[2] == 3);
}

TEST_CASE("mst::static_map: works with a POD struct value type", "[static_map]")
{
	mst::static_map<static_map_test_point, 4> sm;

	sm.push_back({ 1, 2 });
	sm.push_back({ 3, 4 });

	REQUIRE(sm.size() == 2);
	REQUIRE(sm[0].x == 1);
	REQUIRE(sm[0].y == 2);
	REQUIRE(sm.back().x == 3);
	REQUIRE(sm.back().y == 4);
}

TEST_CASE("mst::static_map: capacity of one is a valid degenerate case", "[static_map]")
{
	mst::static_map<int, 1> sm;

	REQUIRE(sm.capacity() == 1);
	REQUIRE(!sm.full());

	sm.push_back(7);
	REQUIRE(sm.full());
	REQUIRE(sm.front() == 7);
	REQUIRE(sm.back() == 7);

	sm.pop_back();
	REQUIRE(sm.empty());
}
