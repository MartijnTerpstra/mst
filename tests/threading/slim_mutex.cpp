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

using mst::threading::slim::mutex;

TEST_CASE("threading::slim::mutex: creation unowned starts unlocked", "[thread][slim][mutex]")
{
	mutex m{ false };

	REQUIRE(m.try_wait());

	m.unlock();
}

TEST_CASE("threading::slim::mutex: creation owned starts locked", "[thread][slim][mutex]")
{
	mutex m{ true };

	REQUIRE(!m.try_wait());

	m.unlock();
}

TEST_CASE("threading::slim::mutex: unlike recursive_mutex, the owning thread cannot reenter it",
	"[thread][slim][mutex]")
{
	mutex m{ false };

	m.lock();

	/* a second try_wait() from the very same thread that already holds it must fail -- unlike
		recursive_mutex, mutex tracks no owning thread and allows no reentrancy */
	REQUIRE(!m.try_wait());

	m.unlock();
}

TEST_CASE("threading::slim::mutex: unlock() releases it for other threads to acquire",
	"[thread][slim][mutex]")
{
	mutex m{ false };

	m.lock();
	REQUIRE(!m.try_wait());

	m.unlock();
	REQUIRE(m.try_wait());

	m.unlock();
}

TEST_CASE("threading::slim::mutex: a second thread cannot acquire it while locked",
	"[thread][slim][mutex]")
{
	mutex m{ false };

	m.lock();

	std::atomic_bool acquiredByOtherThread{ false };
	std::thread other([&] { acquiredByOtherThread = m.try_wait(); });
	other.join();

	REQUIRE(!acquiredByOtherThread);

	m.unlock();
}

TEST_CASE(
	"threading::slim::mutex: wait() blocks a second thread until unlock()", "[thread][slim][mutex]")
{
	mutex m{ false };

	m.lock();

	std::atomic_bool acquiredByOtherThread{ false };

	std::thread other([&] {
		/* bounded instead of an unbounded wait() so a regression fails the test instead of
			hanging the suite */
		acquiredByOtherThread = m.wait_for(std::chrono::seconds(5));
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	m.unlock();

	other.join();

	REQUIRE(acquiredByOtherThread);

	m.unlock();
}
