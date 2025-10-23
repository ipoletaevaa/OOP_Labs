#include "array.h"
#include <iostream>
#include <stdexcept>

void Array::add_figure(std::shared_ptr<Figure> figure) {
    figures.push_back(figure);
}

void Array::remove_figure(int index) {
    if (index < 0 || index >= static_cast<int>(figures.size())) {
        throw std::out_of_range("Index out of range");
    }
    figures.erase(figures.begin() + index);
}

std::shared_ptr<Figure> Array::get_figure(int index) const {
    if (index < 0 || index >= static_cast<int>(figures.size())) {
        throw std::out_of_range("Index out of range");
    }
    return figures[index];
}

size_t Array::size() const {
    return figures.size();
}

void Array::print_all() const {
    std::cout << "Фигуры в массиве:\n";
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Фигура " << i + 1 << ":\n";
        auto center = figures[i]->center();
        std::cout << "Центр: (" << center.first << ", " 
                  << center.second << ")\n";
        std::cout << "Площадь: " << static_cast<double>(*figures[i]) << "\n";
        std::cout << *figures[i] << "\n";
    }
}

double Array::total_area() const {
    double total = 0;
    for (const auto& figure : figures) {
        total += static_cast<double>(*figure);
    }
    return total;
}

void Array::clear() {
    figures.clear();
}