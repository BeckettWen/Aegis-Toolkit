
#include "../Aegis.hpp"
#include <gtest/gtest.h>
#include <chrono>

#include <cstring>
#include <limits>
#include <type_traits>

TEST(memory_manager_test, write_memory_test){
    Aegis_MemoryManager::Aegis_allocator write_allocator;
    std::size_t index = write_allocator.allocateMemory_Optimized(Default_Memory_Size - 1);
    write_allocator.writeDataToMemory_Optimized<std::string>(index, "this is the test message 1");
    std::size_t index_02 = write_allocator.allocateMemory_Optimized(Default_Memory_Size);
    write_allocator.writeDataToMemory_Optimized<std::string>(index_02, "test message 2");
    auto result = write_allocator.readData_Optimized(index);
    if(result.has_value()){
        for(auto item: result.value()){ std::cout<< std::to_integer<uint8_t>(item);}
        std::cout<<"\n";
    }
}

TEST(memory_manager_test, memory_Compression_test) {
    Aegis_MemoryManager::Aegis_allocator test_allocator;
    auto index_holder = test_allocator.allocateMemory_Optimized(1024*1024);
    test_allocator.writeDataToMemory_Optimized<std::string>(index_holder, "this is test1");
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

    auto result_sliced = test_allocator.read_Sliced_data_Tellurium(index_to_be_fragmented);
    if(result_sliced.has_value()){
        for(std::byte item: result_sliced.value()){
            std::cout<<std::to_integer<uint8_t>(item);
        }
        std::cout<<"Sliced test\n";
    }
    else{ std::cout<<result_sliced.error();}
}

TEST(memory_manager_test, defender_test){
    class TestableAegisDefender: public Aegis_MemoryManager::Aegis_Allocator_withdefender {
        public:
            TestableAegisDefender() = default;
            ~TestableAegisDefender() = default;
        };

        TestableAegisDefender defender;
        ASSERT_TRUE(defender.Initialize().has_value());

        std::size_t record = 42;
        long long token = 0;

        auto missingRecordResult = defender.read_protected_data(record, token);
        ASSERT_FALSE(missingRecordResult.has_value());
        EXPECT_EQ(missingRecordResult.error(), "Access Denied");

        ASSERT_TRUE(defender.Generate_Token(record).has_value());

        auto challenge = defender.challenge(token, record);
        ASSERT_TRUE(challenge.has_value());
        EXPECT_EQ(static_cast<int>(challenge.value()), 1);  // Denied

        auto protectedReadResult = defender.read_protected_data(record, token);
        ASSERT_FALSE(protectedReadResult.has_value());
        EXPECT_EQ(protectedReadResult.error(), "Access Denied");
}

TEST(memory_manager_test, security_test){
    class TestableAegisDefender
            : public Aegis_MemoryManager::Aegis_Allocator_withdefender {
        public:
            TestableAegisDefender() = default;
            ~TestableAegisDefender() = default;
        };

        TestableAegisDefender defender;
        ASSERT_TRUE(defender.Initialize().has_value());

        long long invalidToken = std::numeric_limits<long long>::min();

        std::size_t unknownRecord = 101;
        auto unknownRead =
            defender.read_protected_data(unknownRecord, invalidToken);
        ASSERT_FALSE(unknownRead.has_value());
        EXPECT_EQ(unknownRead.error(), "Access Denied");

        std::size_t protectedRecord = 202;
        ASSERT_TRUE(defender.Generate_Token(protectedRecord).has_value());

        auto challenge = defender.challenge(invalidToken, protectedRecord);
        ASSERT_TRUE(challenge.has_value());
        EXPECT_EQ(static_cast<int>(challenge.value()), 1);  // Denied

        auto protectedRead =
            defender.read_protected_data(protectedRecord, invalidToken);
        ASSERT_FALSE(protectedRead.has_value());
        EXPECT_EQ(protectedRead.error(), "Access Denied");

        std::size_t differentRecord = 303;
        auto differentRecordRead =
            defender.read_protected_data(differentRecord, invalidToken);
        ASSERT_FALSE(differentRecordRead.has_value());
        EXPECT_EQ(differentRecordRead.error(), "Access Denied");

        ASSERT_TRUE(defender.Generate_Token(protectedRecord).has_value());
        auto repeatedGenerationChallenge =
            defender.challenge(invalidToken, protectedRecord);
        ASSERT_TRUE(repeatedGenerationChallenge.has_value());
        EXPECT_EQ(static_cast<int>(repeatedGenerationChallenge.value()), 1);
}
