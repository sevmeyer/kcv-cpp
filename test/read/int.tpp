TEST_CASE_TEMPLATE("read int syntax", D, kcv::Document, kcv::DocumentView<8>)
{
	int i{1};

	SUBCASE("negative zero is parsed as zero")
	{
		std::string data{u8"i:-0"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == 0);
	}
	SUBCASE("leading zero must not be parsed as octal")
	{
		std::string data{u8"i:010"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == 10);
	}
	SUBCASE("missing digits after minus")
	{
		std::string data{u8"i:-"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("must not include fraction")
	{
		std::string data{u8"i:2.0"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("must not include exponent")
	{
		std::string data{u8"i:1e2"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("bool instead of integer")
	{
		std::string data{u8"i:yes"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("string instead of integer")
	{
		std::string data{u8"i:\"1\""};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read int to char", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("char is treated as integral type")
	{
		char c{0};
		std::string data{u8"c:42"};
		D doc{data};
		doc[u8"c"] >> c;
		REQUIRE(c == 42);
	}
	SUBCASE("signed char is treated as integral type")
	{
		signed char c{0};
		std::string data{u8"c:-128"};
		D doc{data};
		doc[u8"c"] >> c;
		REQUIRE(c == -128);
	}
	SUBCASE("unsigned char is treated as integral type")
	{
		unsigned char c{0};
		std::string data{u8"c:255"};
		D doc{data};
		doc[u8"c"] >> c;
		REQUIRE(c == 255);
	}
}


TEST_CASE_TEMPLATE("read int to int32", D, kcv::Document, kcv::DocumentView<8>)
{
	std::int32_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:-2147483648"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:2147483647"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("underflow")
	{
		std::string data{u8"i:-2147483649"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:2147483648"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read int to uint32", D, kcv::Document, kcv::DocumentView<8>)
{
	std::uint32_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:0"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:4294967295"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("underflow")
	{
		std::string data{u8"i:-1"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:4294967296"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read int to int64", D, kcv::Document, kcv::DocumentView<8>)
{
	std::int64_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:-9223372036854775808"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:9223372036854775807"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("underflow")
	{
		std::string data{u8"i:-9223372036854775809"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:9223372036854775808"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read int to uint64", D, kcv::Document, kcv::DocumentView<8>)
{
	std::uint64_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:0"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:18446744073709551615"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("underflow")
	{
		std::string data{u8"i:-1"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:18446744073709551616"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}
