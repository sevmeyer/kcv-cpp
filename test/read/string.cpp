#include <catch2/catch.hpp>

#include <string>
#include <string_view>

#include "kcv/kcv.hpp"


TEST_CASE("read string syntax")
{
	std::string s{"original"};

	SECTION("missing quotes")
	{
		kcv::Document doc{u8"s:hello"};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing opening quote")
	{
		kcv::Document doc{u8"s:hello\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("missing closing quote")
	{
		kcv::Document doc{u8"s:\"hello"};
		REQUIRE_FALSE(doc);
	}
	SECTION("single quotes instead of double quotes")
	{
		kcv::Document doc{u8"s:'hello'"};
		REQUIRE_FALSE(doc);
	}
	SECTION("unescaped quote")
	{
		kcv::Document doc{u8"s:\"\"\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("unescaped backslash")
	{
		kcv::Document doc{u8"s:\"\\\""};
		REQUIRE_FALSE(doc);
	}
	SECTION("bool instead of string")
	{
		kcv::Document doc{u8"s:yes"};
		auto item{doc[u8"s"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> s);
		REQUIRE(s == "original");
	}
	SECTION("number instead of string")
	{
		kcv::Document doc{u8"s:1"};
		auto item{doc[u8"s"]};
		REQUIRE(item);
		REQUIRE_FALSE(item >> s);
		REQUIRE(s == "original");
	}
}


TEST_CASE("read string content")
{
	std::string s{"original"};

	SECTION("empty")
	{
		kcv::Document doc{u8"s:\"\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"");
	}
	SECTION("whitespace")
	{
		kcv::Document doc{u8"s:\" \t \n \r \r\n \""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8" \t \n \r \r\n ");
	}
	SECTION("integer")
	{
		kcv::Document doc{u8"s:\"42\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"42");
	}
	SECTION("hexadecimal")
	{
		kcv::Document doc{u8"s:\"0x42\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"0x42");
	}
	SECTION("decimal")
	{
		kcv::Document doc{u8"s:\"314.159e-2\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"314.159e-2");
	}
	SECTION("bool")
	{
		kcv::Document doc{u8"s:\"yes\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"yes");
	}
	SECTION("item")
	{
		kcv::Document doc{u8"s:\"foo:42\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"foo:42");
	}
	SECTION("ascii")
	{
		kcv::Document doc{u8"s:\" !\\\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
	}
	SECTION("international")
	{
		kcv::Document doc{u8"s:\"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"中文 Español English हिन्दी العربية বাংলা Português Русский 日本語 پنجابی");
	}
	SECTION("combining characters")
	{
		kcv::Document doc{u8"s:\"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u1EAD \u00E2\u0323 \u1EA1\u0302 \u0061\u0302\u0323 \u0061\u0323\u0302");
	}
	SECTION("supplementary multilingual plane")
	{
		kcv::Document doc{u8"s:\"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0001F600 \U0001F601\uFE0E \U0001F602\uFE0F \u270C\U0001F3FD");
	}
	SECTION("leading BOM")
	{
		kcv::Document doc{u8"s:\"\xEF\xBB\xBF...\""};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\xEF\xBB\xBF...");
	}
}


TEST_CASE("read string encoding")
{
	SECTION("invalid utf16")
	{
		std::string s{'\x73','\x3A','\x22','\x00','\x66','\x00','\xF6','\x00','\xF6','\x22'};
		kcv::Document doc{s};
		REQUIRE_FALSE(doc);
	}
	SECTION("invalid iso8859-1")
	{
		kcv::Document doc{"\x73\x3A\x22\x66\xF6\xF6\x22"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read string with character escape")
{
	std::string s{"original"};

	SECTION("double quote")
	{
		kcv::Document doc{u8R"(s:"\"")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\"");
	}
	SECTION("backslash")
	{
		kcv::Document doc{u8R"(s:"\\")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\\");
	}
	SECTION("tab")
	{
		kcv::Document doc{u8R"(s:"\t")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\t");
	}
	SECTION("line feed")
	{
		kcv::Document doc{u8R"(s:"\n")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\n");
	}
	SECTION("carriage return")
	{
		kcv::Document doc{u8R"(s:"\r")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\r");
	}
	SECTION("windows newline")
	{
		kcv::Document doc{u8R"(s:"\r\n")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\r\n");
	}
	SECTION("cannot escape 0")
	{
		kcv::Document doc{u8R"(s:"\0")"};
		REQUIRE_FALSE(doc);
	}
	SECTION("cannot escape f")
	{
		kcv::Document doc{u8R"(s:"\f")"};
		REQUIRE_FALSE(doc);
	}
	SECTION("cannot escape x")
	{
		kcv::Document doc{u8R"(s:"\x42")"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read string with unicode escape")
{
	std::string s{"original"};

	SECTION("case insensitive")
	{
		kcv::Document doc{u8R"(s:"\u1E9e")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u1E9E");
	}
	SECTION("minimum code point")
	{
		kcv::Document doc{u8R"(s:"\u0000")"};
		doc[u8"s"] >> s;
		REQUIRE(s == std::string{'\0'});
	}
	SECTION("maximum code point for 2 byte expansion")
	{
		kcv::Document doc{u8R"(s:"\u07FF")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\u07FF");
	}
	SECTION("maximum code point for 3 byte expansion")
	{
		kcv::Document doc{u8R"(s:"\uFFFF")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uFFFF");
	}
	SECTION("maximum code point for 4 byte expansion")
	{
		kcv::Document doc{u8R"(s:"\U0010FFFF")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\U0010FFFF");
	}
	SECTION("code point overflow")
	{
		kcv::Document doc{u8R"(s:"\U00110000")"};
		auto item{doc[u8"s"]};
		REQUIRE_FALSE(item >> s);
		REQUIRE(s == "original");
	}
	SECTION("one before surrogate range")
	{
		kcv::Document doc{u8R"(s:"\uD7FF")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uD7FF");
	}
	SECTION("one after surrogate range")
	{
		kcv::Document doc{u8R"(s:"\uE000")"};
		doc[u8"s"] >> s;
		REQUIRE(s == u8"\uE000");
	}
	SECTION("first surrogate")
	{
		kcv::Document doc{u8R"(s:"\uD800")"};
		auto item{doc[u8"s"]};
		REQUIRE_FALSE(item >> s);
		REQUIRE(s == "original");
	}
	SECTION("last surrogate")
	{
		kcv::Document doc{u8R"(s:"\uDFFF")"};
		auto item{doc[u8"s"]};
		REQUIRE_FALSE(item >> s);
		REQUIRE(s == "original");
	}
	SECTION("not enough digits for uXXXX")
	{
		kcv::Document doc{u8R"(s:"\u123x")"};
		REQUIRE_FALSE(doc);
	}
	SECTION("not enough digits for UXXXXXXXX")
	{
		kcv::Document doc{u8R"(s:"\U0001234x")"};
		REQUIRE_FALSE(doc);
	}
}


TEST_CASE("read string_view")
{
	std::string_view v{};

	SECTION("literal string")
	{
		kcv::Document doc{u8"v:\"Hello!\""};
		doc[u8"v"] >> v;
		REQUIRE(v == u8"Hello!");
	}
	SECTION("cannot read string with escape sequence")
	{
		kcv::Document doc{u8R"(v:"Hello\u0021")"};
		auto item{doc[u8"v"]};
		REQUIRE_FALSE(item >> v);
		REQUIRE(v == std::string_view{});
	}
}
