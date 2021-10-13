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
namespace _Details {

constexpr size_t compiletime_strlen_iterator(const char* cString, size_t index)
{
	return (cString[index] == 0 ? index : compiletime_strlen_iterator(cString, index + 1));
}

constexpr size_t compiletime_strlen(const char* cString)
{
	return compiletime_strlen_iterator(cString, 0);
}

constexpr size_t compiletime_strpos_compare_iterator(const char* cString, const char* cSearch,
	size_t offset, size_t index, size_t strLenString, size_t strLenSearch)
{
	return (index == strLenSearch) ? offset
								   : (cString[offset + index] != cSearch[index]
											 ? SIZE_MAX
											 : compiletime_strpos_compare_iterator(cString, cSearch,
												   offset, index + 1, strLenString, strLenSearch));
}

constexpr size_t compiletime_strpos_iterator(const char* cString, const char* cSearch, size_t index,
	size_t strLenString, size_t strLenSearch)
{
	return (index > size_t(strLenString - strLenSearch))
			   ? SIZE_MAX
			   : (compiletime_strpos_compare_iterator(
					  cString, cSearch, index, 0, strLenString, strLenSearch) != SIZE_MAX
						 ? compiletime_strpos_compare_iterator(
							   cString, cSearch, index, 0, strLenString, strLenSearch)
						 : compiletime_strpos_iterator(
							   cString, cSearch, index + 1, strLenString, strLenSearch));
}

constexpr size_t compiletime_strpos_impl(
	const char* cString, const char* cSearch, size_t strLenString, size_t strLenSearch)
{
	return (strLenSearch > strLenString)
			   ? SIZE_MAX
			   : compiletime_strpos_iterator(cString, cSearch, 0, strLenString, strLenSearch);
}

constexpr size_t compiletime_strpos(const char* cString, const char* cSearch)
{
	return compiletime_strpos_impl(
		cString, cSearch, compiletime_strlen(cString), compiletime_strlen(cSearch));
}

constexpr uint32_t compiletime_hash32_interator(
	const char* cString, size_t strLen, size_t index, uint32_t currentHash)
{
	return (strLen <= index) ? currentHash
							 : compiletime_hash32_interator(cString, strLen, index + 1,
								   (currentHash ^ ((uint32_t)cString[index])) * 16777619U);
}

constexpr uint64_t compiletime_hash64_interator(
	const char* cString, size_t strLen, size_t index, uint64_t currentHash)
{
	return (strLen <= index) ? (currentHash ^ (currentHash >> 32))
							 : compiletime_hash64_interator(cString, strLen, index + 1,
								   (currentHash ^ ((uint64_t)cString[index])) * 1099511628211ULL);
}


constexpr uint32_t compiletime_hash32(const char* cString, size_t offset, size_t strLen)
{
	return compiletime_hash32_interator(cString, strLen, offset, 2166136261UL);
}

constexpr uint64_t compiletime_hash64(const char* cString, size_t offset, size_t strLen)
{
	return compiletime_hash64_interator(cString, strLen, offset, 14695981039346656037ULL);
}

#if _MST_USING_VC_COMPILER

constexpr size_t compiletime_hash_of_forward_iterator2(
	const char* cString, size_t index, size_t notFoundIndex)
{
	return (index == compiletime_strlen(cString))
			   ? notFoundIndex
			   : (cString[index] == ' ')
					 ? index + 1
					 : compiletime_hash_of_forward_iterator2(cString, index + 1, notFoundIndex);
}

constexpr size_t compiletime_hash_of_forward_iterator(const char* cString, size_t index)
{
	return (cString[index] == '<')
			   ? compiletime_hash_of_forward_iterator2(cString, index + 1, index + 1)
			   : compiletime_hash_of_forward_iterator(cString, index + 1);
}

constexpr size_t compiletime_hash_of_backward_iterator(const char* cString, size_t index)
{
	return (cString[index] == '>') ? index
								   : compiletime_hash_of_backward_iterator(cString, index - 1);
}

constexpr uint32_t compiletime_hash_of32_impl(const char* cString)
{
	return compiletime_hash32(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(cString, compiletime_strlen(cString) - 1));
}

constexpr uint64_t compiletime_hash_of64_impl(const char* cString)
{
	return compiletime_hash64(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(cString, compiletime_strlen(cString) - 1));
}

#elif _MST_USING_CLANG_COMPILER

constexpr size_t compiletime_hash_of_forward_iterator2(
	const char* cString, size_t index, size_t notFoundIndex)
{
	return (index == compiletime_strlen(cString))
			   ? notFoundIndex
			   : (cString[index] == ' ')
					 ? index + 1
					 : compiletime_hash_of_forward_iterator2(cString, index + 1, notFoundIndex);
}

constexpr size_t compiletime_hash_of_forward_iterator(const char* cString, size_t index)
{
	return compiletime_strpos(cString, "[T = ") + 5;
}

constexpr size_t compiletime_hash_of_backward_iterator(const char* cString, size_t index)
{
	return (cString[index] == ';' || cString[index] == ']')
			   ? index
			   : compiletime_hash_of_backward_iterator(cString, index + 1);
}

constexpr uint32_t compiletime_hash_of32_impl(const char* cString)
{
	return compiletime_hash32(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(
			cString, compiletime_hash_of_forward_iterator(cString, 0)));
}

constexpr uint64_t compiletime_hash_of64_impl(const char* cString)
{
	return compiletime_hash64(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(
			cString, compiletime_hash_of_forward_iterator(cString, 0)));
}

#elif _MST_USING_GCC_COMPILER

constexpr size_t compiletime_hash_of_forward_iterator2(
	const char* cString, size_t index, size_t notFoundIndex)
{
	return (index == compiletime_strlen(cString))
			   ? notFoundIndex
			   : (cString[index] == ' ')
					 ? index + 1
					 : compiletime_hash_of_forward_iterator2(cString, index + 1, notFoundIndex);
}

constexpr size_t compiletime_hash_of_forward_iterator(const char* cString, size_t index)
{
	return compiletime_strpos(cString, "[with T = ") + 10;
}

constexpr size_t compiletime_hash_of_backward_iterator(const char* cString, size_t index)
{
	return (cString[index] == ';' || cString[index] == ']')
			   ? index
			   : compiletime_hash_of_backward_iterator(cString, index + 1);
}

constexpr uint32_t compiletime_hash_of32_impl(const char* cString)
{
	return compiletime_hash32(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(
			cString, compiletime_hash_of_forward_iterator(cString, 0)));
}

constexpr uint64_t compiletime_hash_of64_impl(const char* cString)
{
	return compiletime_hash64(cString, compiletime_hash_of_forward_iterator(cString, 0),
		compiletime_hash_of_backward_iterator(
			cString, compiletime_hash_of_forward_iterator(cString, 0)));
}

#else

#error "Compiler not supported"

#endif

template<typename T>
constexpr uint32_t compiletime_hash_of32()
{
	return ::mst::_Details::compiletime_hash_of32_impl(_MST_FUNCTION_SIGNATURE);
}

template<typename T>
constexpr uint64_t compiletime_hash_of64()
{
	return ::mst::_Details::compiletime_hash_of64_impl(_MST_FUNCTION_SIGNATURE);
}

template<typename ValueType, ValueType CompiletimeHash>
struct compiletime_enforcer
{
	static constexpr ValueType value() noexcept
	{
		return CompiletimeHash;
	}
};

} // namespace _Details
} // namespace mst