#include <gtest/gtest.h>
#include "observer.h"
#include <fstream>
#include <filesystem>

TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    testing::internal::CaptureStdout();
    observer.onKill("Killer", "Victim", "Test reason");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Killer") != std::string::npos);
    EXPECT_TRUE(output.find("Victim") != std::string::npos);
}

TEST(ObserverTest, FileObserver) {
    const std::string testFile = "test_log.txt";
    FileObserver observer(testFile);
    
    observer.onKill("Killer", "Victim", "Test reason");
    
    std::ifstream file(testFile);
    EXPECT_TRUE(file.is_open());
    
    std::string content;
    std::getline(file, content);
    EXPECT_TRUE(content.find("Killer") != std::string::npos);
    EXPECT_TRUE(content.find("Victim") != std::string::npos);
    
    file.close();
    std::filesystem::remove(testFile);
}

TEST(ObserverTest, KillNotifier) {
    class TestObserver : public Observer {
    public:
        int callCount = 0;
        void onKill(const std::string&, const std::string&, const std::string&) override {
            callCount++;
        }
    };
    
    KillNotifier notifier;
    auto observer1 = std::make_shared<TestObserver>();
    auto observer2 = std::make_shared<TestObserver>();
    
    notifier.addObserver(observer1);
    notifier.addObserver(observer2);
    
    notifier.notify("K1", "V1", "R1");
    
    EXPECT_EQ(observer1->callCount, 1);
    EXPECT_EQ(observer2->callCount, 1);
}