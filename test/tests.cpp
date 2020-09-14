#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "doctest/doctest.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

#include "kcv/kcv.hpp"


TYPE_TO_STRING(kcv::Document);
TYPE_TO_STRING(kcv::DocumentView<0>);
TYPE_TO_STRING(kcv::DocumentView<1>);
TYPE_TO_STRING(kcv::DocumentView<2>);
TYPE_TO_STRING(kcv::DocumentView<8>);


#include "read/bool.tpp"
#include "read/float.tpp"
#include "read/hex.tpp"
#include "read/int.tpp"
#include "read/item.tpp"
#include "read/string.tpp"
#include "read/utf8.tpp"

#include "write/bool.tpp"
#include "write/float.tpp"
#include "write/hex.tpp"
#include "write/int.tpp"
#include "write/item.tpp"
#include "write/string.tpp"
