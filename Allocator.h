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
    Allocator(const size_t total_size) : total_size { total_size }, used_size { total_size }{}
    ~Allocator() {
        ::free(buffer_begin);
    }
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
    void free() {
        auto *header = static_cast<Header *>(buffer_begin);
        header->is_free = true;
        header->size = (total_size - sizeof(Header));
        used_size = sizeof(Header);
    };
    virtual Header *find_block(size_t size) = 0;
    void split(Header *header, size_t chunk) {
        //cout << chunk << endl;
        size_t block_size = header->size;
        header->size = chunk;
        header->is_free = false;
        if (block_size - chunk >= sizeof(Header)) {
            auto *next = header->next();
            next->size = block_size - chunk - sizeof(Header);
            //cout << "()()" << next->size << endl;
            next->is_free = true;
            used_size += chunk + sizeof(Header);
            auto *next_next = next->next();
        } else {
            header->size = block_size;
            used_size += block_size;
        }
        //cout << endl;
    }
    bool validate(void *ptr) {
        auto *header = static_cast<Header *>(buffer_begin);
        while (header < buffer_end) {
            if (header + 1 == ptr){ return true; }
            header = header->next();
        }
        return false;
    }
    virtual void reset() = 0;
};

#endif