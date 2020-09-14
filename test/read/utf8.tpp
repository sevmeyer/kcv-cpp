// The following tests are partially based on:
// UTF-8 decoder capability and stress test
// by Markus Kuhn - 2015-08-28 - CC BY 4.0
// https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt


TEST_CASE_TEMPLATE("read utf8 byte order mark", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("ignore valid mark")
	{
		int i{1};
		std::string data{u8"\xEF\xBB\xBFi:42"};
		D doc{data};
		doc[u8"i"] >> i;
		REQUIRE(i == 42);
	}
	SUBCASE("invalid second byte")
	{
		std::string data{u8"\xEF\x42\xBFi:42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("invalid third byte")
	{
		std::string data{u8"\xEF\xBB\x42i:42"};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read utf8 boundary", D, kcv::Document, kcv::DocumentView<8>)
{
	std::string s{};

	SUBCASE("first possible 1-byte sequence")
	{
		std::string data{'s',':','"','\0','"'};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == std::string{'\0'});
	}
	SUBCASE("last possible 1-byte sequence")
	{
		std::string data{u8"s:\"\x7F\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u007F");
	}
	SUBCASE("first possible 2-byte sequence")
	{
		std::string data{u8"s:\"\xC2\x80\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u0080");
	}
	SUBCASE("last possible 2-byte sequence")
	{
		std::string data{u8"s:\"\xDF\xBF\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u07FF");
	}
	SUBCASE("first possible 3-byte sequence")
	{
		std::string data{u8"s:\"\xE0\xA0\x80\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u0800");
	}
	SUBCASE("last possible 3-byte sequence")
	{
		std::string data{u8"s:\"\xEF\xBF\xBF\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uFFFF");
	}
	SUBCASE("first possible 4-byte sequence")
	{
		std::string data{u8"s:\"\xF0\x90\x80\x80\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U00010000");
	}
	SUBCASE("last possible 4-byte sequence")
	{
		std::string data{u8"s:\"\xF4\x8F\xBF\xBF\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0010FFFF");
	}
	SUBCASE("one before surrogate range")
	{
		std::string data{u8"s:\"\xED\x9F\xBF\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uD7FF");
	}
	SUBCASE("one after surrogate range")
	{
		std::string data{u8"s:\"\xEE\x80\x80\""};
		D doc{data};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uE000");
	}
}


TEST_CASE_TEMPLATE("read utf8 incomplete code point", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("first continuation byte")
	{
		std::string data{u8"s:\"\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("last continuation byte")
	{
		std::string data{u8"s:\"\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("only first byte of 2-byte sequence")
	{
		std::string data{u8"s:\"\xC2\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("only first byte of 3-byte sequence")
	{
		std::string data{u8"s:\"\xE0\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("only first byte of 4-byte sequence")
	{
		std::string data{u8"s:\"\xF0\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("2-byte sequence with last byte missing")
	{
		std::string data{u8"s:\"\xDF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("3-byte sequence with last byte missing")
	{
		std::string data{u8"s:\"\xEF\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("4-byte sequence with last byte missing")
	{
		std::string data{u8"s:\"\xF4\x8F\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("concatenation of incomplete sequences")
	{
		std::string data{u8"s:\"\xC2\xDF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("incomplete sequence followed by correct bytes")
	{
		std::string data{u8"s:\"\xE0--\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read utf8 impossible byte", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("0xC0")
	{
		std::string data{u8"s:\"\xC0\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("0xC1")
	{
		std::string data{u8"s:\"\xC1\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("0xF5")
	{
		std::string data{u8"s:\"\xF5\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("0xFE")
	{
		std::string data{u8"s:\"\xFE\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("0xFF")
	{
		std::string data{u8"s:\"\xFF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read utf8 minimum overlong sequence", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("U+0000 incorrectly encoded in 2 bytes")
	{
		std::string data{u8"s:\"\xC0\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("U+0000 incorrectly encoded in 3 bytes")
	{
		std::string data{u8"s:\"\xE0\x80\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("U+0000 incorrectly encoded in 4 bytes")
	{
		std::string data{u8"s:\"\xF0\x80\x80\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read utf8 maximum overlong sequence", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("U+007F incorrectly encoded in 2 bytes")
	{
		std::string data{u8"s:\"\xC1\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("U+07FF incorrectly encoded in 3 bytes")
	{
		std::string data{u8"s:\"\xE0\x9F\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("U+FFFF incorrectly encoded in 4 bytes")
	{
		std::string data{u8"s:\"\xF0\x8F\xBF\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE_TEMPLATE("read utf8 invalid surrogate", D, kcv::Document, kcv::DocumentView<8>)
{
	SUBCASE("smallest high surrogate U+D800")
	{
		std::string data{u8"s:\"\xED\xA0\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("largest high surrogate U+DBFF")
	{
		std::string data{u8"s:\"\xED\xAF\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("smallest low surrogate U+DC00")
	{
		std::string data{u8"s:\"\xED\xB0\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("largest low surrogate U+DFFF")
	{
		std::string data{u8"s:\"\xED\xBF\xBF\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
	SUBCASE("paired surrogates U+D800 U+DC00")
	{
		std::string data{u8"s:\"\xED\xA0\x80\xED\xB0\x80\""};
		D doc{data};
		REQUIRE_FALSE(doc);
	}
}
