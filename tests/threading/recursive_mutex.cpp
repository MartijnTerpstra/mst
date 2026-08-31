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

using mst::threading::recursive_mutex;

TEST_CASE(
	"threading::recursive_mutex: default construction is unowned, wait(0) can acquire it immediately",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;

	REQUIRE(m.wait(0));
	m.signal();
}

TEST_CASE("threading::recursive_mutex: construction with initalOwner=true locks it for the "
		  "constructing thread",
	"[thread][recursive_mutex]")
{
	recursive_mutex m{ true };

	/* a different thread must not be able to acquire it while the constructing thread still
		holds its original (initalOwner=true) lock */
	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(0); });
	other.join();

	REQUIRE(!acquired);

	m.signal();
}

TEST_CASE("threading::recursive_mutex: the owning thread can reenter, and it only releases once "
		  "every wait() has a matching signal()",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;

	/* wait()'s fast path lets the same thread reenter as many times as it wants */
	REQUIRE(m.wait(0));
	REQUIRE(m.wait(0));
	REQUIRE(m.wait(0));

	/* a different thread cannot acquire it while any of those three locks is still held */
	auto tryAcquireFromOtherThread = [&] {
		std::atomic_bool acquired{ false };
		std::thread other([&] { acquired = m.wait(0); });
		other.join();
		return acquired.load();
	};

	REQUIRE(!tryAcquireFromOtherThread());
	m.signal();

	REQUIRE(!tryAcquireFromOtherThread());
	m.signal();

	/* the last matching signal() finally releases it */
	REQUIRE(!tryAcquireFromOtherThread());
	m.signal();

	REQUIRE(tryAcquireFromOtherThread());
}

TEST_CASE("threading::recursive_mutex: wait() with no arguments blocks until the owner signals",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;
	REQUIRE(m.wait(0));

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };

	std::thread other([&] {
		started = true;
		/* the default (InfiniteWait) overload */
		acquired = m.wait();
	});

	/* wait for the other thread to actually be running so the timed check below only has to
		cover its own blocking window, not thread-launch scheduling delay */
	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	m.signal();
	other.join();

	REQUIRE(acquired);
	m.signal();
}

TEST_CASE(
	"threading::recursive_mutex: wait() blocks a second thread until the owner fully unlocks",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;

	REQUIRE(m.wait(0));
	REQUIRE(m.wait(0)); // lock it recursively, so a single signal() must not be enough to release it

	std::atomic_bool started{ false };
	std::atomic_bool acquiredByOtherThread{ false };

	std::thread other([&] {
		started = true;
		/* bounded instead of an unbounded wait() so a regression fails the test instead of
			hanging the suite */
		acquiredByOtherThread = m.wait(std::chrono::seconds(5));
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	m.signal(); // still locked once more

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	m.signal(); // fully released now

	other.join();

	REQUIRE(acquiredByOtherThread);

	m.signal();
}

TEST_CASE("threading::recursive_mutex: wait(duration) times out if not signaled in time",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;
	REQUIRE(m.wait(0));

	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(std::chrono::milliseconds(50)); });
	other.join();

	REQUIRE(!acquired);
	m.signal();
}

TEST_CASE(
	"threading::recursive_mutex: wait_until with a future deadline blocks until signal() or the "
	"deadline, whichever comes first",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;
	REQUIRE(m.wait(0));

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		acquired = m.wait_until(std::chrono::steady_clock::now() + std::chrono::seconds(5));
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	m.signal();
	other.join();

	REQUIRE(acquired);
	m.signal();
}

TEST_CASE("threading::recursive_mutex: wait_until with an already-past deadline fails fast "
		  "instead of blocking",
	"[thread][recursive_mutex]")
{
	recursive_mutex m;
	REQUIRE(m.wait(0));

	const auto past = std::chrono::steady_clock::now() - std::chrono::milliseconds(50);

	/* must return quickly with false -- must not be mistaken for an infinite wait */
	std::atomic_bool acquired{ true };
	std::thread other([&] { acquired = m.wait_until(past); });
	other.join();

	REQUIRE(!acquired);
	m.signal();
}
