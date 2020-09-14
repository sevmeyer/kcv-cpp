cmake_minimum_required(VERSION 3.11)
include(FetchContent)

# Target:  doctest
# Include: doctest/doctest.h
# License: MIT

# Declaration
FetchContent_Declare(doctest
	URL "https://github.com/onqtam/doctest/raw/2.4.0/doctest/doctest.h"
	URL_HASH SHA256=01a30fca28a6c7bb1aa48d0288a029c3bcb1115e492d5b89200635d8ff58041b
	DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/doctest"
	DOWNLOAD_NO_EXTRACT TRUE)

# Download
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
	message(STATUS "Fetching external library: doctest")
	FetchContent_Populate(doctest)
endif()

# Target
add_library(doctest INTERFACE)
target_include_directories(doctest SYSTEM INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
