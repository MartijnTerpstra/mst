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
#include <vector>
#include <map>
#include <memory>

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

template<typename T>
using SamePair = Pair<T, T>;

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

// std::vector/map/shared_ptr are the containers most likely to actually get hashed by consumers,
// and they exercise real-world signature shapes the fixture types above don't: a default
// allocator/comparator argument the compiler fills in, and (unlike Pair<Foo, Bar> above) types
// that live under an std:: inline namespace whose spelling is stdlib-specific. That spelling
// difference is exactly the documented, deliberately-out-of-scope limitation (see the comment in
// mx_hash.h) for cross-compiler comparison, so this only asserts the same relational invariants
// as the fixture-type tests above (typedef transparency, distinctness, stability) rather than any
// pinned string/hash value - those remain guaranteed within one compiler/stdlib build.
TEST_CASE("hash_of/hash_of64/typename_of: std:: container templates (vector, map, shared_ptr)",
	"[common]")
{
	// typedefs of a std:: template instantiation collapse, same as for any other type
	typedef std::vector<int> IntVector;
	using IntVector2 = std::vector<int>;
	REQUIRE(mst::hash_of<std::vector<int>>() == mst::hash_of<IntVector>());
	REQUIRE(mst::hash_of<std::vector<int>>() == mst::hash_of<IntVector2>());
	REQUIRE(std::string(mst::typename_of<std::vector<int>>()) == mst::typename_of<IntVector>());

	// a typedef used as the template argument itself collapses too, not just at the top level
	REQUIRE(mst::hash_of<std::vector<Foo>>() == mst::hash_of<std::vector<FooAlias>>());
	REQUIRE(mst::hash_of64<std::vector<Foo>>() == mst::hash_of64<std::vector<FooAlias>>());

	// distinct element/key/value types never collide, including key/value order in std::map
	REQUIRE(mst::hash_of<std::vector<int>>() != mst::hash_of<std::vector<float>>());
	REQUIRE(mst::hash_of<std::map<int, float>>() != mst::hash_of<std::map<int, int>>());
	REQUIRE(mst::hash_of<std::map<int, float>>() != mst::hash_of<std::map<float, int>>());
	REQUIRE(mst::hash_of<std::shared_ptr<Foo>>() != mst::hash_of<std::shared_ptr<Bar>>());

	// different container templates around the same element type never collide either
	REQUIRE(mst::hash_of<std::vector<int>>() != mst::hash_of<std::shared_ptr<int>>());
	REQUIRE(mst::hash_of<std::vector<int>>() != mst::hash_of<std::map<int, int>>());

	// stable across repeated calls, and hash_of64 agrees with hashing typename_of, same as any
	// other type - the stdlib-specific spelling of these types only affects cross-compiler
	// comparison, not self-consistency within one compiler/build
	REQUIRE(mst::hash_of<std::map<int, Foo>>() == mst::hash_of<std::map<int, Foo>>());
	REQUIRE(mst::hash_of64<std::shared_ptr<Bar>>()
			== mst::hash64(mst::typename_of<std::shared_ptr<Bar>>()));
}

// hash_test_types is a named namespace (see the comment where it's declared above), so unlike
// the std:: container test above, these spellings involve no compiler-specific decoration and
// are expected to be identical - pinned values and all - on every supported compiler.
TEST_CASE("hash_of/hash_of64/typename_of: nested templates and alias templates", "[common]")
{
	// a template argument that is itself a template instantiation: position matters...
	REQUIRE(mst::hash_of<Pair<Pair<Foo, Bar>, Foo>>()
			!= mst::hash_of<Pair<Foo, Pair<Foo, Bar>>>());

	// ...but is otherwise stable, and hash_of64 still agrees with hashing typename_of two levels
	// deep
	REQUIRE(mst::hash_of<Pair<Pair<Foo, Bar>, Foo>>() == mst::hash_of<Pair<Pair<Foo, Bar>, Foo>>());
	REQUIRE(mst::hash_of64<Pair<Pair<Foo, Bar>, Foo>>()
			== mst::hash64(mst::typename_of<Pair<Pair<Foo, Bar>, Foo>>()));

	// typedef transparency still holds for a typedef used at the nested position, not just the
	// outermost or the immediately-nested one
	REQUIRE(mst::hash_of<Pair<Pair<Foo, Bar>, Foo>>()
			== mst::hash_of<Pair<Pair<FooAlias, Bar>, FooAlias2>>());

	// alias templates (template<typename T> using SamePair = Pair<T, T>;) resolve to their
	// underlying instantiation, same as any other alias - std::is_same_v<SamePair<Foo>,
	// Pair<Foo, Foo>> is true, and so is this
	REQUIRE(mst::hash_of<SamePair<Foo>>() == mst::hash_of<Pair<Foo, Foo>>());
	REQUIRE(mst::hash_of64<SamePair<Bar>>() == mst::hash_of64<Pair<Bar, Bar>>());
	REQUIRE(std::string(mst::typename_of<SamePair<Foo>>()) == mst::typename_of<Pair<Foo, Foo>>());

	// ...and a template argument that is itself an alias-template instantiation collapses too
	REQUIRE(mst::hash_of<Pair<SamePair<Foo>, Bar>>() == mst::hash_of<Pair<Pair<Foo, Foo>, Bar>>());

	// pinned exact values: these are the ones that should actually confirm cross-compiler
	// agreement in CI, not just self-consistency
	REQUIRE(std::string(mst::typename_of<Pair<Pair<Foo, Bar>, Foo>>())
			== "hash_test_types::Pair<hash_test_types::Pair<hash_test_types::Foo,"
			   "hash_test_types::Bar>,hash_test_types::Foo>");
	REQUIRE(mst::hash_of<Pair<Pair<Foo, Bar>, Foo>>() == 0xdd7e107fU);
	REQUIRE(mst::hash_of64<Pair<Pair<Foo, Bar>, Foo>>() == 0xb09cfd661a34deb9ULL);

	REQUIRE(std::string(mst::typename_of<Pair<Foo, Pair<Foo, Bar>>>())
			== "hash_test_types::Pair<hash_test_types::Foo,hash_test_types::Pair<"
			   "hash_test_types::Foo,hash_test_types::Bar>>");
	REQUIRE(mst::hash_of<Pair<Foo, Pair<Foo, Bar>>>() == 0x0c77f705U);
	REQUIRE(mst::hash_of64<Pair<Foo, Pair<Foo, Bar>>>() == 0xb93592b1cc5f72b4ULL);

	REQUIRE(std::string(mst::typename_of<SamePair<Foo>>())
			== "hash_test_types::Pair<hash_test_types::Foo,hash_test_types::Foo>");
	REQUIRE(mst::hash_of<SamePair<Foo>>() == 0x09511974U);
	REQUIRE(mst::hash_of64<SamePair<Foo>>() == 0x8f8884b58a203141ULL);
}