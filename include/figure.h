#ifndef FIGURE_H
#define FIGURE_H

#include "point.h"
#include <memory>

template<class T>
class Figure {
public:
    virtual ~Figure() = default;
    
    virtual double area() const = 0;
    virtual Point<T> center() const = 0;
    virtual std::unique_ptr<Point<T>[]> vertices() const = 0;
    virtual void print(std::ostream& os) const = 0;
    
    operator double() const {
        return area();
    }
    
    bool operator==(const Figure& other) const {
        return area() == other.area();
    }
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
    fig.print(os);
    return os;
}

#endif