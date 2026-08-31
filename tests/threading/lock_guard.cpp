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
#include <type_traits>
#include <utility>

using mst::threading::critical_section;
using mst::threading::lock_guard;
using mst::threading::mutex;

static_assert(!std::is_copy_constructible<lock_guard<mutex>>::value,
	"lock_guard must not be copy-constructible");
static_assert(
	!std::is_copy_assignable<lock_guard<mutex>>::value, "lock_guard must not be copy-assignable");
static_assert(
	std::is_move_constructible<lock_guard<mutex>>::value, "lock_guard must be move-constructible");

TEST_CASE("threading::lock_guard: construction locks a mutex, destruction releases it", "[thread][lock_guard]")
{
	mutex m;

	{
		lock_guard<mutex> g(m);

		/* held by the guard -- a non-blocking acquire attempt from a different thread must fail.
			(checking from the constructing thread itself wouldn't prove anything: the underlying
			Win32 mutex handle lets its owning thread reacquire without blocking) */
		std::atomic_bool acquired{ true };
		std::thread other([&] { acquired = m.wait(0); });
		other.join();
		REQUIRE(!acquired);
	}

	/* released once the guard's destructor ran */
	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(0); });
	other.join();
	REQUIRE(acquired);

	m.signal();
}

TEST_CASE("threading::lock_guard: construction locks a critical_section, destruction releases it",
	"[thread][lock_guard]")
{
	critical_section cs;

	std::atomic_bool started{ false };
	std::atomic_bool acquiredByOtherThread{ false };
	std::thread other;

	{
		lock_guard<critical_section> g(cs);

		other = std::thread([&] {
			started = true;
			cs.enter();
			acquiredByOtherThread = true;
			cs.leave();
		});

		/* wait for the other thread to actually be running so the timed check below only has
			to cover its own blocking window, not thread-launch scheduling delay */
		while(!started)
		{
			std::this_thread::yield();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		REQUIRE(!acquiredByOtherThread);

		// g's destructor runs here, releasing cs
	}

	other.join();
	REQUIRE(acquiredByOtherThread);
}

TEST_CASE("threading::lock_guard: move construction transfers ownership, and only the final "
		  "owner releases the lock",
	"[thread][lock_guard]")
{
	mutex m;

	{
		lock_guard<mutex> first(m);
		lock_guard<mutex> second(std::move(first));

		/* still held -- ownership moved, not released, and not reacquired either. Checked from a
			different thread since the underlying Win32 mutex handle lets its owning thread
			(this one) reacquire without blocking. */
		std::atomic_bool acquired{ true };
		std::thread other([&] { acquired = m.wait(0); });
		other.join();
		REQUIRE(!acquired);

		/* the moved-from guard's own destructor runs at the end of this scope too; if it still
			held a pointer to m, it would signal() a second time here */
	}

	/* released exactly once, by the (only) owning guard's destructor -- if the moved-from guard
		had also called signal(), the underlying handle would be over-released */
	std::atomic_bool acquired{ false };
	std::thread other([&] { acquired = m.wait(0); });
	other.join();
	REQUIRE(acquired);

	m.signal();
}
