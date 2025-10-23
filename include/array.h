#ifndef ARRAY_H
#define ARRAY_H

#include "figure.h"
#include <memory>
#include <vector>

class Array {
private:
    std::vector<std::shared_ptr<Figure>> figures;

public:
    void add_figure(std::shared_ptr<Figure> figure);
    void remove_figure(int index);
    std::shared_ptr<Figure> get_figure(int index) const;
    size_t size() const;
    void print_all() const;
    double total_area() const;
    void clear();
};

#endif