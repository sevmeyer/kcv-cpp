cmake_minimum_required(VERSION 3.11)

project(kcv)

# Library target
add_library(kcv INTERFACE)
target_include_directories(kcv INTERFACE "include")
target_compile_features(kcv INTERFACE cxx_std_17)

# Top-level build
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory("extern/doctest")
	add_subdirectory("test")
endif ()
