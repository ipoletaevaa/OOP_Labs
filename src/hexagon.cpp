#include "hexagon.h"
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <vector>

const double PI = 3.14159265358979323846;

Hexagon::Hexagon() : x_center(0), y_center(0), side_length(1) {}

Hexagon::Hexagon(double x, double y, double side) 
    : x_center(x), y_center(y), side_length(side) {
    if (side <= 0) throw std::invalid_argument("Длина стороны должна быть положительной");
}

Hexagon::Hexagon(const Hexagon& other)
    : x_center(other.x_center), y_center(other.y_center), side_length(other.side_length) {}

Hexagon::Hexagon(Hexagon&& other) noexcept
    : x_center(std::move(other.x_center)), 
      y_center(std::move(other.y_center)), 
      side_length(std::move(other.side_length)) {}

Hexagon& Hexagon::operator=(const Hexagon& other) {
    if (this != &other) {
        x_center = other.x_center;
        y_center = other.y_center;
        side_length = other.side_length;
    }
    return *this;
}

Hexagon& Hexagon::operator=(Hexagon&& other) noexcept {
    if (this != &other) {
        x_center = std::move(other.x_center);
        y_center = std::move(other.y_center);
        side_length = std::move(other.side_length);
    }
    return *this;
}

bool Hexagon::operator==(const Hexagon& other) const {
    return (x_center == other.x_center) && 
           (y_center == other.y_center) && 
           (side_length == other.side_length);
}

std::vector<std::pair<double, double>> Hexagon::calculate_vertices() const {
    std::vector<std::pair<double, double>> vertices;
    
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * PI * i / 6;
        double radius = side_length;
        double x = x_center + radius * cos(angle);
        double y = y_center + radius * sin(angle);
        vertices.push_back({x, y});
    }
    
    return vertices;
}

std::pair<double, double> Hexagon::center() const {
    auto vertices = calculate_vertices();
    double sum_x = 0, sum_y = 0;
    
    for (const auto& vertex : vertices) {
        sum_x += vertex.first;
        sum_y += vertex.second;
    }
    
    return {sum_x / vertices.size(), sum_y / vertices.size()};
}

double Hexagon::area() const {
    return (3 * sqrt(3) * side_length * side_length) / 2;
}

void Hexagon::print(std::ostream& os) const {
    auto vertices = calculate_vertices();
    os << "Шестиугольник (6 вершин):\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "Вершина " << i + 1 << ": (" 
           << std::fixed << std::setprecision(2) 
           << vertices[i].first << ", " 
           << vertices[i].second << ")\n";
    }
}

void Hexagon::read(std::istream& is) {
    std::cout << "Введите координаты начальной вершины (x y): ";
    is >> x_center >> y_center;
    std::cout << "Введите длину стороны: ";
    is >> side_length;
    
    if (side_length <= 0) {
        throw std::invalid_argument("Длина стороны должна быть положительной");
    }
}

Hexagon::operator double() const {
    return area();
}