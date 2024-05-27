cmake_minimum_required(VERSION 3.12)
project(MinUI_Test C)

# This CMakeLists.txt is for testing purposes only.
add_compile_definitions(TESTING)

# Collect all source files
file(GLOB SOURCES *.c)
add_executable(MinUI_Test ${SOURCES})

# Setup testing
enable_testing()
include(CTest)

add_test(
    NAME MinUI_UnitTest
    COMMAND MinUI_Test
)
