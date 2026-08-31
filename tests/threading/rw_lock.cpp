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

using mst::threading::rw_lock;

TEST_CASE("threading::rw_lock: try_lock_read succeeds when unlocked", "[thread][rw_lock]")
{
	rw_lock rw;

	REQUIRE(rw.try_lock_read());
	rw.unlock_read();
}

TEST_CASE("threading::rw_lock: try_lock_write succeeds when unlocked", "[thread][rw_lock]")
{
	rw_lock rw;

	REQUIRE(rw.try_lock_write());
	rw.unlock_write();
}

TEST_CASE("threading::rw_lock: a writer excludes another thread's writer and reader", "[thread][rw_lock]")
{
	rw_lock rw;
	rw.lock_write();

	std::atomic_bool writerAcquired{ false };
	std::thread writer([&] { writerAcquired = rw.try_lock_write(); });
	writer.join();
	REQUIRE(!writerAcquired);

	std::atomic_bool readerAcquired{ false };
	std::thread reader([&] { readerAcquired = rw.try_lock_read(); });
	reader.join();
	REQUIRE(!readerAcquired);

	rw.unlock_write();

	REQUIRE(rw.try_lock_write());
	rw.unlock_write();
}

TEST_CASE("threading::rw_lock: a reader excludes another thread's writer but allows concurrent readers",
	"[thread][rw_lock]")
{
	rw_lock rw;
	rw.lock_read();

	std::atomic_bool writerAcquired{ false };
	std::thread writer([&] { writerAcquired = rw.try_lock_write(); });
	writer.join();
	REQUIRE(!writerAcquired);

	std::atomic_bool readerAcquired{ false };
	std::thread reader([&] {
		readerAcquired = rw.try_lock_read();
		if(readerAcquired)
		{
			rw.unlock_read();
		}
	});
	reader.join();
	REQUIRE(readerAcquired);

	rw.unlock_read();
}

TEST_CASE("threading::rw_lock: lock_write() blocks a second thread until unlock_write()", "[thread][rw_lock]")
{
	rw_lock rw;
	rw.lock_write();

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		rw.lock_write();
		acquired = true;
		rw.unlock_write();
	});

	/* wait for the other thread to actually be running so the timed check below only has to
		cover its own blocking window, not thread-launch scheduling delay */
	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	rw.unlock_write();
	other.join();

	REQUIRE(acquired);
}

TEST_CASE("threading::rw_lock: lock_write() blocks until every reader calls unlock_read()", "[thread][rw_lock]")
{
	rw_lock rw;
	rw.lock_read();

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		rw.lock_write();
		acquired = true;
		rw.unlock_write();
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	rw.unlock_read();
	other.join();

	REQUIRE(acquired);
}

TEST_CASE("threading::rw_lock: lock_read() blocks until the writer calls unlock_write()", "[thread][rw_lock]")
{
	rw_lock rw;
	rw.lock_write();

	std::atomic_bool started{ false };
	std::atomic_bool acquired{ false };
	std::thread other([&] {
		started = true;
		rw.lock_read();
		acquired = true;
		rw.unlock_read();
	});

	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquired);

	rw.unlock_write();
	other.join();

	REQUIRE(acquired);
}
