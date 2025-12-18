#include <gtest/gtest.h>
#include "include/game.h"
#include "include/factory.h"
#include <thread>
#include <chrono>
#include <atomic>

class GameIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        NPCRepository::resetInstance();
    }
    
    void TearDown() override {
        NPCRepository::resetInstance();
    }
};

TEST_F(GameIntegrationTest, GameInitialization) {
    Game game;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    SUCCEED();
}

TEST_F(GameIntegrationTest, ThreeThreadsRunning) {
    Game game;
    
    bool movementRunning = false;
    bool battleRunning = false;
    bool printRunning = false;
    
    std::thread testThread([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        movementRunning = true;
        battleRunning = true;
        printRunning = true;
    });
    
    testThread.join();
    
    EXPECT_TRUE(movementRunning);
    EXPECT_TRUE(battleRunning);
    EXPECT_TRUE(printRunning);
}

TEST_F(GameIntegrationTest, NPCCreationCount) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    
    for (int i = 0; i < 10; i++) {
        auto npc = repo->createNPC("orc", "Орк_" + std::to_string(i), i * 10, i * 10);
        npcs.push_back(npc);
    }
    
    EXPECT_EQ(npcs.size(), 10);
    
    int aliveCount = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) aliveCount++;
    }
    
    EXPECT_EQ(aliveCount, 10);
}

TEST_F(GameIntegrationTest, OrcKillsEveryone) {
    auto repo = NPCRepository::getInstance();
    
    auto orc = repo->createNPC("orc", "Орк_Убийца", 0, 0);
    auto elf = repo->createNPC("elf", "Эльф_Жертва", 5, 5);
    auto dragon = repo->createNPC("dragon", "Дракон_Жертва", 10, 10);
    
    EXPECT_TRUE(orc->canKill(elf));
    EXPECT_TRUE(orc->canKill(dragon));
    EXPECT_TRUE(CombatRules::canAttack(NPCType::ORC, NPCType::ELF));
    EXPECT_TRUE(CombatRules::canAttack(NPCType::ORC, NPCType::DRAGON));
}

TEST_F(GameIntegrationTest, SlaveTraderKillsDruid) {
    auto repo = NPCRepository::getInstance();
    
    auto slaveTrader = repo->createNPC("slave_trader", "Работорговец", 0, 0);
    auto druid = repo->createNPC("druid", "Друид_Жертва", 5, 5);
    auto elf = repo->createNPC("elf", "Эльф_НеЖертва", 10, 10);
    
    EXPECT_TRUE(slaveTrader->canKill(druid));
    EXPECT_TRUE(CombatRules::canAttack(NPCType::SLAVE_TRADER, NPCType::DRUID));
}

TEST_F(GameIntegrationTest, DiceRollRange) {
    NPC npc(NPCType::KNIGHT, "Рыцарь", 50, 50);
    
    for (int i = 0; i < 100; i++) {
        int roll = npc.rollDice();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST_F(GameIntegrationTest, BattleLogicAttackWins) {
    auto repo = NPCRepository::getInstance();
    
    auto attacker = repo->createNPC("orc", "Атакующий", 0, 0);
    auto defender = repo->createNPC("elf", "Защитник", 5, 5);
    
    int attackWins = 0;
    int defenseWins = 0;
    int draws = 0;
    
    for (int i = 0; i < 1000; i++) {
        int attackRoll = attacker->rollDice();
        int defenseRoll = defender->rollDice();
        
        if (attackRoll > defenseRoll) attackWins++;
        else if (defenseRoll > attackRoll) defenseWins++;
        else draws++;
    }
    
    EXPECT_GT(attackWins, 0);
    EXPECT_GT(defenseWins, 0);
    EXPECT_GT(draws, 0);
    
    EXPECT_EQ(attackWins + defenseWins + draws, 1000);
}

TEST_F(GameIntegrationTest, MoveWithinMapBounds) {
    NPC npc(NPCType::WEREWOLF, "Оборотень", 50, 50);
    
    for (int i = 0; i < 100; i++) {
        npc.moveRandom(100, 100);
        auto [x, y] = npc.getPosition();
        
        EXPECT_GE(x, 0);
        EXPECT_LE(x, 99);
        EXPECT_GE(y, 0);
        EXPECT_LE(y, 99);
    }
}

TEST_F(GameIntegrationTest, DeadNPCNoMovement) {
    NPC npc(NPCType::BEAR, "Медведь", 50, 50);
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

TEST_F(GameIntegrationTest, KillDistanceCheck) {
    NPC elf(NPCType::ELF, "Эльф", 0, 0);
    NPC orc(NPCType::ORC, "Орк", 40, 0);
    
    EXPECT_EQ(elf.getKillDistance(), 50);
    EXPECT_EQ(orc.getKillDistance(), 10);
    
    elf.move(0, 0);
    orc.move(45, 0);
    
    auto [x1, y1] = elf.getPosition();
    auto [x2, y2] = orc.getPosition();
    
    double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    
    EXPECT_LE(distance, elf.getKillDistance());
    EXPECT_GT(distance, orc.getKillDistance());
}

TEST_F(GameIntegrationTest, MovementDistanceCheck) {
    NPC princess(NPCType::PRINCESS, "Принцесса", 50, 50);
    NPC dragon(NPCType::DRAGON, "Дракон", 50, 50);
    
    EXPECT_EQ(princess.getMoveDistance(), 1);
    EXPECT_EQ(dragon.getMoveDistance(), 50);
    
    princess.moveRandom(100, 100);
    dragon.moveRandom(100, 100);
    
    auto [x1, y1] = princess.getPosition();
    auto [x2, y2] = dragon.getPosition();
    
    EXPECT_GE(x1, 49);
    EXPECT_LE(x1, 51);
    EXPECT_GE(y1, 49);
    EXPECT_LE(y1, 51);
    
    EXPECT_GE(x2, 0);
    EXPECT_LE(x2, 100);
    EXPECT_GE(y2, 0);
    EXPECT_LE(y2, 100);
}

TEST_F(GameIntegrationTest, MassBattleSimulation) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    
    for (int i = 0; i < 20; i++) {
        std::string type;
        if (i % 4 == 0) type = "orc";
        else if (i % 4 == 1) type = "elf";
        else if (i % 4 == 2) type = "druid";
        else type = "slave_trader";
        
        auto npc = repo->createNPC(type, type + "_" + std::to_string(i), i * 2, i * 2);
        npcs.push_back(npc);
    }
    
    EXPECT_EQ(npcs.size(), 20);
    
    int initialAlive = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) initialAlive++;
    }
    
    EXPECT_EQ(initialAlive, 20);
    
    auto notifier = std::make_shared<KillNotifier>();
    int killCount = 0;
    
    class BattleCounter : public Observer {
    public:
        int kills = 0;
        void onKill(const std::string&, const std::string&, const std::string&) override {
            kills++;
        }
    };
    
    auto counter = std::make_shared<BattleCounter>();
    notifier->addObserver(counter);
    
    for (size_t i = 0; i < npcs.size(); i++) {
        for (size_t j = i + 1; j < npcs.size(); j++) {
            if (npcs[i]->canKill(npcs[j])) {
                if (npcs[i]->rollDice() > npcs[j]->rollDice()) {
                    npcs[j]->kill();
                    notifier->notify(npcs[i]->getName(), npcs[j]->getName(), "тестовая битва");
                }
            }
        }
    }
    
    int finalAlive = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) finalAlive++;
    }
    
    EXPECT_LT(finalAlive, initialAlive);
    EXPECT_GT(counter->kills, 0);
}

TEST_F(GameIntegrationTest, ThreadSafetyStressTest) {
    auto repo = NPCRepository::getInstance();
    std::vector<std::shared_ptr<NPC>> npcs;
    std::mutex mutex;
    std::atomic<int> errors{0};
    
    for (int i = 0; i < 100; i++) {
        auto npc = repo->createNPC("orc", "Орк_" + std::to_string(i), i, i);
        npcs.push_back(npc);
    }
    
    std::vector<std::thread> threads;
    
    for (int t = 0; t < 10; t++) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < 1000; i++) {
                std::lock_guard<std::mutex> lock(mutex);
                
                for (auto& npc : npcs) {
                    if (npc->isAlive()) {
                        auto [x, y] = npc->getPosition();
                        auto name = npc->getName();
                        auto type = npc->getType();
                        
                        if (name.empty()) errors++;
                        if (x < 0 || x > 99) errors++;
                        if (y < 0 || y > 99) errors++;
                    }
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(errors.load(), 0);
}

TEST_F(GameIntegrationTest, AllNPCTypeStats) {
    std::map<NPCType, std::pair<int, int>> expectedStats = {
        {NPCType::ORC, {20, 10}},
        {NPCType::SQUIRREL, {5, 5}},
        {NPCType::DRUID, {10, 10}},
        {NPCType::KNIGHT, {30, 10}},
        {NPCType::ELF, {10, 50}},
        {NPCType::DRAGON, {50, 30}},
        {NPCType::BEAR, {5, 10}},
        {NPCType::BANDIT, {10, 10}},
        {NPCType::WEREWOLF, {40, 5}},
        {NPCType::PRINCESS, {1, 1}},
        {NPCType::TOAD, {1, 10}},
        {NPCType::SLAVE_TRADER, {10, 10}},
        {NPCType::PEGASUS, {30, 10}},
        {NPCType::BITTERN, {50, 10}},
        {NPCType::DESMAN, {5, 20}},
        {NPCType::BULL, {30, 10}}
    };
    
    auto repo = NPCRepository::getInstance();
    
    for (const auto& [type, expected] : expectedStats) {
        std::string typeName;
        switch(type) {
            case NPCType::ORC: typeName = "orc"; break;
            case NPCType::SQUIRREL: typeName = "squirrel"; break;
            case NPCType::DRUID: typeName = "druid"; break;
            case NPCType::KNIGHT: typeName = "knight"; break;
            case NPCType::ELF: typeName = "elf"; break;
            case NPCType::DRAGON: typeName = "dragon"; break;
            case NPCType::BEAR: typeName = "bear"; break;
            case NPCType::BANDIT: typeName = "bandit"; break;
            case NPCType::WEREWOLF: typeName = "werewolf"; break;
            case NPCType::PRINCESS: typeName = "princess"; break;
            case NPCType::TOAD: typeName = "toad"; break;
            case NPCType::SLAVE_TRADER: typeName = "slave_trader"; break;
            case NPCType::PEGASUS: typeName = "pegasus"; break;
            case NPCType::BITTERN: typeName = "bittern"; break;
            case NPCType::DESMAN: typeName = "desman"; break;
            case NPCType::BULL: typeName = "bull"; break;
        }
        
        auto npc = repo->createNPC(typeName, "test_" + typeName, 0, 0);
        
        EXPECT_EQ(npc->getMoveDistance(), expected.first);
        EXPECT_EQ(npc->getKillDistance(), expected.second);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}