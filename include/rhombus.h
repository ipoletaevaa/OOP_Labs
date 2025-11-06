#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "figure.h"
#include <cmath>

template<class T>
class Rhombus : public Figure<T> {
private:
    Point<T> points[4];

public:
    Rhombus(T centerX, T centerY, T diag1, T diag2) {
        points[0] = Point<T>(centerX - diag1/2, centerY);
        points[1] = Point<T>(centerX, centerY + diag2/2);
        points[2] = Point<T>(centerX + diag1/2, centerY);
        points[3] = Point<T>(centerX, centerY - diag2/2);
    }

    double area() const override {
        T diag1 = points[2].getX() - points[0].getX();
        T diag2 = points[1].getY() - points[3].getY();
        return (diag1 * diag2) / 2.0;
    }

    Point<T> center() const override {
        T centerX = (points[0].getX() + points[2].getX()) / 2;
        T centerY = (points[1].getY() + points[3].getY()) / 2;
        return Point<T>(centerX, centerY);
    }

    std::unique_ptr<Point<T>[]> vertices() const override {
        auto vert = std::make_unique<Point<T>[]>(4);
        for(int i = 0; i < 4; i++) {
            vert[i] = points[i];
        }
        return vert;
    }

    void print(std::ostream& os) const override {
        os << "Ромб [";
        for(int i = 0; i < 4; i++) {
            os << points[i];
            if(i < 3) os << ", ";
        }
        os << "] Центр: " << center() << " Площадь: " << area();
    }

    Rhombus(const Rhombus& other) {
        for(int i = 0; i < 4; i++) {
            points[i] = other.points[i];
        }
    }

    Rhombus& operator=(const Rhombus& other) {
        if(this != &other) {
            for(int i = 0; i < 4; i++) {
                points[i] = other.points[i];
            }
        }
        return *this;
    }
};

#endif