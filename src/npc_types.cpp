#include "npc_types.h"
#include <iostream>
#include <random>
#include <cmath>

bool CombatRules::canAttack(NPCType attacker, NPCType defender) {
    if (attacker == NPCType::ORC) {
        return true;
    }
    
    if (attacker == NPCType::SLAVE_TRADER && defender == NPCType::DRUID) {
        return true;
    }
    
    return true;
}

std::string CombatRules::getKillReason(NPCType attacker, NPCType defender) {
    if (attacker == NPCType::ORC) {
        return "Орк убивает всех";
    }
    if (attacker == NPCType::SLAVE_TRADER && defender == NPCType::DRUID) {
        return "Работорговец охотится на Друидов";
    }
    return "обычная битва";
}

NPC::NPC(NPCType t, const std::string& n, double xPos, double yPos) 
    : type(t), name(n), x(xPos), y(yPos), alive(true) {
    
    switch(type) {
        case NPCType::ORC:           moveDistance = 20; killDistance = 10; break;
        case NPCType::SQUIRREL:      moveDistance = 5;  killDistance = 5;  break;
        case NPCType::DRUID:         moveDistance = 10; killDistance = 10; break;
        case NPCType::KNIGHT:        moveDistance = 30; killDistance = 10; break;
        case NPCType::ELF:           moveDistance = 10; killDistance = 50; break;
        case NPCType::DRAGON:        moveDistance = 50; killDistance = 30; break;
        case NPCType::BEAR:          moveDistance = 5;  killDistance = 10; break;
        case NPCType::BANDIT:        moveDistance = 10; killDistance = 10; break;
        case NPCType::WEREWOLF:      moveDistance = 40; killDistance = 5;  break;
        case NPCType::PRINCESS:      moveDistance = 1;  killDistance = 1;  break;
        case NPCType::TOAD:          moveDistance = 1;  killDistance = 10; break;
        case NPCType::SLAVE_TRADER:  moveDistance = 10; killDistance = 10; break;
        case NPCType::PEGASUS:       moveDistance = 30; killDistance = 10; break;
        case NPCType::BITTERN:       moveDistance = 50; killDistance = 10; break;
        case NPCType::DESMAN:        moveDistance = 5;  killDistance = 20; break;
        case NPCType::BULL:          moveDistance = 30; killDistance = 10; break;
    }
}

NPCType NPC::getType() const {
    std::shared_lock lock(mtx);
    return type;
}

std::string NPC::getName() const {
    std::shared_lock lock(mtx);
    return name;
}

std::pair<double, double> NPC::getPosition() const {
    std::shared_lock lock(mtx);
    return {x, y};
}

bool NPC::isAlive() const {
    std::shared_lock lock(mtx);
    return alive;
}

int NPC::getMoveDistance() const {
    std::shared_lock lock(mtx);
    return moveDistance;
}

int NPC::getKillDistance() const {
    std::shared_lock lock(mtx);
    return killDistance;
}

void NPC::kill() {
    std::unique_lock lock(mtx);
    alive = false;
}

void NPC::move(double newX, double newY) {
    std::unique_lock lock(mtx);
    if (alive) {
        x = newX;
        y = newY;
    }
}

bool NPC::moveRandom(int mapWidth, int mapHeight) {
    std::unique_lock lock(mtx);
    if (!alive) return false;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> dirDist(-1, 1);
    std::uniform_int_distribution<> distDist(0, moveDistance);
    
    double dx = dirDist(gen) * distDist(gen);
    double dy = dirDist(gen) * distDist(gen);
    
    double newX = x + dx;
    double newY = y + dy;
    
    if (newX < 0) newX = 0;
    if (newX >= mapWidth) newX = mapWidth - 1;
    if (newY < 0) newY = 0;
    if (newY >= mapHeight) newY = mapHeight - 1;
    
    x = newX;
    y = newY;
    
    return true;
}

void NPC::print() const {
    std::shared_lock lock(mtx);
    std::cout << "[" << getTypeName() << "] " << name 
              << " (" << x << ", " << y << ")";
    if (!alive) std::cout << " [МЁРТВ]";
    std::cout << std::endl;
}

std::string NPC::getTypeName() const {
    switch(type) {
        case NPCType::ORC: return "Орк";
        case NPCType::SQUIRREL: return "Белка";
        case NPCType::DRUID: return "Друид";
        case NPCType::KNIGHT: return "Рыцарь";
        case NPCType::ELF: return "Эльф";
        case NPCType::DRAGON: return "Дракон";
        case NPCType::BEAR: return "Медведь";
        case NPCType::BANDIT: return "Разбойник";
        case NPCType::WEREWOLF: return "Оборотень";
        case NPCType::PRINCESS: return "Принцесса";
        case NPCType::TOAD: return "Жаба";
        case NPCType::SLAVE_TRADER: return "Работорговец";
        case NPCType::PEGASUS: return "Пегас";
        case NPCType::BITTERN: return "Выпь";
        case NPCType::DESMAN: return "Выхухоль";
        case NPCType::BULL: return "Бык";
        default: return "Неизвестный";
    }
}

int NPC::rollDice() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 6);
    return dist(gen);
}

bool NPC::canKill(const std::shared_ptr<NPC>& other) const {
    return CombatRules::canAttack(this->getType(), other->getType());
}