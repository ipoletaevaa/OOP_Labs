#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <map>
#include <functional>
#include "npc_types.h"

class NPCFactory {
public:
    virtual ~NPCFactory() = default;
    virtual std::shared_ptr<NPC> createNPC(const std::string& name, double x, double y) = 0;
};

class WarriorFactory : public NPCFactory {
public:
    std::shared_ptr<NPC> createNPC(const std::string& name, double x, double y) override;
};

class MageFactory : public NPCFactory {
public:
    std::shared_ptr<NPC> createNPC(const std::string& name, double x, double y) override;
};

class ArcherFactory : public NPCFactory {
public:
    std::shared_ptr<NPC> createNPC(const std::string& name, double x, double y) override;
};

class NPCRepository {
private:
    std::map<std::string, std::function<std::shared_ptr<NPC>(const std::string&, double, double)>> creators;
    static NPCRepository* instance;
    
    NPCRepository();
    
public:
    static NPCRepository* getInstance();
    static void resetInstance();
    
    void registerCreator(const std::string& type, 
                         std::function<std::shared_ptr<NPC>(const std::string&, double, double)> creator);
    
    std::shared_ptr<NPC> createNPC(const std::string& type, 
                                   const std::string& name, 
                                   double x, double y);
    
    void saveToFile(const std::string& filename, const std::vector<std::shared_ptr<NPC>>& npcs);
    std::vector<std::shared_ptr<NPC>> loadFromFile(const std::string& filename);
};

#endif