#include "test.h"

TEST(CoordinateTest, TestEquality) {
  Coordinate coord1(5, 10);
  Coordinate coord2(5, 10);
  EXPECT_TRUE(coord1 == coord2);
}

TEST(CoordinateTest, TestMovement) {
  Coordinate coord(5, 10);
  EXPECT_EQ(coord.left(), Coordinate(4, 10));
  EXPECT_EQ(coord.right(), Coordinate(6, 10));
  EXPECT_EQ(coord.up(), Coordinate(5, 9));
  EXPECT_EQ(coord.down(), Coordinate(5, 11));
}

TEST(CoordinateTest, TestOutOfField) {
  Coordinate coord(0, 0);
  EXPECT_TRUE(coord.out_of_field(1, 10, 1, 10));   // За пределами
  EXPECT_FALSE(coord.out_of_field(0, 10, 0, 10));  // В пределах
}