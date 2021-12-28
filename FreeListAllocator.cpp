#include "FreeListAllocator.h"
#include <stdlib.h>
#include <algorithm>
using namespace std;

FreeListAllocator::FreeListAllocator(size_t size) {
    if (buffer_begin != nullptr) {
        ::free(buffer_begin);
        buffer_begin = nullptr;
    }
    buffer_begin = malloc(size);
    if (buffer_begin == nullptr) {
        std::cerr << "Failed to allocate memory\n";
        return;
    }
    total_size = size;
    buffer_end = (void*)((char*)(buffer_begin) + total_size);
    auto *header = (Header *) buffer_begin;
    header->is_free = true;
    header->size = (total_size - sizeof(Header));
    used_size = sizeof(Header);
    peak_size = used_size;
}

void* FreeListAllocator::allocate(std::size_t new_size) {
    if (new_size == 0) throw std::runtime_error("Size must be bigger than 0.");
    if (new_size > total_size - used_size) throw std::runtime_error("Not enough memory.");
    auto *header = find_block(new_size);
    if (header == nullptr) throw std::runtime_error("No required blocks.");
    split(header, new_size);
    peak_size += new_size;
    return header + 1; //find_block returns header*
}

Allocator::Header *FreeListAllocator::find_block(size_t size) {
    auto *header = static_cast<Header *>(buffer_begin);
    while (!header->is_free || header->size < size) {
        header = header->next();
        if (header >= buffer_end) { return nullptr; }
    }
    return header;
}

void FreeListAllocator::deallocate(void* ptr) {
    if (!validate(ptr)) return; // if address is not valid 
    auto *header = (Header*)ptr - 1;
    header->is_free = true;
    used_size -= header->size;
    merge(header);
}

void FreeListAllocator::reset() {
    FreeListAllocator(this->total_size);
}

void FreeListAllocator::merge(Header *header) {
    auto *next = header->next();
    if (header != buffer_end && next->is_free) { // if next free
        header->size += sizeof(Header) + header->next()->size;
        used_size -= sizeof(Header);
    }
}