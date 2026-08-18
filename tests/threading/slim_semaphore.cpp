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
#include <thread>
#include <vector>

using mst::threading::slim::semaphore;

TEST_CASE("threading::slim::semaphore: default construction starts at zero permits",
	"[thread][slim][semaphore]")
{
	semaphore s;

	REQUIRE(!s.try_wait());
}

TEST_CASE("threading::slim::semaphore: construction with an initial count grants that many permits",
	"[thread][slim][semaphore]")
{
	semaphore s{ 3 };

	REQUIRE(s.try_wait());
	REQUIRE(s.try_wait());
	REQUIRE(s.try_wait());
	REQUIRE(!s.try_wait());
}

TEST_CASE("threading::slim::semaphore: signal() grants a single additional permit",
	"[thread][slim][semaphore]")
{
	semaphore s;

	REQUIRE(!s.try_wait());

	s.signal();

	REQUIRE(s.try_wait());
	REQUIRE(!s.try_wait());
}

TEST_CASE("threading::slim::semaphore: signal(count) grants that many additional permits",
	"[thread][slim][semaphore]")
{
	semaphore s;

	s.signal(4);

	REQUIRE(s.try_wait());
	REQUIRE(s.try_wait());
	REQUIRE(s.try_wait());
	REQUIRE(s.try_wait());
	REQUIRE(!s.try_wait());
}

TEST_CASE("threading::slim::semaphore: permits acquired concurrently never exceed what was granted",
	"[thread][slim][semaphore]")
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
			if(s.try_wait())
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
	REQUIRE(!s.try_wait());
}
