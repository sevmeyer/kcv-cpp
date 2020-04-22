#include <catch2/catch.hpp>

#include <limits>
#include <string>
#include <string_view>

#include "kcv/kcv.hpp"


TEST_CASE("write item key")
{
	kcv::Document doc{};

	SECTION("valid key characters")
	{
		doc[u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._"] << 42;
		REQUIRE(doc.dump() == u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._: 42\n");
	}
	SECTION("invalid leading digit")
	{
		auto foo{doc[u8"1foo"]};
		REQUIRE_FALSE(foo);
	}
	SECTION("invalid leading minus")
	{
		auto foo{doc[u8"-foo"]};
		REQUIRE_FALSE(foo);
	}
	SECTION("invalid leading underscore")
	{
		auto foo{doc[u8"_foo"]};
		REQUIRE_FALSE(foo);
	}
	SECTION("invalid key character")
	{
		auto foo{doc[u8"a/b"]};
		REQUIRE_FALSE(foo);
	}
	SECTION("empty key")
	{
		auto foo{doc[u8""]};
		REQUIRE_FALSE(foo);
	}
	SECTION("string object as key")
	{
		std::string s{u8"foo"};
		auto foo{doc[s]};
		REQUIRE(foo);
		REQUIRE(doc.dump() == u8"foo:\n");
	}
	SECTION("string_view object as key")
	{
		std::string s{u8"foo"};
		std::string_view v{s};
		auto foo{doc[v]};
		REQUIRE(foo);
		REQUIRE(doc.dump() == u8"foo:\n");
	}
	SECTION("writing to invalid item has no effect")
	{
		auto foo{doc[u8"???"]};
		REQUIRE_FALSE(foo);
		foo << 42 << kcv::nl() << 1.0 << u8"Hello";
		REQUIRE(doc.dump() == u8"");
	}
}


TEST_CASE("write item values")
{
	kcv::Document doc{};

	SECTION("no value")
	{
		doc[u8"a"];
		REQUIRE(doc.dump() == u8"a:\n");
	}
	SECTION("one value")
	{
		doc[u8"a"] << 1;
		REQUIRE(doc.dump() == u8"a: 1\n");
	}
	SECTION("two values")
	{
		doc[u8"a"] << 1 << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("can continue after invalid value")
	{
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		a << std::numeric_limits<float>::quiet_NaN();
		REQUIRE_FALSE(a);
		a << 2;
		REQUIRE(a);
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("mixed value types")
	{
		doc[u8"a"] << 42 << 3.14159 << true << u8"Hello";
		REQUIRE(doc.dump() == u8"a: 42 3.14159 yes \"Hello\"\n");
	}
	SECTION("write more values than original")
	{
		kcv::Document abc{"a:1 b:2 c:3"};
		abc[u8"a"] << 1 << 11 << 111 << 1111 << 11111 << 111111 << 1111111 << 11111111;
		abc[u8"b"] << 2 << 22 << 222 << 2222 << 22222 << 222222 << 2222222 << 22222222;
		abc[u8"c"] << 3 << 33 << 333 << 3333 << 33333 << 333333 << 3333333 << 33333333;
		REQUIRE(abc.dump() ==
			u8"a: 1 11 111 1111 11111 111111 1111111 11111111\n"
			u8"b: 2 22 222 2222 22222 222222 2222222 22222222\n"
			u8"c: 3 33 333 3333 33333 333333 3333333 33333333\n");
	}
}


TEST_CASE("write item sequence")
{
	kcv::Document doc{};

	SECTION("no item")
	{
		REQUIRE(doc.dump() == "");
	}
	SECTION("one item")
	{
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		REQUIRE(doc.dump() == u8"a: 1\n");
	}
	SECTION("two items ordered lexicographically")
	{
		auto b{doc[u8"b"]};
		b << 2;
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		REQUIRE(b);
		REQUIRE(doc.dump() == u8"a: 1\nb: 2\n");
	}
	SECTION("first write to new item instance clears old content")
	{
		auto a1{doc[u8"a"]};
		a1 << 1;
		REQUIRE(a1);
		REQUIRE(doc.dump() == u8"a: 1\n");
		auto a2{doc[u8"a"]};
		a2 << 2;
		REQUIRE(a2);
		REQUIRE(doc.dump() == u8"a: 2\n");
		a2 << 3;
		REQUIRE(a2);
		REQUIRE(doc.dump() == u8"a: 2 3\n");
	}
	SECTION("changing stream direction clears old content")
	{
		int i{1};
		auto a{doc[u8"a"]};
		a << 11;
		a >> i;
		REQUIRE(i == 11);
		a << 22;
		a >> i;
		REQUIRE(i == 22);
		REQUIRE(doc.dump() == u8"a: 22\n");
	}
	SECTION("mixed write and read of items")
	{
		int i{1};
		doc[u8"b"] << 22;
		doc[u8"a"] << 11;
		doc[u8"c"] << 33;

		doc[u8"a"] >> i;
		REQUIRE(i == 11);
		doc[u8"c"] >> i;
		REQUIRE(i == 33);
		doc[u8"b"] >> i;
		REQUIRE(i == 22);

		doc[u8"c"] << 3333;
		doc[u8"b"] << 2222;
		doc[u8"a"] << 1111;

		doc[u8"b"] >> i;
		REQUIRE(i == 2222);
		doc[u8"a"] >> i;
		REQUIRE(i == 1111);
		doc[u8"c"] >> i;
		REQUIRE(i == 3333);

		REQUIRE(doc.dump() == u8"a: 1111\nb: 2222\nc: 3333\n");
	}
}


TEST_CASE("write item after failed document construction")
{
	SECTION("invalid string is discarded")
	{
		kcv::Document doc{u8"a:?"};
		REQUIRE_FALSE(doc);
		REQUIRE(doc.dump() == u8"");
	}
	SECTION("create new item in empty document")
	{
		kcv::Document doc{u8"?"};
		REQUIRE_FALSE(doc);
		auto a{doc[u8"a"]};
		REQUIRE(a);
		REQUIRE(a << 42);
		REQUIRE(doc.dump() == u8"a: 42\n");
	}
	SECTION("create new empty item instead of reading invalid data")
	{
		kcv::Document doc{u8"a:42 b:?"};
		REQUIRE_FALSE(doc);
		int i{1};
		auto a{doc[u8"a"]};
		REQUIRE(a);
		REQUIRE_FALSE(a >> i);
		REQUIRE(i == 1);
		REQUIRE(doc.dump() == u8"a:\n");
	}
}


TEST_CASE("write item whitespace formatting")
{
	kcv::Document doc{};

	SECTION("default newline count is 1")
	{
		doc[u8"a"] << 1 << kcv::nl() << 2;
		REQUIRE(doc.dump() == u8"a: 1\n2\n");
	}
	SECTION("invalid newline count has no effect")
	{
		doc[u8"a"] << 1 << kcv::nl(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("newline count 2")
	{
		doc[u8"a"] << 1 << kcv::nl(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1\n\n2\n");
	}
	SECTION("default space count is 1")
	{
		doc[u8"a"] << 1 << kcv::sp() << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("invalid space count has no effect")
	{
		doc[u8"a"] << 1 << kcv::sp(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("space count 2")
	{
		doc[u8"a"] << 1 << kcv::sp(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1  2\n");
	}
	SECTION("default tab count is 1")
	{
		doc[u8"a"] << 1 << kcv::tab() << 2;
		REQUIRE(doc.dump() == u8"a: 1\t2\n");
	}
	SECTION("invalid tab count has no effect")
	{
		doc[u8"a"] << 1 << kcv::tab(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SECTION("tab count 2")
	{
		doc[u8"a"] << 1 << kcv::tab(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1\t\t2\n");
	}
}


TEST_CASE("write item whitespace consistency")
{
	SECTION("do not propagate utf8 byte order mark")
	{
		kcv::Document doc{u8"\xEF\xBB\xBFi: 42\n"};
		REQUIRE(doc.dump() == u8"i: 42\n");
	}
	SECTION("preserve existing whitespace before first value")
	{
		kcv::Document doc{u8"a:\n \t42"};
		REQUIRE(doc.dump() == u8"a:\n \t42\n");
	}
	SECTION("preserve custom whitespace before first value")
	{
		kcv::Document doc{};
		doc[u8"a"] << kcv::nl() << kcv::sp() << kcv::tab() << 42;
		REQUIRE(doc.dump() == u8"a:\n \t42\n");
	}
	SECTION("ignore existing whitespace after last newline of item")
	{
		kcv::Document doc{u8"a: 42\n \t"};
		REQUIRE(doc.dump() == u8"a: 42\n");
	}
	SECTION("ignore custom whitespace after last newline of item")
	{
		kcv::Document doc{};
		doc[u8"a"] << 42 << kcv::nl() << kcv::sp() << kcv::tab();
		REQUIRE(doc.dump() == u8"a: 42\n");
	}
	SECTION("preserve custom newlines")
	{
		kcv::Document doc{};
		doc[u8"a"] << 42 << kcv::nl() << kcv::nl() << kcv::nl();
		REQUIRE(doc.dump() == u8"a: 42\n\n\n");
	}
}
