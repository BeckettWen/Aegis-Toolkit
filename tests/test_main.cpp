// this is the main file of the test, specifies which test to run

#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::GTEST_FLAG(filter) =
        "StateMachineTest.*";

    return RUN_ALL_TESTS();
}
