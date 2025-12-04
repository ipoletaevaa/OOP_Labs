#include "include/factory.h"
#include "include/visitor.h"
#include "include/observer.h"
#include <iostream>
#include <memory>
#include <vector>

void printMenu() {
    std::cout << "\n=== РЕДАКТОР ПОДЗЕМЕЛЬЯ ===" << std::endl;
    std::cout << "1. Добавить NPC" << std::endl;
    std::cout << "2. Показать всех NPC" << std::endl;
    std::cout << "3. Сохранить в файл" << std::endl;
    std::cout << "4. Загрузить из файла" << std::endl;
    std::cout << "5. Начать бой" << std::endl;
    std::cout << "6. Выход" << std::endl;
    std::cout << "Выберите действие: ";
}

void addNPC(std::vector<std::shared_ptr<NPC>>& npcs) {
    std::cout << "\n--- Добавление NPC ---" << std::endl;
    std::cout << "Выберите тип:\n1. Воин\n2. Маг\n3. Лучник" << std::endl;
    std::cout << "Ваш выбор: ";
    
    int typeChoice;
    std::cin >> typeChoice;
    
    std::string type;
    switch(typeChoice) {
        case 1: type = "warrior"; break;
        case 2: type = "mage"; break;
        case 3: type = "archer"; break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
            return;
    }
    
    std::string name;
    double x, y;
    
    std::cout << "Введите имя NPC: ";
    std::cin >> name;
    std::cout << "Введите координату X (0-500): ";
    std::cin >> x;
    std::cout << "Введите координату Y (0-500): ";
    std::cin >> y;
    
    try {
        auto repository = NPCRepository::getInstance();
        auto npc = repository->createNPC(type, name, x, y);
        npcs.push_back(npc);
        std::cout << "NPC успешно создан!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void showNPCs(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::cout << "\n--- Список NPC ---" << std::endl;
    if (npcs.empty()) {
        std::cout << "Список пуст." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < npcs.size(); i++) {
        std::cout << i + 1 << ". ";
        npcs[i]->print();
    }
}

int main() {
    std::vector<std::shared_ptr<NPC>> npcs;
    auto repository = NPCRepository::getInstance();
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("log.txt");
    
    auto notifier = std::make_shared<KillNotifier>();
    notifier->addObserver(consoleObserver);
    notifier->addObserver(fileObserver);
    
    int choice;
    
    do {
        printMenu();
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                addNPC(npcs);
                break;
                
            case 2:
                showNPCs(npcs);
                break;
                
            case 3: {
                std::string filename;
                std::cout << "Введите имя файла для сохранения: ";
                std::cin >> filename;
                repository->saveToFile(filename, npcs);
                break;
            }
                
            case 4: {
                std::string filename;
                std::cout << "Введите имя файла для загрузки: ";
                std::cin >> filename;
                npcs = repository->loadFromFile(filename);
                break;
            }
                
            case 5: {
                if (npcs.size() < 2) {
                    std::cout << "Для боя нужно как минимум 2 NPC!" << std::endl;
                    break;
                }
                
                double range;
                std::cout << "Введите дальность атаки (в метрах): ";
                std::cin >> range;
                
                BattleVisitor battle(range, notifier);
                battle.visit(npcs);
                break;
            }
                
            case 6:
                std::cout << "Выход из программы..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор!" << std::endl;
        }
        
    } while (choice != 6);
    
    NPCRepository::resetInstance();
    
    return 0;
}