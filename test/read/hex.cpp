#include <catch2/catch.hpp>

#include <cstdint>
#include <limits>

#include "kcv/kcv.hpp"


TEST_CASE("read hex syntax")
{
	int i{1};

	SECTION("case insensitive")
	{
		kcv::Document doc{u8"i:0xAb"};
		doc[u8"i"] >> i;
		REQUIRE(i == 171);
	}
	SECTION("ignore leading zero")
	{
		kcv::Document doc{u8"i:0x000000000000000000000000000000002"};
		doc[u8"i"] >> i;
		REQUIRE(i == 2);
	}
	SECTION("missing 0x prefix")
	{
		kcv::Document doc{u8"i:ffdd55"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing digits")
	{
		kcv::Document doc{u8"i:0x"};
		REQUIRE_FALSE(doc);
	}
	SECTION("cannot be negative")
	{
		kcv::Document doc{u8"i:-0x42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("invalid digit character")
	{
		kcv::Document doc{u8"i:0xG"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read hex to int32")
{
	std::int32_t i{1};

	SECTION("maximum")
	{
		kcv::Document doc{u8"i:0x7fffffff"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:0x80000000"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read hex to uint32")
{
	std::uint32_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:0x0"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:0xffffffff"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:0x100000000"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read hex to int64")
{
	std::int64_t i{1};

	SECTION("maximum")
	{
		kcv::Document doc{u8"i:0x7fffffffffffffff"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:0x8000000000000000"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}


TEST_CASE("read hex to uint64")
{
	std::uint64_t i{1};

	SECTION("minimum")
	{
		kcv::Document doc{u8"i:0x0"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SECTION("maximum")
	{
		kcv::Document doc{u8"i:0xffffffffffffffff"};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SECTION("overflow")
	{
		kcv::Document doc{u8"i:0x10000000000000000"};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> i);
		REQUIRE(i == 1);
	}
}
