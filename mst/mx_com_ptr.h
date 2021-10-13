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

template<typename T>
class com_ptr;

namespace _Details {

template<typename _Ty>
class _Com_ptr_combiner
{
public:
	_Com_ptr_combiner(::mst::com_ptr<_Ty>& _ComPtr)
		: _MyComPtr(_ComPtr),
		  _NewObject(nullptr)
	{
	}

	~_Com_ptr_combiner()
	{
		_MyComPtr = ::mst::com_ptr<_Ty>(_NewObject);
	}

	operator _Ty* *()
	{
		return &_NewObject;
	}

	operator _Ty* const *() = delete;

private:
	::mst::com_ptr<_Ty>& _MyComPtr;
	_Ty* _NewObject;

}; // class _Com_ptr_combiner

template<typename _Ty>
class _Com_ptr_combiner_ppv_args
{
public:
	_Com_ptr_combiner_ppv_args(::mst::com_ptr<_Ty>& _ComPtr)
		: _MyComPtr(_ComPtr),
		  _NewObject(nullptr)
	{
	}

	~_Com_ptr_combiner_ppv_args()
	{
		_MyComPtr = ::mst::com_ptr<_Ty>(_NewObject);
	}

	operator void* *()
	{
		return reinterpret_cast<void**>(&_NewObject);
	}

private:
	::mst::com_ptr<_Ty>& _MyComPtr;
	_Ty* _NewObject;

}; // class _Com_ptr_combiner

template<typename T>
_Com_ptr_combiner_ppv_args<T> _Initialize_ppv_args(::mst::com_ptr<T>& _Ptr)
{
	return _Com_ptr_combiner_ppv_args<T>(_Ptr);
}

} // namespace _Details
} // namespace mst
