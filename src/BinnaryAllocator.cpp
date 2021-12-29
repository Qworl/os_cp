#include <cmath>
#include <unistd.h>
#include <stdlib.h>
#include "BinaryAllocator.h"
using namespace std;

size_t BinaryAllocator::align(size_t size) { // closest next power of 2
    size_t i = 1;
    size_t k = 0;
    while (i < size){
        i *= 2; ++k;
    }
    return i / 2;
}

BinaryAllocator::BinaryAllocator(size_t new_size) {
    if (buffer_begin != nullptr) {
        free(buffer_begin);
        buffer_begin = nullptr;
    }
    total_size = align(new_size) * 2;
    buffer_begin = malloc(total_size);
    if (buffer_begin == nullptr) throw runtime_error("Failed to allocate memory.");
    buffer_end = (void*)((char*)(buffer_begin) + total_size);
    auto *header = (Header *) buffer_begin;
    header->is_free = true;
    header->size = total_size - sizeof(Header);
    used_size = sizeof(Header);
    peak_size = used_size;
}

void* BinaryAllocator::allocate(size_t new_size) {
    if (new_size == 0) throw runtime_error("Size must be bigger than 0.");
    size_t size = align(new_size) * 2;
    if (size > total_size - used_size) throw runtime_error("Not enough memory.");
    auto *header = find_block(size);
    if (header == nullptr) throw runtime_error("No required blocks.");
    split(header, size);
    used_size += size;
    peak_size += size;
    return header + 1; //find_block returns header*
}

void BinaryAllocator::deallocate(void* ptr) {
    if (!validate(ptr)) return; // if address is not valid 
    auto *header = (Header*)ptr - 1;
    header->is_free = true;
    used_size -= header->size;
}

void BinaryAllocator::reset() {
    if (buffer_begin != nullptr) {
        free(buffer_begin);
        buffer_begin = nullptr;
    }
    buffer_begin = malloc(total_size);
    if (buffer_begin == nullptr) throw runtime_error("Failed to allocate memory.");
    buffer_end = (void*)((char*)(buffer_begin) + total_size);
    auto *header = (Header *) buffer_begin;
    header->is_free = true;
    header->size = total_size - sizeof(Header);
    used_size = sizeof(Header);
    peak_size = used_size;
}
