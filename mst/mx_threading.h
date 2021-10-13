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

#pragma once

namespace mst {
namespace threading {

class threading_object;

namespace _Details {

int _Wait_for_objects(const ::mst::threading::threading_object* arrayPtr, size_t arrSize,
	bool waitForAllObjects, long long milliseconds);
int _Wait_for_objects(const ::mst::threading::threading_object* const* arrayPtr, size_t arrSize,
	bool waitForAllObjects, long long milliseconds);

template<typename Iterator>
inline int _Wait_for_objects(
	Iterator beginIter, Iterator endIter, bool waitForAllObjects, long long milliseconds)
{
	return _Wait_for_objects(&(*beginIter), endIter - beginIter, waitForAllObjects, milliseconds);
}

int _Wait_for_objects_until(const ::mst::threading::threading_object* arrayPtr, size_t arrSize,
	bool waitForAllObjects, long long durationFromEpoch);
int _Wait_for_objects_until(const ::mst::threading::threading_object* const* arrayPtr,
	size_t arrSize, bool waitForAllObjects, long long durationFromEpoch);

template<typename Iterator>
inline int _Wait_for_objects_until(
	Iterator beginIter, Iterator endIter, bool waitForAllObjects, long long durationFromEpoch)
{
	return _Wait_for_objects_until(
		&(*beginIter), endIter - beginIter, waitForAllObjects, durationFromEpoch);
}

template<typename Iterator, class _Rep, class _Period>
inline int _Wait_for_objects_until(Iterator beginIter, Iterator endIter, bool waitForAllObjects,
	const ::std::chrono::duration<_Rep, _Period>& durationFromEpoch)
{
	return _Wait_for_objects_until(beginIter, endIter, waitForAllObjects,
		::std::chrono::duration_cast<::std::chrono::milliseconds>(durationFromEpoch).count());
}


class _Thread_func_wrapper
{
public:
	virtual void _Call() = 0;
};

template<typename _Ty>
class _Thread_func_impl final : public _Thread_func_wrapper
{
public:
	_Thread_func_impl(_Ty _Fun)
		: _Myfun(::std::move(_Fun))
	{ }

	void _Call() override
	{

		_Myfun();

		delete this;
	}

	_Ty _Myfun;
};

} // namespace _Details
} // namespace threading
} // namespace mst