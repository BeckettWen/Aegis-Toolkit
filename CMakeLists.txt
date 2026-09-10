
cmake_minimum_required(VERSION 3.14)
project(Aegis LANGUAGES CXX)

# Set C++ standard (C++23 or C++26 if supported)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and include GTest
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(benchmark REQUIRED)

# Add your executable
add_executable(tests
    tests/test_memoryManager.cpp
        # Add other test source files here
)

add_executable(benchmark
    tests/benchmark_memoryManager.cpp
)

# Link GTest to your executable
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main benchmark::benchmark)
target_link_libraries(benchmark PRIVATE GTest::GTest GTest::Main benchmark::benchmark)

# Add a custom command to run tests (optional)
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests..."
)

# Add tests to CTest (optional)
gtest_discover_tests(tests)
