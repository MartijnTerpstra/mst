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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <mcom_ptr.h>


struct __declspec(uuid("E98256E7-0E2B-4840-BFAD-C56C19E502E2")) IFakeIUnknown : public IUnknown
{ };

struct __declspec(uuid("F9704466-3BFC-4BFB-A48A-02EAFE3A4115")) IFakeIUnknown2
	: public IFakeIUnknown
{ };

struct __declspec(uuid("B995C65E-755D-410A-BD5B-52D99DD2EBF0")) IWrongUnknown : public IFakeIUnknown
{ };

class CFakeUnknown : public IFakeIUnknown2
{
	// Inherited via IFakeIUnknown2
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
	{
		if(riid == __uuidof(IUnknown))
		{
			*((IUnknown**)ppvObject) = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}

		if(riid == __uuidof(IFakeIUnknown))
		{
			*((IUnknown**)ppvObject) = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}

		if(riid == __uuidof(IFakeIUnknown2))
		{
			*((IUnknown**)ppvObject) = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	virtual ULONG __stdcall AddRef(void) override
	{
		return ++refcount;
	}
	virtual ULONG __stdcall Release(void) override
	{
		const auto refs = --refcount;

		if(refs == 0)
		{
			delete this;
		}

		return refs;
	}

	ULONG refcount = 1;
};

void CreationFunc(IFakeIUnknown** outResult)
{
	*outResult = new CFakeUnknown();
}

TEST_CASE("com_ptr<T: creation", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	REQUIRE(obj->AddRef() == 2);

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	REQUIRE(ptr.reset() == 1);

	REQUIRE(obj->Release() == 0);
}

TEST_CASE("com_ptr<T>: initialize", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	REQUIRE(obj->AddRef() == 2);

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	CreationFunc(mst::initialize(ptr));

	REQUIRE(obj->Release() == 0);

	REQUIRE(ptr != nullptr);

	REQUIRE(ptr.reset() == 0);
}