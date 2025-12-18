#include "observer.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim, const std::string& reason) {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&now_time), "%H:%M:%S") 
              << "] " << killer << " убил " << victim 
              << ". Причина: " << reason << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    std::filesystem::path logDir = "logs";
    std::filesystem::create_directories(logDir);
    this->filename = (logDir / filename).string();
}

void FileObserver::onKill(const std::string& killer, const std::string& victim, const std::string& reason) {
    std::lock_guard<std::mutex> lock(fileMutex);
    
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        
        file << "[" << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") 
             << "] " << killer << " убил " << victim 
             << ". Причина: " << reason << std::endl;
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