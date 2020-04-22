#include <catch2/catch.hpp>

#include "kcv/kcv.hpp"


TEST_CASE("read item key")
{
	int i{1};

	SECTION("key order is irrelevant")
	{
		kcv::Document doc{u8"c:33 a:11 b:22"};
		doc[u8"a"] >> i;
		REQUIRE(i == 11);
		doc[u8"b"] >> i;
		REQUIRE(i == 22);
		doc[u8"c"] >> i;
		REQUIRE(i == 33);
	}
	SECTION("valid key characters")
	{
		kcv::Document doc{u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._:42"};
		doc[u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._"] >> i;
		REQUIRE(i == 42);
	}
	SECTION("case sensitive key")
	{
		kcv::Document doc{u8"A:11 a:22"};
		doc[u8"A"] >> i;
		REQUIRE(i == 11);
		doc[u8"a"] >> i;
		REQUIRE(i == 22);
	}
	SECTION("keyword as key")
	{
		kcv::Document doc{u8"yes:42"};
		doc[u8"yes"] >> i;
		REQUIRE(i == 42);
	}
	SECTION("invalid numeric key")
	{
		kcv::Document doc{u8"42:24"};
		REQUIRE_FALSE(doc);
	}
	SECTION("invalid key character")
	{
		kcv::Document doc{u8"a/b:42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing key name")
	{
		kcv::Document doc{u8":42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing colon")
	{
		kcv::Document doc{u8"foo 42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("whitespace before colon")
	{
		kcv::Document doc{u8"foo :42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("equal sign instead of colon")
	{
		kcv::Document doc{u8"foo=42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("duplicate key")
	{
		kcv::Document doc{u8"a:11 a:22"};
		REQUIRE_FALSE(doc);
	}
	SECTION("value before first key")
	{
		kcv::Document doc{u8"42 a:11"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read item values")
{
	int i{1};

	SECTION("no value")
	{
		kcv::Document doc{u8"a:"};
		auto item{doc[u8"a"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("one value")
	{
		kcv::Document doc{u8"a:11"};
		auto item{doc[u8"a"]};
		REQUIRE(item >> i);
		REQUIRE(i == 11);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 11);
	}
	SECTION("two values")
	{
		kcv::Document doc{u8"a:11 22"};
		auto item{doc[u8"a"]};
		REQUIRE(item >> i);
		REQUIRE(i == 11);
		REQUIRE(item >> i);
		REQUIRE(i == 22);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 22);
	}
}


TEST_CASE("read item maxItemCount")
{
	int a{1};
	int b{1};

	SECTION("maxItemCount=0 defaults to no limit")
	{
		kcv::Document doc{u8"a:11 b:22", 0};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("maxItemCount=1")
	{
		kcv::Document doc{u8"a:11", 1};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
	}
	SECTION("maxItemCount=1 overflow")
	{
		kcv::Document doc{u8"a:11 b:22", 1};
		REQUIRE_FALSE(doc);
	}
	SECTION("maxItemCount=2")
	{
		kcv::Document doc{u8"a:11 b:22", 2};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("maxItemCount=2 overflow")
	{
		kcv::Document doc{u8"a:11 b:22 c:33", 2};
		REQUIRE_FALSE(doc);
	}
	SECTION("cannot create new items when maxItemCount is reached")
	{
		kcv::Document doc{u8"a:11", 1};
		REQUIRE(doc);
		REQUIRE(doc[u8"a"]);
		REQUIRE_FALSE(doc[u8"b"]);
	}
}


TEST_CASE("read item whitespace")
{
	int a{1};
	int b{1};

	SECTION("whitespace before item")
	{
		kcv::Document doc{u8"\n \r \t \r\na:42"};
		doc[u8"a"] >> a;
		REQUIRE(a == 42);
	}
	SECTION("whitespace after item")
	{
		kcv::Document doc{u8"a:42\r\n \t \r \n"};
		doc[u8"a"] >> a;
		REQUIRE(a == 42);
	}
	SECTION("whitespace between items")
	{
		kcv::Document doc{u8"a:11\r\n \t \r \nb:22"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("values are separated by space (U+0020)")
	{
		kcv::Document doc{u8"a:11 b:22"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("values are separated by tab (U+0009)")
	{
		kcv::Document doc{u8"a:11\tb:22"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("values are separated by line feed (U+000A)")
	{
		kcv::Document doc{u8"a:11\nb:22"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("values are separated by carriage return (U+000D)")
	{
		kcv::Document doc{u8"a:11\rb:22"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SECTION("whitespace after colon")
	{
		kcv::Document doc{u8"a:\r\n \t \r \n11"};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
	}
	SECTION("whitespace between values")
	{
		kcv::Document doc{u8"a:11\n \r \t \r\n22\r\n \t \r \n"};
		doc[u8"a"] >> a >> b;
		REQUIRE(a == 11);
		REQUIRE(b == 22);
	}
	SECTION("missing whitespace after string value")
	{
		kcv::Document doc{u8"a:\"A\"b:11"};
		REQUIRE_FALSE(doc);
	}
}
