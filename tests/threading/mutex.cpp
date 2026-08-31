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
#include <future>
#include <thread>

using mst::threading::mutex;

TEST_CASE("threading::mutex: default construction is unlocked, and wait(0) can acquire it "
		  "immediately",
	"[thread][mutex]")
{
	mutex m;

	REQUIRE(m.wait(0));
	m.signal();
}

TEST_CASE("threading::mutex: handle() returns a non-null handle once constructed", "[thread][mutex]")
{
	mutex m;

	REQUIRE(m.handle() != nullptr);

	m.wait(0);
	m.signal();
}

TEST_CASE(
	"threading::mutex: constructing with initalOwner=true locks it for the constructing thread",
	"[thread][mutex]")
{
	mutex m{ true };

	/* a different thread must not be able to acquire it while the constructing thread still
		holds its original (initalOwner=true) lock -- unlike recursive_mutex, mutex tracks no
		owning thread, so this is only tested from a different thread */
	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(0); });
	other.join();

	REQUIRE(!acquired);

	m.signal();
}

TEST_CASE("threading::mutex: signal() releases it for other threads to acquire", "[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	std::atomic_bool acquired{ false };
	std::thread first([&] { acquired = m.wait(0); });
	first.join();
	REQUIRE(!acquired);

	m.signal();

	std::thread second([&] { acquired = m.wait(0); });
	second.join();
	REQUIRE(acquired);

	m.signal();
}

TEST_CASE("threading::mutex: wait() with no arguments blocks a second thread until signal()",
	"[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	std::promise<void> started;
	std::atomic_bool acquired{ false };

	std::thread other([&] {
		started.set_value();
		/* the default (InfiniteWait) overload */
		acquired = m.wait();
	});

	/* block until the other thread has actually started running, so the timed check below only
		has to cover its own blocking window, not thread-launch scheduling delay */
	started.get_future().wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	m.signal();
	other.join();

	REQUIRE(acquired);
	m.signal();
}

TEST_CASE("threading::mutex: wait(duration) times out if not signaled in time", "[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(std::chrono::milliseconds(50)); });
	other.join();

	REQUIRE(!acquired);
	m.signal();
}

TEST_CASE("threading::mutex: wait(duration) succeeds once another thread signals it in time",
	"[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	std::promise<void> started;
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started.set_value();
		/* bounded instead of an unbounded wait() so a regression fails the test instead of
			hanging the suite */
		acquired = m.wait(std::chrono::milliseconds(5500));
	});

	started.get_future().wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	m.signal();
	other.join();

	REQUIRE(acquired);
	m.signal();
}

TEST_CASE("threading::mutex: wait_until with a future deadline behaves like wait(duration)",
	"[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	std::promise<void> started;
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started.set_value();
		acquired = m.wait_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(5500));
	});

	started.get_future().wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	m.signal();
	other.join();

	REQUIRE(acquired);
	m.signal();
}

TEST_CASE(
	"threading::mutex: wait_until with an already-past deadline fails fast instead of blocking",
	"[thread][mutex]")
{
	mutex m;
	REQUIRE(m.wait(0));

	const auto past = std::chrono::steady_clock::now() - std::chrono::milliseconds(50);

	/* must return quickly with false -- must not be mistaken for an infinite wait */
	std::atomic_bool acquired{ true };
	std::thread other([&] { acquired = m.wait_until(past); });
	other.join();

	REQUIRE(!acquired);
	m.signal();
}
