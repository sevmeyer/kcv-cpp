cmake_minimum_required(VERSION 3.11)

# Test executable
add_executable(kcv-test
	"main.cpp"
	"read/bool.cpp"
	"read/float.cpp"
	"read/hex.cpp"
	"read/int.cpp"
	"read/item.cpp"
	"read/string.cpp"
	"read/utf8.cpp"
	"write/bool.cpp"
	"write/float.cpp"
	"write/hex.cpp"
	"write/int.cpp"
	"write/item.cpp"
	"write/string.cpp")
target_link_libraries(kcv-test PRIVATE kcv catch2)

# Language properties
set_property(TARGET kcv-test PROPERTY CXX_STANDARD_REQUIRED TRUE)
set_property(TARGET kcv-test PROPERTY CXX_EXTENSIONS FALSE)

# Verbose compiler warnings
if(MSVC)
	target_compile_options(kcv-test PRIVATE /W4 /WX)
else()
	target_compile_options(kcv-test PRIVATE -Wall -Wextra -Werror -pedantic)
endif()
