#include <catch2/catch.hpp>

#include <string>

#include "kcv/kcv.hpp"


// Tests partially based on:
// UTF-8 decoder capability and stress test
// by Markus Kuhn - 2015-08-28 - CC BY 4.0
// https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt


TEST_CASE("read utf8 byte order mark")
{
	SECTION("ignore valid mark")
	{
		int i{1};
		kcv::Document doc{u8"\xEF\xBB\xBFi:42"};
		doc[u8"i"] >> i;
		REQUIRE(i == 42);
	}
	SECTION("invalid second byte")
	{
		kcv::Document doc{u8"\xEF\x42\xBFi:42"};
		REQUIRE_FALSE(doc);
	}
	SECTION("invalid third byte")
	{
		kcv::Document doc{u8"\xEF\xBB\x42i:42"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read utf8 boundary")
{
	std::string s{};

	SECTION("first possible 1-byte sequence")
	{
		kcv::Document doc{std::string{'s',':','"','\0','"'}};
		doc[u8"s"] >> s;
		REQUIRE(s == std::string{'\0'});
	}
	SECTION("last possible 1-byte sequence")
	{
		kcv::Document doc{u8"s:\"\x7F\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u007F");
	}
	SECTION("first possible 2-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xC2\x80\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u0080");
	}
	SECTION("last possible 2-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xDF\xBF\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u07FF");
	}
	SECTION("first possible 3-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xE0\xA0\x80\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u0800");
	}
	SECTION("last possible 3-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xEF\xBF\xBF\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uFFFF");
	}
	SECTION("first possible 4-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xF0\x90\x80\x80\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U00010000");
	}
	SECTION("last possible 4-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xF4\x8F\xBF\xBF\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0010FFFF");
	}
	SECTION("one before surrogate range")
	{
		kcv::Document doc{u8"s:\"\xED\x9F\xBF\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uD7FF");
	}
	SECTION("one after surrogate range")
	{
		kcv::Document doc{u8"s:\"\xEE\x80\x80\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uE000");
	}
}


TEST_CASE("read utf8 incomplete code point")
{
	SECTION("first continuation byte")
	{
		kcv::Document doc{u8"s:\"\x80\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("last continuation byte")
	{
		kcv::Document doc{u8"s:\"\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("only first byte of 2-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xC2\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("only first byte of 3-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xE0\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("only first byte of 4-byte sequence")
	{
		kcv::Document doc{u8"s:\"\xF0\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("2-byte sequence with last byte missing")
	{
		kcv::Document doc{u8"s:\"\xDF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("3-byte sequence with last byte missing")
	{
		kcv::Document doc{u8"s:\"\xEF\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("4-byte sequence with last byte missing")
	{
		kcv::Document doc{u8"s:\"\xF4\x8F\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("concatenation of incomplete sequences")
	{
		kcv::Document doc{u8"s:\"\xC2\xDF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("incomplete sequence followed by correct bytes")
	{
		kcv::Document doc{u8"s:\"\xE0--\""};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read utf8 impossible byte")
{
	SECTION("0xC0")
	{
		kcv::Document doc{u8"s:\"\xC0\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("0xC1")
	{
		kcv::Document doc{u8"s:\"\xC1\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("0xF5")
	{
		kcv::Document doc{u8"s:\"\xF5\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("0xFE")
	{
		kcv::Document doc{u8"s:\"\xFE\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("0xFF")
	{
		kcv::Document doc{u8"s:\"\xFF\""};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read utf8 minimum overlong sequence")
{
	SECTION("U+0000 incorrectly encoded in 2 bytes")
	{
		kcv::Document doc{u8"s:\"\xC0\x80\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("U+0000 incorrectly encoded in 3 bytes")
	{
		kcv::Document doc{u8"s:\"\xE0\x80\x80\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("U+0000 incorrectly encoded in 4 bytes")
	{
		kcv::Document doc{u8"s:\"\xF0\x80\x80\x80\""};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read utf8 maximum overlong sequence")
{
	SECTION("U+007F incorrectly encoded in 2 bytes")
	{
		kcv::Document doc{u8"s:\"\xC1\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("U+07FF incorrectly encoded in 3 bytes")
	{
		kcv::Document doc{u8"s:\"\xE0\x9F\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("U+FFFF incorrectly encoded in 4 bytes")
	{
		kcv::Document doc{u8"s:\"\xF0\x8F\xBF\xBF\""};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read utf8 invalid surrogate")
{
	SECTION("smallest high surrogate U+D800")
	{
		kcv::Document doc{u8"s:\"\xED\xA0\x80\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("largest high surrogate U+DBFF")
	{
		kcv::Document doc{u8"s:\"\xED\xAF\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("smallest low surrogate U+DC00")
	{
		kcv::Document doc{u8"s:\"\xED\xB0\x80\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("largest low surrogate U+DFFF")
	{
		kcv::Document doc{u8"s:\"\xED\xBF\xBF\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("paired surrogates U+D800 U+DC00")
	{
		kcv::Document doc{u8"s:\"\xED\xA0\x80\xED\xB0\x80\""};
		REQUIRE_FALSE(doc);
	}
}
