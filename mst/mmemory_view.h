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

#include <mcore.h>

namespace mst {

class memory_view
{
public:
	inline explicit memory_view(const void* dataPtr, size_t dataSize)
		: _Mydata(dataPtr)
		, _Mysize(dataSize)
	{ }

	template<typename T, size_t ArraySize>
	inline explicit memory_view(T (&arr)[ArraySize])
		: _Mydata(arr)
		, _Mysize(ArraySize * sizeof(T))
	{
		static_assert(::std::is_trivial<T>::value, "ContainerType::value_type is not trivial");
	}

	template<typename T, size_t ArraySize>
	inline explicit memory_view(const T (&arr)[ArraySize])
		: _Mydata(arr)
		, _Mysize(ArraySize * sizeof(T))
	{
		static_assert(::std::is_trivial<T>::value, "ContainerType::value_type is not trivial");
	}

	template<typename ContainerType>
	inline explicit memory_view(const ContainerType& container)
		: _Mydata(container.data())
		, _Mysize(container.size() * sizeof(typename ContainerType::value_type))
	{
		static_assert(::std::is_trivial<typename ContainerType::value_type>::value,
			"ContainerType::value_type is not trivial");
	}

	[[nodiscard]] inline size_t size() const
	{
		return _Mysize;
	}

	[[nodiscard]] inline bool empty() const
	{
		return _Mysize == 0;
	}

	[[nodiscard]] inline const void* data() const
	{
		return _Mydata;
	}

	[[nodiscard]] inline const byte* begin() const
	{
		return reinterpret_cast<const byte*>(_Mydata);
	}

	[[nodiscard]] inline const byte* end() const
	{
		return reinterpret_cast<const byte*>(_Mydata) + _Mysize;
	}

	[[nodiscard]] inline const byte* cbegin() const
	{
		return reinterpret_cast<const byte*>(_Mydata);
	}

	[[nodiscard]] inline const byte* cend() const
	{
		return reinterpret_cast<const byte*>(_Mydata) + _Mysize;
	}

private:
	// Non const to enable copy assignment, which is required by some containers

	const void* _Mydata;
	size_t _Mysize;
};

} // namespace mst