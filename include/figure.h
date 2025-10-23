#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include <vector>
#include <cmath>

class Figure{
public:
    virtual ~Figure() = default;

    virtual std::pair<double, double> center() const = 0;

    virtual double area() const = 0;

    virtual void print(std::ostream& os) const = 0;

    virtual void read(std::istream& is) = 0;

    virtual operator double() const = 0;   
};

std::ostream& operator<<(std::ostream& os, const Figure& figure);
std::istream& operator>>(std::istream& is, Figure& figure);


#endif