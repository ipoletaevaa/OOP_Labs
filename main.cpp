#include <iostream>
#include <memory>
#include "include/point.h"
#include "include/figure.h"
#include "include/rhombus.h"
#include "include/trapezoid.h"
#include "include/array.h"

void showMenu() {
    std::cout << "\n=== Меню управления фигурами ===" << std::endl;
    std::cout << "1. Добавить ромб" << std::endl;
    std::cout << "2. Добавить трапецию" << std::endl;
    std::cout << "3. Показать все фигуры" << std::endl;
    std::cout << "4. Удалить фигуру по индексу" << std::endl;
    std::cout << "5. Общая площадь всех фигур" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите действие: ";
}

void addRhombus(Array<std::shared_ptr<Figure<int>>>& figures) {
    int centerX, centerY, diag1, diag2;
    
    std::cout << "Введите координату X центра: ";
    std::cin >> centerX;
    std::cout << "Введите координату Y центра: ";
    std::cin >> centerY;
    std::cout << "Введите длину первой диагонали: ";
    std::cin >> diag1;
    std::cout << "Введите длину второй диагонали: ";
    std::cin >> diag2;
    
    auto rhomb = std::make_shared<Rhombus<int>>(centerX, centerY, diag1, diag2);
    figures.push_back(rhomb);
    std::cout << "Ромб добавлен!" << std::endl;
}

void addTrapezoid(Array<std::shared_ptr<Figure<int>>>& figures) {
    int centerX, centerY, topBase, bottomBase, height;
    
    std::cout << "Введите координату X центра: ";
    std::cin >> centerX;
    std::cout << "Введите координату Y центра: ";
    std::cin >> centerY;
    std::cout << "Введите длину верхнего основания: ";
    std::cin >> topBase;
    std::cout << "Введите длину нижнего основания: ";
    std::cin >> bottomBase;
    std::cout << "Введите высоту: ";
    std::cin >> height;
    
    auto trap = std::make_shared<Trapezoid<int>>(centerX, centerY, topBase, bottomBase, height);
    figures.push_back(trap);
    std::cout << "Трапеция добавлена!" << std::endl;
}

void showAllFigures(Array<std::shared_ptr<Figure<int>>>& figures) {
    if(figures.size() == 0) {
        std::cout << "Список фигур пуст" << std::endl;
        return;
    }
    
    std::cout << "\n=== Список всех фигур ===" << std::endl;
    for(int i = 0; i < figures.size(); i++) {
        std::cout << i << ": " << *figures[i] << std::endl;
        
        auto vert = figures[i]->vertices();
        std::cout << "   Вершины: ";
        for(int j = 0; j < 4; j++) {
            std::cout << vert[j] << " ";
        }
        std::cout << std::endl;
    }
}

void removeFigure(Array<std::shared_ptr<Figure<int>>>& figures) {
    if(figures.size() == 0) {
        std::cout << "Список фигур пуст" << std::endl;
        return;
    }
    
    int index;
    std::cout << "Введите индекс фигуры для удаления (0-" << figures.size()-1 << "): ";
    std::cin >> index;
    
    try {
        figures.remove(index);
        std::cout << "Фигура удалена!" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Ошибка: неверный индекс!" << std::endl;
    }
}

void showTotalArea(Array<std::shared_ptr<Figure<int>>>& figures) {
    if(figures.size() == 0) {
        std::cout << "Список фигур пуст" << std::endl;
        return;
    }
    
    std::cout << "Общая площадь всех фигур: " << figures.totalArea() << std::endl;
}

int main() {
    Array<std::shared_ptr<Figure<int>>> figures;
    int choice;
    
    std::cout << "=== Программа для работы с геометрическими фигурами ===" << std::endl;
    
    do {
        showMenu();
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                addRhombus(figures);
                break;
            case 2:
                addTrapezoid(figures);
                break;
            case 3:
                showAllFigures(figures);
                break;
            case 4:
                removeFigure(figures);
                break;
            case 5:
                showTotalArea(figures);
                break;
            case 0:
                std::cout << "Выход из программы..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор!" << std::endl;
        }
    } while(choice != 0);
    
    return 0;
}