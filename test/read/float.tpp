TEST_CASE_TEMPLATE("read float syntax", D, kcv::Document, kcv::DocumentView<8>)
{
	float f{1.0f};

	SUBCASE("missing digit before decimal point")
	{
		std::string data{u8"f:.0"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing digit after minus")
	{
		std::string data{u8"f:-.5"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing digit after decimal point")
	{
		std::string data{u8"f:2."};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing digit in exponent")
	{
		std::string data{u8"f:2.0e"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("invalid plus sign in exponent")
	{
		std::string data{u8"f:2e+3"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("must not accept nan constant")
	{
		std::string data{u8"f:nan"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("must not accept inf constant")
	{
		std::string data{u8"f:inf"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("hexadecimal instead of decimal")
	{
		std::string data{u8"f:0x5"};
		D doc{data};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		item >> f;
		REQUIRE_FALSE(item);
		REQUIRE(f == 1.0f);
	}
	SUBCASE("bool instead of decimal")
	{
		std::string data{u8"f:yes"};
		D doc{data};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		item >> f;
		REQUIRE_FALSE(item);
		REQUIRE(f == 1.0f);
	}
	SUBCASE("string instead of decimal")
	{
		std::string data{u8"f:\"2.0\""};
		D doc{data};
		auto item{doc[u8"f"]};
		REQUIRE(item);
		item >> f;
		REQUIRE_FALSE(item);
		REQUIRE(f == 1.0f);
	}
}


TEST_CASE_TEMPLATE("read float fixed notation", D, kcv::Document, kcv::DocumentView<8>)
{
	float f{1.0f};

	SUBCASE("positive zero")
	{
		std::string data{u8"f:0.0"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 0.0f);
	}
	SUBCASE("negative zero")
	{
		std::string data{u8"f:-0.0"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == -0.0f);
		REQUIRE(std::signbit(f) == true);
	}
	SUBCASE("big negative")
	{
		std::string data{u8"f:-123456"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == -123456.0f);
	}
	SUBCASE("big positive")
	{
		std::string data{u8"f:123456"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 123456.0f);
	}
	SUBCASE("small negative")
	{
		std::string data{u8"f:-0.123456"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == doctest::Approx(-0.123456f).epsilon(0.000001f));
	}
	SUBCASE("small positive")
	{
		std::string data{u8"f:0.123456"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == doctest::Approx(0.123456f).epsilon(0.000001f));
	}
}


TEST_CASE_TEMPLATE("read float scientific notation", D, kcv::Document, kcv::DocumentView<8>)
{
	float f{1.0f};

	SUBCASE("zero exponent")
	{
		std::string data{u8"f:2e0"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 2.0f);
	}
	SUBCASE("lowercase exponent")
	{
		std::string data{u8"f:1e2"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 100.0f);
	}
	SUBCASE("uppercase exponent")
	{
		std::string data{u8"f:2E3"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 2000.0f);
	}
	SUBCASE("negative exponent")
	{
		std::string data{u8"f:3e-4"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == doctest::Approx(0.0003f));
	}
	SUBCASE("big negative")
	{
		std::string data{u8"f:-0.123456e6"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == -123456.0f);
	}
	SUBCASE("big positive")
	{
		std::string data{u8"f:0.123456e6"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == 123456.0f);
	}
	SUBCASE("small negative")
	{
		std::string data{u8"f:-123456e-6"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == doctest::Approx(-0.123456f).epsilon(0.000001f));
	}
	SUBCASE("small positive")
	{
		std::string data{u8"f:123456e-6"};
		D doc{data};
		doc[u8"f"] >> f;
		REQUIRE(f == doctest::Approx(0.123456f).epsilon(0.000001f));
	}
}
