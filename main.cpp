#include "../include/hexagon.h"
#include "../include/octagon.h"
#include "../include/triangle.h"
#include "../include/array.h"
#include <iostream>
#include <memory>
#include <limits>

void print_menu() {
    std::cout << "\n=== Менеджер фигур ===\n";
    std::cout << "1. Добавить шестиугольник (6 сторон)\n";
    std::cout << "2. Добавить восьмиугольник (8 сторон)\n";
    std::cout << "3. Добавить треугольник (3 стороны)\n";
    std::cout << "4. Удалить фигуру по индексу\n";
    std::cout << "5. Показать все фигуры\n";
    std::cout << "6. Вычислить общую площадь\n";
    std::cout << "7. Очистить массив\n";
    std::cout << "8. Выход\n";
    std::cout << "Выберите опцию: ";
}

void add_figure_menu(Array& array, int figure_type) {
    try {
        switch (figure_type) {
            case 1: {
                auto hexagon = std::make_shared<Hexagon>();
                std::cin >> *hexagon;  // Используем оператор >> для ввода
                array.add_figure(hexagon);
                std::cout << "Шестиугольник успешно добавлен!\n";
                break;
            }
            case 2: {
                auto octagon = std::make_shared<Octagon>();
                std::cin >> *octagon;
                array.add_figure(octagon);
                std::cout << "Восьмиугольник успешно добавлен!\n";
                break;
            }
            case 3: {
                auto triangle = std::make_shared<Triangle>();
                std::cin >> *triangle;
                array.add_figure(triangle);
                std::cout << "Треугольник успешно добавлен!\n";
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

int main() {
    Array array;
    int choice;
    
    while (true) {
        print_menu();
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1:
                case 2:
                case 3:
                    add_figure_menu(array, choice);
                    break;
                    
                case 4: {
                    int index;
                    std::cout << "Введите индекс для удаления: ";
                    std::cin >> index;
                    array.remove_figure(index - 1);
                    std::cout << "Фигура успешно удалена!\n";
                    break;
                }
                
                case 5:
                    array.print_all();
                    break;
                    
                case 6:
                    std::cout << "Общая площадь: " << array.total_area() << "\n";
                    break;
                    
                case 7:
                    array.clear();
                    std::cout << "Массив очищен!\n";
                    break;
                    
                case 8:
                    std::cout << "До свидания!\n";
                    return 0;
                    
                default:
                    std::cout << "Неверная опция!\n";
                    break;
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    return 0;
}