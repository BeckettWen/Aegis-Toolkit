
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>

TEST(memory_manager_test, write_memory_test){
    Aegis_MemoryManager::Aegis_allocator write_allocator;
    std::size_t index = write_allocator.allocateMemory_Optimized(Default_Memory_Size);
    write_allocator.writeDataToMemory_Optimized<std::string>(index, "this is the test message\n");
    std::cout<<std::to_integer<uint8_t>(write_allocator.readData_Optimized(index).value()[0])<<"\n";
}

TEST(memory_manager_test, memory_Compression_test) {
    Aegis_MemoryManager::Aegis_allocator test_allocator;
    auto index_holder = test_allocator.allocateMemory_Optimized(1024*1024);
    test_allocator.writeDataToMemory_Optimized<std::string>(index_holder, "test1");
    auto index_to_be_fragmented = test_allocator.allocateMemory_Optimized(1024*512);
    test_allocator.writeDataToMemory_Optimized<std::string>(index_to_be_fragmented, "test2");
    test_allocator.Memory_Compression();

    auto result_normal = test_allocator.readData_Optimized(index_holder);
    if(result_normal.has_value()){
        for(std::byte item: result_normal.value()){
            std::cout<<std::to_integer<uint8_t>(item);
        }
        std::cout<<"Normal test\n";
    }

    auto result_sliced = test_allocator.readData_Optimized(index_to_be_fragmented);
    if(result_sliced.has_value()){
        for(std::byte item: result_sliced.value()){
            std::cout<<std::to_integer<uint8_t>(item);
        }
        std::cout<<"Sliced test\n";
    }
    else{ std::cout<<result_sliced.error();}
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}