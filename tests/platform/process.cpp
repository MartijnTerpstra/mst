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

#include <algorithm>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

#include <mplatform.h>
#include <mprocess.h>

TEST_CASE("platform::current_process_id", "[platform]")
{
	REQUIRE(mst::platform::current_process_id() > 0);
}

TEST_CASE("platform::get_process_info - current process", "[platform]")
{
	const auto pid = mst::platform::current_process_id();

	const auto info = mst::platform::get_process_info(pid);

	REQUIRE(info.has_value());
	REQUIRE(info->pid == pid);
	REQUIRE(info->name.length() > 0);
}

TEST_CASE("platform::get_process_info - non-existing process", "[platform]")
{
	// unlikely to ever be a valid pid on any supported platform
	const auto info =
		mst::platform::get_process_info(static_cast<mst::platform::process_id_t>(-1));

	REQUIRE(!info.has_value());
}

TEST_CASE("platform::find_pids_by_name - finds the current process", "[platform]")
{
	const auto pid = mst::platform::current_process_id();

	const auto selfInfo = mst::platform::get_process_info(pid);
	REQUIRE(selfInfo.has_value());

	const auto pids = mst::platform::find_pids_by_name(selfInfo->name);

	REQUIRE(std::find(pids.begin(), pids.end(), pid) != pids.end());
}

TEST_CASE("platform::find_pids_by_name - unknown name", "[platform]")
{
	const auto pids =
		mst::platform::find_pids_by_name("this_process_name_should_never_exist_12345");

	REQUIRE(pids.empty());
}

TEST_CASE("platform::process::current", "[platform]")
{
	const auto self = mst::platform::process::current();

	REQUIRE(static_cast<bool>(self));
	REQUIRE(self.id() == mst::platform::current_process_id());
	REQUIRE(self.is_running());

	// current() doesn't own a waitable handle to itself
	REQUIRE(!self.wait());
	REQUIRE(!self.exit_code().has_value());

	const auto info = self.info();
	REQUIRE(info.has_value());
	REQUIRE(info->pid == self.id());
}

TEST_CASE("platform::process::from_pid", "[platform]")
{
	const auto pid = mst::platform::current_process_id();

	const auto wrapped = mst::platform::process::from_pid(pid);

	REQUIRE(static_cast<bool>(wrapped));
	REQUIRE(wrapped.id() == pid);
	REQUIRE(wrapped.is_running());

	// from_pid() doesn't own a waitable handle either
	REQUIRE(!wrapped.wait());
	REQUIRE(!wrapped.exit_code().has_value());
}

TEST_CASE("platform::process::from_pid - non-existing process", "[platform]")
{
	const auto wrapped =
		mst::platform::process::from_pid(static_cast<mst::platform::process_id_t>(-1));

	REQUIRE(static_cast<bool>(wrapped));
	REQUIRE(!wrapped.is_running());
	REQUIRE(!wrapped.info().has_value());
}

TEST_CASE("platform::process - default construction is falsy", "[platform]")
{
	const mst::platform::process p;

	REQUIRE(!static_cast<bool>(p));
	REQUIRE(p.id() == 0);
	REQUIRE(!p.is_running());
	REQUIRE(!p.info().has_value());
	REQUIRE(!p.wait());
	REQUIRE(!p.exit_code().has_value());
}

#if MST_PLATFORM_WINDOWS
static const char* TestChildExecutable = "cmd.exe";
static const std::vector<std::string> TestChildArgs = { "/C", "exit 7" };
#else
static const char* TestChildExecutable = "/bin/sh";
static const std::vector<std::string> TestChildArgs = { "-c", "exit 7" };
#endif

TEST_CASE("platform::process::create - runs and exits with the expected code", "[platform]")
{
	auto child = mst::platform::process::create(TestChildExecutable, TestChildArgs);

	REQUIRE(child.has_value());
	REQUIRE(static_cast<bool>(*child));
	REQUIRE(child->id() > 0);

	REQUIRE(child->wait());
	REQUIRE(!child->is_running());
	REQUIRE(child->exit_code() == 7);
}

TEST_CASE("platform::process::create - unknown executable fails", "[platform]")
{
	const auto child =
		mst::platform::process::create("this_executable_should_never_exist_12345.exe");

	REQUIRE(!child.has_value());
}

TEST_CASE("platform::process::create - nonexistent working directory fails", "[platform]")
{
	const auto child = mst::platform::process::create(
		TestChildExecutable, TestChildArgs, "this_directory_should_never_exist_12345");

	REQUIRE(!child.has_value());
}

#if !MST_PLATFORM_WINDOWS
TEST_CASE(
	"platform::process::create - reports the shell-style exit code for a signaled child",
	"[platform]")
{
	auto child = mst::platform::process::create("/bin/sh", { "-c", "kill -KILL $$" });

	REQUIRE(child.has_value());
	REQUIRE(child->wait());
	REQUIRE(child->exit_code() == 128 + 9); // SIGKILL
}
#endif

TEST_CASE("platform::process - move construction transfers ownership", "[platform]")
{
	auto child = mst::platform::process::create(TestChildExecutable, TestChildArgs);
	REQUIRE(child.has_value());

	const auto pid = child->id();

	mst::platform::process moved(std::move(*child));

	REQUIRE(!static_cast<bool>(*child));
	REQUIRE(static_cast<bool>(moved));
	REQUIRE(moved.id() == pid);

	REQUIRE(moved.wait());
	REQUIRE(moved.exit_code() == 7);
}

TEST_CASE(
	"platform::process - move assignment transfers ownership and releases the previous target",
	"[platform]")
{
	auto source = mst::platform::process::create(TestChildExecutable, TestChildArgs);
	auto target = mst::platform::process::create(TestChildExecutable, TestChildArgs);
	REQUIRE(source.has_value());
	REQUIRE(target.has_value());

	const auto sourcePid = source->id();

	*target = std::move(*source);

	REQUIRE(!static_cast<bool>(*source));
	REQUIRE(static_cast<bool>(*target));
	REQUIRE(target->id() == sourcePid);

	REQUIRE(target->wait());
	REQUIRE(target->exit_code() == 7);
}

#if MST_PLATFORM_WINDOWS
static const char* SleepExecutable = "cmd.exe";
static const std::vector<std::string> SleepArgs = { "/C", "ping -n 3 127.0.0.1 >NUL" };
#else
static const char* SleepExecutable = "/bin/sh";
static const std::vector<std::string> SleepArgs = { "-c", "sleep 2" };
#endif

TEST_CASE("platform::process::wait_for - succeeds once the process exits in time", "[platform]")
{
	auto child = mst::platform::process::create(TestChildExecutable, TestChildArgs);
	REQUIRE(child.has_value());

	REQUIRE(child->wait_for(std::chrono::seconds(5)));
	REQUIRE(child->exit_code() == 7);
}

TEST_CASE("platform::process::wait_for - times out while the process is still running",
	"[platform]")
{
	auto child = mst::platform::process::create(SleepExecutable, SleepArgs);
	REQUIRE(child.has_value());

	REQUIRE(!child->wait_for(std::chrono::milliseconds(50)));
	REQUIRE(child->is_running());
	REQUIRE(!child->exit_code().has_value());

	// don't leave the sleeping child dangling for the rest of the suite
	REQUIRE(child->wait());
}
