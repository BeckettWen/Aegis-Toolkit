
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>


TEST(memory_manager_test, final_test) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timer
        = std::chrono::high_resolution_clock::now();

    
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}