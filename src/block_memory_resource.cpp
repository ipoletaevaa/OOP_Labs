#include "block_memory_resource.hpp"
#include <cstdlib>

BlockMemoryResource::BlockMemoryResource(size_t size) : blocks(nullptr), block_size(size) {}

BlockMemoryResource::~BlockMemoryResource() {
    Block* current = blocks;
    while (current) {
        Block* next = current->next;
        if (!current->is_free) {
            std::cout << "Warning: Not deallocated: " << current->ptr << " (" << current->size << " bytes)\n";
        }
        std::free(current->ptr);
        delete current;
        current = next;
    }
}

void* BlockMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    Block* current = blocks;
    while (current) {
        if (current->is_free && current->size >= bytes) {
            current->is_free = false;
            return current->ptr;
        }
        current = current->next;
    }
    
    size_t actual_size = (bytes > block_size) ? bytes : block_size;
    void* memory = std::malloc(actual_size);
    
    Block* new_block = new Block(memory, actual_size);
    new_block->next = blocks;
    blocks = new_block;
    
    return memory;
}

void BlockMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    Block* current = blocks;
    while (current) {
        if (current->ptr == p) {
            current->is_free = true;
            return;
        }
        current = current->next;
    }
}

bool BlockMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

void BlockMemoryResource::print_stats() const {
    size_t total = 0;
    size_t free = 0;
    Block* current = blocks;
    
    while (current) {
        total++;
        if (current->is_free) free++;
        current = current->next;
    }
    
    std::cout << "Blocks: " << total << ", Free: " << free << "\n";
}