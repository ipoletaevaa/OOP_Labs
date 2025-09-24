#include <gtest/gtest.h>
#include "lab1_functions.h"

TEST(PerfectSquareTest, BasicCases) {
    EXPECT_TRUE(isPerfectSquare(1));
    EXPECT_TRUE(isPerfectSquare(4));
    EXPECT_TRUE(isPerfectSquare(9));
    EXPECT_TRUE(isPerfectSquare(16));
    EXPECT_TRUE(isPerfectSquare(25));
    EXPECT_TRUE(isPerfectSquare(36));
    EXPECT_TRUE(isPerfectSquare(81));
    
    EXPECT_FALSE(isPerfectSquare(2));
    EXPECT_FALSE(isPerfectSquare(3));
    EXPECT_FALSE(isPerfectSquare(5));
    EXPECT_FALSE(isPerfectSquare(10));
    EXPECT_FALSE(isPerfectSquare(15));
    EXPECT_FALSE(isPerfectSquare(26));
}

TEST(PerfectSquareTest, EdgeCases) {
    EXPECT_FALSE(isPerfectSquare(0));
    EXPECT_FALSE(isPerfectSquare(-1));
    EXPECT_FALSE(isPerfectSquare(-4));
}

TEST(ClosestPairTest, GivenExamples) {
    // Тесты из задания
    auto result1 = closest_pair_tonum(10);
    EXPECT_EQ(result1.first, 5);
    EXPECT_EQ(result1.second, 4);
    
    auto result2 = closest_pair_tonum(30);
    EXPECT_EQ(result2.first, 29);
    EXPECT_EQ(result2.second, 20);
    
    auto result3 = closest_pair_tonum(50);
    EXPECT_EQ(result3.first, 45);
    EXPECT_EQ(result3.second, 36);
}

TEST(ClosestPairTest, AdditionalCases) {
    // Дополнительные тесты
    auto result1 = closest_pair_tonum(20);
    EXPECT_TRUE(isPerfectSquare(result1.first + result1.second));
    EXPECT_TRUE(isPerfectSquare(result1.first - result1.second));
    
    auto result2 = closest_pair_tonum(40);
    EXPECT_TRUE(isPerfectSquare(result2.first + result2.second));
    EXPECT_TRUE(isPerfectSquare(result2.first - result2.second));
}

TEST(ClosestPairTest, NoPairFound) {
    // Для очень маленьких значений пара не должна находиться
    auto result = closest_pair_tonum(3);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
}

TEST(ClosestPairTest, PropertyValidation) {
    // Проверяем что найденная пара удовлетворяет условиям
    auto result = closest_pair_tonum(50);
    
    int m = result.first;
    int n = result.second;
    
    // Проверяем условия: n < m < upper_limit
    EXPECT_LT(n, m);
    EXPECT_LT(m, 50);
    EXPECT_GT(n, 0);
    
    // Проверяем свойства совершенных квадратов
    EXPECT_TRUE(isPerfectSquare(m + n));
    EXPECT_TRUE(isPerfectSquare(m - n));
}