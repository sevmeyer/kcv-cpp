#include <catch2/catch.hpp>

#include "kcv/kcv.hpp"


TEST_CASE("read bool")
{
	bool b{false};

	SECTION("yes")
	{
		kcv::Document doc{u8"b:yes"};
		doc[u8"b"] >> b;
		REQUIRE(b == true);
	}
	SECTION("no")
	{
		b = true;
		kcv::Document doc{u8"b:no"};
		doc[u8"b"] >> b;
		REQUIRE(b == false);
	}
	SECTION("number instead of bool")
	{
		kcv::Document doc{u8"b:1"};
		auto item{doc[u8"b"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> b);
		REQUIRE(b == false);
	}
	SECTION("string instead of bool")
	{
		kcv::Document doc{u8"b:\"yes\""};
		auto item{doc[u8"b"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> b);
		REQUIRE(b == false);
	}
	SECTION("true instead of yes")
	{
		kcv::Document doc{u8"b:true"};
		REQUIRE_FALSE(doc);
	}
	SECTION("uppercase instead of lowercase")
	{
		kcv::Document doc{u8"b:YES"};
		REQUIRE_FALSE(doc);
	}
}
