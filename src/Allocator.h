#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
using namespace std;

class Allocator {
public:
    void* buffer_begin = nullptr;
    void* buffer_end = nullptr;
    size_t total_size = 0;
    size_t used_size = 0;
    size_t peak_size = 0;
    Allocator() = default;
    Allocator(const size_t total_size);
    ~Allocator();
    struct Header {
    public:
        size_t size;
        bool is_free;
        Header *next() {
            return (Header *) ((char *) (this + 1) + size);
        }
    };
    virtual void* allocate(size_t size) = 0;
    virtual void deallocate(void* ptr) = 0;
    void free();
    virtual Header *find_block(size_t size) = 0;
    void split(Header *header, size_t chunk);
    bool validate(void *ptr);
    virtual void reset() = 0;
};

#endif
