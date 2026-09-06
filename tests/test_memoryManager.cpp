
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>


TEST(memory_manager_test, final_test) {

    Aegis_MemoryManager::Aegis_allocator test_memory_manager;


    // the custom aegis allocator
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timer
        = std::chrono::high_resolution_clock::now();

    test_memory_manager.allocateMemory(1024);

    std::chrono::time_point<std::chrono::high_resolution_clock> stop_timer
        = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration_aegis = start_timer - stop_timer;
    std::cout<<"before optimization"<<duration_aegis.count() * 1000<<"ms\n";

    // the regular malloc approach
    start_timer = std::chrono::high_resolution_clock::now();

    test_memory_manager.allocateMemory_Optimized(1024);

    stop_timer = std::chrono::high_resolution_clock::now();
    duration_aegis = start_timer - stop_timer;

    std::cout<<"after optimization"<<duration_aegis.count()*1000<<"ms\n";
    
}

TEST(memory_manager_test, optimization_test){}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}