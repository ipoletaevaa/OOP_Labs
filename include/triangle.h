#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "figure.h"
#include <vector>

class Triangle : public Figure {
private:
    double x_center, y_center;
    double side_length;
    
    std::vector<std::pair<double, double>> calculate_vertices() const;

public:
    Triangle();
    Triangle(double x, double y, double side);
    Triangle(const Triangle& other);
    Triangle(Triangle&& other) noexcept;
    Triangle& operator=(const Triangle& other);
    Triangle& operator=(Triangle&& other) noexcept;
    bool operator==(const Triangle& other) const;
    
    std::pair<double, double> center() const override;
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    operator double() const override;
    
    double get_x() const { return x_center; }
    double get_y() const { return y_center; }
    double get_side() const { return side_length; }
};

#endif