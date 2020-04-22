#include <catch2/catch.hpp>

#include "kcv/kcv.hpp"


TEST_CASE("write bool")
{
	kcv::Document doc{};

	SECTION("yes")
	{
		doc[u8"b"] << true;
		REQUIRE(doc.dump() == u8"b: yes\n");
	}
	SECTION("no")
	{
		doc[u8"b"] << false;
		REQUIRE(doc.dump() == u8"b: no\n");
	}
}
