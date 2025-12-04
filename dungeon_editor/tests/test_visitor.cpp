#include <gtest/gtest.h>
#include "visitor.h"
#include "observer.h"
#include <memory>

class MockObserver : public Observer {
public:
    std::vector<std::string> events;
    
    void onKill(const std::string& killer, const std::string& victim, const std::string& reason) override {
        events.push_back(killer + " killed " + victim);
    }
};

class VisitorTest : public ::testing::Test {
protected:
    std::shared_ptr<KillNotifier> notifier;
    std::shared_ptr<MockObserver> mockObserver;
    
    void SetUp() override {
        notifier = std::make_shared<KillNotifier>();
        mockObserver = std::make_shared<MockObserver>();
        notifier->addObserver(mockObserver);
    }
};

TEST_F(VisitorTest, CalculateDistance) {
    BattleVisitor visitor(100, notifier);
    
    EXPECT_NEAR(visitor.calculateDistance(0, 0, 3, 4), 5.0, 0.001);
    EXPECT_NEAR(visitor.calculateDistance(0, 0, 0, 0), 0.0, 0.001);
    EXPECT_NEAR(visitor.calculateDistance(100, 100, 104, 100), 4.0, 0.001);
}

TEST_F(VisitorTest, CalculatePower) {
    BattleVisitor visitor(100, notifier);
    
    EXPECT_EQ(visitor.calculatePower(NPCType::WARRIOR), 5);
    EXPECT_EQ(visitor.calculatePower(NPCType::MAGE), 3);
    EXPECT_EQ(visitor.calculatePower(NPCType::ARCHER), 4);
}

TEST_F(VisitorTest, BattleWithDistance) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    
    npcs.push_back(repo->createNPC("warrior", "W1", 100, 100));
    npcs.push_back(repo->createNPC("mage", "M1", 101, 101));
    
    BattleVisitor visitor(2, notifier);
    visitor.visit(npcs);
    
    EXPECT_TRUE(mockObserver->events.size() > 0 || npcs.size() < 2);
}

TEST_F(VisitorTest, NoBattleOutOfRange) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    
    npcs.push_back(repo->createNPC("warrior", "W1", 100, 100));
    npcs.push_back(repo->createNPC("mage", "M1", 400, 400));
    
    size_t initialSize = npcs.size();
    BattleVisitor visitor(10, notifier);
    visitor.visit(npcs);
    
    EXPECT_EQ(npcs.size(), initialSize);
    EXPECT_TRUE(mockObserver->events.empty());
}