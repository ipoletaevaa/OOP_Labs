#include "../include/factory.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::shared_ptr<NPC> WarriorFactory::createNPC(const std::string& name, double x, double y) {
    return std::make_shared<Warrior>(name, x, y);
}

std::shared_ptr<NPC> MageFactory::createNPC(const std::string& name, double x, double y) {
    return std::make_shared<Mage>(name, x, y);
}

std::shared_ptr<NPC> ArcherFactory::createNPC(const std::string& name, double x, double y) {
    return std::make_shared<Archer>(name, x, y);
}

NPCRepository* NPCRepository::instance = nullptr;

NPCRepository::NPCRepository() {
    registerCreator("warrior", [](const std::string& name, double x, double y) {
        return std::make_shared<Warrior>(name, x, y);
    });
    registerCreator("mage", [](const std::string& name, double x, double y) {
        return std::make_shared<Mage>(name, x, y);
    });
    registerCreator("archer", [](const std::string& name, double x, double y) {
        return std::make_shared<Archer>(name, x, y);
    });
}

NPCRepository* NPCRepository::getInstance() {
    if (!instance) {
        instance = new NPCRepository();
    }
    return instance;
}

void NPCRepository::resetInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void NPCRepository::registerCreator(const std::string& type, 
                                   std::function<std::shared_ptr<NPC>(const std::string&, double, double)> creator) {
    creators[type] = creator;
}

std::shared_ptr<NPC> NPCRepository::createNPC(const std::string& type, 
                                              const std::string& name, 
                                              double x, double y) {
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(name, x, y);
    }
    throw std::invalid_argument("Неизвестный тип NPC: " + type);
}

void NPCRepository::saveToFile(const std::string& filename, const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::string type;
            switch(npc->getType()) {
                case NPCType::WARRIOR: type = "warrior"; break;
                case NPCType::MAGE: type = "mage"; break;
                case NPCType::ARCHER: type = "archer"; break;
            }
            file << type << " " << npc->getName() << " " 
                 << npc->getX() << " " << npc->getY() << std::endl;
        }
    }
    file.close();
    std::cout << "Сохранено " << npcs.size() << " NPC в файл " << filename << std::endl;
}

std::vector<std::shared_ptr<NPC>> NPCRepository::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<NPC>> npcs;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл. Будет создан новый список NPC." << std::endl;
        return npcs;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type, name;
        double x, y;
        
        if (iss >> type >> name >> x >> y) {
            try {
                auto npc = createNPC(type, name, x, y);
                npcs.push_back(npc);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при создании NPC: " << e.what() << std::endl;
            }
        }
    }
    file.close();
    std::cout << "Загружено " << npcs.size() << " NPC из файла " << filename << std::endl;
    return npcs;
}