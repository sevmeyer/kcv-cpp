TEST_CASE("write float default format")
{
	kcv::Document doc{};

	SUBCASE("positive zero")
	{
		doc[u8"f"] << 0.0;
		REQUIRE(doc.dump() == u8"f: 0\n");
	}
	SUBCASE("negative zero")
	{
		doc[u8"f"] << -0.0;
		REQUIRE(doc.dump() == u8"f: -0\n");
	}
	SUBCASE("big negative")
	{
		doc[u8"f"] << -123456.0;
		REQUIRE(doc.dump() == u8"f: -123456\n");
	}
	SUBCASE("big positive")
	{
		doc[u8"f"] << 123456.0;
		REQUIRE(doc.dump() == u8"f: 123456\n");
	}
	SUBCASE("small negative")
	{
		doc[u8"f"] << -0.123456;
		REQUIRE(doc.dump() == u8"f: -0.123456\n");
	}
	SUBCASE("small positive")
	{
		doc[u8"f"] << 0.123456;
		REQUIRE(doc.dump() == u8"f: 0.123456\n");
	}
}


TEST_CASE("write float fixed format")
{
	kcv::Document doc{};

	SUBCASE("default precision is 6")
	{
		doc[u8"f"] << kcv::fixed(1.0);
		REQUIRE(doc.dump() == u8"f: 1.000000\n");
	}
	SUBCASE("negative precision is same as 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, -1);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SUBCASE("zero precision is same as 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, 0);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SUBCASE("precision 1")
	{
		doc[u8"f"] << kcv::fixed(1.0, 1);
		REQUIRE(doc.dump() == u8"f: 1.0\n");
	}
	SUBCASE("precision 2")
	{
		doc[u8"f"] << kcv::fixed(1.0, 2);
		REQUIRE(doc.dump() == u8"f: 1.00\n");
	}
	SUBCASE("rounded negative")
	{
		doc[u8"f"] << kcv::fixed(-0.123456, 4);
		REQUIRE(doc.dump() == u8"f: -0.1235\n");
	}
	SUBCASE("rounded positive")
	{
		doc[u8"f"] << kcv::fixed(0.123456, 4);
		REQUIRE(doc.dump() == u8"f: 0.1235\n");
	}
	SUBCASE("excessive precision may be clamped")
	{
		doc[u8"f"] << kcv::fixed(0.123456, 1000);
		std::string s{doc.dump()};
		REQUIRE(s.substr(0, 10) == u8"f: 0.12345");
	}
}


TEST_CASE("write float rounded format")
{
	kcv::Document doc{};

	SUBCASE("default precision is 6")
	{
		doc[u8"f"] << kcv::general(0.12345678);
		REQUIRE(doc.dump() == u8"f: 0.123457\n");
	}
	SUBCASE("negative precision is same as 1")
	{
		doc[u8"f"] << kcv::general(1.0, -1);
		REQUIRE(doc.dump() == u8"f: 1\n");
	}
	SUBCASE("zero precision is same as 1")
	{
		doc[u8"f"] << kcv::general(1.0, 0);
		REQUIRE(doc.dump() == u8"f: 1\n");
	}
	SUBCASE("extended negative")
	{
		doc[u8"f"] << kcv::general(-16777216.0f, 8);
		REQUIRE(doc.dump() == u8"f: -16777216\n");
	}
	SUBCASE("extended positive")
	{
		doc[u8"f"] << kcv::general(16777216.0f, 8);
		REQUIRE(doc.dump() == u8"f: 16777216\n");
	}
	SUBCASE("rounded negative")
	{
		doc[u8"f"] << kcv::general(-0.123456, 4);
		REQUIRE(doc.dump() == u8"f: -0.1235\n");
	}
	SUBCASE("rounded positive")
	{
		doc[u8"f"] << kcv::general(0.123456, 4);
		REQUIRE(doc.dump() == u8"f: 0.1235\n");
	}
	SUBCASE("excessive precision may be clamped")
	{
		doc[u8"f"] << kcv::general(0.123456, 1000);
		std::string s{doc.dump()};
		REQUIRE(s.substr(0, 10) == u8"f: 0.12345");
	}
}


TEST_CASE("write float extreme values")
{
	kcv::Document doc{};

	SUBCASE("do not write inf")
	{
		auto f{doc[u8"f"]};
		REQUIRE(f);
		f << std::numeric_limits<float>::infinity();
		REQUIRE_FALSE(f);
		REQUIRE(doc.dump() == u8"f:\n");
	}
	SUBCASE("do not write nan")
	{
		auto f{doc[u8"f"]};
		REQUIRE(f);
		f << std::numeric_limits<float>::quiet_NaN();
		REQUIRE_FALSE(f);
		REQUIRE(doc.dump() == u8"f:\n");
	}
}
