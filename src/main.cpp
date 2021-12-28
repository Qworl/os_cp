#include <iostream>
#include <vector>
#include "FreeListAllocator.h"
#include "Benchmark.h"
#include "BinaryAllocator.h"
using namespace std;

/**
 * Вариант 13
 * Сравнить 2 алгоритма аллокации: списки свободных блоков и блоки по 2 в степени n.
 * Каждый аллокатор должен обладать следующим интерфейсом:
 * Allocator* createMemoryAllocator(void* realMemory, size_t memorySize) - создание аллокатора
 * void* alloc(Allocator* allocator, size_t block_size) - выделение памяти
 * void* free(Allocator* allocator, void* block) - возвращение выделенной памяти
 */

int main(){
	try{
	    size_t size = 1e8;
	    vector<std::size_t> allocations {32, 64, 256, 512, 1024, 2048, 4096};
	    Allocator * freeListAllocator = new FreeListAllocator(size);
	    Benchmark benchmark(OPERATIONS);
	    cout << "FreeListAllocator:" << endl;
	    benchmark.MultipleAllocation(freeListAllocator, allocations);
	    benchmark.MultipleFree(freeListAllocator, allocations);
	    benchmark.RandomAllocation(freeListAllocator, allocations);
	    benchmark.RandomFree(freeListAllocator, allocations);    
	} catch(runtime_error& err){
		cout << err.what() << endl;
	}
	try{
	    size_t size = 1e8;
	    vector<std::size_t> allocations {32, 64, 256, 512, 1024, 2048, 4096};
	    Allocator * binaryAllocator = new BinaryAllocator(size);
	    Benchmark benchmark(OPERATIONS);
	    cout << "BinaryAllocator:" << endl;
	    benchmark.MultipleAllocation(binaryAllocator, allocations);
	    benchmark.MultipleFree(binaryAllocator, allocations);
	    benchmark.RandomAllocation(binaryAllocator, allocations);
	    benchmark.RandomFree(binaryAllocator, allocations);    
	} catch(runtime_error& err){
		cout << err.what() << endl;
	}
	return 0;
}
