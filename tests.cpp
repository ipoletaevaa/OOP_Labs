#include <cassert>
#include <cmath>
#include <iostream>
#include "include/point.h"
#include "include/rhombus.h"
#include "include/trapezoid.h"
#include "include/array.h"

void testPoint() {
    Point<int> p1(3, 4);
    assert(p1.getX() == 3);
    assert(p1.getY() == 4);
    
    Point<double> p2(1.5, 2.5);
    assert(p2.getX() == 1.5);
    assert(p2.getY() == 2.5);
    
    std::cout << "Тесты Point пройдены" << std::endl;
}

void testRhombus() {
    Rhombus<int> rhomb(0, 0, 6, 8);
    assert(std::abs(rhomb.area() - 24.0) < 0.001);
    
    auto center = rhomb.center();
    assert(center.getX() == 0);
    assert(center.getY() == 0);
    
    auto vertices = rhomb.vertices();
    assert(vertices[0].getX() == -3);
    assert(vertices[1].getY() == 4);
    
    std::cout << "Тесты Rhombus пройдены" << std::endl;
}

void testTrapezoid() {
    Trapezoid<int> trap(0, 0, 4, 8, 6);
    assert(std::abs(trap.area() - 36.0) < 0.001);
    
    auto center = trap.center();
    assert(center.getX() == 0);
    assert(center.getY() == 0);
    
    std::cout << "Тесты Trapezoid пройдены" << std::endl;
}

void testArray() {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    
    assert(arr.size() == 3);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
    
    arr.remove(1);
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 3);
    
    std::cout << "Тесты Array пройдены" << std::endl;
}

void testFigureArray() {
    Array<std::shared_ptr<Figure<int>>> figures;
    
    figures.push_back(std::make_shared<Rhombus<int>>(0, 0, 4, 6));
    figures.push_back(std::make_shared<Trapezoid<int>>(0, 0, 3, 5, 4));
    
    assert(figures.size() == 2);
    assert(figures.totalArea() > 0);
    
    std::cout << "Тесты Figure array пройдены" << std::endl;
}

int main() {
    testPoint();
    testRhombus();
    testTrapezoid();
    testArray();
    testFigureArray();
    
    std::cout << "Все тесты пройдены успешно!" << std::endl;
    return 0;
}