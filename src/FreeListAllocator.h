#ifndef FREEBLOCKSALLOCATOR_H
#define FREEBLOCKSALLOCATOR_H

#include "Allocator.h"

class FreeListAllocator : public Allocator {
public:
    FreeListAllocator(size_t size);
    void* allocate(std::size_t new_size) override;
    void deallocate(void* ptr) override;
    void reset();
private:
    void merge(Header *header);
};

#endif
