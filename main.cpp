#include <iostream>
#include "include/block_memory_resource.hpp"
#include "include/my_stack.hpp"
#include "include/student_struct.hpp"

void test_int_stack() {
    std::cout << "\n=== Testing int stack ===\n";
    
    BlockMemoryResource my_mr;
    MyStack<int> int_stack(&my_mr);
    
    for (int i = 1; i <= 5; i++) {
        int_stack.push(i * 10);
        std::cout << "Pushed: " << i * 10 << "\n";
    }
    
    std::cout << "\nStack size: " << int_stack.size() << "\n";
    std::cout << "Top: " << int_stack.top() << "\n";
    
    std::cout << "\nUsing iterator:\n";
    for (auto it = int_stack.begin(); it != int_stack.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    std::cout << "\nUsing range-based for:\n";
    for (const auto& val : int_stack) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    std::cout << "\nPopping all:\n";
    while (!int_stack.empty()) {
        std::cout << int_stack.top() << " ";
        int_stack.pop();
    }
    std::cout << "\n";
    
    my_mr.print_stats();
}

void test_student_stack() {
    std::cout << "\n=== Testing StudentData stack ===\n";
    
    BlockMemoryResource student_mr;
    MyStack<StudentData> student_stack(&student_mr);
    
    student_stack.emplace(1, 4.5, "Ivan", 20);
    student_stack.emplace(2, 4.8, "Maria", 21);
    student_stack.emplace(3, 3.9, "Alex", 22);
    
    std::cout << "\nStack contents:\n";
    for (const auto& student : student_stack) {
        std::cout << student << "\n";
    }
    
    std::cout << "\nTop student: " << student_stack.top() << "\n";
    
    student_mr.print_stats();
}

int main() {
    std::cout << "Lab 05: Iterators and Allocators\n";
    
    test_int_stack();
    test_student_stack();
    
    std::cout << "\n=== All tests completed ===\n";
    return 0;
}