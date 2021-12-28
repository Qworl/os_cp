#ifndef FREEBLOCKSALLOCATOR_H
#define FREEBLOCKSALLOCATOR_H

#include "Allocator.h"

class FreeListAllocator : public Allocator {
public:
    explicit FreeListAllocator(size_t size);
    void* allocate(std::size_t new_size) override;
    void deallocate(void* ptr) override;
    void reset();
    Header *find_block(size_t size);
private:
    void merge(Header *header);
};

#endif