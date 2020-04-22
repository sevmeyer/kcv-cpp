#include <catch2/catch.hpp>

#include <limits>
#include <string>

#include "kcv/kcv.hpp"


TEST_CASE("write float default format")
{
	kcv::Document doc{};

	SECTION("positive zero")
	{
		doc[u8"f"] << 0.0;
		REQUIRE(doc.dump() == u8"f: 0\n");
	}
	SECTION("negative zero")
	{
		doc[u8"f"] << -0.0;
		REQUIRE(doc.dump() == u8"f: -0\n");
	}
	SECTION("big negative")
	{
		doc[u8"f"] << -123456.0;
		REQUIRE(doc.dump() == u8"f: -123456\n");
	}
	SECTION("big positive")
	{
		doc[u8"f"] << 123456.0;
		REQUIRE(doc.dump() == u8"f: 123456\n");
	}
	SECTION("small negative")
	{
		doc[u8"f"] << -0.123456;
		REQUIRE(doc.dump() == u8"f: -0.123456\n");
	}
	SECTION("small positive")
	{
		doc[u8"f"] << 0.123456;
		REQUIRE(doc.dump() == u8"f: 0.123456\n");
	}
}


TEST_CASE("write float fixed format")
{
	kcv::Document doc{};

	SECTION("default precision is 6")
	{
		doc[u8"f"] << kcv::fixed(1.0);
		REQUIRE(doc.dump() == u8"f: 1.000000\n");
	}
	SECTION("negative precision is same as 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, -1);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SECTION("zero precision is same as 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, 0);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SECTION("precision 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, 1);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SECTION("precision 2")
	{
		doc[u8"f"] << kcv::fixed(1.0, 2);
		REQUIRE(doc.dump() == u8"f: 1.00\n");
	}
	SECTION("rounded negative")
	{
		doc[u8"f"] << kcv::fixed(-0.123456, 4);
		REQUIRE(doc.dump() == u8"f: -0.1235\n");
	}
	SECTION("rounded positive")
	{
		doc[u8"f"] << kcv::fixed(0.123456, 4);
		REQUIRE(doc.dump() == u8"f: 0.1235\n");
	}
	SECTION("excessive precision may be clamped")
	{
		doc[u8"f"] << kcv::fixed(0.123456, 1000);
		std::string s{doc.dump()};
		REQUIRE(s.substr(0, 10) == u8"f: 0.12345");
	}
}


TEST_CASE("write float rounded format")
{
	kcv::Document doc{};

	SECTION("default precision is 6")
	{
		doc[u8"f"] << kcv::general(0.12345678);
		REQUIRE(doc.dump() == u8"f: 0.123457\n");
	}
	SECTION("negative precision is same as 1")
	{
		doc[u8"f"] << kcv::general(1.0, -1);
		REQUIRE(doc.dump() == u8"f: 1\n");
	}
	SECTION("zero precision is same as 1")
	{
		doc[u8"f"] << kcv::general(1.0, 0);
		REQUIRE(doc.dump() == u8"f: 1\n");
	}
	SECTION("extended negative")
	{
		doc[u8"f"] << kcv::general(-16777216.0f, 8);
		REQUIRE(doc.dump() == u8"f: -16777216\n");
	}
	SECTION("extended positive")
	{
		doc[u8"f"] << kcv::general(16777216.0f, 8);
		REQUIRE(doc.dump() == u8"f: 16777216\n");
	}
	SECTION("rounded negative")
	{
		doc[u8"f"] << kcv::general(-0.123456, 4);
		REQUIRE(doc.dump() == u8"f: -0.1235\n");
	}
	SECTION("rounded positive")
	{
		doc[u8"f"] << kcv::general(0.123456, 4);
		REQUIRE(doc.dump() == u8"f: 0.1235\n");
	}
	SECTION("excessive precision may be clamped")
	{
		doc[u8"f"] << kcv::general(0.123456, 1000);
		std::string s{doc.dump()};
		REQUIRE(s.substr(0, 10) == u8"f: 0.12345");
	}
}


TEST_CASE("write float extreme values")
{
	kcv::Document doc{};

	if constexpr (std::numeric_limits<float>::has_infinity)
	{
		SECTION("do not write inf")
		{
			auto f{doc[u8"f"]};
			REQUIRE(f);
			f << std::numeric_limits<float>::infinity();
			REQUIRE_FALSE(f);
			REQUIRE(doc.dump() == u8"f:\n");
		}
	}
	if constexpr (std::numeric_limits<float>::has_quiet_NaN)
	{
		SECTION("do not write nan")
		{
			auto f{doc[u8"f"]};
			REQUIRE(f);
			f << std::numeric_limits<float>::quiet_NaN();
			REQUIRE_FALSE(f);
			REQUIRE(doc.dump() == u8"f:\n");
		}
	}
}
