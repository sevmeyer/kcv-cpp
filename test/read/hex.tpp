TEST_CASE_TEMPLATE("read hex syntax", D, kcv::Document, kcv::DocumentView<8>)
{
	int i{1};

	SUBCASE("case insensitive")
	{
		std::string data{u8"i:0xAb"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == 171);
	}
	SUBCASE("ignore leading zero")
	{
		std::string data{u8"i:0x000000000000000000000000000000002"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == 2);
	}
	SUBCASE("missing 0x prefix")
	{
		std::string data{u8"i:ffdd55"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing digits")
	{
		std::string data{u8"i:0x"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("cannot be negative")
	{
		std::string data{u8"i:-0x42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("invalid digit character")
	{
		std::string data{u8"i:0xG"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read hex to int32", D, kcv::Document, kcv::DocumentView<8>)
{
	std::int32_t i{1};

	SUBCASE("maximum")
	{
		std::string data{u8"i:0x7fffffff"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:0x80000000"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read hex to uint32", D, kcv::Document, kcv::DocumentView<8>)
{
	std::uint32_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:0x0"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:0xffffffff"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:0x100000000"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read hex to int64", D, kcv::Document, kcv::DocumentView<8>)
{
	std::int64_t i{1};

	SUBCASE("maximum")
	{
		std::string data{u8"i:0x7fffffffffffffff"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:0x8000000000000000"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}


TEST_CASE_TEMPLATE("read hex to uint64", D, kcv::Document, kcv::DocumentView<8>)
{
	std::uint64_t i{1};

	SUBCASE("minimum")
	{
		std::string data{u8"i:0x0"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::min());
	}
	SUBCASE("maximum")
	{
		std::string data{u8"i:0xffffffffffffffff"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == std::numeric_limits<decltype(i)>::max());
	}
	SUBCASE("overflow")
	{
		std::string data{u8"i:0x10000000000000000"};
		D doc{data};
		auto item{doc[u8"i"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
}
