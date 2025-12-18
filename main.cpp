#include "include/game.h"
#include <iostream>
#include <filesystem>

int main() {
    try {
        std::cout << "Лабораторная работа №7: Многопоточная игра NPC" << std::endl;
        std::cout << "===============================================" << std::endl;
        
        std::filesystem::create_directories("logs");
        
        Game game;
        game.run();
        
        std::cout << "\nИгра завершена. Логи сохранены в logs/game_log.txt" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}