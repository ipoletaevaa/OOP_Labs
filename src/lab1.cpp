#include <iostream>
#include "lab1_functions.h"

int main() {
    int upper_limit;
    
    std::cout << "Введите верхнюю границу > 2: ";
    std::cin >> upper_limit;
    
    if (upper_limit <= 2) {
        std::cout << "Введите число больше 2" << std::endl;
        return 1;
    }
    
    auto result = closest_pair_tonum(upper_limit);
    
    if (result.first == 0 && result.second == 0) {
        std::cout << "Пара не найдена для upper_limit = " << upper_limit << std::endl;
    } else {
        std::cout << "Ближайшая пара: (" << result.first << ", " << result.second << ")" << std::endl;
    }
    
    return 0;
}