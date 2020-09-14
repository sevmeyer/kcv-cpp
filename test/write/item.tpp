TEST_CASE("write item key")
{
	kcv::Document doc{};

	SUBCASE("valid key characters")
	{
		doc[u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._"] << 42;
		REQUIRE(doc.dump() == u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._: 42\n");
	}
	SUBCASE("invalid leading digit")
	{
		auto foo{doc[u8"1foo"]};
		REQUIRE_FALSE(foo);
	}
	SUBCASE("invalid leading minus")
	{
		auto foo{doc[u8"-foo"]};
		REQUIRE_FALSE(foo);
	}
	SUBCASE("invalid leading underscore")
	{
		auto foo{doc[u8"_foo"]};
		REQUIRE_FALSE(foo);
	}
	SUBCASE("invalid key character")
	{
		auto foo{doc[u8"a/b"]};
		REQUIRE_FALSE(foo);
	}
	SUBCASE("empty key")
	{
		auto foo{doc[u8""]};
		REQUIRE_FALSE(foo);
	}
	SUBCASE("string object as key")
	{
		std::string s{u8"foo"};
		auto foo{doc[s]};
		REQUIRE(foo);
		REQUIRE(doc.dump() == u8"foo:\n");
	}
	SUBCASE("writing to invalid item has no effect")
	{
		auto foo{doc[u8"???"]};
		REQUIRE_FALSE(foo);
		foo << 42 << kcv::nl() << 1.0 << u8"Hello";
		REQUIRE(doc.dump() == u8"");
	}
}


TEST_CASE("write item values")
{
	kcv::Document doc{};

	SUBCASE("no value")
	{
		doc[u8"a"];
		REQUIRE(doc.dump() == u8"a:\n");
	}
	SUBCASE("one value")
	{
		doc[u8"a"] << 1;
		REQUIRE(doc.dump() == u8"a: 1\n");
	}
	SUBCASE("two values")
	{
		doc[u8"a"] << 1 << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("can continue after invalid value")
	{
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		a << std::numeric_limits<float>::quiet_NaN();
		REQUIRE_FALSE(a);
		a << 2;
		REQUIRE(a);
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("mixed value types")
	{
		doc[u8"a"] << 42 << 3.14159 << true << u8"Hello";
		REQUIRE(doc.dump() == u8"a: 42 3.14159 yes \"Hello\"\n");
	}
	SUBCASE("write more values than original")
	{
		kcv::Document abc{"a:1 b:2 c:3"};
		abc[u8"a"] << 1 << 11 << 111 << 1111 << 11111 << 111111 << 1111111 << 11111111;
		abc[u8"b"] << 2 << 22 << 222 << 2222 << 22222 << 222222 << 2222222 << 22222222;
		abc[u8"c"] << 3 << 33 << 333 << 3333 << 33333 << 333333 << 3333333 << 33333333;
		REQUIRE(abc.dump() ==
			u8"a: 1 11 111 1111 11111 111111 1111111 11111111\n"
			u8"b: 2 22 222 2222 22222 222222 2222222 22222222\n"
			u8"c: 3 33 333 3333 33333 333333 3333333 33333333\n");
	}
}


TEST_CASE("write item sequence")
{
	kcv::Document doc{};

	SUBCASE("no item")
	{
		REQUIRE(doc.dump() == "");
	}
	SUBCASE("one item")
	{
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		REQUIRE(doc.dump() == u8"a: 1\n");
	}
	SUBCASE("two items ordered lexicographically")
	{
		auto b{doc[u8"b"]};
		b << 2;
		auto a{doc[u8"a"]};
		a << 1;
		REQUIRE(a);
		REQUIRE(b);
		REQUIRE(doc.dump() == u8"a: 1\nb: 2\n");
	}
	SUBCASE("first write to new item instance clears old content")
	{
		auto a1{doc[u8"a"]};
		a1 << 1;
		REQUIRE(a1);
		REQUIRE(doc.dump() == u8"a: 1\n");
		auto a2{doc[u8"a"]};
		a2 << 2;
		REQUIRE(a2);
		REQUIRE(doc.dump() == u8"a: 2\n");
		a2 << 3;
		REQUIRE(a2);
		REQUIRE(doc.dump() == u8"a: 2 3\n");
	}
	SUBCASE("changing stream direction clears old content")
	{
		int i{1};
		auto a{doc[u8"a"]};
		a << 11;
		a >> i;
		REQUIRE(i == 11);
		a << 22;
		a >> i;
		REQUIRE(i == 22);
		REQUIRE(doc.dump() == u8"a: 22\n");
	}
	SUBCASE("mixed write and read of items")
	{
		int i{1};
		doc[u8"b"] << 22;
		doc[u8"a"] << 11;
		doc[u8"c"] << 33;

		doc[u8"a"] >> i;
		REQUIRE(i == 11);
		doc[u8"c"] >> i;
		REQUIRE(i == 33);
		doc[u8"b"] >> i;
		REQUIRE(i == 22);

		doc[u8"c"] << 3333;
		doc[u8"b"] << 2222;
		doc[u8"a"] << 1111;

		doc[u8"b"] >> i;
		REQUIRE(i == 2222);
		doc[u8"a"] >> i;
		REQUIRE(i == 1111);
		doc[u8"c"] >> i;
		REQUIRE(i == 3333);

		REQUIRE(doc.dump() == u8"a: 1111\nb: 2222\nc: 3333\n");
	}
}


TEST_CASE("write item after failed document construction")
{
	SUBCASE("invalid string is discarded")
	{
		std::string data{u8"a:?"};
		kcv::Document doc{data};
		REQUIRE_FALSE(doc);
		REQUIRE(doc.dump() == u8"");
	}
	SUBCASE("create new item in empty document")
	{
		std::string data{u8"?"};
		kcv::Document doc{data};
		REQUIRE_FALSE(doc);
		auto a{doc[u8"a"]};
		REQUIRE(a);
		a << 42;
		REQUIRE(a);
		REQUIRE(doc.dump() == u8"a: 42\n");
	}
	SUBCASE("create new empty item instead of reading invalid data")
	{
		std::string data{u8"a:42 b:?"};
		kcv::Document doc{data};
		REQUIRE_FALSE(doc);
		int i{1};
		auto a{doc[u8"a"]};
		REQUIRE(a);
		a >> i;
		REQUIRE_FALSE(a);
		REQUIRE(i == 1);
		REQUIRE(doc.dump() == u8"a:\n");
	}
}


TEST_CASE("write item whitespace formatting")
{
	kcv::Document doc{};

	SUBCASE("default newline count is 1")
	{
		doc[u8"a"] << 1 << kcv::nl() << 2;
		REQUIRE(doc.dump() == u8"a: 1\n2\n");
	}
	SUBCASE("invalid newline count has no effect")
	{
		doc[u8"a"] << 1 << kcv::nl(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("newline count 2")
	{
		doc[u8"a"] << 1 << kcv::nl(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1\n\n2\n");
	}
	SUBCASE("default space count is 1")
	{
		doc[u8"a"] << 1 << kcv::sp() << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("invalid space count has no effect")
	{
		doc[u8"a"] << 1 << kcv::sp(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("space count 2")
	{
		doc[u8"a"] << 1 << kcv::sp(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1  2\n");
	}
	SUBCASE("default tab count is 1")
	{
		doc[u8"a"] << 1 << kcv::tab() << 2;
		REQUIRE(doc.dump() == u8"a: 1\t2\n");
	}
	SUBCASE("invalid tab count has no effect")
	{
		doc[u8"a"] << 1 << kcv::tab(-1) << 2;
		REQUIRE(doc.dump() == u8"a: 1 2\n");
	}
	SUBCASE("tab count 2")
	{
		doc[u8"a"] << 1 << kcv::tab(2) << 2;
		REQUIRE(doc.dump() == u8"a: 1\t\t2\n");
	}
}


TEST_CASE("write item whitespace consistency")
{
	SUBCASE("do not propagate utf8 byte order mark")
	{
		std::string data{u8"\xEF\xBB\xBFi: 42\n"};
		kcv::Document doc{data};
		REQUIRE(doc.dump() == u8"i: 42\n");
	}
	SUBCASE("preserve existing whitespace before first value")
	{
		std::string data{u8"a:\n \t42"};
		kcv::Document doc{data};
		REQUIRE(doc.dump() == u8"a:\n \t42\n");
	}
	SUBCASE("preserve custom whitespace before first value")
	{
		kcv::Document doc{};
		doc[u8"a"] << kcv::nl() << kcv::sp() << kcv::tab() << 42;
		REQUIRE(doc.dump() == u8"a:\n \t42\n");
	}
	SUBCASE("preserve existing whitespace after last value")
	{
		std::string data{u8"a: 42\n \t"};
		kcv::Document doc{data};
		REQUIRE(doc.dump() == u8"a: 42\n \t\n");
	}
	SUBCASE("preserve custom whitespace after last value")
	{
		kcv::Document doc{};
		doc[u8"a"] << 42 << kcv::nl() << kcv::sp() << kcv::tab();
		REQUIRE(doc.dump() == u8"a: 42\n \t\n");
	}
	SUBCASE("do not write additional newline after custom newline")
	{
		kcv::Document doc{};
		doc[u8"a"] << 42 << kcv::nl();
		REQUIRE(doc.dump() == u8"a: 42\n");
	}
}
