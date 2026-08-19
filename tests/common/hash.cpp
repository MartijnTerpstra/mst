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

#include <set_assertions.h>

#include <mcommon.h>
#include <string>

TEST_CASE("hash32: literal & std::string are equal", "[common]")
{
	REQUIRE(mst::hash32("") == mst::hash32(std::string("")));
	REQUIRE(mst::hash32("test") == mst::hash32(std::string("test")));
}

TEST_CASE("hash64: literal std::string are equal", "[common]")
{
	REQUIRE(mst::hash64("") == mst::hash64(std::string("")));
	REQUIRE(mst::hash64("test") == mst::hash64(std::string("test")));
}

TEST_CASE("hash32: different values give different hashes", "[common]")
{
	REQUIRE(mst::hash32("1") != mst::hash32("2"));
	REQUIRE(mst::hash32("test1") != mst::hash32(std::string("test2")));
}

TEST_CASE("hash64: different values give different hashes", "[common]")
{
	REQUIRE(mst::hash64("1") != mst::hash64("2"));
	REQUIRE(mst::hash64("test1") != mst::hash64(std::string("test2")));
}

// a named (not anonymous) namespace: MSVC's __FUNCSIG__ spells the anonymous-namespace anchor
// inconsistently even within a single signature (`anonymous-namespace'` vs. `anonymous
// namespace'` vs. a synthesized per-TU tag were all observed for the very same anonymous
// namespace in one typename_of<Pair<Foo, Bar>>() call) - exactly the kind of thing this rewrite
// doesn't try to unify, so it's deliberately avoided here to keep the content assertions below
// meaningful.
namespace hash_test_types {

struct Foo
{ };
typedef Foo FooAlias;
using FooAlias2 = Foo;

struct Bar
{
	int intMember;
	float floatMember;
};

template<typename A, typename B>
struct Pair
{ };

enum class Color
{
	Red,
	Green
};

} // namespace hash_test_types

using namespace hash_test_types;

TEST_CASE("hash_of/hash_of64/typename_of: typedefs and aliases of the same type match", "[common]")
{
	REQUIRE(mst::hash_of<Foo>() == mst::hash_of<FooAlias>());
	REQUIRE(mst::hash_of<Foo>() == mst::hash_of<FooAlias2>());
	REQUIRE(mst::hash_of64<Foo>() == mst::hash_of64<FooAlias>());
	REQUIRE(mst::hash_of64<Foo>() == mst::hash_of64<FooAlias2>());
	REQUIRE(std::string(mst::typename_of<Foo>()) == mst::typename_of<FooAlias>());
	REQUIRE(std::string(mst::typename_of<Foo>()) == mst::typename_of<FooAlias2>());
}

TEST_CASE("hash_of/hash_of64/typename_of: distinct types never collide", "[common]")
{
	// these two specifically used to collide: the elaborated-keyword-stripping heuristic ate
	// the "unsigned" token along with it
	REQUIRE(mst::hash_of<unsigned int>() != mst::hash_of<int>());
	REQUIRE(mst::hash_of64<unsigned int>() != mst::hash_of64<int>());
	REQUIRE(std::string(mst::typename_of<unsigned int>()) != mst::typename_of<int>());

	REQUIRE(mst::hash_of<Foo>() != mst::hash_of<Bar>());
	REQUIRE(mst::hash_of<int>() != mst::hash_of<float>());
	REQUIRE(mst::hash_of<int>() != mst::hash_of<long long>());
	REQUIRE(mst::hash_of<int*>() != mst::hash_of<int>());
	REQUIRE(mst::hash_of<int*>() != mst::hash_of<int&>());
	REQUIRE(mst::hash_of<int[3]>() != mst::hash_of<int[4]>());
	REQUIRE(mst::hash_of<Color>() != mst::hash_of<int>());

	// pointer-to-member: the payload type used to be lost entirely
	REQUIRE(
		mst::hash_of<decltype(&Bar::intMember)>() != mst::hash_of<decltype(&Bar::floatMember)>());

	// template arguments: nested tag-type keywords (e.g. MSVC's "struct ") used to only be
	// stripped from the outermost type, so two structurally-different instantiations could
	// still be told apart, but the same instantiation disagreed with typename_of's own spelling
	REQUIRE(mst::hash_of<Pair<Foo, Foo>>() != mst::hash_of<Pair<Foo, Bar>>());
}

TEST_CASE("hash_of/hash_of64/typename_of: same type is stable across repeated calls", "[common]")
{
	REQUIRE(mst::hash_of<Pair<Foo, Bar>>() == mst::hash_of<Pair<Foo, Bar>>());
	REQUIRE(mst::hash_of64<Pair<Foo, Bar>>() == mst::hash_of64<Pair<Foo, Bar>>());
	REQUIRE(std::string(mst::typename_of<Pair<Foo, Bar>>()) == mst::typename_of<Pair<Foo, Bar>>());
}

TEST_CASE("hash_of64/typename_of: hash_of64<T>() agrees with hashing typename_of<T>()", "[common]")
{
	REQUIRE(mst::hash_of64<Foo>() == mst::hash64(mst::typename_of<Foo>()));
	REQUIRE(mst::hash_of64<Bar>() == mst::hash64(mst::typename_of<Bar>()));
	REQUIRE(mst::hash_of64<Pair<Foo, Bar>>() == mst::hash64(mst::typename_of<Pair<Foo, Bar>>()));
	REQUIRE(mst::hash_of64<unsigned long long>() ==
			mst::hash64(mst::typename_of<unsigned long long>()));
}

// These canonical spellings involve no anonymous-namespace decoration and no std:: library
// spelling - the two things this rewrite deliberately doesn't try to unify across compilers
// (see the comment in mx_hash.h) - so the exact string is expected to be identical on every
// supported compiler.
TEST_CASE(
	"typename_of: canonical spelling of fundamental types, cv/pointer/reference/array", "[common]")
{
	REQUIRE(std::string(mst::typename_of<int>()) == "int");
	REQUIRE(std::string(mst::typename_of<bool>()) == "bool");
	REQUIRE(std::string(mst::typename_of<char>()) == "char");
	REQUIRE(std::string(mst::typename_of<float>()) == "float");
	REQUIRE(std::string(mst::typename_of<double>()) == "double");

	REQUIRE(std::string(mst::typename_of<short>()) == "short");
	REQUIRE(std::string(mst::typename_of<unsigned short>()) == "unsigned short");
	REQUIRE(std::string(mst::typename_of<unsigned int>()) == "unsigned int");
	REQUIRE(std::string(mst::typename_of<long>()) == "long");
	REQUIRE(std::string(mst::typename_of<unsigned long>()) == "unsigned long");
	REQUIRE(std::string(mst::typename_of<long long>()) == "long long");
	REQUIRE(std::string(mst::typename_of<unsigned long long>()) == "unsigned long long");

	REQUIRE(std::string(mst::typename_of<const int>()) == "const int");
	REQUIRE(std::string(mst::typename_of<int*>()) == "int*");
	REQUIRE(std::string(mst::typename_of<const int*>()) == "const int*");
	REQUIRE(std::string(mst::typename_of<int&>()) == "int&");
	REQUIRE(std::string(mst::typename_of<int&&>()) == "int&&");
	REQUIRE(std::string(mst::typename_of<int[3]>()) == "int[3]");
	REQUIRE(std::string(mst::typename_of<int[4]>()) == "int[4]");
}

TEST_CASE("typename_of: elaborated-type keywords are stripped everywhere, "
		  "including nested template arguments",
	"[common]")
{
	auto hasNoElaboratedKeyword = [](const std::string& name) {
		return name.find("struct ") == std::string::npos &&
			   name.find("class ") == std::string::npos &&
			   name.find("enum ") == std::string::npos && name.find("union ") == std::string::npos;
	};

	const std::string fooName = mst::typename_of<Foo>();
	const std::string pairName = mst::typename_of<Pair<Foo, Bar>>();

	REQUIRE(hasNoElaboratedKeyword(fooName));
	REQUIRE(hasNoElaboratedKeyword(pairName));

	// both template arguments are present, in order, with no space around the separating comma
	REQUIRE(pairName.find("Pair<") != std::string::npos);
	REQUIRE(pairName.find("Foo,hash_test_types::Bar") != std::string::npos);
}

// Pinned FNV-1a values (independently computed - not derived from mst::hash32/hash64) for the
// canonical spellings asserted above. hash_of/hash_of64 are FNV-1a over exactly that text, so for
// these compiler-independent spellings the values below are expected to be identical on every
// supported compiler, and to stay identical across library versions - if either the
// canonicalization or the FNV constants ever change, this is the test that should catch it, since
// existing serialized data keyed on these hashes would otherwise silently break.
TEST_CASE("hash_of/hash_of64: pinned values for compiler-independent canonical spellings", "[common]")
{
	REQUIRE(mst::hash_of<int>() == 0x95e97e5eU);
	REQUIRE(mst::hash_of64<int>() == 0x2b9fff19004b3727ULL);

	REQUIRE(mst::hash_of<float>() == 0xa6c45d85U);
	REQUIRE(mst::hash_of64<float>() == 0xa00a62a9e2b863ccULL);

	REQUIRE(mst::hash_of<unsigned long long>() == 0x4ccbc7e2U);
	REQUIRE(mst::hash_of64<unsigned long long>() == 0xc2a861677cc58405ULL);

	REQUIRE(mst::hash_of<int*>() == 0x7292109cU);
	REQUIRE(mst::hash_of64<int*>() == 0xf5a68bc58f297239ULL);

	REQUIRE(mst::hash_of<const int*>() == 0x6d60f45dU);
	REQUIRE(mst::hash_of64<const int*>() == 0x4dd3a7d8edf13c65ULL);

	REQUIRE(mst::hash_of<int&&>() == 0x75f9e64aU);
	REQUIRE(mst::hash_of64<int&&>() == 0xfa073a8fb8b6aee5ULL);

	REQUIRE(mst::hash_of<int[3]>() == 0xe024279bU);
	REQUIRE(mst::hash_of64<int[3]>() == 0x2681c96febbf9814ULL);
}