#ifndef HEXAGON_H
#define HEXAGON_H

#include "figure.h"
#include <vector>

class Hexagon : public Figure {
private:
    double x_center, y_center;
    double side_length;
    std::vector<std::pair<double, double>> calculate_vertices() const;

public:
    Hexagon();
    Hexagon(double x, double y, double side);
    Hexagon(const Hexagon& other);
    Hexagon(Hexagon&& other) noexcept;
    Hexagon& operator=(const Hexagon& other);
    Hexagon& operator=(Hexagon&& other) noexcept;
    bool operator==(const Hexagon& other) const;
    
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