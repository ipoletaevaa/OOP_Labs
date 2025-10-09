#include <gtest/gtest.h>
#include "../include/money.h"

TEST(MoneyTest, DefaultConstructor) {
    Money m;
    EXPECT_NO_THROW(m.print());
}

TEST(MoneyTest, ParameterConstructor) {
    Money m(123, 45);
    EXPECT_NO_THROW(m.print());
}

TEST(MoneyTest, ParameterConstructorInvalidKopecks) {
    EXPECT_THROW(Money(100, 100), std::invalid_argument);
}

TEST(MoneyTest, ParameterConstructorNegativeRubles) {
    EXPECT_THROW(Money(-100, 50), std::invalid_argument);
}

TEST(MoneyTest, CopyConstructor) {
    Money m1(100, 50);
    Money m2(m1);
    EXPECT_TRUE(m1.isEqual(m2));
}

TEST(MoneyTest, AddOperation) {
    Money m1(100, 50);
    Money m2(50, 25);
    Money result = m1.add(m2);
    
    Money expected(150, 75);
    EXPECT_TRUE(result.isEqual(expected));
}

TEST(MoneyTest, AddWithCarry) {
    Money m1(100, 99);
    Money m2(100, 1);
    Money result = m1.add(m2);
    
    Money expected(201, 0);
    EXPECT_TRUE(result.isEqual(expected));
}

TEST(MoneyTest, SubtractOperation) {
    Money m1(150, 75);
    Money m2(50, 25);
    Money result = m1.subtract(m2);
    
    Money expected(100, 50);
    EXPECT_TRUE(result.isEqual(expected));
}

TEST(MoneyTest, SubtractWithBorrow) {
    Money m1(100, 0);
    Money m2(50, 50);
    Money result = m1.subtract(m2);
    
    Money expected(49, 50);
    EXPECT_TRUE(result.isEqual(expected));
}

TEST(MoneyTest, SubtractNegativeResult) {
    Money m1(50, 0);
    Money m2(100, 0);
    
    EXPECT_THROW(m1.subtract(m2), std::invalid_argument);
}

TEST(MoneyTest, EqualComparison) {
    Money m1(100, 50);
    Money m2(100, 50);
    Money m3(200, 50);
    
    EXPECT_TRUE(m1.isEqual(m2));
    EXPECT_FALSE(m1.isEqual(m3));
}

TEST(MoneyTest, GreaterComparison) {
    Money m1(200, 0);
    Money m2(100, 0);
    
    EXPECT_TRUE(m1.isGreater(m2));
    EXPECT_FALSE(m2.isGreater(m1));
}

TEST(MoneyTest, LessComparison) {
    Money m1(100, 0);
    Money m2(200, 0);
    
    EXPECT_TRUE(m1.isLess(m2));
    EXPECT_FALSE(m2.isLess(m1));
}

TEST(MoneyTest, Immutability) {
    Money m1(100, 50);
    Money m2(50, 25);
    
    Money result = m1.add(m2);
    
    Money original1(100, 50);
    Money original2(50, 25);
    
    EXPECT_TRUE(m1.isEqual(original1));
    EXPECT_TRUE(m2.isEqual(original2));
}