TEST_CASE("write bool")
{
	kcv::Document doc{};

	SUBCASE("yes")
	{
		doc[u8"b"] << true;
		REQUIRE(doc.dump() == u8"b: yes\n");
	}
	SUBCASE("no")
	{
		doc[u8"b"] << false;
		REQUIRE(doc.dump() == u8"b: no\n");
	}
}
