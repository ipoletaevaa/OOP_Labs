#include <gtest/gtest.h>
#include "../include/hexagon.h"
#include "../include/octagon.h"
#include "../include/triangle.h"
#include "../include/array.h"

TEST(HexagonTest, AreaCalculation) {
    Hexagon h(0, 0, 1);
    EXPECT_NEAR(static_cast<double>(h), 2.598, 0.001);
}

TEST(OctagonTest, AreaCalculation) {
    Octagon o(0, 0, 1);
    EXPECT_NEAR(static_cast<double>(o), 4.828, 0.001); 
}

TEST(TriangleTest, AreaCalculation) {
    Triangle t(0, 0, 1);
    EXPECT_NEAR(static_cast<double>(t), 0.433, 0.001); 
}

TEST(ArrayTest, MixedFigures) {
    Array array;
    array.add_figure(std::make_shared<Hexagon>(0, 0, 1));
    array.add_figure(std::make_shared<Octagon>(0, 0, 1));
    array.add_figure(std::make_shared<Triangle>(0, 0, 1));
    
    EXPECT_EQ(array.size(), 3);
    
    double total = 2.598 + 4.828 + 0.433; 
    EXPECT_NEAR(array.total_area(), total, 0.01);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}