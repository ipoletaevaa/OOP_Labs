#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <memory>
#include <vector>
#include <mutex>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const std::string& killer, const std::string& victim, const std::string& reason) = 0;
};

class ConsoleObserver : public Observer {
public:
    void onKill(const std::string& killer, const std::string& victim, const std::string& reason) override;
};

class FileObserver : public Observer {
private:
    std::string filename;
    std::mutex fileMutex;
    
public:
    FileObserver(const std::string& filename = "game_log.txt");
    void onKill(const std::string& killer, const std::string& victim, const std::string& reason) override;
};

class KillNotifier {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    
public:
    void addObserver(std::shared_ptr<Observer> observer);
    void notify(const std::string& killer, const std::string& victim, const std::string& reason);
};

#endif