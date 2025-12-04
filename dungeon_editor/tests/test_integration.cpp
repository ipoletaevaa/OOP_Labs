#include <gtest/gtest.h>
#include "factory.h"
#include "visitor.h"
#include "observer.h"
#include <filesystem>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        NPCRepository::resetInstance();
    }
    
    void TearDown() override {
        NPCRepository::resetInstance();
    }
};

TEST_F(IntegrationTest, FullWorkflow) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    
    npcs.push_back(repo->createNPC("warrior", "Conan", 100, 100));
    npcs.push_back(repo->createNPC("mage", "Gandalf", 105, 105));
    npcs.push_back(repo->createNPC("archer", "Legolas", 400, 400));
    
    EXPECT_EQ(npcs.size(), 3);
    
    auto notifier = std::make_shared<KillNotifier>();
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    notifier->addObserver(consoleObserver);
    
    BattleVisitor visitor(50, notifier);
    visitor.visit(npcs);
    
    EXPECT_TRUE(npcs.size() <= 3);
}

TEST_F(IntegrationTest, FilePersistence) {
    auto repo = NPCRepository::getInstance();
    
    std::vector<std::shared_ptr<NPC>> original;
    original.push_back(repo->createNPC("warrior", "W1", 100, 100));
    original.push_back(repo->createNPC("mage", "M1", 200, 200));
    original.push_back(repo->createNPC("archer", "A1", 300, 300));
    
    const std::string testFile = "integration_test.txt";
    repo->saveToFile(testFile, original);
    
    auto loaded = repo->loadFromFile(testFile);
    EXPECT_EQ(loaded.size(), 3);
    
    for (size_t i = 0; i < loaded.size(); ++i) {
        EXPECT_EQ(loaded[i]->getName(), original[i]->getName());
        EXPECT_EQ(loaded[i]->getX(), original[i]->getX());
        EXPECT_EQ(loaded[i]->getY(), original[i]->getY());
    }
    
    std::filesystem::remove(testFile);
}