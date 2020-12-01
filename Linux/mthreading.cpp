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

#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>

#include <condition_variable>

pthread_mutex_t str;

using namespace mst::threading;

class WaitObjectWrapperBase
{
public:
	virtual void Destroy() = 0;
	virtual bool Wait(uint32_t waittime) = 0;
};

class WaitObjectMutex final : public WaitObjectWrapperBase
{
public:
	WaitObjectMutex(bool initialOwner)
	{
		if(pthread_mutex_init(&m_mutex, nullptr) != 0)
		{
			std::abort();
		}

		if(initialOwner)
		{
			pthread_mutex_lock(&m_mutex);
		}
	}

	void Signal()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	void Destroy() override
	{
		pthread_mutex_destroy(&m_mutex);
	}

	bool Wait(uint32_t waittime) override
	{
		if(waittime == 0xFFFFFFFF)
		{
			pthread_mutex_lock(&m_mutex);
			return true;
		}
		else if(waittime == 0)
		{
			return pthread_mutex_trylock(&m_mutex) == 0;
		}
		else
		{
			struct timespec timeoutTime;
			clock_gettime(CLOCK_REALTIME, &timeoutTime);

			timeoutTime.tv_sec += waittime / 1000;
			timeoutTime.tv_nsec += (waittime % 1000) * 1000;

			return pthread_mutex_timedlock(&m_mutex, &timeoutTime) == 0;
		}
	}

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
};

class WaitObjectSemaphore final : public WaitObjectWrapperBase
{
public:
	WaitObjectSemaphore(uint32_t initialCount)
	{
		if(sem_init(&m_semaphore, 0, initialCount) != 0)
		{
			std::abort();
		}
	}

	void Signal(uint32_t count)
	{
		for(uint32_t i = 0; i < count; ++i)
		{
			sem_post(&m_semaphore);
		}
	}

	void Destroy() override
	{
		sem_destroy(&m_semaphore);
	}

	bool Wait(uint32_t waittime) override
	{
		if(waittime == 0xFFFFFFFF)
		{
			sem_wait(&m_semaphore);
			return true;
		}
		else if(waittime == 0)
		{
			return sem_trywait(&m_semaphore) == 0;
		}
		else
		{
			struct timespec timeoutTime;
			clock_gettime(CLOCK_REALTIME, &timeoutTime);

			timeoutTime.tv_sec += waittime / 1000;
			timeoutTime.tv_nsec += (waittime % 1000) * 1000;

			return sem_timedwait(&m_semaphore, &timeoutTime) == 0;
		}
	}

	sem_t m_semaphore;
};

threading_object::threading_object()
	: _Myhandle(nullptr)
{ }

threading_object::~threading_object()
{
	if(_Myhandle)
	{
		((WaitObjectWrapperBase*)_Myhandle)->Destroy();
	}
}

recursive_mutex::recursive_mutex(bool initalOwner)
{
	_Myhandle = new WaitObjectMutex(initalOwner);

	_Mycount = 0;
	_Myholder = UINT32_MAX;

	if(initalOwner)
	{
		_Myholder = pthread_self();
		_Mycount = 1;
	}
}

recursive_mutex::~recursive_mutex()
{
	if(_Myhandle)
	{
		((WaitObjectMutex*)_Myhandle)->Destroy();
	}
}

void recursive_mutex::signal() const
{
	uint32_t count = --_Mycount;

	MST_ASSERT(count != 0xFFFFFFFF, "error in RecursiveMutex");

	if(count == 0)
	{
		_Myholder = UINT32_MAX;

		((WaitObjectMutex*)_Myhandle)->Signal();
	}
}

bool recursive_mutex::wait(int64_t milliseconds) const
{
	ulong curr_thread_id = pthread_self();

	if(curr_thread_id == _Myholder)
	{
		++_Mycount;
		return true;
	}

	// not already holding, wait for the non-recursive mutex
	uint32_t waitTime;
	if(milliseconds < 0 || milliseconds > 0xFFFFFFFFll)
	{
		waitTime = 0xFFFFFFFF;
	}
	else
	{
		waitTime = (milliseconds > 0xFFFFFFFF
						? 0xFFFFFFFF
						: (milliseconds < 0 ? 0 : static_cast<uint32_t>(milliseconds)));
	}

	if(((WaitObjectMutex*)_Myhandle)->Wait(waitTime))
	{
		++_Mycount;
		_Myholder = curr_thread_id;
		return true;
	}
	return false;
}

bool threading_object::wait(int64_t milliseconds) const
{
	if(_Myhandle == nullptr)
	{
		return false;
	}

	uint32_t waitTime;
	if(milliseconds < 0 || milliseconds > 0xFFFFFFFFll)
	{
		waitTime = 0xFFFFFFFF;
	}
	else
	{
		waitTime = (milliseconds > 0xFFFFFFFF
						? 0xFFFFFFFF
						: (milliseconds < 0 ? 0 : static_cast<uint32_t>(milliseconds)));
	}

	return ((WaitObjectWrapperBase*)_Myhandle)->Wait(waitTime);
}

bool threading_object::_Wait_until(int64_t durationFromEpoch) const
{
	const auto millisecondsNow = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
		::std::chrono::steady_clock::now().time_since_epoch())
									 .count();

	return wait(durationFromEpoch - millisecondsNow);
}

mutex::mutex(bool initalOwner)
{
	_Myhandle = new WaitObjectMutex(initalOwner);
}

void mutex::signal() const
{
	((WaitObjectMutex*)_Myhandle)->Signal();
}

semaphore::semaphore(uint32_t initialCount)
{
	_Myhandle = new WaitObjectSemaphore(initialCount);
}

void semaphore::signal(uint32_t count) const
{
	((WaitObjectSemaphore*)_Myhandle)->Signal(count);
}