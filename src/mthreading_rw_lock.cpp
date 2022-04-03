//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2022 Martinus Terpstra                                                 //
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

#include <mthreading.h>

#include <mx_platform.h>

#if MST_PLATFORM_MAC || MST_PLATFORM_LINUX

using namespace mst::threading;

rw_lock::rw_lock()
{
	_Myhandle = new pthread_rwlock_t();
	pthread_rwlock_init(reinterpret_cast<pthread_rwlock_t*>(_Myhandle), nullptr);
}

rw_lock::~rw_lock()
{
	// do nothing
	delete reinterpret_cast<pthread_rwlock_t*>(_Myhandle);
}

bool rw_lock::try_lock_read() const
{
	return pthread_rwlock_tryrdlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle)) == 0;
}

bool rw_lock::try_lock_write() const
{
	return pthread_rwlock_trywrlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle)) == 0;
}

void rw_lock::lock_read() const
{
	pthread_rwlock_rdlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle));
}

void rw_lock::lock_write() const
{
	pthread_rwlock_wrlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle));
}

void rw_lock::unlock_read() const
{
	pthread_rwlock_unlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle));
}

void rw_lock::unlock_write() const
{
	pthread_rwlock_unlock(reinterpret_cast<pthread_rwlock_t*>(_Myhandle));
}

#elif MST_PLATFORM_WINDOWS

#include <Windows.h>

using namespace mst::threading;

rw_lock::rw_lock()
{
	_Myhandle = new SRWLOCK();
	InitializeSRWLock(reinterpret_cast<PSRWLOCK>(_Myhandle));
}

rw_lock::~rw_lock()
{
	// do nothing
	delete reinterpret_cast<PSRWLOCK>(_Myhandle);
}

bool rw_lock::try_lock_read() const
{
	return TryAcquireSRWLockShared(reinterpret_cast<PSRWLOCK>(_Myhandle)) != FALSE;
}

bool rw_lock::try_lock_write() const
{
	return TryAcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(_Myhandle)) != FALSE;
}

void rw_lock::lock_read() const
{
	AcquireSRWLockShared(reinterpret_cast<PSRWLOCK>(_Myhandle));
}

void rw_lock::lock_write() const
{
	AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(_Myhandle));
}

void rw_lock::unlock_read() const
{
	ReleaseSRWLockShared(reinterpret_cast<PSRWLOCK>(_Myhandle));
}

void rw_lock::unlock_write() const
{
	ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(_Myhandle));
}

#else

#error "Operating system not supported"

#endif