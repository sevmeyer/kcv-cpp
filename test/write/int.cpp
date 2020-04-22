#include <catch2/catch.hpp>

#include <cstdint>
#include <limits>

#include "kcv/kcv.hpp"


TEST_CASE("write int from char")
{
	kcv::Document doc{};

	SECTION("char is treated as integral type")
	{
		char c{42};
		doc[u8"i"] << c;
		REQUIRE(doc.dump() == u8"i: 42\n");
	}
	SECTION("signed char is treated as integral type")
	{
		signed char c{-128};
		doc[u8"i"] << c;
		REQUIRE(doc.dump() == u8"i: -128\n");
	}
	SECTION("unsigned char is treated as integral type")
	{
		unsigned char c{255};
		doc[u8"i"] << c;
		REQUIRE(doc.dump() == u8"i: 255\n");
	}
}


TEST_CASE("write int from int32")
{
	kcv::Document doc{};

	SECTION("minimum")
	{
		doc[u8"i"] << std::numeric_limits<std::int32_t>::min();
		REQUIRE(doc.dump() == u8"i: -2147483648\n");
	}
	SECTION("maximum")
	{
		doc[u8"i"] << std::numeric_limits<std::int32_t>::max();
		REQUIRE(doc.dump() == u8"i: 2147483647\n");
	}
}


TEST_CASE("write int from uint32")
{
	kcv::Document doc{};

	SECTION("minimum")
	{
		doc[u8"i"] << std::numeric_limits<std::uint32_t>::min();
		REQUIRE(doc.dump() == u8"i: 0\n");
	}
	SECTION("maximum")
	{
		doc[u8"i"] << std::numeric_limits<std::uint32_t>::max();
		REQUIRE(doc.dump() == u8"i: 4294967295\n");
	}
}


TEST_CASE("write int from int64")
{
	kcv::Document doc{};

	SECTION("minimum")
	{
		doc[u8"i"] << std::numeric_limits<std::int64_t>::min();
		REQUIRE(doc.dump() == u8"i: -9223372036854775808\n");
	}
	SECTION("maximum")
	{
		doc[u8"i"] << std::numeric_limits<std::int64_t>::max();
		REQUIRE(doc.dump() == u8"i: 9223372036854775807\n");
	}
}


TEST_CASE("write int from uint64")
{
	kcv::Document doc{};

	SECTION("minimum")
	{
		doc[u8"i"] << std::numeric_limits<std::uint64_t>::min();
		REQUIRE(doc.dump() == u8"i: 0\n");
	}
	SECTION("maximum")
	{
		doc[u8"i"] << std::numeric_limits<std::uint64_t>::max();
		REQUIRE(doc.dump() == u8"i: 18446744073709551615\n");
	}
}
