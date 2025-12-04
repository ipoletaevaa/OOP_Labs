#ifndef VISITOR_H
#define VISITOR_H

#include <memory>
#include <vector>
#include "npc_types.h"

class KillNotifier;

class BattleVisitor {
private:
    double attackRange;
    std::shared_ptr<KillNotifier> notifier;
    
public:
    BattleVisitor(double range, std::shared_ptr<KillNotifier> notifier);
    
    void visit(std::vector<std::shared_ptr<NPC>>& npcs);
    
    double calculateDistance(double x1, double y1, double x2, double y2);
    std::string fight(std::shared_ptr<NPC>& npc1, std::shared_ptr<NPC>& npc2);
    int calculatePower(NPCType type);
};

#endif