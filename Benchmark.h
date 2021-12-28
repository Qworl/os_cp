#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h> // timespec
#include <cstddef> // size_t
#include <chrono>
#include <ratio>
#include <vector>

#include "Allocator.h" // base class allocator
using namespace std;

#define OPERATIONS 1000
struct BenchmarkResults
{
	size_t Operations;
	chrono::milliseconds Milliseconds;
	double OperationsPerSec;
	double TimePerOperation;
    size_t MemoryPeak;
};

class Benchmark {
public:
    Benchmark() = delete;
    Benchmark(unsigned int newOperationGoal) : operationGoal { newOperationGoal } { }
	void SingleAllocation(Allocator* allocator, size_t size);
	void SingleFree(Allocator* allocator, size_t size);
	void MultipleAllocation(Allocator* allocator, vector<size_t>& allocationSizes);
	void MultipleFree(Allocator* allocator, vector<size_t>& allocationSizes);
	void RandomAllocation(Allocator* allocator, vector<size_t>& allocationSizes);
	void RandomFree(Allocator* allocator, vector<size_t>& allocationSizes);
private:
	void PrintResults(BenchmarkResults& results);
	void RandomAllocationAttr(vector<size_t>& allocationSizes, size_t & size);
	BenchmarkResults buildResults(size_t newOperationGoal, chrono::milliseconds&& ellapsedTime, size_t memoryUsed);
    void SetStartTime() noexcept { Start = chrono::high_resolution_clock::now(); }
    void SetFinishTime() noexcept { Finish = chrono::high_resolution_clock::now(); }
    void SetElapsedTime() noexcept { TimeElapsed = chrono::duration_cast<chrono::milliseconds>(Finish - Start); }
    void StartRound() noexcept { SetStartTime(); }
    void FinishRound() noexcept
    {
        SetFinishTime();
        SetElapsedTime();
    }
private:
	size_t operationGoal;
    chrono::time_point<chrono::high_resolution_clock> Start;
    chrono::time_point<chrono::high_resolution_clock> Finish;
    chrono::milliseconds TimeElapsed;
};

#endif