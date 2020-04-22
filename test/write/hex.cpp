#include <catch2/catch.hpp>

#include <cstdint>
#include <limits>
#include <string>

#include "kcv/kcv.hpp"


TEST_CASE("write hex syntax")
{
	kcv::Document doc{};

	SECTION("zero with default width")
	{
		doc[u8"h"] << kcv::hex(0);
		REQUIRE(doc.dump() == u8"h: 0x0\n");
	}
	SECTION("negative width defaults to 1")
	{
		doc[u8"h"] << kcv::hex(0x42, -8);
		REQUIRE(doc.dump() == u8"h: 0x42\n");
	}
	SECTION("width 0 defaults to 1")
	{
		doc[u8"h"] << kcv::hex(0x42, 0);
		REQUIRE(doc.dump() == u8"h: 0x42\n");
	}
	SECTION("width 16")
	{
		doc[u8"h"] << kcv::hex(0xabcd, 16);
		REQUIRE(doc.dump() == u8"h: 0x000000000000abcd\n");
	}
	SECTION("excessive width may be clamped")
	{
		doc[u8"h"] << kcv::hex(0xabcd, 1000);
		std::string s{doc.dump()};
		REQUIRE(s.substr(0, 5) == u8"h: 0x");
		REQUIRE(s.substr(s.size() - 5) == u8"abcd\n");
	}
}


TEST_CASE("write hex limit")
{
	kcv::Document doc{};

	SECTION("int32 maximum")
	{
		doc[u8"h"] << kcv::hex(std::numeric_limits<std::int32_t>::max());
		REQUIRE(doc.dump() == u8"h: 0x7fffffff\n");
	}
	SECTION("uint32 maximum")
	{
		doc[u8"h"] << kcv::hex(std::numeric_limits<std::uint32_t>::max());
		REQUIRE(doc.dump() == u8"h: 0xffffffff\n");
	}
	SECTION("int64 maximum")
	{
		doc[u8"h"] << kcv::hex(std::numeric_limits<std::int64_t>::max());
		REQUIRE(doc.dump() == u8"h: 0x7fffffffffffffff\n");
	}
	SECTION("uint64 maximum")
	{
		doc[u8"h"] << kcv::hex(std::numeric_limits<std::uint64_t>::max());
		REQUIRE(doc.dump() == u8"h: 0xffffffffffffffff\n");
	}
}
