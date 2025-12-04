#include <gtest/gtest.h>
#include "npc_types.h"

TEST(NPCTest, CreateNPC) {
    Warrior warrior("Conan", 100, 100);
    EXPECT_EQ(warrior.getName(), "Conan");
    EXPECT_EQ(warrior.getX(), 100);
    EXPECT_EQ(warrior.getY(), 100);
    EXPECT_TRUE(warrior.isAlive());
    EXPECT_EQ(warrior.getType(), NPCType::WARRIOR);
}

TEST(NPCTest, NPCKill) {
    Mage mage("Gandalf", 200, 200);
    EXPECT_TRUE(mage.isAlive());
    mage.kill();
    EXPECT_FALSE(mage.isAlive());
}

TEST(NPCTest, InvalidCoordinates) {
    EXPECT_THROW(Warrior("Test", 0, 100), std::invalid_argument);
    EXPECT_THROW(Archer("Test", 500, 100), std::invalid_argument);
    EXPECT_THROW(Mage("Test", 100, -10), std::invalid_argument);
}

TEST(NPCTest, ValidCoordinates) {
    EXPECT_NO_THROW(Warrior("Test", 1, 1));
    EXPECT_NO_THROW(Archer("Test", 499, 499));
    EXPECT_NO_THROW(Mage("Test", 250, 250));
}

TEST(NPCTest, NPCTypeTest) {
    Warrior w("W", 10, 10);
    Mage m("M", 20, 20);
    Archer a("A", 30, 30);
    
    EXPECT_EQ(w.getType(), NPCType::WARRIOR);
    EXPECT_EQ(m.getType(), NPCType::MAGE);
    EXPECT_EQ(a.getType(), NPCType::ARCHER);
}