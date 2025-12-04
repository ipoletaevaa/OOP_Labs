#include "../include/visitor.h"
#include "../include/observer.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

BattleVisitor::BattleVisitor(double range, std::shared_ptr<KillNotifier> notifier) 
    : attackRange(range), notifier(notifier) {}

double BattleVisitor::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int BattleVisitor::calculatePower(NPCType type) {
    switch(type) {
        case NPCType::WARRIOR: return 5;
        case NPCType::MAGE: return 3;
        case NPCType::ARCHER: return 4;
        default: return 1;
    }
}

std::string BattleVisitor::fight(std::shared_ptr<NPC>& npc1, std::shared_ptr<NPC>& npc2) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int power1 = calculatePower(npc1->getType());
    int power2 = calculatePower(npc2->getType());
    
    std::uniform_int_distribution<> dist(0, 2);
    power1 += dist(gen);
    power2 += dist(gen);
    
    if (power1 > power2) {
        npc2->kill();
        return npc1->getName() + " победил " + npc2->getName() + " в ближнем бою";
    } else if (power2 > power1) {
        npc1->kill();
        return npc2->getName() + " победил " + npc1->getName() + " в ближнем бою";
    } else {
        npc1->kill();
        npc2->kill();
        return "Оба NPC погибли в бою";
    }
}

void BattleVisitor::visit(std::vector<std::shared_ptr<NPC>>& npcs) {
    std::cout << "\n=== НАЧАЛО БОЯ ===" << std::endl;
    std::cout << "Дальность атаки: " << attackRange << " метров" << std::endl;
    
    for (size_t i = 0; i < npcs.size(); i++) {
        if (!npcs[i]->isAlive()) continue;
        
        for (size_t j = i + 1; j < npcs.size(); j++) {
            if (!npcs[j]->isAlive()) continue;
            
            double distance = calculateDistance(npcs[i]->getX(), npcs[i]->getY(),
                                                npcs[j]->getX(), npcs[j]->getY());
            
            if (distance <= attackRange) {
                std::string result = fight(npcs[i], npcs[j]);
                
                if (!npcs[i]->isAlive() && npcs[j]->isAlive()) {
                    notifier->notify(npcs[j]->getName(), npcs[i]->getName(), result);
                } else if (!npcs[j]->isAlive() && npcs[i]->isAlive()) {
                    notifier->notify(npcs[i]->getName(), npcs[j]->getName(), result);
                } else if (!npcs[i]->isAlive() && !npcs[j]->isAlive()) {
                    notifier->notify("НИЧЬЯ", npcs[i]->getName() + " и " + npcs[j]->getName(), result);
                }
            }
        }
    }
    
    auto newEnd = std::remove_if(npcs.begin(), npcs.end(), 
                                [](const std::shared_ptr<NPC>& npc) { return !npc->isAlive(); });
    npcs.erase(newEnd, npcs.end());
    
    std::cout << "=== БОЙ ЗАКОНЧЕН ===" << std::endl;
    std::cout << "Осталось в живых: " << npcs.size() << " NPC" << std::endl;
}