//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <mcore.h>

//#define MST_MATH_ALL_VECTORS_MATRICES_SIMD 1
#include <mmath2.h>

#include <DirectXMath.h>

#include "Windows.h"
#include <string>
#include <chrono>

namespace chrono {

using std::chrono::high_resolution_clock;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

using std::chrono::duration_cast;

typedef std::chrono::time_point<high_resolution_clock> time_point;
}

std::string g_currentBenchmark;

class Stopwatch
{
public:
	void Start()
	{
		m_start = chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		m_stop = chrono::high_resolution_clock::now();
	}

	std::string GetSeconds()
	{
		return GetTime<chrono::seconds, chrono::milliseconds>("seconds");
	}

	std::string GetMilliseconds()
	{
		return GetTime<chrono::milliseconds, chrono::microseconds>("milliseconds");
	}

	std::string GetMicroseconds()
	{
		return GetTime<chrono::microseconds, chrono::nanoseconds>("microseconds");
	}

	std::string GetTime()
	{
		auto whole = GetWhole<chrono::milliseconds>();
		if(whole == 0)
		{
			return GetMicroseconds();
		}
		else if(whole > 1000)
		{
			return GetSeconds();
		}
		return GetMilliseconds();
	}

	chrono::nanoseconds GetNanos() const noexcept
	{
		return m_stop - m_start;
	}

private:
	template<typename WholeRep, typename PartRep>
	std::string GetTime(const char* postfix) const noexcept
	{
		char buffer[1024];
		sprintf_s(buffer, "%lld.%03lld %s", GetWhole<WholeRep>(), GetPart<PartRep>(), postfix);

		return buffer;
	}

	template<typename Rep>
	int64_t GetWhole() const noexcept
	{
		return chrono::duration_cast<Rep>(m_stop - m_start).count();
	}

	template<typename Rep>
	int64_t GetPart() const noexcept
	{
		return chrono::duration_cast<Rep>(m_stop - m_start).count() % 1000;
	}

private:
	chrono::time_point m_start, m_stop;
};

using mst::math::vector;
using mst::math::matrix;

template<typename _Value_type, size_t _Columns, size_t _RowsColumns, size_t _Rows>
inline constexpr matrix<_Value_type, _Columns, _Rows> ReferenceMatrixMultiply(
	const matrix<_Value_type, _Columns, _RowsColumns>& _Left,
	const matrix<_Value_type, _RowsColumns, _Rows>& _Right) noexcept
{
	matrix<_Value_type, _Columns, _Rows> retval = matrix<_Value_type, _Columns, _Rows>::zero;

	for(int x = 0; x < _Rows; ++x)
		for(int y = 0; y < _Columns; ++y)
			for(int i = 0; i < _RowsColumns; ++i)
			{
				retval[x][y] += _Left[x][i] * _Right[i][y];
			}

	return retval;
}

constexpr uint32_t TRYCOUNT = 10000000;

template<typename Fn, typename Callback>
void Benchmark(std::string name, Fn func, Callback callback)
{
	static int index = 0;

	auto number = std::to_string(++index);

	while(number.length() < 5)
		number.insert(0, 5 - number.length(), '0');

	g_currentBenchmark = number + " " + name;

	Stopwatch s;

	s.Start();

	func();

	s.Stop();

	printf("Benchmark completed for job %s: [%s] in %s\n", number.c_str(), name.c_str(),
		s.GetTime().c_str());
	callback(name, s);
}

#define _MST_TESTING 1

#define INFO(...) (std::cout << __VA_ARGS__)
#define CHECK(...)

#include <vector>
#include <muuid.h>
#include <fstream>
#include <mlock_free_queue.h>
#include <thread>
#include <mcommon.h>

mst::lock_free::queue<int>* current_q;

std::atomic_uint32_t g_failCount = 0;
std::atomic_uint32_t g_tryCount = 0;

int ELEMCOUNT = 1000000;

template<typename QueueType>
void TestReadersWriters(size_t writerCount, size_t readerCount, bool sequential = false)
{
	// constexpr int ELEMCOUNT = 10000000;

	const uint32_t val = 0xFFFFFFFF & ~3221225472U;

	QueueType q;

	if constexpr(std::is_same_v<QueueType, mst::lock_free::queue<int>>)
	{
		current_q = &q;
	}

	std::atomic_int index = 0;
	std::vector<std::atomic_bool> completed(writerCount);

	struct data
	{
		std::atomic_int visited;
		char dummy[64 - sizeof(std::atomic_int)];
	};

	std::vector<data> isVisited(ELEMCOUNT);

	std::vector<std::thread> readers(readerCount);
	std::vector<std::thread> writers(writerCount);

	HANDLE evt = CreateEventA(nullptr, TRUE, FALSE, nullptr);

	for(auto& b : completed)
	{
		b = false;
	}

	for(auto& i : isVisited)
	{
		i.visited = 0;
	}

	for(size_t i = 0; i < writerCount; ++i)
	{
		auto& t = writers[i];

		t = std::thread([&, threadIndex = i] {
			WaitForSingleObject(evt, INFINITE);

			while(true)
			{
				int val = index++;
				if(val >= ELEMCOUNT)
					break;

				q.push(val);
			}

			completed[threadIndex].store(true);
		});
	}

	if(!sequential)
	{
		for(size_t i = 0; i < readerCount; ++i)
		{
			auto& t = readers[i];

			t = std::thread([&, threadIndex = i] {
				WaitForSingleObject(evt, INFINITE);

				int val;
				while(true)
				{
					while(q.try_pop(val))
					{
						++isVisited[val].visited;
					}

					bool exit = true;

					for(auto& b : completed)
					{
						if(!b.load())
							exit = false;
					}

					if(exit)
					{
						while(q.try_pop(val))
						{
							++isVisited[val].visited;
						}

						break;
					}
				}
			});
		}
	}

	std::atomic_bool quit = false;
	/*
	std::thread exporter([&]()
		{
			while (true)
			{
				std::ifstream infile("Export.txt");

				if (infile.is_open())
				{
					infile.close();
					DeleteFileA("Export.txt");
					q.export_states_and_values();
					std::ofstream outfile("export_completed.txt");
				}

				for (int i = 0; i < 100; ++i)
				{
					if (quit.load())
						break;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
		});*/

	// Sleep(5);
	SetEvent(evt);

	for(auto& t : writers)
	{
		t.join();
	}

	if(sequential)
	{
		for(size_t i = 0; i < readerCount; ++i)
		{
			auto& t = readers[i];

			t = std::thread([&, threadIndex = i] {
				WaitForSingleObject(evt, INFINITE);

				int val;
				while(true)
				{
					while(q.try_pop(val))
					{
						++isVisited[val].visited;
					}

					bool exit = true;

					for(auto& b : completed)
					{
						if(!b.load())
							exit = false;
					}

					if(exit)
					{
						while(q.try_pop(val))
						{
							++isVisited[val].visited;
						}

						break;
					}
				}
			});
		}
	}

	for(auto& t : readers)
	{
		t.join();
	}

	quit = true;

	// exporter.join();

	bool failed = false;

	for(int i = 0; i < ELEMCOUNT; ++i)
	{
		if(isVisited[i].visited.load() != 1)
		{
			failed = true;
		}
	}

	if(failed)
	{
		++g_failCount;

		if constexpr(std::is_same<QueueType, mst::lock_free::queue<int>>::value)
		{
			auto filename = "failure" + g_currentBenchmark;

			for(auto& c : filename)
			{
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					continue;
				}

				c = '_';
			}
		}

		puts("Failure");
		Sleep(10000);
	}

	++g_tryCount;
}

#include <deque>
#include <mutex>
#include <mthreading_slim.h>
#include "moodycamel.h"
#include <concurrent_queue.h>

class locked_queue
{
	std::deque<int> container;
	std::mutex mutex;

public:
	void push(const int& elem)
	{
		std::lock_guard m{ mutex };

		container.push_back(elem);
	}

	bool try_pop(int& elem)
	{
		std::lock_guard m{ mutex };

		if(container.empty())
			return false;

		elem = container.front();
		container.pop_front();
		return true;
	}

	void export_states_and_values()
	{ }

	void check_empty()
	{ }
};

class slim_locked_queue
{
	std::deque<int> container;
	mst::threading::slim::mutex mutex;

public:
	slim_locked_queue()
		: mutex(false)
	{ }

	void push(const int& elem)
	{
		std::lock_guard m{ mutex };

		container.push_back(elem);
	}

	bool try_pop(int& elem)
	{
		std::lock_guard m{ mutex };

		if(container.empty())
			return false;

		elem = container.front();
		container.pop_front();
		return true;
	}

	void export_states_and_values()
	{ }

	void check_empty()
	{ }
};

class moodycamel_queue
{
public:
	void push(const int& elem)
	{
		queue.enqueue(elem);
	}

	bool try_pop(int& elem)
	{
		return queue.try_dequeue(elem);
	}

	void export_states_and_values()
	{ }

	void check_empty()
	{ }

	moodycamel::ConcurrentQueue<int> queue;
};

class microsoft_queue
{
public:
	void push(const int& elem)
	{
		queue.push(elem);
	}

	bool try_pop(int& elem)
	{
		return queue.try_pop(elem);
	}

	void export_states_and_values()
	{ }

	void check_empty()
	{ }

	concurrency::concurrent_queue<int> queue;
};

#include <mplatform.h>
#include <map>

struct AvgData
{

	chrono::milliseconds Get() const noexcept
	{
		return chrono::duration_cast<chrono::milliseconds>(nanos / count);
	}

	std::string GetString() const noexcept
	{
		const auto avg = Get();

		char buffer[1024];
		sprintf_s(buffer, "%lld.%03lld seconds",
			chrono::duration_cast<chrono::seconds>(avg).count(), avg.count() % 1000);

		return buffer;
	}

	int64_t count = 0;
	chrono::nanoseconds nanos = chrono::nanoseconds(0);
};

_MST_CONSTEXPR17 size_t _Strstr(
	const char* _First, size_t _First_length, const char* _Needle) noexcept
{
	const size_t _Needle_length = std::char_traits<char>::length(_Needle);

	for(size_t _Index = 0; _Index <= _First_length - _Needle_length; ++_Index)
	{
		for(size_t _Offset = 0; _Offset < _Needle_length; ++_Offset)
		{
			if(_First[_Index + _Offset] != _Needle[_Offset])
				break;

			if(_Offset == _Needle_length - 1)
				return _Index;
		}
	}

	return SIZE_MAX;
}

typedef mst::math::matrix<int, 4, 4> matrixi;

template<int X, int Y, int Z, int W>
__m128 swizzle(__m128 value)
{
	return _mm_permute_ps(value, X | (Y << 2) | (Z << 4) | (W << 6));
}

class NoExceptFunctor
{
public:
	void operator()(int value) noexcept
	{
		std::cout << value << std::endl;
	}
};

class Functor
{
public:
	void operator()(int value)
	{
		throw std::runtime_error("");
	}
};

template<typename Fn, typename T>
class Handler
{
public:
	void Call(const T& value) noexcept(std::is_nothrow_invocable_v<Fn, const T&>)
	{
		m_function(value);
	}

private:
	Fn m_function;
};

enum
{
	IsNoExceptFunctor = noexcept(std::declval<Handler<NoExceptFunctor, int>>().Call(1)) ? 1 : 0,
	IsFunctor = noexcept(std::declval<Handler<Functor, int>>().Call(1)) ? 1 : 0,

	IsNoExceptFunctorCheck = std::is_nothrow_invocable_v<NoExceptFunctor, const int&> ? 1 : 0,
	IsFunctorCheck = std::is_nothrow_invocable_v<Functor, const int&> ? 1 : 0
};

int main()
{
	std::cout << (IsNoExceptFunctor == IsFunctor ? "Same" : "Different") << std::endl;
	system("pause");

	__m128 vector = { 1, 2, 3, 4 };

	__m128 xxxx = _mm_permute_ps(vector, 0);
	__m128 yyyy = _mm_permute_ps(vector, (1) | (1 << 2) | (1 << 4) | (1 << 6));
	__m128 zzzz = _mm_permute_ps(vector, (2) | (2 << 2) | (2 << 4) | (2 << 6));
	__m128 wwww = _mm_permute_ps(vector, (3) | (3 << 2) | (3 << 4) | (3 << 6));

	__m128 xxyy = swizzle<0, 0, 1, 1>(vector);

	float inputv[4];
	float outputv[4];

	memcpy(inputv, &vector, sizeof(vector));
	memcpy(outputv, &xxyy, sizeof(xxyy));

	const auto coreCount = mst::platform::processor_core_count();
	const auto threadCount = mst::platform::processor_thread_count();

	std::map<std::string, AvgData> totals;

	const auto addAvg = [&](const std::string& name, Stopwatch& s) {
		++totals[name].count, totals[name].nanos += s.GetNanos();
	};

	HANDLE evt = CreateEventA(nullptr, FALSE, FALSE, nullptr);

	std::thread t([&] {
		while(WaitForSingleObject(evt, 0) == WAIT_TIMEOUT)
		{
			Benchmark(
				" [SPSC] lock_free::queue<int> mixed (each 1 core)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(1, 1); }, addAvg);
			Benchmark(
				" [SPMC] lock_free::queue<int> mixed (core count)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(1, coreCount - 1); }, addAvg);
			Benchmark(
				" [SPMC] lock_free::queue<int> mixed (thread count)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(1, threadCount - 1); },
				addAvg);
			Benchmark(
				" [MPSC] lock_free::queue<int> mixed (core count)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(coreCount - 1, 1); }, addAvg);
			Benchmark(
				" [MPSC] lock_free::queue<int> mixed (thread count)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(threadCount - 1, 1); },
				addAvg);
			Benchmark(
				" [MPMC] lock_free::queue<int> mixed (core count)",
				[&] {
					TestReadersWriters<mst::lock_free::queue<int>>(coreCount / 2, coreCount / 2);
				},
				addAvg);
			Benchmark(
				" [MPMC] lock_free::queue<int> mixed (thread count)",
				[&] {
					TestReadersWriters<mst::lock_free::queue<int>>(
						threadCount / 2, threadCount / 2);
				},
				addAvg);
			Benchmark(
				" [MPMC] lock_free::queue<int> fill first (core count)",
				[&] { TestReadersWriters<mst::lock_free::queue<int>>(coreCount, coreCount, true); },
				addAvg);
			Benchmark(
				" [MPMC] lock_free::queue<int> fill first (thread count)",
				[&] {
					TestReadersWriters<mst::lock_free::queue<int>>(threadCount, threadCount, true);
				},
				addAvg);
		}
	});

	while(true)
	{
		std::string line;
		std::getline(std::cin, line);

		if(line == "quit")
		{
			SetEvent(evt);
			t.join();

			for(auto& i : totals)
			{
				std::cout << i.first << ": " << i.second.GetString() << std::endl;
			}

			std::cout << std::endl << "Press enter to exit..." << std::endl;

			std::getline(std::cin, line);

			return 0;
		}

		if(line == "averages")
		{
			for(auto& i : totals)
			{
				std::cout << i.first << ": " << i.second.GetString() << std::endl;
			}

			std::cout << "averages created" << std::endl;
		}

		if(line == "state")
		{
			const auto fails = g_failCount.load();
			const auto tries = g_tryCount.load();
			const auto percentage = fails * 100U / tries;

			if(g_failCount.load() > 0)
			{
				printf("Failure rate: %u/%u, %u%%\n", g_failCount.load(), g_tryCount.load(),
					percentage);
			}
			else
			{
				printf("No failures during %u tries\n", tries);
			}
		}
	}

	return 0;

	Benchmark(
		"Warmup", [&] { TestReadersWriters<mst::lock_free::queue<int>>(1, 1); },
		[](const std::string&, Stopwatch&) {});
	puts("");

	for(int i = 0; i < 5; ++i)
	{
		printf("ElemCount: %d\n", ELEMCOUNT);

		Benchmark(
			"[SPSC] lock_free::queue<int>",
			[&] { TestReadersWriters<mst::lock_free::queue<int>>(1, 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[SPSC] moodycamel_queue", [&] { TestReadersWriters<moodycamel_queue>(1, 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[SPSC] microsoft_queue", [&] { TestReadersWriters<microsoft_queue>(1, 1); },
			[](const std::string&, Stopwatch&) {});
		puts("");
		Benchmark(
			"[SPMC] lock_free::queue<int>",
			[&] { TestReadersWriters<mst::lock_free::queue<int>>(1, threadCount - 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[SPMC] moodycamel_queue",
			[&] { TestReadersWriters<moodycamel_queue>(1, threadCount - 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[SPMC] microsoft_queue",
			[&] { TestReadersWriters<microsoft_queue>(1, threadCount - 1); },
			[](const std::string&, Stopwatch&) {});
		puts("");
		Benchmark(
			"[MPSC] lock_free::queue<int>",
			[&] { TestReadersWriters<mst::lock_free::queue<int>>(threadCount - 1, 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPSC] moodycamel_queue",
			[&] { TestReadersWriters<moodycamel_queue>(threadCount - 1, 1); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPSC] microsoft_queue",
			[&] { TestReadersWriters<microsoft_queue>(threadCount - 1, 1); },
			[](const std::string&, Stopwatch&) {});
		puts("");
		Benchmark(
			"[MPMC] lock_free::queue<int>",
			[&] { TestReadersWriters<mst::lock_free::queue<int>>(threadCount / 2, coreCount / 2); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPMC] moodycamel_queue",
			[&] { TestReadersWriters<moodycamel_queue>(threadCount / 2, threadCount / 2); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPMC] microsoft_queue",
			[&] { TestReadersWriters<microsoft_queue>(threadCount / 2, threadCount / 2); },
			[](const std::string&, Stopwatch&) {});
		puts("");
		Benchmark(
			"[MPMC] lock_free::queue<int> fill first",
			[&] { TestReadersWriters<mst::lock_free::queue<int>>(threadCount, coreCount, true); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPMC] moodycamel_queue fill first",
			[&] { TestReadersWriters<moodycamel_queue>(threadCount, threadCount, true); },
			[](const std::string&, Stopwatch&) {});
		Benchmark(
			"[MPMC] microsoft_queue fill first",
			[&] { TestReadersWriters<microsoft_queue>(threadCount, threadCount, true); },
			[](const std::string&, Stopwatch&) {});
		puts("");

		ELEMCOUNT *= 10;
	}

	system("pause");

	return 0;

	std::vector<std::string> idstrings;
	idstrings.resize(TRYCOUNT);
	for(uint32_t i = 0; i < TRYCOUNT; ++i)
	{
		idstrings.push_back(mst::uuid::create().to_string());
	}

	std::vector<mst::uuid> ids(TRYCOUNT);
	std::vector<mst::uuid> ids2(TRYCOUNT);

	Benchmark(
		"simd",
		[&] {
			for(uint32_t i = 0; i < TRYCOUNT; ++i)
			{
				mst::uuid::try_parse(idstrings[i], ids[i]);
			}
		},
		[](const std::string&, Stopwatch&) {});

	byte checksum = 0;
	for(size_t i = 0; i < sizeof(mst::uuid) * ids.size(); ++i)
	{
		checksum += reinterpret_cast<byte*>(ids.data())[i];
		checksum += reinterpret_cast<byte*>(ids2.data())[i];
	}

	std::string s;
	std::getline(std::cin, s);

	printf("%d\n", (int)checksum);
}