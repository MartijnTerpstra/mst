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

#include <mlock_free_queue.h>
#include <vector>
#include <thread>
#include <mcommon.h>
#include <fstream>
#include <mplatform.h>
#include <mthreading_slim.h>
#include <mutex>

using namespace mst::lock_free;

std::atomic_int counter;

struct Counter
{
	Counter()
	{
		++counter;
	}
	Counter(const Counter&)
	{
		++counter;
	}
	~Counter()
	{
		--counter;
	}

	Counter& operator=(const Counter&)
	{
		return *this;
	}
};

TEST_CASE("lock_free::queue: creation", "[lock_free][queue]")
{
	queue<Counter> q;
}

TEST_CASE("lock_free::queue: single thread", "[lock_free][queue]")
{
	queue<int> q;

	for(int i = 0; i < 10; ++i)
	{
		q.push(i);
	}

	int val;

	for(int i = 0; i < 10; ++i)
	{
		REQUIRE(q.try_pop(val));
		REQUIRE(val == i);
	}
}

template<int WriterCount, int ReaderCount, typename QueueType>
void TestReadersWriters(bool sequential = false)
{
	constexpr int ELEMCOUNT = 100000;

	QueueType q;

	std::atomic_int index{ 0 };
	std::atomic_bool completed[WriterCount];

	std::vector<std::atomic_int> isVisited(ELEMCOUNT);

	std::thread readers[ReaderCount];
	std::thread writers[WriterCount];

	mst::threading::slim::event evt(false, true);

	for(auto& b : completed)
	{
		b = false;
	}

	for(auto& i : isVisited)
	{
		i = 0;
	}

	if(sequential)
	{
		for(int i = 0; i < WriterCount; ++i)
		{
			auto& t = writers[i];

			t = std::thread([&, i] {
				evt.wait();

				while(true)
				{
					int val = index++;
					if(val >= ELEMCOUNT)
						break;

					q.push(val);
				}

				completed[i].store(true);
			});
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		evt.set();

		for(auto& t : writers)
		{
			t.join();
		}

		evt.reset();
	}

	for(int i = 0; i < (int)ReaderCount; ++i)
	{
		auto& t = readers[i];

		t = std::thread([&] {
			evt.wait();

			int val;
			while(true)
			{
				while(q.try_pop(val))
				{
					++isVisited[(size_t)val];
				}

				bool exit = true;

				for(auto& b : completed)
				{
					if(!b.load())
						exit = false;
				}

				if(exit)
				{
					while(q.try_pop(val))
					{
						++isVisited[(size_t)val];
					}

					break;
				}
			}
		});
	}

	if(!sequential)
	{
		for(int i = 0; i < (int)WriterCount; ++i)
		{
			auto& t = writers[i];

			t = std::thread([&, i] {
				evt.wait();

				while(true)
				{
					int val = index++;
					if(val >= ELEMCOUNT)
						break;

					q.push(val);
				}

				completed[i].store(true);
			});
		}
	}

	std::atomic_bool quit{ false };

	std::this_thread::sleep_for(std::chrono::milliseconds(2));
	evt.set();

	if(!sequential)
	{
		for(auto& t : writers)
		{
			t.join();
		}
	}

	for(auto& t : readers)
	{
		t.join();
	}

	quit = true;

	for(size_t i = 0; i < ELEMCOUNT; ++i)
	{
		if(isVisited[i].load() != 1)
		{
			FAIL("Invalid visiting: " << i << " is visited " << isVisited[i].load() << " times");
		}
	}
}

TEST_CASE("lock_free::queue: queue multithreaded SPSC", "[lock_free][queue]")
{
	for(int i = 0; i < 50; ++i)
	{
		TestReadersWriters<1, 1, queue<int>>();
	}
}

TEST_CASE("lock_free_queue_multithreaded_mpsc", "[lock_free][queue]")
{
	for(int i = 0; i < 50; ++i)
	{
		TestReadersWriters<4, 1, queue<int>>();
	}
}

TEST_CASE("lock_free_queue_multithreaded_spmc", "[lock_free][queue]")
{
	for(int i = 0; i < 50; ++i)
	{
		TestReadersWriters<1, 4, queue<int>>();
	}
}

TEST_CASE("lock_free_queue_multithreaded_mpmc", "[lock_free][queue]")
{
	for(int i = 0; i < 50; ++i)
	{
		TestReadersWriters<4, 4, queue<int>>();
	}
}

TEST_CASE("lock_free_queue_multithreaded_mpmc_fill_first", "[lock_free][queue]")
{
	for(int i = 0; i < 50; ++i)
	{
		TestReadersWriters<4, 4, queue<int>>(true);
	}
}

TEST_CASE("lock_free_queue_precisely_full", "[lock_free][queue]")
{
	queue<int> q;
	for(int i = 0; i < (int)q.capacity_approx(); ++i)
	{
		q.push(i);
	}

	int result = -1;
	REQUIRE(q.try_pop(result));
	REQUIRE(result == 0);
}

TEST_CASE("lock_free_queue_singlethreaded_resize", "[lock_free][queue]")
{
	queue<int> q;
	int index = 0;
	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 1000; ++j)
		{
			q.push(index++);
		}

		int temp;
		REQUIRE(q.try_pop(temp));
	}


	for(int i = 0; i < 9990; ++i)
	{
		int result = -1;
		REQUIRE(q.try_pop(result));
	}

	REQUIRE(!q.try_pop(index));
}