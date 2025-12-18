#ifndef GAME_H
#define GAME_H

#include "npc_types.h"
#include "observer.h"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <map>

struct BattleTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    std::string reason;
};

class Game {
private:
    const int MAP_WIDTH = 100;
    const int MAP_HEIGHT = 100;
    const int TOTAL_NPCS = 50;
    const int GAME_DURATION = 30;
    
    std::vector<std::shared_ptr<NPC>> npcs;
    std::shared_ptr<KillNotifier> notifier;
    
    std::thread movementThread;
    std::thread battleThread;
    std::thread printThread;
    
    std::atomic<bool> gameRunning{true};
    mutable std::mutex npcsMutex;  // Добавил mutable
    std::mutex battleQueueMutex;
    mutable std::mutex coutMutex;  // Добавил mutable
    std::condition_variable battleCV;
    
    std::queue<BattleTask> battleQueue;
    
    std::shared_ptr<ConsoleObserver> consoleObserver;
    std::shared_ptr<FileObserver> fileObserver;
    
    void initializeNPCs();
    void movementWorker();
    void battleWorker();
    void printWorker();
    
    bool checkBattleDistance(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2);
    void processBattle(BattleTask task);
    
public:
    Game();
    ~Game();
    
    void run();
    void stop();
    void printSurvivors() const;   // Оставляем const
    void printMap() const;         // Оставляем const  
    void printStatistics() const;  // Оставляем const
};

#endif