TEST_CASE_TEMPLATE("read string syntax", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string s{"original"};

	SUBCASE("missing quotes")
	{
		std::string data{u8"s:hello"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing opening quote")
	{
		std::string data{u8"s:hello\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("missing closing quote")
	{
		std::string data{u8"s:\"hello"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("single quotes instead of double quotes")
	{
		std::string data{u8"s:'hello'"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("unescaped quote")
	{
		std::string data{u8"s:\"\"\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("unescaped backslash")
	{
		std::string data{u8"s:\"\\\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("bool instead of string")
	{
		std::string data{u8"s:yes"};
		D doc{data};
		auto item{doc[u8"s"]};
		REQUIRE(item);
		item >> s;
		REQUIRE_FALSE(item);
		REQUIRE(s == "original");
	}
	SUBCASE("number instead of string")
	{
		std::string data{u8"s:1"};
		D doc{data};
		auto item{doc[u8"s"]};
		REQUIRE(item);
		item >> s;
		REQUIRE_FALSE(item);
		REQUIRE(s == "original");
	}
}


TEST_CASE_TEMPLATE("read string content", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string s{"original"};

	SUBCASE("empty")
	{
		std::string data{u8"s:\"\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"");
	}
	SUBCASE("whitespace")
	{
		std::string data{u8"s:\" \t \n \r \r\n \""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8" \t \n \r \r\n ");
	}
	SUBCASE("integer")
	{
		std::string data{u8"s:\"42\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"42");
	}
	SUBCASE("hexadecimal")
	{
		std::string data{u8"s:\"0x42\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"0x42");
	}
	SUBCASE("decimal")
	{
		std::string data{u8"s:\"314.159e-2\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"314.159e-2");
	}
	SUBCASE("bool")
	{
		std::string data{u8"s:\"yes\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"yes");
	}
	SUBCASE("item")
	{
		std::string data{u8"s:\"foo:42\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"foo:42");
	}
	SUBCASE("ascii")
	{
		std::string data{u8"s:\" !\\\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
	}
	SUBCASE("international")
	{
		std::string data{u8"s:\"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی");
	}
	SUBCASE("combining characters")
	{
		std::string data{u8"s:\"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302");
	}
	SUBCASE("supplementary multilingual plane")
	{
		std::string data{u8"s:\"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD");
	}
	SUBCASE("leading BOM")
	{
		std::string data{u8"s:\"\xEF\xBB\xBF...\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\xEF\xBB\xBF...");
	}
}


TEST_CASE_TEMPLATE("read string encoding", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("invalid utf16")
	{
		std::string data{'\x73','\x3A','\x22','\x00','\x66','\x00','\xF6','\x00','\xF6','\x22'};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("invalid iso8859-1")
	{
		std::string data{"\x73\x3A\x22\x66\xF6\xF6\x22"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read string with character escape", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string s{"original"};

	SUBCASE("double quote")
	{
		std::string data{u8R"(s:"\"")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\"");
	}
	SUBCASE("backslash")
	{
		std::string data{u8R"(s:"\\")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\\");
	}
	SUBCASE("tab")
	{
		std::string data{u8R"(s:"\t")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\t");
	}
	SUBCASE("line feed")
	{
		std::string data{u8R"(s:"\n")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\n");
	}
	SUBCASE("carriage return")
	{
		std::string data{u8R"(s:"\r")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\r");
	}
	SUBCASE("windows newline")
	{
		std::string data{u8R"(s:"\r\n")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\r\n");
	}
	SUBCASE("cannot escape 0")
	{
		std::string data{u8R"(s:"\0")"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("cannot escape f")
	{
		std::string data{u8R"(s:"\f")"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("cannot escape x")
	{
		std::string data{u8R"(s:"\x42")"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read string with unicode escape", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string s{"original"};

	SUBCASE("case insensitive")
	{
		std::string data{u8R"(s:"\u1E9e")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u1E9E");
	}
	SUBCASE("minimum code point")
	{
		std::string data{u8R"(s:"\u0000")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == std::string{'\0'});
	}
	SUBCASE("maximum code point for 2 byte expansion")
	{
		std::string data{u8R"(s:"\u07FF")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u07FF");
	}
	SUBCASE("maximum code point for 3 byte expansion")
	{
		std::string data{u8R"(s:"\uFFFF")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uFFFF");
	}
	SUBCASE("maximum code point for 4 byte expansion")
	{
		std::string data{u8R"(s:"\U0010FFFF")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0010FFFF");
	}
	SUBCASE("code point overflow")
	{
		std::string data{u8R"(s:"\U00110000")"};
		D doc{data};
		auto item{doc[u8"s"]};
		item >> s;
		REQUIRE_FALSE(item);
		REQUIRE(s == "original");
	}
	SUBCASE("one before surrogate range")
	{
		std::string data{u8R"(s:"\uD7FF")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uD7FF");
	}
	SUBCASE("one after surrogate range")
	{
		std::string data{u8R"(s:"\uE000")"};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uE000");
	}
	SUBCASE("first surrogate")
	{
		std::string data{u8R"(s:"\uD800")"};
		D doc{data};
		auto item{doc[u8"s"]};
		item >> s;
		REQUIRE_FALSE(item);
		REQUIRE(s == "original");
	}
	SUBCASE("last surrogate")
	{
		std::string data{u8R"(s:"\uDFFF")"};
		D doc{data};
		auto item{doc[u8"s"]};
		item >> s;
		REQUIRE_FALSE(item);
		REQUIRE(s == "original");
	}
	SUBCASE("not enough digits for uXXXX")
	{
		std::string data{u8R"(s:"\u123x")"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("not enough digits for UXXXXXXXX")
	{
		std::string data{u8R"(s:"\U0001234x")"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read string_view", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string_view v{};

	SUBCASE("literal string")
	{
		std::string data{u8"v:\"Hello!\""};
		D doc{data};
		doc[u8"v"] >> v;
		REQUIRE(v == u8"Hello!");
	}
	SUBCASE("cannot read string with escape sequence")
	{
		std::string data{u8R"(v:"Hello\u0021")"};
		D doc{data};
		auto item{doc[u8"v"]};
		item >> v;
		REQUIRE_FALSE(item);
		REQUIRE(v == std::string_view{});
	}
}
