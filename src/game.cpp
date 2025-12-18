#include "game.h"
#include "factory.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>

Game::Game() {
    consoleObserver = std::make_shared<ConsoleObserver>();
    fileObserver = std::make_shared<FileObserver>("game_log.txt");
    
    notifier = std::make_shared<KillNotifier>();
    notifier->addObserver(consoleObserver);
    notifier->addObserver(fileObserver);
    
    initializeNPCs();
}

Game::~Game() {
    stop();
}

void Game::initializeNPCs() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xDist(0, MAP_WIDTH);
    std::uniform_real_distribution<> yDist(0, MAP_HEIGHT);
    
    std::vector<std::string> npcTypes = {
        "orc", "squirrel", "druid", "knight", "elf",
        "dragon", "bear", "bandit", "werewolf", "princess",
        "toad", "slave_trader", "pegasus", "bittern", "desman", "bull"
    };
    
    std::discrete_distribution<> typeDist({3, 4, 3, 3, 3, 2, 4, 4, 2, 2, 4, 3, 2, 2, 3, 3});
    
    auto repo = NPCRepository::getInstance();
    
    for (int i = 0; i < TOTAL_NPCS; i++) {
        std::string type = npcTypes[typeDist(gen)];
        std::string name = type + "_" + std::to_string(i);
        double x = xDist(gen);
        double y = yDist(gen);
        
        try {
            auto npc = repo->createNPC(type, name, x, y);
            
            {
                std::lock_guard<std::mutex> lock(npcsMutex);
                npcs.push_back(npc);
            }
            
        } catch (const std::exception& e) {
        }
    }
}

bool Game::checkBattleDistance(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2) {
    if (!npc1->isAlive() || !npc2->isAlive()) return false;
    
    auto [x1, y1] = npc1->getPosition();
    auto [x2, y2] = npc2->getPosition();
    
    double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    
    return distance <= npc1->getKillDistance() || distance <= npc2->getKillDistance();
}

void Game::movementWorker() {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        std::unique_lock<std::mutex> lock(npcsMutex);
        
        for (auto& npc : npcs) {
            if (npc->isAlive()) {
                npc->moveRandom(MAP_WIDTH, MAP_HEIGHT);
            }
        }
        
        for (size_t i = 0; i < npcs.size(); i++) {
            if (!npcs[i]->isAlive()) continue;
            
            for (size_t j = i + 1; j < npcs.size(); j++) {
                if (!npcs[j]->isAlive()) continue;
                
                if (checkBattleDistance(npcs[i], npcs[j])) {
                    if (npcs[i]->canKill(npcs[j])) {
                        BattleTask task{npcs[i], npcs[j], 
                                       CombatRules::getKillReason(npcs[i]->getType(), npcs[j]->getType())};
                        
                        std::lock_guard<std::mutex> battleLock(battleQueueMutex);
                        battleQueue.push(task);
                        battleCV.notify_one();
                    }
                    
                    if (npcs[j]->canKill(npcs[i])) {
                        BattleTask task{npcs[j], npcs[i], 
                                       CombatRules::getKillReason(npcs[j]->getType(), npcs[i]->getType())};
                        
                        std::lock_guard<std::mutex> battleLock(battleQueueMutex);
                        battleQueue.push(task);
                        battleCV.notify_one();
                    }
                }
            }
        }
    }
}

void Game::processBattle(BattleTask task) {
    auto attacker = task.attacker;
    auto defender = task.defender;
    
    if (!attacker->isAlive() || !defender->isAlive()) {
        return;
    }
    
    int attackRoll = attacker->rollDice();
    int defenseRoll = defender->rollDice();
    
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "[БИТВА] " << attacker->getName() 
                  << " (" << attackRoll << ") vs " 
                  << defender->getName() << " (" << defenseRoll << ")"
                  << " - " << task.reason << std::endl;
    }
    
    if (attackRoll > defenseRoll) {
        defender->kill();
        notifier->notify(attacker->getName(), defender->getName(), 
                        task.reason + " (атака: " + std::to_string(attackRoll) + 
                        ", защита: " + std::to_string(defenseRoll) + ")");
    } else if (defenseRoll > attackRoll) {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "[БИТВА] " << defender->getName() 
                  << " успешно защитился от " << attacker->getName() << std::endl;
    } else {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "[БИТВА] Ничья между " << attacker->getName() 
                  << " и " << defender->getName() << std::endl;
    }
}

void Game::battleWorker() {
    while (gameRunning) {
        BattleTask task;
        
        {
            std::unique_lock<std::mutex> lock(battleQueueMutex);
            battleCV.wait_for(lock, std::chrono::milliseconds(100), 
                            [this] { return !battleQueue.empty() || !gameRunning; });
            
            if (!gameRunning && battleQueue.empty()) break;
            
            if (!battleQueue.empty()) {
                task = battleQueue.front();
                battleQueue.pop();
            }
        }
        
        if (task.attacker && task.defender) {
            processBattle(task);
        }
    }
}

void Game::printWorker() {
    int seconds = 0;
    while (gameRunning && seconds < GAME_DURATION) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        seconds++;
        
        if (!gameRunning) break;
        
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "\n=== СЕКУНДА " << seconds << " ===" << std::endl;
            printMap();
            
            std::unique_lock<std::mutex> lock(npcsMutex);
            int aliveCount = std::count_if(npcs.begin(), npcs.end(), 
                                          [](auto& npc) { return npc->isAlive(); });
            std::cout << "Живых NPC: " << aliveCount << " из " << npcs.size() << std::endl;
        }
    }
}

void Game::printMap() const {
    const int DISPLAY_WIDTH = 60;
    const int DISPLAY_HEIGHT = 20;
    
    std::vector<std::vector<char>> grid(DISPLAY_HEIGHT, 
                                       std::vector<char>(DISPLAY_WIDTH, '.'));
    
    std::lock_guard<std::mutex> lock(npcsMutex);
    
    std::map<NPCType, char> symbols = {
        {NPCType::ORC, 'O'},
        {NPCType::DRAGON, 'D'},
        {NPCType::ELF, 'E'},
        {NPCType::KNIGHT, 'K'},
        {NPCType::BEAR, 'B'},
        {NPCType::WEREWOLF, 'W'},
        {NPCType::DRUID, 'R'},
        {NPCType::SQUIRREL, 'S'},
        {NPCType::BANDIT, 'T'},
        {NPCType::PRINCESS, 'P'},
        {NPCType::SLAVE_TRADER, '$'},
        {NPCType::PEGASUS, 'F'},
        {NPCType::BITTERN, 'I'},
        {NPCType::DESMAN, 'M'},
        {NPCType::BULL, 'U'},
        {NPCType::TOAD, 'G'}
    };
    
    for (const auto& npc : npcs) {
        if (!npc->isAlive()) continue;
        
        auto [x, y] = npc->getPosition();
        
        int displayX = static_cast<int>((x / MAP_WIDTH) * DISPLAY_WIDTH);
        int displayY = static_cast<int>((y / MAP_HEIGHT) * DISPLAY_HEIGHT);
        
        if (displayX >= 0 && displayX < DISPLAY_WIDTH && 
            displayY >= 0 && displayY < DISPLAY_HEIGHT) {
            
            char symbol = symbols.count(npc->getType()) ? symbols[npc->getType()] : '?';
            grid[displayY][displayX] = symbol;
        }
    }
    
    std::cout << std::string(DISPLAY_WIDTH + 2, '=') << std::endl;
    
    for (const auto& row : grid) {
        std::cout << "|";
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << std::string(DISPLAY_WIDTH + 2, '=') << std::endl;
    
    std::cout << "Легенда: O-Орк, D-Дракон, E-Эльф, K-Рыцарь, B-Медведь, W-Оборотень" << std::endl;
    std::cout << "         R-Друид, S-Белка, T-Разбойник, P-Принцесса, $-Работорговец" << std::endl;
}

void Game::run() {
    std::cout << "\n=== ЗАПУСК ИГРЫ ===" << std::endl;
    std::cout << "Длительность: " << GAME_DURATION << " секунд" << std::endl;
    std::cout << "NPC на карте: " << TOTAL_NPCS << std::endl;
    std::cout << "Размер карты: " << MAP_WIDTH << "x" << MAP_HEIGHT << std::endl;
    std::cout << "Логи: logs/game_log.txt" << std::endl;
    std::cout << "===================\n" << std::endl;
    
    movementThread = std::thread(&Game::movementWorker, this);
    battleThread = std::thread(&Game::battleWorker, this);
    printThread = std::thread(&Game::printWorker, this);
    
    std::this_thread::sleep_for(std::chrono::seconds(GAME_DURATION + 1));
    
    stop();
    
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
    printSurvivors();
    printStatistics();
}

void Game::stop() {
    gameRunning = false;
    battleCV.notify_all();
    
    if (movementThread.joinable()) movementThread.join();
    if (battleThread.joinable()) battleThread.join();
    if (printThread.joinable()) printThread.join();
}

void Game::printSurvivors() const {
    std::lock_guard<std::mutex> lock(npcsMutex);
    
    std::cout << "\n=== ВЫЖИВШИЕ NPC ===" << std::endl;
    
    std::map<std::string, int> survivorsByType;
    int aliveCount = 0;
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            aliveCount++;
            npc->print();
            survivorsByType[npc->getTypeName()]++;
        }
    }
    
    std::cout << "\n=== СТАТИСТИКА ВЫЖИВШИХ ===" << std::endl;
    for (const auto& [type, count] : survivorsByType) {
        std::cout << type << ": " << count << std::endl;
    }
    
    std::cout << "\nВсего выжило: " << aliveCount << " из " << npcs.size() 
              << " (" << std::fixed << std::setprecision(1) 
              << (aliveCount * 100.0 / npcs.size()) << "%)" << std::endl;
}

void Game::printStatistics() const {
    std::lock_guard<std::mutex> lock(npcsMutex);
    
    std::map<std::string, int> typeCount;
    for (const auto& npc : npcs) {
        typeCount[npc->getTypeName()]++;
    }
    
    std::cout << "\n=== НАЧАЛЬНОЕ РАСПРЕДЕЛЕНИЕ NPC ===" << std::endl;
    for (const auto& [type, count] : typeCount) {
        std::cout << type << ": " << count << std::endl;
    }
}