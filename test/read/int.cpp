#include <catch2/catch.hpp>

#include <cstdint>
#include <limits>

#include "kcv/kcv.hpp"


TEST_CASE("read int syntax")
{
	int i{1};

	SECTION("negative zero is parsed as zero")
	{
		kcv::Document doc{u8"i:-0"};
		doc[u8"i"] >> i;
		REQUIRE(i == 0);
	}
	SECTION("leading zero must not be parsed as octal")
	{
		kcv::Document doc{u8"i:010"};
		doc[u8"i"] >> i;
		REQUIRE(i == 10);
	}
	SECTION("missing digits after minus")
	{
		kcv::Document doc{u8"i:-"};
		REQUIRE_FALSE(doc);
	}
	SECTION("must not include fraction")
	{
		kcv::Document doc{u8"i:2.0"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("must not include exponent")
	{
		kcv::Document doc{u8"i:1e2"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("bool instead of integer")
	{
		kcv::Document doc{u8"i:yes"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("string instead of integer")
	{
		kcv::Document doc{u8"i:\"1\""};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read int to char")
{
	SECTION("char is treated as integral type")
	{
		char c{0};
		kcv::Document doc{u8"c:42"};
		doc[u8"c"] >> c;
		REQUIRE(c == 42);
	}
	SECTION("signed char is treated as integral type")
	{
		signed char c{0};
		kcv::Document doc{u8"c:-128"};
		doc[u8"c"] >> c;
		REQUIRE(c == -128);
	}
	SECTION("unsigned char is treated as integral type")
	{
		unsigned char c{0};
		kcv::Document doc{u8"c:255"};
		doc[u8"c"] >> c;
		REQUIRE(c == 255);
	}
}


TEST_CASE("read int to int32")
{
	std::int32_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:-2147483648"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:2147483647"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("underflow")
	{
		kcv::Document doc{u8"i:-2147483649"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:2147483648"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read int to uint32")
{
	std::uint32_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:0"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:4294967295"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("underflow")
	{
		kcv::Document doc{u8"i:-1"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:4294967296"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read int to int64")
{
	std::int64_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:-9223372036854775808"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:9223372036854775807"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("underflow")
	{
		kcv::Document doc{u8"i:-9223372036854775809"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:9223372036854775808"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read int to uint64")
{
	std::uint64_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:0"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:18446744073709551615"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("underflow")
	{
		kcv::Document doc{u8"i:-1"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:18446744073709551616"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}
