cmake_minimum_required(VERSION 3.16)
project(libposture LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- The library ----------------------------------------------------------
# Add new .cpp files here as the library grows (filter, rep counter, ...).
add_library(libposture
    src/geometry.cpp
    src/filter.cpp
    src/rep_counter.cpp
)
target_include_directories(libposture PUBLIC include)

# --- Tests ----------------------------------------------------------------
enable_testing()

# Pull in GoogleTest automatically so nobody has to install it by hand.
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(libposture_tests
    tests/geometry_test.cpp
    tests/filter_test.cpp
    tests/rep_counter_test.cpp
)
target_link_libraries(libposture_tests
    PRIVATE
        libposture
        GTest::gtest_main
)

# Let `ctest` discover and run every TEST() automatically.
include(GoogleTest)
gtest_discover_tests(libposture_tests)
