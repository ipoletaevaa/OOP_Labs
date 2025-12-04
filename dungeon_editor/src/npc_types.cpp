#include "npc_types.h"
#include <iostream>

NPC::NPC(NPCType t, const std::string& n, double xPos, double yPos) 
    : type(t), name(n), x(xPos), y(yPos), alive(true) {
    if (xPos <= 0 || xPos >= 500 || yPos <= 0 || yPos >= 500) {
        throw std::invalid_argument("Координаты должны быть в диапазоне (0, 500)");
    }
}

NPCType NPC::getType() const { return type; }
std::string NPC::getName() const { return name; }
double NPC::getX() const { return x; }
double NPC::getY() const { return y; }
bool NPC::isAlive() const { return alive; }
void NPC::kill() { alive = false; }

void NPC::print() const {
    std::string typeStr;
    switch(type) {
        case NPCType::WARRIOR: typeStr = "Воин"; break;
        case NPCType::MAGE: typeStr = "Маг"; break;
        case NPCType::ARCHER: typeStr = "Лучник"; break;
    }
    std::cout << typeStr << " " << name << " (" << x << ", " << y << ")";
    if (!alive) std::cout << " [МЁРТВ]";
    std::cout << std::endl;
}

Warrior::Warrior(const std::string& name, double x, double y) 
    : NPC(NPCType::WARRIOR, name, x, y) {}

void Warrior::print() const {
    std::cout << "[Воин] " << name << " (" << x << ", " << y << ")";
    if (!alive) std::cout << " [МЁРТВ]";
    std::cout << std::endl;
}

Mage::Mage(const std::string& name, double x, double y) 
    : NPC(NPCType::MAGE, name, x, y) {}

void Mage::print() const {
    std::cout << "[Маг] " << name << " (" << x << ", " << y << ")";
    if (!alive) std::cout << " [МЁРТВ]";
    std::cout << std::endl;
}

Archer::Archer(const std::string& name, double x, double y) 
    : NPC(NPCType::ARCHER, name, x, y) {}

void Archer::print() const {
    std::cout << "[Лучник] " << name << " (" << x << ", " << y << ")";
    if (!alive) std::cout << " [МЁРТВ]";
    std::cout << std::endl;
}