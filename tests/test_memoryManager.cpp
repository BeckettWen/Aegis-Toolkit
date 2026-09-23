
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>


TEST(memory_manager_test, read_memory_test) {
    Aegis_MemoryManager::Aegis_allocator allocator;
    std::size_t index = allocator.allocateMemory_Optimized(1024*1024);
    std::string data = "this is the test string";
    allocator.writeDataToMemory_Optimized<std::string>(index, data);
    auto read_data = allocator.readData_Optimized(index);
    for (std::byte item: read_data.value()) {
        std::cout<<static_cast<char>(std::to_integer<unsigned char>(item));
    }
}

// here will test all the optimized functions and the original function
TEST(memory_manager_test, optimization_test) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timer, stop_timer;
    start_timer = std::chrono::high_resolution_clock::now();

    // the optimized write function

}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}