#ifndef BLOCK_MEMORY_RESOURCE_HPP
#define BLOCK_MEMORY_RESOURCE_HPP

#include <memory_resource>
#include <iostream>

class BlockMemoryResource : public std::pmr::memory_resource {
private:
    struct Block {
        void* ptr;
        size_t size;
        bool is_free;
        Block* next;
        Block(void* p, size_t s) : ptr(p), size(s), is_free(false), next(nullptr) {}
    };
    
    Block* blocks;
    size_t block_size;
    
public:
    BlockMemoryResource(size_t size = 1024);
    BlockMemoryResource(const BlockMemoryResource&) = delete;
    BlockMemoryResource& operator=(const BlockMemoryResource&) = delete;
    ~BlockMemoryResource();
    
    void print_stats() const;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};

#endif