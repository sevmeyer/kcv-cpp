TEST_CASE("read item from constructed Document")
{
	SUBCASE("default document")
	{
		kcv::Document doc{};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE(item);
	}
	SUBCASE("default string_view")
	{
		std::string_view data{};
		kcv::Document doc{data};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE(item);
	}
	SUBCASE("empty string")
	{
		std::string data{};
		kcv::Document doc{data};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE(item);
	}
}


TEST_CASE("read item from constructed DocumentView")
{
	SUBCASE("default document")
	{
		kcv::DocumentView<8> doc{};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE_FALSE(item);
	}
	SUBCASE("default string_view")
	{
		std::string_view data{};
		kcv::DocumentView<8> doc{data};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE_FALSE(item);
	}
	SUBCASE("empty string")
	{
		std::string data{};
		kcv::DocumentView<8> doc{data};
		REQUIRE(doc);
		auto item{doc[u8"a"]};
		REQUIRE_FALSE(item);
	}
}


TEST_CASE_TEMPLATE("read item key", D, kcv::Document, kcv::DocumentView<8>)
{
	int i{1};

	SUBCASE("key order is irrelevant")
	{
		std::string data{u8"c:33 a:11 b:22"};
		D doc{data};
		doc[u8"a"] >> i;
		REQUIRE(i == 11);
		doc[u8"b"] >> i;
		REQUIRE(i == 22);
		doc[u8"c"] >> i;
		REQUIRE(i == 33);
	}
	SUBCASE("valid key characters")
	{
		std::string data{u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._:42"};
		D doc{data};
		doc[u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._"] >> i;
		REQUIRE(i == 42);
	}
	SUBCASE("case sensitive key")
	{
		std::string data{u8"A:11 a:22"};
		D doc{data};
		doc[u8"A"] >> i;
		REQUIRE(i == 11);
		doc[u8"a"] >> i;
		REQUIRE(i == 22);
	}
	SUBCASE("keyword as key")
	{
		std::string data{u8"yes:42"};
		D doc{data};
		doc[u8"yes"] >> i;
		REQUIRE(i == 42);
	}
	SUBCASE("invalid numeric key")
	{
		std::string data{u8"42:24"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("invalid key character")
	{
		std::string data{u8"a/b:42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing key name")
	{
		std::string data{u8":42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing colon")
	{
		std::string data{u8"foo 42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("whitespace before colon")
	{
		std::string data{u8"foo :42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("equal sign instead of colon")
	{
		std::string data{u8"foo=42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("duplicate key")
	{
		std::string data{u8"a:11 a:22"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("value before first key")
	{
		std::string data{u8"42 a:11"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read item values", D, kcv::Document, kcv::DocumentView<8>)
{
	int i{1};

	SUBCASE("no value")
	{
		std::string data{u8"a:"};
		D doc{data};
		auto item{doc[u8"a"]};
		REQUIRE(item);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 1);
	}
	SUBCASE("one value")
	{
		std::string data{u8"a:11"};
		D doc{data};
		auto item{doc[u8"a"]};
		item >> i;
		REQUIRE(item);
		REQUIRE(i == 11);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 11);
	}
	SUBCASE("two values")
	{
		std::string data{u8"a:11 22"};
		D doc{data};
		auto item{doc[u8"a"]};
		item >> i;
		REQUIRE(item);
		REQUIRE(i == 11);
		item >> i;
		REQUIRE(item);
		REQUIRE(i == 22);
		item >> i;
		REQUIRE_FALSE(item);
		REQUIRE(i == 22);
	}
}


TEST_CASE_TEMPLATE("read item whitespace", D, kcv::Document, kcv::DocumentView<8>)
{
	int a{1};
	int b{1};

	SUBCASE("whitespace before item")
	{
		std::string data{u8"\n \r \t \r\na:42"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 42);
	}
	SUBCASE("whitespace after item")
	{
		std::string data{u8"a:42\r\n \t \r \n"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 42);
	}
	SUBCASE("whitespace between items")
	{
		std::string data{u8"a:11\r\n \t \r \nb:22"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("values are separated by space (U+0020)")
	{
		std::string data{u8"a:11 b:22"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("values are separated by tab (U+0009)")
	{
		std::string data{u8"a:11\tb:22"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("values are separated by line feed (U+000A)")
	{
		std::string data{u8"a:11\nb:22"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("values are separated by carriage return (U+000D)")
	{
		std::string data{u8"a:11\rb:22"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("whitespace after colon")
	{
		std::string data{u8"a:\r\n \t \r \n11"};
		D doc{data};
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
	}
	SUBCASE("whitespace between values")
	{
		std::string data{u8"a:11\n \r \t \r\n22\r\n \t \r \n"};
		D doc{data};
		doc[u8"a"] >> a >> b;
		REQUIRE(a == 11);
		REQUIRE(b == 22);
	}
	SUBCASE("missing whitespace after string value")
	{
		std::string data{u8"a:\"A\"b:11"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read item from limited Document")
{
	int a{1};
	int b{1};

	SUBCASE("maxItemCount=0 defaults to no limit")
	{
		std::string data{u8"a:11 b:22"};
		kcv::Document doc{data, 0};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("maxItemCount=1")
	{
		std::string data{u8"a:11"};
		kcv::Document doc{data, 1};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
	}
	SUBCASE("maxItemCount=1 overflow")
	{
		std::string data{u8"a:11 b:22"};
		kcv::Document doc{data, 1};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("maxItemCount=2")
	{
		std::string data{u8"a:11 b:22"};
		kcv::Document doc{data, 2};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("maxItemCount=2 overflow")
	{
		std::string data{u8"a:11 b:22 c:33"};
		kcv::Document doc{data, 2};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read item from limited DocumentView")
{
	int a{1};
	int b{1};

	SUBCASE("MaxItemCount=0")
	{
		std::string data{u8""};
		kcv::DocumentView<0> doc{data};
		REQUIRE(doc);
	}
	SUBCASE("MaxItemCount=0 overflow")
	{
		std::string data{u8"a:11"};
		kcv::DocumentView<0> doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("MaxItemCount=1")
	{
		std::string data{u8"a:11"};
		kcv::DocumentView<1> doc{data};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
	}
	SUBCASE("MaxItemCount=1 overflow")
	{
		std::string data{u8"a:11 b:22"};
		kcv::DocumentView<1> doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("MaxItemCount=2")
	{
		std::string data{u8"a:11 b:22"};
		kcv::DocumentView<2> doc{data};
		REQUIRE(doc);
		doc[u8"a"] >> a;
		REQUIRE(a == 11);
		doc[u8"b"] >> b;
		REQUIRE(b == 22);
	}
	SUBCASE("MaxItemCount=2 overflow")
	{
		std::string data{u8"a:11 b:22 c:33"};
		kcv::DocumentView<2> doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("key size is 255 bytes")
	{
		std::string key {
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA987654321"};

		std::string data{key + ": 11"};
		kcv::DocumentView<8> doc{data};
		REQUIRE(doc);
		doc[key] >> a;
		REQUIRE(a == 11);
	}
	SUBCASE("key size is 256 bytes")
	{
		std::string key {
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			u8"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"};

		std::string data{key + ": 11"};
		kcv::DocumentView<8> doc{data};
		REQUIRE_FALSE(doc);
		doc[key] >> a;
		REQUIRE(a == 1);
	}
}
