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

using mst::threading::critical_section;

TEST_CASE("threading::critical_section: enter()/leave() can be acquired and released repeatedly",
	"[thread][critical_section]")
{
	critical_section cs;

	cs.enter();
	cs.leave();

	cs.enter();
	cs.leave();
}

TEST_CASE(
	"threading::critical_section: enter() blocks a second thread until leave()", "[thread][critical_section]")
{
	critical_section cs;
	cs.enter();

	std::atomic_bool started{ false };
	std::atomic_bool acquiredByOtherThread{ false };

	std::thread other([&] {
		started = true;
		cs.enter();
		acquiredByOtherThread = true;
		cs.leave();
	});

	/* wait for the other thread to actually be running before timing its attempt to enter() --
		this keeps the window below from having to absorb thread-launch scheduling delay, so it
		only has to cover the (much smaller, and otherwise unobservable) time between the other
		thread calling enter() and it actually blocking */
	while(!started)
	{
		std::this_thread::yield();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(!acquiredByOtherThread);

	cs.leave();

	other.join();

	REQUIRE(acquiredByOtherThread);
}

TEST_CASE("threading::critical_section: enforces mutual exclusion under concurrent contention",
	"[thread][critical_section]")
{
	critical_section cs;
	int counter = 0;

	constexpr int threadCount = 8;
	constexpr int incrementsPerThread = 1000;

	std::vector<std::thread> threads;
	threads.reserve(threadCount);

	for(int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back([&] {
			for(int j = 0; j < incrementsPerThread; ++j)
			{
				cs.enter();
				++counter;
				cs.leave();
			}
		});
	}

	for(auto& t : threads)
	{
		t.join();
	}

	/* if enter()/leave() ever let two threads in at once, some increments would be lost to the
		unguarded ++counter race, and the final count would fall short */
	REQUIRE(counter == threadCount * incrementsPerThread);
}
