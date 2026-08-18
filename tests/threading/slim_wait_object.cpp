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
#include <functional>
#include <thread>

using mst::threading::slim::event;
using mst::threading::slim::wait_object;

TEST_CASE("threading::slim::wait_object: wait_all returns once every object is already signaled",
	"[thread][slim][wait_object]")
{
	event a{ true, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	wait_object::wait_all(objects);

	/* manual-reset events stay signaled */
	REQUIRE(a.try_wait());
	REQUIRE(b.try_wait());
}

TEST_CASE("threading::slim::wait_object: wait_all_for times out if not every object becomes "
		  "ready in time",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(!wait_object::wait_all_for(objects, std::chrono::milliseconds(50)));
}

TEST_CASE("threading::slim::wait_object: wait_all_for succeeds once every object is signaled",
	"[thread][slim][wait_object]")
{
	event a{ true, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_all_for(objects, std::chrono::milliseconds(50)));
}

TEST_CASE("threading::slim::wait_object: wait_all_for/wait_all_until with zero objects succeed "
		  "immediately",
	"[thread][slim][wait_object]")
{
	REQUIRE(wait_object::wait_all_for(nullptr, 0, std::chrono::milliseconds(10)));
	REQUIRE(wait_object::wait_all_until(
		nullptr, 0, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10)));
}

TEST_CASE("threading::slim::wait_object: wait_all_for with a zero duration still requires every "
		  "object to be ready",
	"[thread][slim][wait_object]")
{
	event a{ false, true }; // not signaled
	event b{ true, true };	// signaled

	const wait_object* objects[] = { &a, &b };

	/* a zero duration means "check right now, don't spin" -- it must not succeed just because
	 *one* of the two objects happens to already be ready */
	REQUIRE(!wait_object::wait_all_for(objects, std::chrono::seconds(0)));
}

TEST_CASE("threading::slim::wait_object: wait_all_until with an already-past deadline still "
		  "requires every object to be ready",
	"[thread][slim][wait_object]")
{
	event a{ false, true }; // not signaled
	event b{ true, true };	// signaled

	const wait_object* objects[] = { &a, &b };
	const auto past = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(50);

	REQUIRE(!wait_object::wait_all_until(objects, past));

	event c{ true, true };
	const wait_object* allReady[] = { &b, &c };
	REQUIRE(wait_object::wait_all_until(allReady, past));
}

TEST_CASE("threading::slim::wait_object: wait_any_for with a zero duration checks once without "
		  "waiting",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_any_for(objects, std::chrono::seconds(0)) == 1);

	event c{ false, true };
	const wait_object* noneReady[] = { &a, &c };
	REQUIRE(wait_object::wait_any_for(noneReady, std::chrono::seconds(0)) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_until with an already-past deadline checks "
		  "once without waiting",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	const auto past = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(50);

	REQUIRE(wait_object::wait_any_until(objects, past) == 1);

	event c{ false, true };
	const wait_object* noneReady[] = { &a, &c };
	REQUIRE(wait_object::wait_any_until(noneReady, past) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_for/wait_until on a single object with a "
		  "zero/past deadline act like try_wait",
	"[thread][slim][wait_object]")
{
	event ready{ true, true };
	event notReady{ false, true };

	REQUIRE(ready.wait_for(std::chrono::seconds(0)));
	REQUIRE(!notReady.wait_for(std::chrono::seconds(0)));

	const auto past = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(50);
	REQUIRE(ready.wait_until(past));
	REQUIRE(!notReady.wait_until(past));
}

TEST_CASE("threading::slim::wait_object: wait_until on a single object blocks until the "
		  "deadline or the object becomes ready",
	"[thread][slim][wait_object]")
{
	event neverSignaled{ false, true };
	REQUIRE(!neverSignaled.wait_until(
		std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50)));

	event alreadySignaled{ true, true };
	REQUIRE(alreadySignaled.wait_until(
		std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50)));
}

TEST_CASE("threading::slim::wait_object: wait_all_until behaves like wait_all_for",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* notReady[] = { &a, &b };
	REQUIRE(!wait_object::wait_all_until(
		notReady, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50)));

	event c{ true, true };
	const wait_object* ready[] = { &b, &c };
	REQUIRE(wait_object::wait_all_until(
		ready, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50)));
}

TEST_CASE("threading::slim::wait_object: wait_any_for returns the index of whichever object is "
		  "already signaled",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_any_for(objects, std::chrono::milliseconds(50)) == 1);
}

TEST_CASE(
	"threading::slim::wait_object: wait_any_for times out returning -1 when none become ready",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_any_for(objects, std::chrono::milliseconds(50)) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_for/wait_any_until with zero objects return -1 "
		  "immediately",
	"[thread][slim][wait_object]")
{
	REQUIRE(wait_object::wait_any_for(nullptr, 0, std::chrono::milliseconds(10)) == (size_t)-1);
	REQUIRE(wait_object::wait_any_until(nullptr, 0,
				std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10)) ==
			(size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_until behaves like wait_any_for",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_any_until(objects,
				std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50)) == 1);
}

TEST_CASE("threading::slim::wait_object: the pointer+count overloads match the array overloads",
	"[thread][slim][wait_object]")
{
	event a{ true, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };

	wait_object::wait_all(objects, 2);
	REQUIRE(wait_object::wait_all_for(objects, (size_t)2, std::chrono::milliseconds(10)));
	REQUIRE(wait_object::wait_any(objects, 2) < 2);
	REQUIRE(wait_object::wait_any_for(objects, (size_t)2, std::chrono::milliseconds(10)) < 2);
}

TEST_CASE("threading::slim::wait_object: the initializer_list overloads accept wait objects by "
		  "reference, without needing to build a pointer array",
	"[thread][slim][wait_object]")
{
	event a{ true, true };
	event b{ true, true };

	/* wait_all/wait_any */
	wait_object::wait_all({ a, b });
	REQUIRE(wait_object::wait_any({ a, b }) < 2);

	/* wait_all_for/wait_any_for */
	REQUIRE(wait_object::wait_all_for({ a, b }, std::chrono::milliseconds(10)));
	REQUIRE(wait_object::wait_any_for({ a, b }, std::chrono::milliseconds(10)) < 2);

	event c{ false, true };
	REQUIRE(!wait_object::wait_all_for({ a, c }, std::chrono::milliseconds(10)));
	REQUIRE(wait_object::wait_any_for({ a, c }, std::chrono::milliseconds(10)) == 0);

	/* wait_all_until/wait_any_until */
	const auto deadline = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10);
	REQUIRE(wait_object::wait_all_until({ a, b }, deadline));
	REQUIRE(wait_object::wait_any_until({ a, b }, deadline) < 2);
}

TEST_CASE("threading::slim::wait_object: wait_all blocks until every object is signaled by other "
		  "threads",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	const wait_object* objects[] = { &a, &b };

	std::atomic_bool completed{ false };
	std::thread waiter([&] {
		wait_object::wait_all(objects);
		completed = true;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!completed);

	a.set();

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!completed); // b is still unsignaled

	b.set();
	waiter.join();

	REQUIRE(completed);
}

TEST_CASE("threading::slim::wait_object: wait_any returns as soon as any single object is signaled",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	const wait_object* objects[] = { &a, &b };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		b.set();
	});

	const size_t index = wait_object::wait_any(objects, 2);
	setter.join();

	REQUIRE(index == 1);
	REQUIRE(!a.try_wait());
}

TEST_CASE("threading::slim::wait_object: wait_any(initializer_list) blocks until one object is "
		  "signaled by another thread",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		b.set();
	});

	const size_t index = wait_object::wait_any({ a, b });
	setter.join();

	REQUIRE(index == 1);
}

TEST_CASE("threading::slim::wait_object: wait_for on a single object times out and returns "
		  "false if it never becomes ready",
	"[thread][slim][wait_object]")
{
	event neverSignaled{ false, true };
	REQUIRE(!neverSignaled.wait_for(std::chrono::milliseconds(30)));
}

TEST_CASE("threading::slim::wait_object: wait_all/wait_any treat zero wait objects as trivially "
		  "satisfied",
	"[thread][slim][wait_object]")
{
	wait_object::wait_all(nullptr, 0); // must not block or crash

	REQUIRE(wait_object::wait_any(nullptr, 0) == (size_t)-1);

	std::initializer_list<std::reference_wrapper<wait_object>> empty{};
	REQUIRE(wait_object::wait_any(empty) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_all_for with a zero duration succeeds when every "
		  "object is already ready",
	"[thread][slim][wait_object]")
{
	event a{ true, true };
	event b{ true, true };

	const wait_object* objects[] = { &a, &b };
	REQUIRE(wait_object::wait_all_for(objects, std::chrono::seconds(0)));

	/* the initializer_list overload takes the same zero-duration fast path */
	REQUIRE(wait_object::wait_all_for({ a, b }, std::chrono::seconds(0)));

	event c{ false, true };
	REQUIRE(!wait_object::wait_all_for({ a, c }, std::chrono::seconds(0)));
}

TEST_CASE("threading::slim::wait_object: wait_all_until(initializer_list) with an already-past "
		  "deadline still requires every object to be ready",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };
	const auto past = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(50);

	REQUIRE(!wait_object::wait_all_until({ a, b }, past));

	event c{ true, true };
	REQUIRE(wait_object::wait_all_until({ b, c }, past));
}

TEST_CASE("threading::slim::wait_object: wait_all_until(initializer_list) blocks until every "
		  "object is signaled, or times out",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		a.set();
		b.set();
	});

	REQUIRE(wait_object::wait_all_until(
		{ a, b }, std::chrono::high_resolution_clock::now() + std::chrono::seconds(5)));
	setter.join();

	event c{ false, true };
	REQUIRE(!wait_object::wait_all_until(
		{ b, c }, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(30)));
}

TEST_CASE("threading::slim::wait_object: wait_any_for/wait_any_until(initializer_list) with a "
		  "zero duration or past deadline check once without waiting",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ true, true };

	REQUIRE(wait_object::wait_any_for({ a, b }, std::chrono::seconds(0)) == 1);

	event c{ false, true };
	REQUIRE(wait_object::wait_any_for({ a, c }, std::chrono::seconds(0)) == (size_t)-1);

	const auto past = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(50);
	REQUIRE(wait_object::wait_any_until({ a, b }, past) == 1);
	REQUIRE(wait_object::wait_any_until({ a, c }, past) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_for(initializer_list) spins until one object "
		  "is signaled by another thread, or times out",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		b.set();
	});

	REQUIRE(wait_object::wait_any_for({ a, b }, std::chrono::seconds(5)) == 1);
	setter.join();

	event c{ false, true };
	REQUIRE(wait_object::wait_any_for({ a, c }, std::chrono::milliseconds(30)) == (size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_until spins until one object is signaled by "
		  "another thread, or times out",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	const wait_object* objects[] = { &a, &b };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		b.set();
	});

	REQUIRE(wait_object::wait_any_until(
				objects, std::chrono::high_resolution_clock::now() + std::chrono::seconds(5)) == 1);
	setter.join();

	event c{ false, true };
	const wait_object* noneReady[] = { &a, &c };
	REQUIRE(wait_object::wait_any_until(noneReady,
				std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(30)) ==
			(size_t)-1);
}

TEST_CASE("threading::slim::wait_object: wait_any_until(initializer_list) spins until one "
		  "object is signaled by another thread, or times out",
	"[thread][slim][wait_object]")
{
	event a{ false, true };
	event b{ false, true };

	std::thread setter([&] {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		b.set();
	});

	REQUIRE(wait_object::wait_any_until({ a, b },
				std::chrono::high_resolution_clock::now() + std::chrono::seconds(5)) == 1);
	setter.join();

	event c{ false, true };
	REQUIRE(wait_object::wait_any_until({ a, c }, std::chrono::high_resolution_clock::now() +
													  std::chrono::milliseconds(30)) == (size_t)-1);
}
