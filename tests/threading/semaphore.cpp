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

#include <mthreading.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

using mst::threading::semaphore;

TEST_CASE("threading::semaphore: default construction starts at zero permits", "[thread][semaphore]")
{
	semaphore s;

	REQUIRE(!s.wait(0));
}

TEST_CASE("threading::semaphore: construction with an initial count grants that many permits",
	"[thread][semaphore]")
{
	semaphore s{ 3 };

	REQUIRE(s.wait(0));
	REQUIRE(s.wait(0));
	REQUIRE(s.wait(0));
	REQUIRE(!s.wait(0));
}

TEST_CASE("threading::semaphore: handle() returns a non-null handle once constructed", "[thread][semaphore]")
{
	semaphore s;

	REQUIRE(s.handle() != nullptr);
}

TEST_CASE("threading::semaphore: signal() grants a single additional permit", "[thread][semaphore]")
{
	semaphore s;
	REQUIRE(!s.wait(0));

	s.signal();

	REQUIRE(s.wait(0));
	REQUIRE(!s.wait(0));
}

TEST_CASE("threading::semaphore: signal(count) grants that many additional permits", "[thread][semaphore]")
{
	semaphore s;

	s.signal(4);

	REQUIRE(s.wait(0));
	REQUIRE(s.wait(0));
	REQUIRE(s.wait(0));
	REQUIRE(s.wait(0));
	REQUIRE(!s.wait(0));
}

TEST_CASE("threading::semaphore: wait() with no arguments blocks until another thread signals it",
	"[thread][semaphore]")
{
	semaphore s;

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };

	std::thread other([&] {
		started = true;
		/* the default (InfiniteWait) overload */
		acquired = s.wait();
	});

	/* wait for the other thread to actually be running so the timed check below only has to
		cover its own blocking window, not thread-launch scheduling delay */
	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	s.signal();
	other.join();

	REQUIRE(acquired);
}

TEST_CASE("threading::semaphore: wait(duration) times out if not signaled in time", "[thread][semaphore]")
{
	semaphore s;

	REQUIRE(!s.wait(std::chrono::milliseconds(50)));
}

TEST_CASE("threading::semaphore: wait(duration) succeeds once another thread signals it in time",
	"[thread][semaphore]")
{
	semaphore s;

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		/* bounded instead of an unbounded wait() so a regression fails the test instead of
			hanging the suite */
		acquired = s.wait(std::chrono::seconds(5));
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	s.signal();
	other.join();

	REQUIRE(acquired);
}

TEST_CASE("threading::semaphore: wait_until with a future deadline behaves like wait(duration)",
	"[thread][semaphore]")
{
	semaphore s;

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		acquired = s.wait_until(std::chrono::steady_clock::now() + std::chrono::seconds(5));
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	s.signal();
	other.join();

	REQUIRE(acquired);
}

TEST_CASE(
	"threading::semaphore: wait_until with an already-past deadline fails fast instead of blocking",
	"[thread][semaphore]")
{
	semaphore s;

	const auto past = std::chrono::steady_clock::now() - std::chrono::milliseconds(50);

	/* must return quickly with false -- must not be mistaken for an infinite wait */
	REQUIRE(!s.wait_until(past));
}

TEST_CASE("threading::semaphore: permits acquired concurrently never exceed what was granted",
	"[thread][semaphore]")
{
	constexpr int permitCount = 10;
	constexpr int threadCount = 40;

	semaphore s{ (uint32_t)permitCount };

	std::atomic_int acquiredCount{ 0 };
	std::vector<std::thread> threads;
	threads.reserve(threadCount);

	for(int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back([&] {
			if(s.wait(0))
			{
				++acquiredCount;
			}
		});
	}

	for(auto& t : threads)
	{
		t.join();
	}

	/* exactly permitCount of the threads should have won a permit -- no more (over-subscription)
		and no fewer (a permit lost to a race) */
	REQUIRE(acquiredCount.load() == permitCount);
	REQUIRE(!s.wait(0));
}
