#ifndef BINARYALLOCATOR_H
#define BINARYALLOCATOR_H

#include <cmath>
#include <unistd.h>
#include <stdlib.h>
#include "Allocator.h"

class BinaryAllocator : public Allocator {
public:
    static size_t align(size_t size);
    BinaryAllocator(size_t new_size);
    void* allocate(size_t new_size) override;
    void deallocate(void* ptr) override;
    Allocator::Header *find_block(size_t size);
    void reset();
};

#endif
