//
// Created by Beckett on 9/9/26.
//

#include <benchmark/benchmark.h>
#include "../Aegis.hpp"

static void BM_AllocateMemory(benchmark::State& state) {
    for (auto _ : state) {
        Aegis_MemoryManager::Aegis_allocator allocator;
        benchmark::DoNotOptimize(allocator.allocateMemory(state.range(0)));
    }
}
BENCHMARK(BM_AllocateMemory)->Range(1024, 1024 * 1024 * 10);

static void BM_AllocateMemory_Optimized(benchmark::State& state) {
    for (auto _ : state) {
        Aegis_MemoryManager::Aegis_allocator allocator;
        benchmark::DoNotOptimize(allocator.allocateMemory_Optimized(state.range(0)));
    }
}
BENCHMARK(BM_AllocateMemory_Optimized)->Range(1024, 1024 * 1024 * 10);

static void BM_GetAllocatedSize(benchmark::State& state) {
    Aegis_MemoryManager::Aegis_allocator allocator;
    std::size_t handle = allocator.allocateMemory(1024);
    for (auto _ : state) {
        auto result = allocator.getAllocatedSize(handle);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetAllocatedSize);

static void BM_ReadData(benchmark::State& state) {
    Aegis_MemoryManager::Aegis_allocator allocator;
    std::size_t handle = allocator.allocateMemory(state.range(0));
    for (auto _ : state) {
        auto result = allocator.readData(handle);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ReadData)->Range(1024, 1024 * 1024);

static void BM_DeleteMemory(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Aegis_MemoryManager::Aegis_allocator allocator;
        std::size_t handle = allocator.allocateMemory(state.range(0));
        state.ResumeTiming();

        benchmark::DoNotOptimize(allocator.DeleteMemory(handle));
    }
}
BENCHMARK(BM_DeleteMemory)->Range(1024, 1024 * 1024);

BENCHMARK_MAIN();