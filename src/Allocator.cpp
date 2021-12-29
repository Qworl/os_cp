#include <iostream>
#include "Allocator.h"
using namespace std;


Allocator::Allocator(const size_t total_size) : total_size { total_size }, used_size { 0 }{}

Allocator::~Allocator() {
    free(buffer_begin);
}

void Allocator::split(Header *header, size_t chunk) {
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

bool Allocator::validate(void *ptr) {
    auto *header = static_cast<Header *>(buffer_begin);
    while (header < buffer_end) {
        if (header + 1 == ptr){ return true; }
        header = header->next();
    }
    return false;
}

Allocator::Header *Allocator::find_block(size_t size) {
    auto *header = static_cast<Allocator::Header *>(buffer_begin);
    while (!header->is_free || header->size < size) {
        header = header->next();
        if (header >= buffer_end) { return nullptr; }
    }
    return header;
}
