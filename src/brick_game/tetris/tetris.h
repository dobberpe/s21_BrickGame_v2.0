#ifndef TETRIS_H
#define TETRIS_H

#define I_0_X 3  // ▣▣▣▣
#define I_1_X 4
#define I_2_X 5
#define I_3_X 6

#define J_0_X 3  // ▣▣▣
#define J_1_X 4  //     ▣
#define J_2_X 5
#define J_3_X 5
#define J_3_Y 1

#define L_0_X 4  // ▣▣▣
#define L_1_X 5  // ▣
#define L_2_X 6
#define L_3_X 4
#define L_3_Y 1

#define O_0_X 4  // ▣▣
#define O_1_X 5  // ▣▣
#define O_2_X 4
#define O_2_Y 1
#define O_3_X 5
#define O_3_Y 1

#define S_0_X 4  //   ▣▣
#define S_1_X 5  // ▣▣
#define S_2_X 3
#define S_2_Y 1
#define S_3_X 4
#define S_3_Y 1

#define Z_0_X 3  // ▣▣
#define Z_1_X 4  //   ▣▣
#define Z_2_X 4
#define Z_2_Y 1
#define Z_3_X 5
#define Z_3_Y 1

#define T_0_X 3  // ▣▣▣
#define T_1_X 4  //   ▣
#define T_2_X 5
#define T_3_X 4
#define T_3_Y 1

#define FALL_DELAY                                      \
  tetris_info->game_info.speed < 5                      \
      ? 1000 - (tetris_info->game_info.speed - 1) * 250 \
      : 200 - (tetris_info->game_info.speed - 5) * 25

#define HIGHSCORE_FILE "build/tetris.score"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../../gui/cli/interface.h"
#include "../library_specification.h"

typedef enum { I, J, L, O, S, Z, T } type_t;

typedef struct {
  int x;
  int y;
} coordinate_t;

typedef enum { normal, counterclockwise, inverted, clockwise } rotation_t;

typedef struct {
  type_t type;
  coordinate_t coords[4];
  rotation_t rotation;
} figure_t;

typedef enum { EMPTY, TMP, CONST } field_t;

typedef enum {
  START,
  SPAWN,
  MOVING,
  FALLING,
  ATTACHING,
  REMOVING,
  GAMEOVER,
  EXIT
} tetris_fsm_t;

typedef enum { ESCAPE, LEFT, RIGHT, UP, DOWN, ENTER, NOSIG } tetris_signal_t;

typedef struct {
  GameInfo_t game_info;
  field_t **field;
  figure_t figure;
  figure_t next_figure;
  int filled_lines[4];
  long long timer;
  tetris_fsm_t state;
  tetris_signal_t signal;
} tetris_t;

void tetris();
tetris_t *get_tetris_info();
void init_tetris(tetris_t *tetris_info);
GameInfo_t init_game_info();
figure_t init_figure(type_t type);
void process_signal(tetris_t *tetris_info);
void update_game_info(tetris_t *tetris_info, type_t next_figure_type);

int read_highscore(char *filename);
void write_highscore(char *filename, int *highscore);

typedef void (*action)(tetris_t *tetris_info);

void spawn(tetris_t *tetris_info);
void rotate(tetris_t *tetris_info);
void moveleft(tetris_t *tetris_info);
void moveright(tetris_t *tetris_info);
void put_down(tetris_t *tetris_info);
void movedown(tetris_t *tetris_info);
void check_timer(tetris_t *tetris_info);
void falling(tetris_t *tetris_info);
void attaching(tetris_t *tetris_info);
void remove_line(tetris_t *tetris_info);
void gameover(tetris_t *tetris_info);
void exitstate(tetris_t *tetris_info);

long long get_time_ms();
bool timer(long long *prev_time, long long curr_time, int delay);

void remove_temp(field_t **field);
void update_field(field_t **field, figure_t figure, field_t value);
void add_figure(field_t **field, figure_t figure, field_t value);
void rotate_i(figure_t *figure, figure_t *rotated);
void rotate_j(figure_t *figure, figure_t *rotated);
void rotate_l(figure_t *figure, figure_t *rotated);
void rotate_s(figure_t *figure, figure_t *rotated);
void rotate_z(figure_t *figure, figure_t *rotated);
void rotate_t(figure_t *figure, figure_t *rotated);
void copy_figure(figure_t src, figure_t *dst);
bool check_intersection(field_t **field, figure_t figure);
bool move_figure(field_t **field, figure_t *figure, UserAction_t user_action);
bool fall(field_t **field, figure_t *figure);
bool fallen(field_t **field, figure_t figure);
bool check_loss(field_t **field);
bool full_lines(field_t **field, int *lines);
void replace_line(field_t **field, int i);

#endif  // TETRIS_H
