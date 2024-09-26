#ifndef SNAKE_H
#define SNAKE_H

#include <algorithm>
#include <chrono>
#include <deque>
#include <fstream>
#include <random>

#include "../library_specification.h"

#define FALL_DELAY                                              \
  ((game_info.speed < 5) ? (1000 - (game_info.speed - 1) * 250) \
                         : (200 - (game_info.speed - 5) * 25))

#define HIGHSCORE_FILE "build/snake.score"

using namespace std;

namespace s21 {

typedef enum { START, SPAWN, ROTATE, MOVE, GROW, GAMEOVER, EXIT } snake_fsm_t;

typedef enum { ESCAPE, LEFT, RIGHT, UP, DOWN, ENTER, NOSIG } snake_signal_t;

typedef enum { left, right, up, down } direction_t;

class Coordinate {
 public:
  Coordinate(int x, int y);
  bool operator==(const Coordinate& other) const;
  Coordinate& operator=(const Coordinate& other);
  Coordinate left() const;
  Coordinate right() const;
  Coordinate up() const;
  Coordinate down() const;
  bool out_of_field(int left, int right, int up, int down) const;
  int x, y;
};

class Snake {
 public:
  Snake(const Snake&) = delete;
  void operator=(const Snake&) = delete;

  static Snake* get_snake_info();
  void set_pause(bool value);
  snake_fsm_t get_state() const;
  void set_signal(snake_signal_t value);
  void process_signal();
  void update_game_info();
  GameInfo_t get_game_info() const;
  static void debug_log(const string& message);
  static void clear_log();

 private:
  deque<Coordinate> snake_body;
  direction_t curr_dir;
  direction_t new_dir;
  Coordinate apple;
  GameInfo_t game_info;
  snake_fsm_t state;
  snake_signal_t signal;
  chrono::steady_clock::time_point timer;
  random_device rd;
  mt19937 gen;
  uniform_int_distribution<> distrib;

  Snake();
  ~Snake();

  void spawn();
  void dir_left();
  void dir_right();
  void dir_up();
  void dir_down();
  void check_timer();
  void move();
  void grow();
  void gameover();
  void exitstate();
  void read_highscore();
  void write_highscore() const;
};

typedef void (Snake::*action)();

}  // namespace s21

#endif  // SNAKE_H
