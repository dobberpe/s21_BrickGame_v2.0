#ifndef LIBRARY_SPECIFICATION_H
#define LIBRARY_SPECIFICATION_H

#include <stdbool.h>

#define FIELD_ROWS 20
#define FIELD_COLS 10

#define NEXT_ROWS 2
#define NEXT_COLS 4

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
// void debug_log(const char *message);

#endif  // LIBRARY_SPECIFICATION_H