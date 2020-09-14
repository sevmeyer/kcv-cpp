TEST_CASE_TEMPLATE("read bool", D, kcv::Document, kcv::DocumentView<8>)
{
	bool b{false};

	SUBCASE("yes")
	{
		std::string data{u8"b:yes"};
		D doc{data};
		doc[u8"b"] >> b;
		REQUIRE(b == true);
	}
	SUBCASE("no")
	{
		b = true;
		std::string data{u8"b:no"};
		D doc{data};
		doc[u8"b"] >> b;
		REQUIRE(b == false);
	}
	SUBCASE("number instead of bool")
	{
		std::string data{u8"b:1"};
		D doc{data};
		auto item{doc[u8"b"]};
		REQUIRE(item);
		item >> b;
		REQUIRE_FALSE(item);
		REQUIRE(b == false);
	}
	SUBCASE("string instead of bool")
	{
		std::string data{u8"b:\"yes\""};
		D doc{data};
		auto item{doc[u8"b"]};
		REQUIRE(item);
		item >> b;
		REQUIRE_FALSE(item);
		REQUIRE(b == false);
	}
	SUBCASE("true instead of yes")
	{
		std::string data{u8"b:true"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("uppercase instead of lowercase")
	{
		std::string data{u8"b:YES"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}
