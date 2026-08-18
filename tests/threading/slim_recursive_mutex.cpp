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

#include <mthreading_slim.h>

#include <atomic>
#include <chrono>
#include <thread>

using mst::threading::slim::recursive_mutex;

TEST_CASE("threading::slim::recursive_mutex: creation", "[thread][slim][recursive_mutex]")
{
	recursive_mutex m{ false };

	/* an unowned mutex isn't locked by anyone yet */
	REQUIRE(m.try_wait());

	m.unlock();
}

TEST_CASE("threading::slim::recursive_mutex: creation with owned=true locks it for the "
		  "constructing thread",
	"[thread][slim][recursive_mutex]")
{
	recursive_mutex m{ true };

	/* the constructing thread already holds it, so it can reenter without blocking */
	REQUIRE(m.try_wait());
	m.unlock();

	/* a different thread must not be able to acquire it while the constructing thread still
		holds its original (owned=true) lock plus the reentrant lock above */
	std::atomic_bool acquiredByOtherThread{ false };

	std::thread other([&] { acquiredByOtherThread = m.try_wait(); });
	other.join();

	REQUIRE(!acquiredByOtherThread);

	m.unlock();
}

TEST_CASE("threading::slim::recursive_mutex: the owning thread can reenter, and it only "
		  "releases once every lock has a matching unlock",
	"[thread][slim][recursive_mutex]")
{
	recursive_mutex m{ false };

	/* try_wait/lock lets the same thread reenter as many times as it wants */
	REQUIRE(m.try_wait());
	REQUIRE(m.try_wait());
	REQUIRE(m.try_wait());

	/* a different thread cannot acquire it while any of those three locks is still held */
	auto tryAcquireFromOtherThread = [&] {
		std::atomic_bool acquired{ false };
		std::thread other([&] { acquired = m.try_wait(); });
		other.join();
		return acquired.load();
	};

	REQUIRE(!tryAcquireFromOtherThread());
	m.unlock();

	REQUIRE(!tryAcquireFromOtherThread());
	m.unlock();

	/* the last matching unlock finally releases it */
	REQUIRE(!tryAcquireFromOtherThread());
	m.unlock();

	REQUIRE(tryAcquireFromOtherThread());
}

TEST_CASE("threading::slim::recursive_mutex: wait() blocks a second thread until the owner "
		  "fully unlocks",
	"[thread][slim][recursive_mutex]")
{
	recursive_mutex m{ false };

	m.lock();
	m.lock(); // lock it recursively, so a single unlock() must not be enough to release it

	std::atomic_bool acquiredByOtherThread{ false };

	std::thread other([&] {
		/* bounded instead of an unbounded wait() so a regression fails the test instead of
			hanging the suite */
		acquiredByOtherThread = m.wait_for(std::chrono::seconds(5));
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	m.unlock(); // still locked once more

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	m.unlock(); // fully released now

	other.join();

	REQUIRE(acquiredByOtherThread);

	m.unlock();
}
