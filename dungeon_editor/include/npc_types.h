#ifndef NPC_TYPES_H
#define NPC_TYPES_H

#include <string>

enum class NPCType {
    WARRIOR,
    MAGE,
    ARCHER
};

class NPC {
protected:
    NPCType type;
    std::string name;
    double x;
    double y;
    bool alive;

public:
    NPC(NPCType t, const std::string& n, double xPos, double yPos);
    virtual ~NPC() = default;
    
    NPCType getType() const;
    std::string getName() const;
    double getX() const;
    double getY() const;
    bool isAlive() const;
    
    void kill();
    
    virtual void print() const;
};

class Warrior : public NPC {
public:
    Warrior(const std::string& name, double x, double y);
    void print() const override;
};

class Mage : public NPC {
public:
    Mage(const std::string& name, double x, double y);
    void print() const override;
};

class Archer : public NPC {
public:
    Archer(const std::string& name, double x, double y);
    void print() const override;
};

#endif