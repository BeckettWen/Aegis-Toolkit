
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>


TEST(memory_manager_test, final_test) {

    Aegis_MemoryManager::Aegis_allocator test_memory_manager;

    std::cout<<"Test Version: "<<test_memory_manager.major_version<<" "<<test_memory_manager.minor_version<<"\n";

    // the custom aegis allocator
    std::chrono::time_point<std::chrono::high_resolution_clock> stop_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timer
        = std::chrono::high_resolution_clock::now();

    test_memory_manager.allocateMemory(1024);

    stop_timer = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration_aegis = start_timer - stop_timer;
    std::cout<<"before optimization"<<duration_aegis.count() * 1000<<"ms\n";

    // the regular malloc approach
    start_timer = std::chrono::high_resolution_clock::now();

    test_memory_manager.allocateMemory_Optimized(1024);

    stop_timer = std::chrono::high_resolution_clock::now();
    duration_aegis = start_timer - stop_timer;

    std::cout<<"after optimization"<<duration_aegis.count()*1000<<"ms\n";

    // this is the original test which tests the optimized version vs the raw malloc function
    start_timer = std::chrono::high_resolution_clock::now();

    for (int loop = 0; loop < 1024; loop ++){ malloc(1); }

    stop_timer = std::chrono::high_resolution_clock::now();
    duration_aegis = start_timer - stop_timer;

    std::cout<<"Raw malloc"<<duration_aegis.count()*1000<<"ms\n";
    
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