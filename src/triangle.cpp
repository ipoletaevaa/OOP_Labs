#include "triangle.h"
#include <cmath>
#include <iomanip>
#include <stdexcept>

const double PI = 3.14159265358979323846;

Triangle::Triangle() : x_center(0), y_center(0), side_length(1) {}

Triangle::Triangle(double x, double y, double side) 
    : x_center(x), y_center(y), side_length(side) {
    if (side <= 0) throw std::invalid_argument("Side length must be positive");
}

Triangle::Triangle(const Triangle& other)
    : x_center(other.x_center), y_center(other.y_center), side_length(other.side_length) {}

Triangle::Triangle(Triangle&& other) noexcept
    : x_center(std::move(other.x_center)), 
      y_center(std::move(other.y_center)), 
      side_length(std::move(other.side_length)) {}

Triangle& Triangle::operator=(const Triangle& other) {
    if (this != &other) {
        x_center = other.x_center;
        y_center = other.y_center;
        side_length = other.side_length;
    }
    return *this;
}

Triangle& Triangle::operator=(Triangle&& other) noexcept {
    if (this != &other) {
        x_center = std::move(other.x_center);
        y_center = std::move(other.y_center);
        side_length = std::move(other.side_length);
    }
    return *this;
}

bool Triangle::operator==(const Triangle& other) const {
    return (x_center == other.x_center) && 
           (y_center == other.y_center) && 
           (side_length == other.side_length);
}

std::vector<std::pair<double, double>> Triangle::calculate_vertices() const {
    std::vector<std::pair<double, double>> vertices;
    double radius = side_length / sqrt(3);
    
    for (int i = 0; i < 3; ++i) {
        double angle = 2 * PI * i / 3 - PI / 2; 
        double x = x_center + radius * cos(angle);
        double y = y_center + radius * sin(angle);
        vertices.push_back({x, y});
    }
    return vertices;
}

std::pair<double, double> Triangle::center() const {
    return {x_center, y_center};
}

double Triangle::area() const {
    return (sqrt(3) * side_length * side_length) / 4;
}

void Triangle::print(std::ostream& os) const {
    auto vertices = calculate_vertices();
    os << "Triangle (3 vertices):\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "Vertex " << i + 1 << ": (" 
           << std::fixed << std::setprecision(2) << vertices[i].first << ", " 
           << vertices[i].second << ")\n";
    }
}

void Triangle::read(std::istream& is) {
    is >> x_center >> y_center >> side_length;
    if (side_length <= 0) throw std::invalid_argument("Side length must be positive");
}

Triangle::operator double() const {
    return area();
}