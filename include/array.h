#ifndef ARRAY_H
#define ARRAY_H

#include <memory>
#include <stdexcept>
#include <type_traits>

template<class T>
class Array {
private:
    std::shared_ptr<T[]> data;
    int _capacity;
    int _size;

    void resize(int new_capacity) {
        auto new_data = std::shared_ptr<T[]>(new T[new_capacity]);
        
        for(int i = 0; i < _size; i++) {
            new_data[i] = std::move(data[i]);
        }
        
        data = new_data;
        _capacity = new_capacity;
    }

public:
    Array() : data(nullptr), _capacity(0), _size(0) {}
    
    Array(int capacity) : data(new T[capacity]), _capacity(capacity), _size(0) {}

    void push_back(const T& value) {
        if(_size >= _capacity) {
            int new_capacity = _capacity == 0 ? 1 : _capacity * 2;
            resize(new_capacity);
        }
        data[_size] = value;
        _size++;
    }

    void push_back(T&& value) {
        if(_size >= _capacity) {
            int new_capacity = _capacity == 0 ? 1 : _capacity * 2;
            resize(new_capacity);
        }
        data[_size] = std::move(value);
        _size++;
    }

    void remove(int index) {
        if(index < 0 || index >= _size) {
            throw std::out_of_range("Неверный индекс");
        }
        
        for(int i = index; i < _size - 1; i++) {
            data[i] = std::move(data[i + 1]);
        }
        _size--;
    }

    T& operator[](int index) {
        if(index < 0 || index >= _size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if(index < 0 || index >= _size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];
    }

    int size() const { return _size; }
    int capacity() const { return _capacity; }

    double totalArea() const {
        double total = 0;
        for(int i = 0; i < _size; i++) {
            total += static_cast<double>(*data[i]);
        }
        return total;
    }
};

#endif