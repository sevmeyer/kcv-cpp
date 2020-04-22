#include <catch2/catch.hpp>

#include <cmath>

#include "kcv/kcv.hpp"


TEST_CASE("read float syntax")
{
	float f{1.0f};

	SECTION("missing digit before decimal point")
	{
		kcv::Document doc{u8"f:.0"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing digit after minus")
	{
		kcv::Document doc{u8"f:-.5"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing digit after decimal point")
	{
		kcv::Document doc{u8"f:2."};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing digit in exponent")
	{
		kcv::Document doc{u8"f:2.0e"};
		REQUIRE_FALSE(doc);
	}
	SECTION("invalid plus sign in exponent")
	{
		kcv::Document doc{u8"f:2e+3"};
		REQUIRE_FALSE(doc);
	}
	SECTION("must not accept nan constant")
	{
		kcv::Document doc{u8"f:nan"};
		REQUIRE_FALSE(doc);
	}
	SECTION("must not accept inf constant")
	{
		kcv::Document doc{u8"f:inf"};
		REQUIRE_FALSE(doc);
	}
	SECTION("hexadecimal instead of decimal")
	{
		kcv::Document doc{u8"f:0x5"};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> f);
		REQUIRE(f == 1.0f);
	}
	SECTION("bool instead of decimal")
	{
		kcv::Document doc{u8"f:yes"};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> f);
		REQUIRE(f == 1.0f);
	}
	SECTION("string instead of decimal")
	{
		kcv::Document doc{u8"f:\"2.0\""};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> f);
		REQUIRE(f == 1.0f);
	}
}


TEST_CASE("read float fixed notation")
{
	float f{1.0f};

	SECTION("positive zero")
	{
		kcv::Document doc{u8"f:0.0"};
		doc[u8"f"] >> f;
		REQUIRE(f == 0.0f);
	}
	SECTION("negative zero")
	{
		kcv::Document doc{u8"f:-0.0"};
		doc[u8"f"] >> f;
		REQUIRE(f == -0.0f);
		REQUIRE(std::signbit(f) == true);
	}
	SECTION("big negative")
	{
		kcv::Document doc{u8"f:-123456"};
		doc[u8"f"] >> f;
		REQUIRE(f == -123456.0f);
	}
	SECTION("big positive")
	{
		kcv::Document doc{u8"f:123456"};
		doc[u8"f"] >> f;
		REQUIRE(f == 123456.0f);
	}
	SECTION("small negative")
	{
		kcv::Document doc{u8"f:-0.123456"};
		doc[u8"f"] >> f;
		REQUIRE(f == Approx(-0.123456f).epsilon(0.000001f));
	}
	SECTION("small positive")
	{
		kcv::Document doc{u8"f:0.123456"};
		doc[u8"f"] >> f;
		REQUIRE(f == Approx(0.123456f).epsilon(0.000001f));
	}
}


TEST_CASE("read float scientific notation")
{
	float f{1.0f};

	SECTION("zero exponent")
	{
		kcv::Document doc{u8"f:2e0"};
		doc[u8"f"] >> f;
		REQUIRE(f == 2.0f);
	}
	SECTION("lowercase exponent")
	{
		kcv::Document doc{u8"f:1e2"};
		doc[u8"f"] >> f;
		REQUIRE(f == 100.0f);
	}
	SECTION("uppercase exponent")
	{
		kcv::Document doc{u8"f:2E3"};
		doc[u8"f"] >> f;
		REQUIRE(f == 2000.0f);
	}
	SECTION("negative exponent")
	{
		kcv::Document doc{u8"f:3e-4"};
		doc[u8"f"] >> f;
		REQUIRE(f == Approx(0.0003f));
	}
	SECTION("big negative")
	{
		kcv::Document doc{u8"f:-0.123456e6"};
		doc[u8"f"] >> f;
		REQUIRE(f == -123456.0f);
	}
	SECTION("big positive")
	{
		kcv::Document doc{u8"f:0.123456e6"};
		doc[u8"f"] >> f;
		REQUIRE(f == 123456.0f);
	}
	SECTION("small negative")
	{
		kcv::Document doc{u8"f:-123456e-6"};
		doc[u8"f"] >> f;
		REQUIRE(f == Approx(-0.123456f).epsilon(0.000001f));
	}
	SECTION("small positive")
	{
		kcv::Document doc{u8"f:123456e-6"};
		doc[u8"f"] >> f;
		REQUIRE(f == Approx(0.123456f).epsilon(0.000001f));
	}
}
