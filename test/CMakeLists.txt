cmake_minimum_required(VERSION 3.11)

# Test executable
add_executable(kcv-test "main.cpp" "tests.cpp")
target_link_libraries(kcv-test PRIVATE kcv doctest)

# Language properties
set_property(TARGET kcv-test PROPERTY CXX_STANDARD_REQUIRED TRUE)
set_property(TARGET kcv-test PROPERTY CXX_EXTENSIONS FALSE)

# Verbose compiler warnings
if(MSVC)
	target_compile_options(kcv-test PRIVATE /W4 /WX)
else()
	target_compile_options(kcv-test PRIVATE -Wall -Wextra -Werror -pedantic)
endif()
