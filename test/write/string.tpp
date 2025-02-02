TEST_CASE("write string content")
{
	kcv::Document doc{};

	SUBCASE("empty")
	{
		doc[u8"s"] << u8"";
		REQUIRE(doc.dump() == u8"s: \"\"\n");
	}
	SUBCASE("whitespace")
	{
		doc[u8"s"] << u8" \t \n \r \r\n ";
		REQUIRE(doc.dump() == u8"s: \" \t \n \r \r\n \"\n");
	}
	SUBCASE("unescaped ascii")
	{
		doc[u8"s"] << u8"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";
		REQUIRE(doc.dump() == u8"s: \"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~\"\n");
	}
	SUBCASE("international")
	{
		doc[u8"s"] << u8"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی";
		REQUIRE(doc.dump() == u8"s: \"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی\"\n");
	}
	SUBCASE("combining characters")
	{
		doc[u8"s"] << u8"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302";
		REQUIRE(doc.dump() == u8"s: \"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302\"\n");
	}
	SUBCASE("supplementary multilingual plane")
	{
		doc[u8"s"] << u8"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD";
		REQUIRE(doc.dump() == u8"s: \"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD\"\n");
	}
	SUBCASE("write empty std::string")
	{
		std::string s{};
		doc[u8"s"] << s;
		REQUIRE(doc.dump() == u8"s: \"\"\n");
	}
	SUBCASE("write non-empty std::string")
	{
		std::string s{u8"This is a string"};
		doc[u8"s"] << s;
		REQUIRE(doc.dump() == u8"s: \"This is a string\"\n");
	}
}


TEST_CASE("write string escape special characters")
{
	kcv::Document doc{};

	SUBCASE("escape double quote")
	{
		doc[u8"s"] << u8"\"";
		REQUIRE(doc.dump() == u8"s: \"\\\"\"\n");
	}
	SUBCASE("escape backslash")
	{
		doc[u8"s"] << u8"\\";
		REQUIRE(doc.dump() == u8"s: \"\\\\\"\n");
	}
}


TEST_CASE("write string_view")
{
	kcv::Document doc{};
	std::string s{u8"abc"};

	SUBCASE("default string_view")
	{
		doc[u8"v"] << std::string_view{};
		REQUIRE(doc.dump() == u8"v: \"\"\n");
	}
	SUBCASE("empty string_view")
	{
		doc[u8"v"] << std::string_view{s.data(), 0};
		REQUIRE(doc.dump() == u8"v: \"\"\n");
	}
	SUBCASE("non-empty string_view")
	{
		doc[u8"v"] << std::string_view{s.data(), s.size()};
		REQUIRE(doc.dump() == u8"v: \"abc\"\n");
	}
}


TEST_CASE("write string invalid utf8 encoding")
{
	kcv::Document doc{};
	auto s{doc[u8"s"]};

	SUBCASE("incomplete code point")
	{
		REQUIRE(s);
		s << u8"\xEF\xBF";
		REQUIRE_FALSE(s);
		REQUIRE(doc.dump() == u8"s:\n");
	}
	SUBCASE("impossible byte")
	{
		REQUIRE(s);
		s << u8"\xF5";
		REQUIRE_FALSE(s);
		REQUIRE(doc.dump() == u8"s:\n");
	}
	SUBCASE("overlong sequence")
	{
		REQUIRE(s);
		s << u8"\xE0\x80\x80";
		REQUIRE_FALSE(s);
		REQUIRE(doc.dump() == u8"s:\n");
	}
	SUBCASE("invalid surrogate")
	{
		REQUIRE(s);
		s << u8"\xED\xAF\xBF";
		REQUIRE_FALSE(s);
		REQUIRE(doc.dump() == u8"s:\n");
	}
}
