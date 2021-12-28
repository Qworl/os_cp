#ifndef BINARYALLOCATOR_H
#define BINARYALLOCATOR_H

#include <cmath>
#include <unistd.h>
#include <stdlib.h>
#include "Allocator.h"

class BinaryAllocator : public Allocator {
public:
    static size_t align(size_t size) { // closest next power of 2
        size_t i = 1;
        size_t k = 0;
        while (i < size){
            i *= 2; ++k;
        }
        return k - 1;
    }
    BinaryAllocator(size_t new_size) {
        if (buffer_begin != nullptr) {
            ::free(buffer_begin);
            buffer_begin = nullptr;
        }
        size_t k = align(new_size);
        total_size = pow(2, k + 1);
        buffer_begin = malloc(total_size);
        if (buffer_begin == nullptr) throw runtime_error("Failed to allocate memory.");
        buffer_end = (void*)((char*)(buffer_begin) + total_size);
        auto *header = (Header *) buffer_begin;
        header->is_free = true;
        header->size = total_size - sizeof(Header);
        used_size = sizeof(Header);
        peak_size = used_size;
        /*long long size_tmp = total_size;
        while(size_tmp > 0){
            for(size_t i = k; i > 0; --i){
                size_t cur = pow(2,i);
                if(size_tmp >= cur + sizeof(Header)){
                    split(header, cur);//- sizeof(Header));
                    header->is_free = true;
                    header = header->next();
                    size_tmp -= cur + sizeof(Header);// + sizeof(Header);
                    peak_size += cur + sizeof(Header);// + sizeof(Header);
                } else if (i == 1){
                    return;
                }
            }
        }*/
        used_size = peak_size;
    }

    void* allocate(size_t new_size) override {
        if (new_size == 0) throw runtime_error("Size must be bigger than 0.");
        size_t k = align(new_size);
        size_t size = pow(2, k + 1);
        if (size > total_size - used_size) throw runtime_error("Not enough memory.");
        auto *header = find_block(size);
        if (header == nullptr) throw runtime_error("No required blocks.");
        split(header, size);
        used_size += size;
        peak_size += size;
        return header + 1; //find_block returns header*
    };
    void deallocate(void* ptr) override {
        if (!validate(ptr)) return; // if address is not valid 
        auto *header = (Header*)ptr - 1;
        header->is_free = true;
        used_size -= header->size;
    };
    Allocator::Header *find_block(size_t size){
        //cout << '\n' << size << endl;
        if(size > total_size) return nullptr;
        auto *header = (Header *)(buffer_begin);
        while(header < buffer_end){
            if((header->size >= size) && (header->is_free)){
                header->is_free = false;
                return header;    
            }
            header = header->next();
        }
        return nullptr;
    }
    void reset() {
        BinaryAllocator(this->total_size);
    }
};

#endif