#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "figure.h"

template<class T>
class Trapezoid : public Figure<T> {
private:
    Point<T> points[4];

public:
    Trapezoid(T centerX, T centerY, T topBase, T bottomBase, T height) {
        T topY = centerY + height/2;
        T bottomY = centerY - height/2;
        
        points[0] = Point<T>(centerX - bottomBase/2, bottomY);
        points[1] = Point<T>(centerX - topBase/2, topY);
        points[2] = Point<T>(centerX + topBase/2, topY);
        points[3] = Point<T>(centerX + bottomBase/2, bottomY);
    }

    double area() const override {
        T topBase = points[2].getX() - points[1].getX();
        T bottomBase = points[3].getX() - points[0].getX();
        T height = points[1].getY() - points[0].getY();
        return (topBase + bottomBase) * height / 2.0;
    }

    Point<T> center() const override {
        T centerX = (points[0].getX() + points[1].getX() + points[2].getX() + points[3].getX()) / 4;
        T centerY = (points[0].getY() + points[1].getY() + points[2].getY() + points[3].getY()) / 4;
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
        os << "Трапеция [";
        for(int i = 0; i < 4; i++) {
            os << points[i];
            if(i < 3) os << ", ";
        }
        os << "] Центр: " << center() << " Площадь: " << area();
    }

    Trapezoid(const Trapezoid& other) {
        for(int i = 0; i < 4; i++) {
            points[i] = other.points[i];
        }
    }

    Trapezoid& operator=(const Trapezoid& other) {
        if(this != &other) {
            for(int i = 0; i < 4; i++) {
                points[i] = other.points[i];
            }
        }
        return *this;
    }
};

#endif