#ifndef POINT_H
#define POINT_H

#include <iostream>

template<class T>
class Point {
private:
    T x, y;

public:
    Point(T x = 0, T y = 0) : x(x), y(y) {}

    T getX() const { return x; }
    T getY() const { return y; }
    
    void setX(T x) { this->x = x; }
    void setY(T y) { this->y = y; }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

#endif