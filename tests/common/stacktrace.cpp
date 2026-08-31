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

#include <mstacktrace.h>
#include <sstream>

TEST_CASE("stacktrace: current captures at least one frame", "[common][stacktrace]")
{
	auto trace = mst::stacktrace::current();

	REQUIRE_FALSE(trace.empty());
	REQUIRE(trace.size() > 0);
	REQUIRE(static_cast<bool>(trace[0]));
	REQUIRE(trace[0].native_handle() != nullptr);
	REQUIRE(trace.front() == trace[0]);
	REQUIRE(trace.back() == trace[trace.size() - 1]);
}

TEST_CASE("stacktrace: maxDepth of 0 captures nothing", "[common][stacktrace]")
{
	auto trace = mst::stacktrace::current(0, 0);

	REQUIRE(trace.empty());
	REQUIRE(trace.size() == 0);
	REQUIRE(trace.begin() == trace.end());
}

TEST_CASE("stacktrace: skip drops the requested number of leading frames", "[common][stacktrace]")
{
	auto full = mst::stacktrace::current(0, 16);
	auto skippedOne = mst::stacktrace::current(1, 16);

	REQUIRE_FALSE(full.empty());
	REQUIRE(full.size() >= skippedOne.size());

	// skippedOne[i] should equal full[i + 1]; capped at full.size() - 1 since both captures share
	// the same maxDepth, so full's last frame may have no counterpart in skippedOne
	const auto comparableCount =
		full.size() - 1 < skippedOne.size() ? full.size() - 1 : skippedOne.size();
	for(size_t i = 0; i < comparableCount; ++i)
	{
		REQUIRE(skippedOne[i] == full[i + 1]);
	}
}

TEST_CASE("stacktrace: default constructed trace is empty and self-equal", "[common][stacktrace]")
{
	mst::stacktrace a;
	mst::stacktrace b;

	REQUIRE(a.empty());
	REQUIRE(a.size() == 0);
	REQUIRE(a == b);
}

TEST_CASE("stacktrace_entry: default constructed entry is empty/null", "[common][stacktrace]")
{
	mst::stacktrace_entry entry;

	REQUIRE_FALSE(static_cast<bool>(entry));
	REQUIRE(entry.native_handle() == nullptr);
	REQUIRE(entry.description().empty());
	REQUIRE(entry.source_file().empty());
	REQUIRE(entry.source_line() == 0);
	REQUIRE(mst::to_string(entry).empty());
}

TEST_CASE("stacktrace_entry: equality is based on the captured address", "[common][stacktrace]")
{
	auto first = mst::stacktrace::current();
	auto second = mst::stacktrace::current();

	REQUIRE(first.size() == second.size());

	// captured one line apart, so everything but the differing call site itself must match
	for(size_t i = 1; i < first.size(); ++i)
	{
		REQUIRE(first[i] == second[i]);
	}
}

TEST_CASE("stacktrace: to_string and operator<< agree and are non-empty", "[common][stacktrace]")
{
	auto trace = mst::stacktrace::current();

	auto text = mst::to_string(trace);

	std::ostringstream stream;
	stream << trace;

	REQUIRE(text == stream.str());
	REQUIRE_FALSE(text.empty());
	REQUIRE(text.find("#0") != std::string::npos);
}

TEST_CASE("stacktrace_entry: to_string and operator<< agree", "[common][stacktrace]")
{
	auto trace = mst::stacktrace::current();
	REQUIRE_FALSE(trace.empty());

	auto text = mst::to_string(trace[0]);

	std::ostringstream stream;
	stream << trace[0];

	REQUIRE(text == stream.str());
	REQUIRE_FALSE(text.empty());
}
