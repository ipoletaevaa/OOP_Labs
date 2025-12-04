#include "observer.h"
#include <iostream>
#include <fstream>
#include <ctime>

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim, const std::string& reason) {
    std::cout << "[СОБЫТИЕ] " << killer << " убил " << victim << ". Причина: " << reason << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename(filename) {}

void FileObserver::onKill(const std::string& killer, const std::string& victim, const std::string& reason) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        file << dt << ": " << killer << " убил " << victim << ". Причина: " << reason << std::endl;
        file.close();
    }
}

void KillNotifier::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void KillNotifier::notify(const std::string& killer, const std::string& victim, const std::string& reason) {
    for (auto& observer : observers) {
        observer->onKill(killer, victim, reason);
    }
}