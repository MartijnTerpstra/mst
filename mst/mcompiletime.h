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

#include <string>
#include <mcore.h>
#include <mx_compiletime.h>

#if _MST_HAS_CONSTEXPR17
#include <mx_hash.h>
#endif

namespace mst {
namespace compiletime {

constexpr size_t strlen(const char* cString)
{
	return ::mst::_Details::compiletime_strlen(cString);
}

constexpr size_t strpos(const char* cString, const char* cSearch)
{
	return ::mst::_Details::compiletime_strpos(cString, cSearch);
}

constexpr uint32_t hash32(const char* cString)
{
	return _Details::compiletime_hash32(cString, 0, ::mst::compiletime::strlen(cString));
}

constexpr uint64_t hash64(const char* cString)
{
	return ::mst::_Details::compiletime_hash64(cString, 0, ::mst::compiletime::strlen(cString));
}

template<typename T>
constexpr uint32_t hash_of()
{
	return ::mst::_Details::compiletime_hash_of32<T>();
}

template<typename T>
constexpr uint64_t hash_of64()
{
	return ::mst::_Details::compiletime_hash_of64<T>();
}

} // namespace compiletime
} // namespace mst