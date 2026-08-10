//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
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

#include <catch2/catch_test_macros.hpp>

#include <mcom_ptr.h>
#include <set_assertions.h>


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

TEST_CASE("com_ptr<T>: initialize releases the previously held object", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	CreationFunc(mst::initialize(ptr));

	// the original object was released as part of `initialize` overwriting `ptr`, so the new
	// object must be a different pointer
	REQUIRE(ptr.get() != obj);
	REQUIRE(ptr != nullptr);

	REQUIRE(ptr.reset() == 0);
}

TEST_CASE("com_ptr<T>: default construction is empty", "[memory][com_ptr]")
{
	mst::com_ptr<IFakeIUnknown> ptr;

	REQUIRE(ptr == nullptr);
	REQUIRE_FALSE(ptr != nullptr);
	REQUIRE_FALSE(static_cast<bool>(ptr));
	REQUIRE(ptr.get() == nullptr);
}

TEST_CASE("com_ptr<T>: nullptr construction and assignment", "[memory][com_ptr]")
{
	mst::com_ptr<IFakeIUnknown> ptr(nullptr);

	REQUIRE(ptr == nullptr);

	IFakeIUnknown* obj = new CFakeUnknown();

	ptr = mst::com_ptr<IFakeIUnknown>(obj);

	REQUIRE(ptr != nullptr);
	REQUIRE(static_cast<bool>(ptr));

	ptr = nullptr;

	REQUIRE(ptr == nullptr);
}

TEST_CASE("com_ptr<T>: copy construction from a related type adds a reference",
	"[memory][com_ptr]")
{
	CFakeUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown2> derived(obj);
	mst::com_ptr<IFakeIUnknown> base(derived);

	REQUIRE(base.get() == derived.get());

	REQUIRE(base.reset() == 1);
	REQUIRE(derived.reset() == 0);
}

TEST_CASE("com_ptr<T>: move construction transfers ownership", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr1(obj);
	mst::com_ptr<IFakeIUnknown> ptr2(std::move(ptr1));

	REQUIRE(ptr1 == nullptr);
	REQUIRE(ptr2.get() == obj);

	// moving does not add a reference
	REQUIRE(ptr2.reset() == 0);
}

TEST_CASE("com_ptr<T>: copy assignment releases the old object and adds a reference",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj1 = new CFakeUnknown();
	IFakeIUnknown* obj2 = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr1(obj1);
	mst::com_ptr<IFakeIUnknown> ptr2(obj2);

	ptr2 = ptr1;

	REQUIRE(ptr1.get() == obj1);
	REQUIRE(ptr2.get() == obj1);

	REQUIRE(ptr2.reset() == 1);
	REQUIRE(ptr1.reset() == 0);
}

TEST_CASE("com_ptr<T>: move assignment releases the old object without adding a reference",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj1 = new CFakeUnknown();
	IFakeIUnknown* obj2 = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr1(obj1);
	mst::com_ptr<IFakeIUnknown> ptr2(obj2);

	ptr2 = std::move(ptr1);

	REQUIRE(ptr1 == nullptr);
	REQUIRE(ptr2.get() == obj1);

	REQUIRE(ptr2.reset() == 0);
}

TEST_CASE("com_ptr<T>: swap exchanges the held objects", "[memory][com_ptr]")
{
	IFakeIUnknown* obj1 = new CFakeUnknown();
	IFakeIUnknown* obj2 = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr1(obj1);
	mst::com_ptr<IFakeIUnknown> ptr2(obj2);

	ptr1.swap(ptr2);

	REQUIRE(ptr1.get() == obj2);
	REQUIRE(ptr2.get() == obj1);

	REQUIRE(ptr1.reset() == 0);
	REQUIRE(ptr2.reset() == 0);
}

TEST_CASE("com_ptr<T>: release relinquishes ownership without releasing the reference",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	IFakeIUnknown* released = ptr.release();

	REQUIRE(released == obj);
	REQUIRE(ptr == nullptr);

	// the reference held by `ptr` is still valid and must be released manually
	REQUIRE(released->Release() == 0);
}

TEST_CASE("com_ptr<T>: operator-> and operator* access the underlying object", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	REQUIRE(ptr->AddRef() == 2);
	REQUIRE(&*ptr == obj);

	REQUIRE(ptr.reset() == 1);
	REQUIRE(obj->Release() == 0);
}

TEST_CASE("com_ptr<T>: equality and inequality compare the held pointer", "[memory][com_ptr]")
{
	IFakeIUnknown* obj1 = new CFakeUnknown();
	IFakeIUnknown* obj2 = new CFakeUnknown();

	REQUIRE(obj1->AddRef() == 2);

	mst::com_ptr<IFakeIUnknown> ptr1(obj1);
	mst::com_ptr<IFakeIUnknown> ptr1Same(obj1);
	mst::com_ptr<IFakeIUnknown> ptr2(obj2);

	REQUIRE(ptr1 == ptr1Same);
	REQUIRE(ptr1 != ptr2);

	REQUIRE(ptr1Same.reset() == 1);
	REQUIRE(ptr1.reset() == 0);
	REQUIRE(ptr2.reset() == 0);
}

TEST_CASE("com_ptr<T>: make_com_ptr wraps a raw pointer without adding a reference",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	auto ptr = mst::make_com_ptr(obj);

	static_assert(
		std::is_same<decltype(ptr), mst::com_ptr<IFakeIUnknown>>::value, "unexpected type");

	REQUIRE(ptr.get() == obj);
	REQUIRE(ptr.reset() == 0);
}

TEST_CASE("com_ptr<T>: as<T2> upcasts, downcasts and rejects unrelated interfaces",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	// downcast: queries the object for the more derived interface
	mst::com_ptr<IFakeIUnknown2> derived = ptr.as<IFakeIUnknown2>();
	REQUIRE(derived != nullptr);
	REQUIRE(derived.get() != nullptr);

	// unrelated interface: the object does not support it
	mst::com_ptr<IWrongUnknown> wrong = ptr.as<IWrongUnknown>();
	REQUIRE(wrong == nullptr);

	// upcast: no QueryInterface call needed, just adds a reference
	mst::com_ptr<IUnknown> asUnknown = ptr.as<IUnknown>();
	REQUIRE(asUnknown != nullptr);

	REQUIRE(asUnknown.reset() == 2);
	REQUIRE(derived.reset() == 1);
	REQUIRE(ptr.reset() == 0);
}

TEST_CASE("com_ptr<T>: is<T2> reports whether the interface is supported", "[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> ptr(obj);

	REQUIRE(ptr.is<IFakeIUnknown>());
	REQUIRE(ptr.is<IFakeIUnknown2>());
	REQUIRE(ptr.is<IUnknown>());
	REQUIRE_FALSE(ptr.is<IWrongUnknown>());

	// `is` must not leak a reference
	REQUIRE(ptr.reset() == 0);
}

TEST_CASE("com_ptr<T>: MST_IID_PPV_ARGS queries for the com_ptr's element type",
	"[memory][com_ptr]")
{
	IFakeIUnknown* obj = new CFakeUnknown();

	mst::com_ptr<IFakeIUnknown> source(obj);
	mst::com_ptr<IFakeIUnknown2> target;

	REQUIRE(SUCCEEDED(source->QueryInterface(MST_IID_PPV_ARGS(target))));

	REQUIRE(target != nullptr);
	REQUIRE(target.get() == obj);

	REQUIRE(source.reset() == 1);
	REQUIRE(target.reset() == 0);
}