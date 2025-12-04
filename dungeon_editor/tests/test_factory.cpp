#include <gtest/gtest.h>
#include "factory.h"
#include <filesystem>
#include <fstream>

class FactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        NPCRepository::resetInstance();
    }
    
    void TearDown() override {
        NPCRepository::resetInstance();
    }
};

TEST_F(FactoryTest, CreateNPCs) {
    auto repo = NPCRepository::getInstance();
    
    auto warrior = repo->createNPC("warrior", "Conan", 100, 100);
    EXPECT_EQ(warrior->getName(), "Conan");
    EXPECT_EQ(warrior->getType(), NPCType::WARRIOR);
    
    auto mage = repo->createNPC("mage", "Gandalf", 200, 200);
    EXPECT_EQ(mage->getName(), "Gandalf");
    EXPECT_EQ(mage->getType(), NPCType::MAGE);
    
    auto archer = repo->createNPC("archer", "Legolas", 300, 300);
    EXPECT_EQ(archer->getName(), "Legolas");
    EXPECT_EQ(archer->getType(), NPCType::ARCHER);
}

TEST_F(FactoryTest, InvalidType) {
    auto repo = NPCRepository::getInstance();
    EXPECT_THROW(repo->createNPC("unknown", "Test", 100, 100), std::invalid_argument);
}

TEST_F(FactoryTest, SaveAndLoad) {
    auto repo = NPCRepository::getInstance();
    
    std::vector<std::shared_ptr<NPC>> npcs;
    npcs.push_back(repo->createNPC("warrior", "Conan", 100, 100));
    npcs.push_back(repo->createNPC("mage", "Gandalf", 200, 200));
    npcs.push_back(repo->createNPC("archer", "Legolas", 300, 300));
    
    const std::string testFile = "test_save.txt";
    repo->saveToFile(testFile, npcs);
    
    auto loaded = repo->loadFromFile(testFile);
    EXPECT_EQ(loaded.size(), 3);
    
    std::filesystem::remove(testFile);
}

TEST_F(FactoryTest, Singleton) {
    auto repo1 = NPCRepository::getInstance();
    auto repo2 = NPCRepository::getInstance();
    EXPECT_EQ(repo1, repo2);
}