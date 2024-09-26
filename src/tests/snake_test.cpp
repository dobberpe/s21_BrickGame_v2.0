#include "test.h"

TEST(SnakeTest, InitialState) {
  EXPECT_EQ(Snake::get_snake_info()->get_state(), START);
  EXPECT_TRUE(updateCurrentState().pause);
}

TEST(SnakeTest, SpawnApple) {
  userInput(Start, false);
  EXPECT_EQ(Snake::get_snake_info()->get_state(), ROTATE);
  EXPECT_FALSE(updateCurrentState().pause);
}

TEST(SnakeTest, Pause) {
  userInput(Pause, false);
  EXPECT_TRUE(updateCurrentState().pause);
}

TEST(SnakeTest, MoveUp) {
  userInput(Start, false);
  userInput(Up, false);
  userInput(Action, false);
  EXPECT_EQ(updateCurrentState().field[6][6], 1);
}

TEST(SnakeTest, MoveLeft) {
  userInput(Left, false);
  userInput(Action, false);
  EXPECT_EQ(updateCurrentState().field[6][5], 1);
}

TEST(SnakeTest, MoveDown) {
  userInput(Down, false);
  userInput(Action, false);
  EXPECT_EQ(updateCurrentState().field[7][5], 1);
}

TEST(SnakeTest, MoveRight) {
  userInput(Left, false);
  userInput(Action, false);
  userInput(Down, false);
  userInput(Action, false);
  userInput(Right, false);
  userInput(Action, false);
  EXPECT_EQ(updateCurrentState().field[8][5], 1);
}

TEST(SnakeTest, Grow) {
  EXPECT_EQ(updateCurrentState().field[8][6], 0);
  Snake::get_snake_info()->set_apple(6, 8);
  EXPECT_EQ(updateCurrentState().field[8][6], 1);
  EXPECT_EQ(Snake::get_snake_info()->get_state(), ROTATE);
  userInput(Action, false);
  EXPECT_EQ(Snake::get_snake_info()->get_state(), GROW);
  EXPECT_EQ(updateCurrentState().field[8][6], 1);
  EXPECT_EQ(updateCurrentState().field[8][7], 1);
  EXPECT_EQ(updateCurrentState().field[8][5], 1);
  EXPECT_EQ(updateCurrentState().field[8][4], 1);
  EXPECT_EQ(updateCurrentState().field[7][4], 1);
}

TEST(SnakeTest, GameOver) {
  userInput(Action, false);
  userInput(Action, false);
  userInput(Action, false);
  userInput(Action, false);
  EXPECT_EQ(Snake::get_snake_info()->get_state(), GAMEOVER);
}

TEST(SnakeTest, UserInputTerminate) {
  userInput(Terminate, false);
  EXPECT_EQ(Snake::get_snake_info()->get_state(), EXIT);
}