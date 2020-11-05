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

#pragma once

#include <mcore.h>
#include <mdll_malloc.h>
#include <mdebug.h>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace mst {
namespace dll_safe {

template<typename T>
using vector = ::std::vector<T, ::mst::dll_safe::allocator<T>>;

template<typename T>
using list = ::std::list<T, ::mst::dll_safe::allocator<T>>;

template<typename KeyType, typename ValueType, typename Predicate = ::std::less<KeyType>>
using map = ::std::map<KeyType, ValueType, Predicate,
	::mst::dll_safe::allocator<::std::pair<KeyType, ValueType>>>;

template<typename KeyType, typename ValueType, typename Predicate = ::std::less<KeyType>>
using multimap = ::std::multimap<KeyType, ValueType, Predicate,
	::mst::dll_safe::allocator<::std::pair<KeyType, ValueType>>>;

template<typename KeyType, typename Predicate = ::std::less<KeyType>>
using set = ::std::set<KeyType, Predicate, ::mst::dll_safe::allocator<KeyType>>;

template<typename KeyType, typename Predicate = ::std::less<KeyType>>
using multiset = ::std::multiset<KeyType, Predicate, ::mst::dll_safe::allocator<KeyType>>;

}; // namespace dll_safe
}; // namespace mst