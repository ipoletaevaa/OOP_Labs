#ifndef OCTAGON_H
#define OCTAGON_H

#include "figure.h"
#include <vector>

class Octagon : public Figure {
private:
    double x_center, y_center;
    double side_length;
    
    std::vector<std::pair<double, double>> calculate_vertices() const;

public:
    Octagon();
    Octagon(double x, double y, double side);
    Octagon(const Octagon& other);
    Octagon(Octagon&& other) noexcept;
    Octagon& operator=(const Octagon& other);
    Octagon& operator=(Octagon&& other) noexcept;
    bool operator==(const Octagon& other) const;
    
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