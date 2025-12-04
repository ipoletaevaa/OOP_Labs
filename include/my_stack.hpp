#ifndef MY_STACK_HPP
#define MY_STACK_HPP

#include <memory_resource>
#include <stdexcept>
#include <utility>

template<typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;
        
        template<typename... Args>
        Node(Node* nxt, Args&&... args) 
            : data(std::forward<Args>(args)...), next(nxt) {}
    };
    
    Node* top_node;
    size_t size_;
    std::pmr::polymorphic_allocator<Node> allocator_;
    
public:
    class Iterator {
    private:
        Node* current;
    
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator(Node* node = nullptr) : current(node) {}
        
        reference operator*() const { return current->data; }
        pointer operator->() const { return &current->data; }
        
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };
    
    MyStack(std::pmr::memory_resource* mr = std::pmr::get_default_resource()) 
        : top_node(nullptr), size_(0), allocator_(mr) {}
    
    MyStack(const MyStack&) = delete;
    MyStack& operator=(const MyStack&) = delete;
    
    MyStack(MyStack&& other) noexcept 
        : top_node(other.top_node), size_(other.size_), allocator_(std::move(other.allocator_)) {
        other.top_node = nullptr;
        other.size_ = 0;
    }
    
    MyStack& operator=(MyStack&& other) noexcept {
        if (this != &other) {
            clear();
            top_node = other.top_node;
            size_ = other.size_;
            allocator_ = std::move(other.allocator_);
            other.top_node = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~MyStack() {
        clear();
    }
    
    void push(const T& value) {
        Node* new_node = allocator_.allocate(1);
        try {
            allocator_.construct(new_node, top_node, value);
        } catch (...) {
            allocator_.deallocate(new_node, 1);
            throw;
        }
        top_node = new_node;
        size_++;
    }
    
    void push(T&& value) {
        Node* new_node = allocator_.allocate(1);
        try {
            allocator_.construct(new_node, top_node, std::move(value));
        } catch (...) {
            allocator_.deallocate(new_node, 1);
            throw;
        }
        top_node = new_node;
        size_++;
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        Node* new_node = allocator_.allocate(1);
        try {
            allocator_.construct(new_node, top_node, std::forward<Args>(args)...);
        } catch (...) {
            allocator_.deallocate(new_node, 1);
            throw;
        }
        top_node = new_node;
        size_++;
    }
    
    void pop() {
        if (!top_node) throw std::out_of_range("Stack empty");
        
        Node* old_top = top_node;
        top_node = top_node->next;
        
        allocator_.destroy(old_top);
        allocator_.deallocate(old_top, 1);
        
        size_--;
    }
    
    T& top() {
        if (!top_node) throw std::out_of_range("Stack empty");
        return top_node->data;
    }
    
    const T& top() const {
        if (!top_node) throw std::out_of_range("Stack empty");
        return top_node->data;
    }
    
    bool empty() const { return top_node == nullptr; }
    size_t size() const { return size_; }
    
    void clear() {
        while (!empty()) {
            pop();
        }
    }
    
    Iterator begin() { return Iterator(top_node); }
    Iterator end() { return Iterator(nullptr); }
    
    std::pmr::memory_resource* get_memory_resource() const {
        return allocator_.resource();
    }
};

#endif