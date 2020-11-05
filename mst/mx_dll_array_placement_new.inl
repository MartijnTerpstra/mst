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

// NOTE: no include guards


MST_VARIADIC_TEMPLATE_DECL
inline void placement_new(size_t index MST_VARIADIC_TEMPLATE_REFFS_COMMA)
{
	new(&_Mydata[index]) T(MST_VARIADIC_TEMPLATE_FORWARDS);
}

MST_VARIADIC_TEMPLATE_DECL
inline void placement_push_back(MST_VARIADIC_TEMPLATE_REFFS)
{
	size_t _Size = _Mycount;
	_Alloc(_Mycount + 1);
	new(&_Mydata[_Size]) T(MST_VARIADIC_TEMPLATE_FORWARDS);
}

/*/

template<typename... Args>
inline void placement_new(size_t index, Args&&... arguments)
{
	new (&_Mydata[index]){ ::std::forward<Args>(arguments)... };
}
// */