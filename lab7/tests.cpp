#include <gtest/gtest.h>
#include "npc_types.h"
#include "factory.h"
#include "observer.h"
#include <memory>
#include <filesystem>
#include <fstream>

class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        NPCRepository::resetInstance();
    }
    
    void TearDown() override {
        NPCRepository::resetInstance();
    }
};

TEST_F(NPCTest, NPCCreation) {
    NPC npc(NPCType::ORC, "Орк1", 50, 50);
    EXPECT_EQ(npc.getName(), "Орк1");
    EXPECT_EQ(npc.getType(), NPCType::ORC);
    EXPECT_TRUE(npc.isAlive());
}

TEST_F(NPCTest, NPCKill) {
    NPC npc(NPCType::ELF, "Эльф1", 10, 10);
    EXPECT_TRUE(npc.isAlive());
    npc.kill();
    EXPECT_FALSE(npc.isAlive());
}

TEST_F(NPCTest, MovementDistance) {
    NPC orc(NPCType::ORC, "Орк", 50, 50);
    EXPECT_EQ(orc.getMoveDistance(), 20);
    
    NPC squirrel(NPCType::SQUIRREL, "Белка", 50, 50);
    EXPECT_EQ(squirrel.getMoveDistance(), 5);
}

TEST_F(NPCTest, KillDistance) {
    NPC dragon(NPCType::DRAGON, "Дракон", 50, 50);
    EXPECT_EQ(dragon.getKillDistance(), 30);
    
    NPC elf(NPCType::ELF, "Эльф", 50, 50);
    EXPECT_EQ(elf.getKillDistance(), 50);
}

TEST_F(NPCTest, MoveWithinBounds) {
    NPC npc(NPCType::KNIGHT, "Рыцарь", 10, 10);
    
    for (int i = 0; i < 100; i++) {
        npc.moveRandom(100, 100);
        auto [x, y] = npc.getPosition();
        EXPECT_GE(x, 0);
        EXPECT_LT(x, 100);
        EXPECT_GE(y, 0);
        EXPECT_LT(y, 100);
    }
}

TEST_F(NPCTest, CombatRules) {
    EXPECT_TRUE(CombatRules::canAttack(NPCType::ORC, NPCType::ELF));
    EXPECT_TRUE(CombatRules::canAttack(NPCType::ORC, NPCType::DRAGON));
    EXPECT_TRUE(CombatRules::canAttack(NPCType::SLAVE_TRADER, NPCType::DRUID));
}

TEST_F(NPCTest, DiceRoll) {
    NPC npc(NPCType::BEAR, "Медведь", 50, 50);
    
    for (int i = 0; i < 100; i++) {
        int roll = npc.rollDice();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST_F(NPCTest, FactoryCreation) {
    auto repo = NPCRepository::getInstance();
    
    auto orc = repo->createNPC("orc", "Орк_Тест", 10, 10);
    EXPECT_EQ(orc->getType(), NPCType::ORC);
    
    auto dragon = repo->createNPC("dragon", "Дракон_Тест", 20, 20);
    EXPECT_EQ(dragon->getType(), NPCType::DRAGON);
    
    auto elf = repo->createNPC("elf", "Эльф_Тест", 30, 30);
    EXPECT_EQ(elf->getType(), NPCType::ELF);
}

TEST_F(NPCTest, InvalidFactoryType) {
    auto repo = NPCRepository::getInstance();
    EXPECT_THROW(repo->createNPC("invalid_type", "Тест", 10, 10), std::invalid_argument);
}

TEST_F(NPCTest, CanKillMethod) {
    auto repo = NPCRepository::getInstance();
    
    auto orc = repo->createNPC("orc", "Орк", 10, 10);
    auto elf = repo->createNPC("elf", "Эльф", 20, 20);
    auto druid = repo->createNPC("druid", "Друид", 30, 30);
    auto slaveTrader = repo->createNPC("slave_trader", "Работорговец", 40, 40);
    
    EXPECT_TRUE(orc->canKill(elf));
    EXPECT_TRUE(orc->canKill(druid));
    EXPECT_TRUE(slaveTrader->canKill(druid));
}

class ObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFilename = "test_log.txt";
    }
    
    void TearDown() override {
        if (std::filesystem::exists(testFilename)) {
            std::filesystem::remove(testFilename);
        }
    }
    
    std::string testFilename;
};

TEST_F(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    testing::internal::CaptureStdout();
    observer.onKill("Убийца", "Жертва", "Тестовая причина");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Убийца") != std::string::npos);
    EXPECT_TRUE(output.find("Жертва") != std::string::npos);
}

TEST_F(ObserverTest, FileObserverCreation) {
    FileObserver observer(testFilename);
    EXPECT_TRUE(std::filesystem::exists("logs"));
}

TEST_F(ObserverTest, FileObserverWrite) {
    FileObserver observer(testFilename);
    observer.onKill("ТестУбийца", "ТестЖертва", "ТестПричина");
    
    std::ifstream file("logs/" + testFilename);
    EXPECT_TRUE(file.is_open());
    
    std::string content;
    std::getline(file, content);
    EXPECT_TRUE(content.find("ТестУбийца") != std::string::npos);
    EXPECT_TRUE(content.find("ТестЖертва") != std::string::npos);
    
    file.close();
}

TEST_F(ObserverTest, KillNotifier) {
    class MockObserver : public Observer {
    public:
        int notificationCount = 0;
        void onKill(const std::string&, const std::string&, const std::string&) override {
            notificationCount++;
        }
    };
    
    KillNotifier notifier;
    auto observer1 = std::make_shared<MockObserver>();
    auto observer2 = std::make_shared<MockObserver>();
    
    notifier.addObserver(observer1);
    notifier.addObserver(observer2);
    
    notifier.notify("У1", "Ж1", "П1");
    
    EXPECT_EQ(observer1->notificationCount, 1);
    EXPECT_EQ(observer2->notificationCount, 1);
}

TEST_F(NPCTest, DeadNPCNoMovement) {
    NPC npc(NPCType::BANDIT, "Бандит", 50, 50);
    auto [initialX, initialY] = npc.getPosition();
    
    npc.kill();
    
    for (int i = 0; i < 10; i++) {
        bool moved = npc.moveRandom(100, 100);
        EXPECT_FALSE(moved);
        
        auto [x, y] = npc.getPosition();
        EXPECT_EQ(x, initialX);
        EXPECT_EQ(y, initialY);
    }
}

TEST_F(NPCTest, AllTypesCreation) {
    auto repo = NPCRepository::getInstance();
    
    std::vector<std::string> types = {
        "orc", "squirrel", "druid", "knight", "elf",
        "dragon", "bear", "bandit", "werewolf", "princess",
        "toad", "slave_trader", "pegasus", "bittern", "desman", "bull"
    };
    
    for (const auto& type : types) {
        EXPECT_NO_THROW({
            auto npc = repo->createNPC(type, type + "_test", 10, 10);
            EXPECT_TRUE(npc != nullptr);
        });
    }
}

TEST_F(NPCTest, TypeNames) {
    NPC orc(NPCType::ORC, "Орк", 10, 10);
    EXPECT_EQ(orc.getTypeName(), "Орк");
    
    NPC dragon(NPCType::DRAGON, "Дракон", 20, 20);
    EXPECT_EQ(dragon.getTypeName(), "Дракон");
    
    NPC elf(NPCType::ELF, "Эльф", 30, 30);
    EXPECT_EQ(elf.getTypeName(), "Эльф");
}

TEST_F(NPCTest, ThreadSafeGetters) {
    NPC npc(NPCType::KNIGHT, "Рыцарь", 50, 50);
    
    auto type = npc.getType();
    auto name = npc.getName();
    auto pos = npc.getPosition();
    auto alive = npc.isAlive();
    
    EXPECT_EQ(type, NPCType::KNIGHT);
    EXPECT_EQ(name, "Рыцарь");
    EXPECT_EQ(pos.first, 50);
    EXPECT_EQ(pos.second, 50);
    EXPECT_TRUE(alive);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}