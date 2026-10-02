
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>

TEST(memory_manager_test, write_memory_test){}

TEST(memory_manager_test, memory_Compression_test) {
    Aegis_MemoryManager::Aegis_allocator test_allocator;
    auto index_holder = test_allocator.allocateMemory_Optimized(1024*512);
    auto index_to_be_fragmented = test_allocator.allocateMemory_Optimized(1024*512);
    test_allocator.Memory_Compression();

    
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}