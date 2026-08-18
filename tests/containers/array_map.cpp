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

#include <marray_map.h>
#include <algorithm>
#include <functional>
#include <random>
#include <vector>

using mst::array_map;

TEST_CASE("array_map<K,V>: creation", "[array_map]")
{
	array_map<int, int> intint;

	intint[100] = 1000;

	REQUIRE(intint.size() == 1);
	REQUIRE(intint.contains(100));
	REQUIRE(intint.at(100) == 1000);

	REQUIRE(*intint.begin() == std::pair<const int, int>(100, 1000));

	REQUIRE(intint.end() - intint.begin() == 1);

	intint.clear();

	REQUIRE(intint.size() == 0);
	REQUIRE(!intint.contains(100));
}

TEST_CASE("array_map<K,V>: default construction is empty", "[array_map]")
{
	array_map<int, int> am;

	REQUIRE(am.empty());
	REQUIRE(am.size() == 0);
	REQUIRE(am.begin() == am.end());
	REQUIRE(am.cbegin() == am.cend());
}

TEST_CASE("array_map<K,V>: initializer_list construction keeps keys sorted", "[array_map]")
{
	array_map<int, int> am = {
		{ 3, 30 },
		{ 1, 10 },
		{ 2, 20 },
	};

	REQUIRE(am.size() == 3);

	int prevKey = am.get_key(0);
	for(size_t i = 1; i < am.size(); ++i)
	{
		REQUIRE(prevKey < (int)am.get_key(i));
		prevKey = (int)am.get_key(i);
	}

	REQUIRE(am.at(1) == 10);
	REQUIRE(am.at(2) == 20);
	REQUIRE(am.at(3) == 30);
}

TEST_CASE("array_map<K,V>: operator[] on an empty container constructs the first element", "[array_map]")
{
	array_map<int, int> am;

	am[5] = 50;

	REQUIRE(am.size() == 1);
	REQUIRE(am.at(5) == 50);
}

TEST_CASE("array_map<K,V>: operator[] updates an existing key in place", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;

	am[1] = 100;

	REQUIRE(am.size() == 2);
	REQUIRE(am.at(1) == 100);
	REQUIRE(am.at(2) == 20);
}

TEST_CASE("array_map<K,V>: operator[] inserts a new key keeping sort order", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[3] = 30;

	// falls in the middle of the existing range
	am[2] = 20;

	REQUIRE(am.size() == 3);
	REQUIRE(am.get_key(0) == 1);
	REQUIRE(am.get_key(1) == 2);
	REQUIRE(am.get_key(2) == 3);
}

TEST_CASE("array_map<K,V>: insert(const value_type&) does not overwrite an existing key", "[array_map]")
{
	array_map<int, int> am;

	const std::pair<const int, int> first(1, 10);
	am.insert(first);

	const std::pair<const int, int> duplicate(1, 999);
	am.insert(duplicate);

	REQUIRE(am.size() == 1);
	REQUIRE(am.at(1) == 10);
}

TEST_CASE("array_map<K,V>: insert(PairType&&) forwards a movable/convertible pair", "[array_map]")
{
	array_map<int, int> am;

	am.insert(std::pair<int, int>(2, 20));
	am.insert(std::pair<int, int>(1, 10));

	REQUIRE(am.size() == 2);
	REQUIRE(am.at(1) == 10);
	REQUIRE(am.at(2) == 20);

	// duplicate key via the forwarding overload: original value is kept
	am.insert(std::pair<int, int>(1, 999));
	REQUIRE(am.size() == 2);
	REQUIRE(am.at(1) == 10);
}

TEST_CASE("array_map<K,V>: contains reflects presence of a key", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	REQUIRE(am.contains(1));
	REQUIRE(!am.contains(2));
}

TEST_CASE("array_map<K,V>: count returns 0 or 1", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	REQUIRE(am.count(1) == 1);
	REQUIRE(am.count(2) == 0);
}

TEST_CASE("array_map<K,V>: find locates an element or returns end()", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;

	auto it = am.find(1);
	REQUIRE(it != am.end());
	REQUIRE(it->second == 10);

	REQUIRE(am.find(99) == am.end());

	const array_map<int, int>& cam = am;
	auto cit = cam.find(2);
	REQUIRE(cit != cam.end());
	REQUIRE(cit->second == 20);

	REQUIRE(cam.find(99) == cam.end());
}

TEST_CASE("array_map<K,V>: at() const and non-const access existing keys", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	REQUIRE(am.at(1) == 10);

	am.at(1) = 100;
	REQUIRE(am.at(1) == 100);

	const array_map<int, int>& cam = am;
	REQUIRE(cam.at(1) == 100);
}

TEST_CASE("array_map<K,V>: const at() throws std::out_of_range for a missing key", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	const array_map<int, int>& cam = am;
	REQUIRE_THROWS_AS(cam.at(2), std::out_of_range);
}

TEST_CASE("array_map<K,V>: non-const at() on a missing key", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	(void)am.at(2);
}

TEST_CASE("array_map<K,V>: at() on an empty container", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	(void)am.at(1);
}

TEST_CASE("array_map<K,V>: const at() on an empty container", "[!shouldfail][array_map]")
{
	const array_map<int, int> am;
	(void)am.at(1);
}

TEST_CASE("array_map<K,V>: front and back, const and non-const", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;
	am[3] = 30;

	REQUIRE(am.front() == 10);
	REQUIRE(am.back() == 30);

	const array_map<int, int>& cam = am;
	REQUIRE(cam.front() == 10);
	REQUIRE(cam.back() == 30);

	am.front() = 100;
	am.back() = 300;
	REQUIRE(am.at(1) == 100);
	REQUIRE(am.at(3) == 300);
}

TEST_CASE("array_map<K,V>: front on an empty container", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	(void)am.front();
}

TEST_CASE("array_map<K,V>: const front on an empty container", "[!shouldfail][array_map]")
{
	const array_map<int, int> am;
	(void)am.front();
}

TEST_CASE("array_map<K,V>: back on an empty container", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	(void)am.back();
}

TEST_CASE("array_map<K,V>: const back on an empty container", "[!shouldfail][array_map]")
{
	const array_map<int, int> am;
	(void)am.back();
}

TEST_CASE("array_map<K,V>: get_key returns the key at a given index", "[array_map]")
{
	array_map<int, int> am;
	am[3] = 30;
	am[1] = 10;
	am[2] = 20;

	REQUIRE(am.get_key(0) == 1);
	REQUIRE(am.get_key(1) == 2);
	REQUIRE(am.get_key(2) == 3);
}

TEST_CASE("array_map<K,V>: erase(key) removes an existing element", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;
	am[3] = 30;

	am.erase(2);

	REQUIRE(am.size() == 2);
	REQUIRE(!am.contains(2));
	REQUIRE(am.contains(1));
	REQUIRE(am.contains(3));
}

TEST_CASE("array_map<K,V>: erase(key) on a missing key", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	am.erase(2);
}

TEST_CASE("array_map<K,V>: erase(key) on an empty container", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	am.erase(1);
}

TEST_CASE("array_map<K,V>: erase(iterator) removes the pointed-to element", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;
	am[3] = 30;

	auto it = am.find(2);
	auto next = am.erase(it);

	REQUIRE(am.size() == 2);
	REQUIRE(!am.contains(2));
	REQUIRE(next->first == 3);
}

TEST_CASE("array_map<K,V>: erase(iterator) on an empty container", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	(void)am.erase(am.begin());
}

TEST_CASE("array_map<K,V>: erase(iterator) with an out-of-range iterator", "[!shouldfail][array_map]")
{
	array_map<int, int> am;
	am[1] = 10;

	(void)am.erase(am.end());
}

TEST_CASE("array_map<K,V>: lower_bound and upper_bound, const and non-const", "[array_map]")
{
	array_map<int, int> am;
	for(int key : { 10, 20, 30, 40, 50 })
		am[key] = key * 10;

	// key before the first element
	REQUIRE(am.lower_bound(5)->first == 10);
	REQUIRE(am.upper_bound(5)->first == 10);

	// key that matches an existing element exactly
	REQUIRE(am.lower_bound(30)->first == 30);
	REQUIRE(am.upper_bound(30)->first == 40);

	// key that falls strictly between two elements
	REQUIRE(am.lower_bound(25)->first == 30);
	REQUIRE(am.upper_bound(25)->first == 30);

	// key past the last element
	REQUIRE(am.lower_bound(100) == am.end());
	REQUIRE(am.upper_bound(100) == am.end());

	const array_map<int, int>& cam = am;
	REQUIRE(cam.lower_bound(30)->first == 30);
	REQUIRE(cam.upper_bound(30)->first == 40);
	REQUIRE(cam.lower_bound(100) == cam.end());
	REQUIRE(cam.upper_bound(100) == cam.end());
}

TEST_CASE("array_map<K,V>: lower_bound/upper_bound on an empty container", "[array_map]")
{
	array_map<int, int> am;

	REQUIRE(am.lower_bound(1) == am.end());
	REQUIRE(am.upper_bound(1) == am.end());
}

TEST_CASE("array_map<K,V>: capacity, reserve and shrink_to_fit", "[array_map]")
{
	array_map<int, int> am;
	REQUIRE(am.capacity() == 0);

	am.reserve(10);
	REQUIRE(am.capacity() >= 10);

	am[1] = 10;
	am[2] = 20;

	am.shrink_to_fit();
	REQUIRE(am.capacity() >= am.size());
	REQUIRE(am.size() == 2);
}

TEST_CASE("array_map<K,V>: begin/end/cbegin/cend, const and non-const", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;
	am[3] = 30;

	int sum = 0;
	for(auto it = am.begin(); it != am.end(); ++it)
		sum += it->second;
	REQUIRE(sum == 60);

	const array_map<int, int>& cam = am;

	sum = 0;
	for(auto it = cam.begin(); it != cam.end(); ++it)
		sum += it->second;
	REQUIRE(sum == 60);

	sum = 0;
	for(auto it = am.cbegin(); it != am.cend(); ++it)
		sum += it->second;
	REQUIRE(sum == 60);
}

TEST_CASE("array_map<K,V>: copy construction and copy assignment are independent", "[array_map]")
{
	array_map<int, int> src;
	src[1] = 10;
	src[2] = 20;

	array_map<int, int> copyCtor(src);
	REQUIRE(copyCtor.size() == 2);
	REQUIRE(copyCtor.at(1) == 10);

	copyCtor[1] = 999;
	REQUIRE(src.at(1) == 10);

	array_map<int, int> assignee;
	assignee = src;
	REQUIRE(assignee.size() == 2);
	REQUIRE(assignee.at(2) == 20);

	assignee[2] = 999;
	REQUIRE(src.at(2) == 20);
}

TEST_CASE("array_map<K,V>: move construction and move assignment", "[array_map]")
{
	array_map<int, int> src;
	src[1] = 10;
	src[2] = 20;

	array_map<int, int> moved(std::move(src));
	REQUIRE(moved.size() == 2);
	REQUIRE(moved.at(1) == 10);
	REQUIRE(moved.at(2) == 20);

	array_map<int, int> moveAssignee;
	moveAssignee = std::move(moved);
	REQUIRE(moveAssignee.size() == 2);
	REQUIRE(moveAssignee.at(1) == 10);
	REQUIRE(moveAssignee.at(2) == 20);
}

TEST_CASE("array_map<K,V>: clear empties the container and it stays usable", "[array_map]")
{
	array_map<int, int> am;
	am[1] = 10;
	am[2] = 20;

	am.clear();

	REQUIRE(am.empty());
	REQUIRE(am.size() == 0);

	am[3] = 30;
	REQUIRE(am.size() == 1);
	REQUIRE(am.at(3) == 30);
}

TEST_CASE("array_map<K,V>: custom comparator reverses iteration order", "[array_map]")
{
	array_map<int, int, std::greater<int>> am;
	am[1] = 10;
	am[2] = 20;
	am[3] = 30;

	REQUIRE(am.get_key(0) == 3);
	REQUIRE(am.get_key(1) == 2);
	REQUIRE(am.get_key(2) == 1);

	REQUIRE(am.lower_bound(2)->first == 2);
	REQUIRE(am.contains(2));
}

TEST_CASE("array_map<K,V>: many out-of-order insertions produce a sorted map", "[array_map]")
{
	array_map<int, int> am;

	std::mt19937 rng(1234);
	std::vector<int> keys;
	for(int i = 0; i < 64; ++i)
		keys.push_back(i);
	std::shuffle(keys.begin(), keys.end(), rng);

	for(int key : keys)
		am[key] = key * 2;

	REQUIRE(am.size() == 64);

	for(size_t i = 0; i < am.size(); ++i)
	{
		REQUIRE((int)am.get_key(i) == (int)i);
	}

	for(int key : keys)
	{
		REQUIRE(am.contains(key));
		REQUIRE(am.at(key) == key * 2);
	}
}
