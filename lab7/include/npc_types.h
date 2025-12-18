#ifndef NPC_TYPES_H
#define NPC_TYPES_H

#include <string>
#include <random>
#include <mutex>
#include <shared_mutex>
#include <memory>

enum class NPCType {
    ORC,
    SQUIRREL,
    DRUID,
    KNIGHT,
    ELF,
    DRAGON,
    BEAR,
    BANDIT,
    WEREWOLF,
    PRINCESS,
    TOAD,
    SLAVE_TRADER,
    PEGASUS,
    BITTERN,
    DESMAN,
    BULL
};

class NPC;
class CombatRules {
public:
    static bool canAttack(NPCType attacker, NPCType defender);
    static std::string getKillReason(NPCType attacker, NPCType defender);
};

class NPC {
protected:
    NPCType type;
    std::string name;
    double x;
    double y;
    bool alive;
    mutable std::shared_mutex mtx;
    
    int moveDistance;
    int killDistance;

public:
    NPC(NPCType t, const std::string& n, double xPos, double yPos);
    virtual ~NPC() = default;
    
    NPCType getType() const;
    std::string getName() const;
    std::pair<double, double> getPosition() const;
    bool isAlive() const;
    int getMoveDistance() const;
    int getKillDistance() const;
    
    void kill();
    void move(double newX, double newY);
    bool moveRandom(int mapWidth, int mapHeight);
    
    virtual void print() const;
    virtual std::string getTypeName() const;
    
    int rollDice() const;
    bool canKill(const std::shared_ptr<NPC>& other) const;
};

#endif