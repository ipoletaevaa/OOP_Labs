#ifndef STUDENT_STRUCT_HPP
#define STUDENT_STRUCT_HPP

#include <iostream>
#include <string>

struct StudentData {
    int id;
    double average_score;
    std::string name;
    int age;
    
    StudentData() : id(0), average_score(0.0), name(""), age(0) {}
    StudentData(int i, double score, std::string n, int a) 
        : id(i), average_score(score), name(n), age(a) {}
    
    friend std::ostream& operator<<(std::ostream& os, const StudentData& data) {
        os << "Student{id=" << data.id 
           << ", name='" << data.name 
           << "', age=" << data.age 
           << ", score=" << data.average_score << "}";
        return os;
    }
};

#endif