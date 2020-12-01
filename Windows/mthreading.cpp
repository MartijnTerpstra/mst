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

#include <mthreading.h>

#include <Windows.h>

using namespace mst::threading;

threading_object::threading_object()
	: _Myhandle(nullptr)
{ }

threading_object::~threading_object()
{
	if(_Myhandle)
	{
		CloseHandle(_Myhandle);
	}
}

recursive_mutex::recursive_mutex(bool initalOwner)
{
	_Myhandle = CreateMutexA(nullptr, initalOwner, nullptr);

	if(_Myhandle == nullptr || _Myhandle == INVALID_HANDLE_VALUE)
	{
		throw std::exception("recursive_mutex was unable to be created");
	}

	_Mycount = 0;
	_Myholder = UINT32_MAX;

	if(initalOwner)
	{
		_Myholder = GetCurrentThreadId();
		_Mycount = 1;
	}
}

recursive_mutex::~recursive_mutex()
{
	if(_Myhandle)
	{
		CloseHandle(_Myhandle);
	}
}

void recursive_mutex::signal() const
{
	uint32_t count = --_Mycount;

	MST_ASSERT(count != 0xFFFFFFFF, "error in RecursiveMutex");

	if(count == 0)
	{
		_Myholder = UINT32_MAX;

		ReleaseMutex(_Myhandle);
	}
}

bool recursive_mutex::wait(long long milliseconds) const
{
	DWORD curr_thread_id = GetCurrentThreadId();

	if(curr_thread_id == _Myholder)
	{
		++_Mycount;
		return true;
	}

	// not already holding, wait for the non-recursive mutex
	DWORD waitTime;
	if(milliseconds == -1)
	{
		waitTime = INFINITE;
	}
	else
	{
		waitTime =
			(milliseconds > 0xFFFFFFFF ? 0xFFFFFFFF
									   : (milliseconds < 0 ? 0 : static_cast<DWORD>(milliseconds)));
	}
	switch(WaitForSingleObject(_Myhandle, waitTime))
	{
	case WAIT_ABANDONED:
		return false;

	case WAIT_OBJECT_0:
		++_Mycount;
		_Myholder = curr_thread_id;
		return true;

	case WAIT_TIMEOUT:
		return false;

	default:
		return false;
	}
}

bool threading_object::wait(long long milliseconds) const
{
	if(_Myhandle == nullptr)
	{
		return false;
	}

	DWORD waitTime;
	if(milliseconds == -1)
	{
		waitTime = INFINITE;
	}
	else
	{
		waitTime =
			(milliseconds > 0xFFFFFFFF ? 0xFFFFFFFF
									   : (milliseconds < 0 ? 0 : static_cast<DWORD>(milliseconds)));
	}

	switch(WaitForSingleObject(_Myhandle, waitTime))
	{
	case WAIT_ABANDONED:
		return false;

	case WAIT_OBJECT_0:
		return true;

	case WAIT_TIMEOUT:
		return false;

	default:
		return false;
	}
}

bool threading_object::_Wait_until(long long durationFromEpoch) const
{
	long long millisecondsNow = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
		::std::chrono::steady_clock::now().time_since_epoch())
									.count();

	return wait(durationFromEpoch - millisecondsNow);
}

mutex::mutex(bool initalOwner)
{
	_Myhandle = CreateMutexA(nullptr, initalOwner, nullptr);
}

void mutex::signal() const
{
	ReleaseMutex(_Myhandle);
}

semaphore::semaphore(uint32_t initialCount)
{
	_Myhandle = CreateSemaphoreA(nullptr, (LONG)initialCount, LONG_MAX, nullptr);
}

void semaphore::signal(uint32_t count) const
{
	ReleaseSemaphore(_Myhandle, (LONG)count, nullptr);
}