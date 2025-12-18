#include "factory.h"
#include "npc_types.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

NPCRepository* NPCRepository::instance = nullptr;

NPCRepository::NPCRepository() {
    registerCreator("orc", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::ORC, name, x, y);
    });
    
    registerCreator("squirrel", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::SQUIRREL, name, x, y);
    });
    
    registerCreator("druid", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::DRUID, name, x, y);
    });
    
    registerCreator("knight", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::KNIGHT, name, x, y);
    });
    
    registerCreator("elf", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::ELF, name, x, y);
    });
    
    registerCreator("dragon", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::DRAGON, name, x, y);
    });
    
    registerCreator("bear", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::BEAR, name, x, y);
    });
    
    registerCreator("bandit", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::BANDIT, name, x, y);
    });
    
    registerCreator("werewolf", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::WEREWOLF, name, x, y);
    });
    
    registerCreator("princess", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::PRINCESS, name, x, y);
    });
    
    registerCreator("toad", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::TOAD, name, x, y);
    });
    
    registerCreator("slave_trader", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::SLAVE_TRADER, name, x, y);
    });
    
    registerCreator("pegasus", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::PEGASUS, name, x, y);
    });
    
    registerCreator("bittern", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::BITTERN, name, x, y);
    });
    
    registerCreator("desman", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::DESMAN, name, x, y);
    });
    
    registerCreator("bull", [](const std::string& name, double x, double y) {
        return std::make_shared<NPC>(NPCType::BULL, name, x, y);
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
    std::filesystem::path filepath = filename;
    if (filepath.has_parent_path()) {
        std::filesystem::create_directories(filepath.parent_path());
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::string type;
            switch(npc->getType()) {
                case NPCType::ORC: type = "orc"; break;
                case NPCType::SQUIRREL: type = "squirrel"; break;
                case NPCType::DRUID: type = "druid"; break;
                case NPCType::KNIGHT: type = "knight"; break;
                case NPCType::ELF: type = "elf"; break;
                case NPCType::DRAGON: type = "dragon"; break;
                case NPCType::BEAR: type = "bear"; break;
                case NPCType::BANDIT: type = "bandit"; break;
                case NPCType::WEREWOLF: type = "werewolf"; break;
                case NPCType::PRINCESS: type = "princess"; break;
                case NPCType::TOAD: type = "toad"; break;
                case NPCType::SLAVE_TRADER: type = "slave_trader"; break;
                case NPCType::PEGASUS: type = "pegasus"; break;
                case NPCType::BITTERN: type = "bittern"; break;
                case NPCType::DESMAN: type = "desman"; break;
                case NPCType::BULL: type = "bull"; break;
            }
            auto pos = npc->getPosition();
            file << type << " " << npc->getName() << " " 
                 << pos.first << " " << pos.second << std::endl;
        }
    }
    file.close();
}

std::vector<std::shared_ptr<NPC>> NPCRepository::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<NPC>> npcs;
    
    if (!std::filesystem::exists(filename)) {
        return npcs;
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
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
            }
        }
    }
    file.close();
    return npcs;
}