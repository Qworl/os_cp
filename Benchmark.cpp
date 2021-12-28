#include <iostream>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <cassert>
#include "Benchmark.h"
using namespace std;

void Benchmark::SingleAllocation(Allocator* allocator, size_t size) {
    cout << "BENCHMARK: ALLOCATION" << endl;
    cout << "\tSize:     \t" << size << endl;
    StartRound();
    auto operations = 0u;
    while (operations < operationGoal) {
        allocator->allocate(size);
        ++operations;
    }
    FinishRound();
    BenchmarkResults results = buildResults(operationGoal, move(TimeElapsed), allocator->peak_size);
    PrintResults(results);
}

void Benchmark::SingleFree(Allocator* allocator, size_t size) {
    cout << "BENCHMARK: ALLOCATION/FREE" << endl;
    cout << "\tSize:     \t" << size << endl;
    void* addresses[OPERATIONS];
    StartRound();
    auto operations = 0u;
    while (operations < operationGoal) {
        addresses[operations] = allocator->allocate(size);
        ++operations;
    }
    while (operations) {
        allocator->deallocate(addresses[--operations]);
    }
    FinishRound();
    BenchmarkResults results = buildResults(operationGoal, move(TimeElapsed), allocator->peak_size);
    PrintResults(results);
}

void Benchmark::MultipleAllocation(Allocator* allocator, vector<size_t>& allocationSizes) {
    for (auto i = 0u; i < allocationSizes.size(); ++i) {
        SingleAllocation(allocator, allocationSizes[i]);
    }
}

void Benchmark::MultipleFree(Allocator* allocator, vector<size_t>& allocationSizes) {
    for (auto i = 0u; i < allocationSizes.size(); ++i) {
        SingleFree(allocator, allocationSizes[i]);
    }
}

void Benchmark::RandomAllocation(Allocator* allocator, vector<size_t>& allocationSizes) {
    srand(1);
    cout << "BENCHMARK: ALLOCATION" << endl;
    StartRound();
    allocator->reset();
    size_t allocation_size;
    auto operations = 0u;
    while (operations < operationGoal) {
        this->RandomAllocationAttr(allocationSizes, allocation_size);
        allocator->allocate(allocation_size);
        ++operations;
    }
    FinishRound();
    BenchmarkResults results = buildResults(operationGoal, move(TimeElapsed), allocator->peak_size);
    PrintResults(results);
}

void Benchmark::RandomFree(Allocator* allocator, vector<size_t>& allocationSizes) {
    srand(1);
    cout << "BENCHMARK: ALLOCATION/FREE" << endl;
    StartRound();
    allocator->reset();
    void* addresses[OPERATIONS];
    size_t allocation_size;
    auto operations = 0u;
    while (operations < operationGoal) {
        this->RandomAllocationAttr(allocationSizes, allocation_size);
        addresses[operations] = allocator->allocate(allocation_size);
        ++operations;
    }
    while (operations) {
        allocator->deallocate(addresses[--operations]);
    }
    FinishRound();
    BenchmarkResults results = buildResults(operationGoal, move(TimeElapsed), allocator->peak_size);
    PrintResults(results);
}

void Benchmark::PrintResults(BenchmarkResults& results) {
    cout << "\tRESULTS:" << endl;
    cout << "\t\tOperations:    \t" << results.Operations << endl;
    cout << "\t\tTime elapsed: \t" << results.Milliseconds.count() << " ms" << endl;
    cout << "\t\tOp per sec:    \t" << results.OperationsPerSec << " ops/ms" << endl;
    cout << "\t\tTimer per op:  \t" << results.TimePerOperation << " ms/ops" << endl;
    cout << "\t\tMemory peak:   \t" << results.MemoryPeak << " bytes" << endl;
    cout << endl;
}

BenchmarkResults Benchmark::buildResults(size_t newOperationGoal, chrono::milliseconds&& elapsedTime, size_t memoryPeak) {
    BenchmarkResults results;
    results.Operations = newOperationGoal;
    results.Milliseconds = move(elapsedTime);
    results.OperationsPerSec = results.Operations / static_cast<double>(results.Milliseconds.count());
    results.TimePerOperation = static_cast<double>(results.Milliseconds.count()) / static_cast<double>(results.Operations);
    results.MemoryPeak = memoryPeak;
    return results;
}

void Benchmark::RandomAllocationAttr(vector<size_t>& allocationSizes, size_t & size) {
    int r = rand() % allocationSizes.size();
    size = allocationSizes[r];
}