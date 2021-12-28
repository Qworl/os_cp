#include <iostream>
#include "Allocator.h"
using namespace std;

Allocator::Allocator(const size_t total_size) : total_size { total_size }, used_size { total_size }{}

Allocator::~Allocator() {
    ::free(buffer_begin);
}

void Allocator::free() {
    auto *header = static_cast<Header *>(buffer_begin);
    header->is_free = true;
    header->size = (total_size - sizeof(Header));
    used_size = sizeof(Header);
}

void Allocator::split(Header *header, size_t chunk) {
    size_t block_size = header->size;
    header->size = chunk;
    header->is_free = false;
    if (block_size - chunk >= sizeof(Header)) {
        auto *next = header->next();
        next->size = block_size - chunk - sizeof(Header);
        next->is_free = true;
        used_size += chunk + sizeof(Header);
        auto *next_next = next->next();
    } else {
        header->size = block_size;
        used_size += block_size;
    }
}

bool Allocator::validate(void *ptr) {
    auto *header = static_cast<Header *>(buffer_begin);
    while (header < buffer_end) {
        if (header + 1 == ptr){ return true; }
        header = header->next();
    }
    return false;
}
