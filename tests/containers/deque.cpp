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
#include <random_data_generator.h>

#include <deque>
#include <string>
#include <utility>
#include <mdeque.h>

using mst::deque;
using namespace mst::test_util;

TEST_CASE("deque<T>: creation", "[deque]")
{
	deque<int> container;

	REQUIRE(container.empty());
	REQUIRE(container.size() == 0);
	REQUIRE(container.capacity() == 0);
}

TEST_CASE("deque<T>: push_back/pop_back", "[deque]")
{
	deque<int> container;

	container.push_back(1);
	container.push_back(2);
	container.push_back(3);

	REQUIRE(container.size() == 3);
	REQUIRE(container.front() == 1);
	REQUIRE(container.back() == 3);
	REQUIRE(container[0] == 1);
	REQUIRE(container[1] == 2);
	REQUIRE(container[2] == 3);

	container.pop_back();

	REQUIRE(container.size() == 2);
	REQUIRE(container.back() == 2);

	container.pop_back();
	container.pop_back();

	REQUIRE(container.empty());
}

TEST_CASE("deque<T>: push_front/pop_front", "[deque]")
{
	deque<int> container;

	container.push_front(1);
	container.push_front(2);
	container.push_front(3);

	REQUIRE(container.size() == 3);
	REQUIRE(container.front() == 3);
	REQUIRE(container.back() == 1);
	REQUIRE(container[0] == 3);
	REQUIRE(container[1] == 2);
	REQUIRE(container[2] == 1);

	container.pop_front();

	REQUIRE(container.size() == 2);
	REQUIRE(container.front() == 2);

	container.pop_front();
	container.pop_front();

	REQUIRE(container.empty());
}

TEST_CASE("deque<T>: mixed push/pop front and back wraps around the buffer", "[deque]")
{
	deque<int> container;

	// force a few grows and wraps: alternate ends so the internal head keeps moving
	for(int i = 0; i < 64; ++i)
	{
		if((i % 2) == 0)
		{
			container.push_back(i);
		}
		else
		{
			container.push_front(i);
		}
	}

	REQUIRE(container.size() == 64);

	// capacity must always be a power of two (or zero)
	REQUIRE((container.capacity() & (container.capacity() - 1)) == 0);

	while(!container.empty())
	{
		if((container.size() % 2) == 0)
		{
			container.pop_back();
		}
		else
		{
			container.pop_front();
		}
	}

	REQUIRE(container.empty());
}

TEST_CASE("deque<T>: random push/pop matches std::deque", "[deque]")
{
	random_data_generator rdg{ true };
	INFO("Seed" << rdg.seed());

	deque<int> container;
	std::deque<int> reference;

	for(int i = 0; i < 4096; ++i)
	{
		const auto op = rdg.scalar_int<int>(0, 5);

		switch(op)
		{
		case 0:
			container.push_back(i);
			reference.push_back(i);
			break;
		case 1:
			container.push_front(i);
			reference.push_front(i);
			break;
		case 2:
			if(!container.empty())
			{
				container.pop_back();
				reference.pop_back();
			}
			break;
		case 3:
			if(!container.empty())
			{
				container.pop_front();
				reference.pop_front();
			}
			break;
		case 4:
		{
			const auto idx = rdg.scalar_int<size_t>(0, container.size());
			container.insert(container.begin() + (ptrdiff_t)idx, i);
			reference.insert(reference.begin() + (ptrdiff_t)idx, i);
			break;
		}
		case 5:
			if(!container.empty())
			{
				const auto idx = rdg.scalar_int<size_t>(0, container.size() - 1);
				container.erase(container.begin() + (ptrdiff_t)idx);
				reference.erase(reference.begin() + (ptrdiff_t)idx);
			}
			break;
		}

		REQUIRE(container.size() == reference.size());

		for(size_t j = 0; j < reference.size(); ++j)
		{
			REQUIRE(container[j] == reference[j]);
		}
	}
}

TEST_CASE("deque<T>: erase single element", "[deque]")
{
	deque<int> container{ 1, 2, 3, 4, 5 };

	// erase closer to the front (shifts the front side)
	auto it = container.erase(container.begin() + 1);

	REQUIRE(container.size() == 4);
	REQUIRE(*it == 3);
	REQUIRE((container == deque<int>{ 1, 3, 4, 5 }));

	// erase closer to the back (shifts the back side): {1,3,4,5} -> erase index 2 ('4') -> {1,3,5}
	it = container.erase(container.begin() + 2);

	REQUIRE(container.size() == 3);
	REQUIRE(*it == 5);
	REQUIRE((container == deque<int>{ 1, 3, 5 }));

	// erasing the actual last element does yield end()
	it = container.erase(container.end() - 1);
	REQUIRE(it == container.end());
	REQUIRE((container == deque<int>{ 1, 3 }));

	// erase the front and back elements directly
	container = { 1, 2, 3 };
	container.erase(container.begin());
	REQUIRE((container == deque<int>{ 2, 3 }));

	container.erase(container.begin() + 1);
	REQUIRE((container == deque<int>{ 2 }));
}

TEST_CASE("deque<T>: erase range", "[deque]")
{
	deque<int> container{ 1, 2, 3, 4, 5, 6, 7, 8 };

	// range closer to the front
	auto it = container.erase(container.begin() + 1, container.begin() + 3);

	REQUIRE(container.size() == 6);
	REQUIRE(*it == 4);
	REQUIRE((container == deque<int>{ 1, 4, 5, 6, 7, 8 }));

	// range closer to the back: {1,4,5,6,7,8} -> erase [3,5) ('6','7') -> {1,4,5,8}
	it = container.erase(container.begin() + 3, container.begin() + 5);

	REQUIRE(container.size() == 4);
	REQUIRE(*it == 8);
	REQUIRE((container == deque<int>{ 1, 4, 5, 8 }));

	// erasing through the actual end() does yield end()
	it = container.erase(container.end() - 1, container.end());
	REQUIRE(it == container.end());
	REQUIRE((container == deque<int>{ 1, 4, 5 }));

	// empty range is a no-op
	it = container.erase(container.begin() + 2, container.begin() + 2);
	REQUIRE(container.size() == 3);
	REQUIRE(*it == 5);

	// erasing the whole thing empties it
	container.erase(container.begin(), container.end());
	REQUIRE(container.empty());
}

TEST_CASE("deque<T>: insert single element", "[deque]")
{
	deque<int> container{ 1, 2, 3, 4, 5 };

	// insert closer to the front (shifts the front side)
	auto it = container.insert(container.begin() + 1, 42);

	REQUIRE(container.size() == 6);
	REQUIRE(*it == 42);
	REQUIRE((container == deque<int>{ 1, 42, 2, 3, 4, 5 }));

	// insert closer to the back (shifts the back side)
	it = container.insert(container.begin() + 4, 99);

	REQUIRE(container.size() == 7);
	REQUIRE(*it == 99);
	REQUIRE((container == deque<int>{ 1, 42, 2, 3, 99, 4, 5 }));

	// insert at begin()/end() matches push_front()/push_back()
	container = { 1, 2, 3 };
	container.insert(container.begin(), 0);
	REQUIRE((container == deque<int>{ 0, 1, 2, 3 }));

	container.insert(container.end(), 4);
	REQUIRE((container == deque<int>{ 0, 1, 2, 3, 4 }));

	// insert into an empty deque
	deque<int> empty;
	empty.insert(empty.begin(), 7);
	REQUIRE((empty == deque<int>{ 7 }));

	// move-insert
	deque<std::string> strings{ "a", "c" };
	std::string movedIn = "b";
	strings.insert(strings.begin() + 1, std::move(movedIn));
	REQUIRE(strings[0] == "a");
	REQUIRE(strings[1] == "b");
	REQUIRE(strings[2] == "c");
}

TEST_CASE("deque<T>: insert count copies", "[deque]")
{
	deque<int> container{ 1, 2, 3 };

	auto it = container.insert(container.begin() + 1, 3, 9);

	REQUIRE(container.size() == 6);
	REQUIRE(*it == 9);
	REQUIRE((container == deque<int>{ 1, 9, 9, 9, 2, 3 }));

	// inserting zero copies is a no-op
	it = container.insert(container.begin() + 2, 0, 5);
	REQUIRE(container.size() == 6);
	REQUIRE(*it == 9);
}

TEST_CASE("deque<T>: emplace forwards constructor arguments", "[deque]")
{
	deque<std::pair<int, int>> container{ { 1, 1 }, { 3, 3 } };

	auto it = container.emplace(container.begin() + 1, 2, 2);

	REQUIRE(container.size() == 3);
	REQUIRE(*it == std::make_pair(2, 2));
	REQUIRE(container[0] == std::make_pair(1, 1));
	REQUIRE(container[1] == std::make_pair(2, 2));
	REQUIRE(container[2] == std::make_pair(3, 3));
}

TEST_CASE("deque<T>: reserve and shrink_to_fit", "[deque]")
{
	deque<int> container;

	container.reserve(5);

	REQUIRE(container.capacity() == 8);

	for(int i = 0; i < 5; ++i)
	{
		container.push_back(i);
	}

	REQUIRE(container.capacity() == 8);

	container.shrink_to_fit();

	REQUIRE(container.capacity() == 8);

	container.pop_back();
	container.pop_back();
	container.shrink_to_fit();

	REQUIRE(container.capacity() == 4);
	REQUIRE(container.size() == 3);

	container.clear();
	container.shrink_to_fit();

	REQUIRE(container.capacity() == 0);
}

TEST_CASE("deque<T>: initializer_list, copy and move", "[deque]")
{
	deque<int> container{ 1, 2, 3, 4, 5 };

	REQUIRE(container.size() == 5);
	REQUIRE(container[0] == 1);
	REQUIRE(container[4] == 5);

	deque<int> copyContainer(container);

	REQUIRE(copyContainer == container);

	// same size, different contents: exercises the element-mismatch branch of operator==
	copyContainer[2] = 42;

	REQUIRE(copyContainer != container);

	copyContainer[2] = 3;
	copyContainer.push_back(6);

	REQUIRE(copyContainer != container);

	deque<int> moveContainer(std::move(copyContainer));

	REQUIRE(moveContainer.size() == 6);
	REQUIRE(moveContainer.back() == 6);
}

TEST_CASE("deque<T>: copy assignment, move assignment and swap", "[deque]")
{
	deque<int> container{ 1, 2, 3 };
	deque<int> other{ 4, 5 };

	other = container;

	REQUIRE(other == container);

	other.push_back(4);
	deque<int> moved{ 9, 9, 9, 9, 9 };
	moved = std::move(other);

	REQUIRE(moved.size() == 4);
	REQUIRE(moved.back() == 4);

	// self-assignment must be a no-op, not a use-after-free
	moved = moved;
	REQUIRE(moved.size() == 4);
	moved = std::move(moved);
	REQUIRE(moved.size() == 4);

	deque<int> a{ 1, 2, 3 };
	deque<int> b{ 4, 5 };

	a.swap(b);

	REQUIRE(a.size() == 2);
	REQUIRE(a[0] == 4);
	REQUIRE(b.size() == 3);
	REQUIRE(b[0] == 1);

	swap(a, b);

	REQUIRE(a.size() == 3);
	REQUIRE(a[0] == 1);
	REQUIRE(b.size() == 2);
	REQUIRE(b[0] == 4);
}

TEST_CASE("deque<T>: iterators", "[deque]")
{
	deque<int> container{ 1, 2, 3, 4, 5 };

	int expected = 1;
	for(auto it = container.begin(); it != container.end(); ++it)
	{
		REQUIRE(*it == expected);
		++expected;
	}

	REQUIRE(std::distance(container.begin(), container.end()) == 5);

	expected = 5;
	for(auto it = container.rbegin(); it != container.rend(); ++it)
	{
		REQUIRE(*it == expected);
		--expected;
	}

	auto it = container.begin() + 2;
	REQUIRE(*it == 3);
	REQUIRE(it[1] == 4);

	const deque<int>& constContainer = container;

	expected = 1;
	for(auto cit = constContainer.begin(); cit != constContainer.end(); ++cit)
	{
		REQUIRE(*cit == expected);
		++expected;
	}

	expected = 1;
	for(auto cit = container.cbegin(); cit != container.cend(); ++cit)
	{
		REQUIRE(*cit == expected);
		++expected;
	}

	expected = 5;
	for(auto cit = container.crbegin(); cit != container.crend(); ++cit)
	{
		REQUIRE(*cit == expected);
		--expected;
	}

	auto cit = constContainer.cbegin() + 2;
	REQUIRE(*cit == 3);
	REQUIRE(cit[1] == 4);
	REQUIRE((cit - constContainer.cbegin()) == 2);

	// deque_iterator -> deque_const_iterator conversion
	deque<int>::const_iterator converted = container.begin();
	REQUIRE(converted == container.cbegin());
}

TEST_CASE("deque<T>: resize", "[deque]")
{
	deque<int> container;

	container.resize(4, 7);

	REQUIRE(container.size() == 4);
	for(size_t i = 0; i < container.size(); ++i)
	{
		REQUIRE(container[i] == 7);
	}

	container.resize(2);

	REQUIRE(container.size() == 2);
	REQUIRE(container[0] == 7);
	REQUIRE(container[1] == 7);

	// growth path of the value-less overload: new elements are value-initialized
	container.resize(5);

	REQUIRE(container.size() == 5);
	REQUIRE(container[0] == 7);
	REQUIRE(container[1] == 7);
	REQUIRE(container[2] == 0);
	REQUIRE(container[3] == 0);
	REQUIRE(container[4] == 0);

	// shrink path of the value-taking overload
	container.resize(1, 99);

	REQUIRE(container.size() == 1);
	REQUIRE(container[0] == 7);
}

TEST_CASE("deque<T>: should call T constructor & destructor", "[deque]")
{
	static int32_t createdCount = 0;

	struct data
	{
	public:
		data()
		{
			++createdCount;
		}
		data(const data&)
		{
			++createdCount;
		}
		data(data&&) noexcept
		{
			++createdCount;
		}
		~data()
		{
			--createdCount;
		}
	};

	{
		deque<data> container;

		for(int i = 0; i < 1024; ++i)
		{
			container.push_back(data());
		}

		for(int i = 0; i < 512; ++i)
		{
			container.pop_front();
		}

		for(int i = 0; i < 512; ++i)
		{
			container.push_front(data());
		}

		container.clear();
	}

	REQUIRE(createdCount == 0);
}
