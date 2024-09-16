#include "tetris_test.h"

START_TEST(process_signal_start_escape) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.state = START;
  tetris_info.signal = ESCAPE;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == EXIT);
  ck_assert(tetris_info.field == NULL);
  ck_assert(tetris_info.game_info.field == NULL);
  ck_assert(tetris_info.game_info.next == NULL);
}
END_TEST

START_TEST(process_signal_start_enter) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.state = START;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.field[tetris_info.figure.coords[i].y]
                               [tetris_info.figure.coords[i].x] == TMP);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_spawn) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.field[tetris_info.figure.coords[i].y]
                               [tetris_info.figure.coords[i].x] == TMP);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_left) {
  tetris_t tetris_info;
  figure_t figure;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  copy_figure(tetris_info.figure, &figure);
  tetris_info.state = MOVING;
  tetris_info.signal = LEFT;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.figure.coords[i].x == figure.coords[i].x - 1);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_right) {
  tetris_t tetris_info;
  figure_t figure;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  copy_figure(tetris_info.figure, &figure);
  tetris_info.state = MOVING;
  tetris_info.signal = RIGHT;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.figure.coords[i].x == figure.coords[i].x + 1);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_down) {
  tetris_t tetris_info;
  figure_t figure;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  copy_figure(tetris_info.figure, &figure);
  tetris_info.state = MOVING;
  tetris_info.signal = DOWN;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.figure.coords[i].y == figure.coords[i].y + 1);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_up) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i) ck_assert(tetris_info.figure.coords[i].y == 19);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_i) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 1);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 2);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 3);
  ck_assert(tetris_info.figure.coords[3].x == 4);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_j_1) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(J);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 3);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 3);
  ck_assert(tetris_info.figure.coords[3].y == 2);
  ck_assert(tetris_info.figure.coords[3].x == 3);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_j_2) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(J);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 3);
  ck_assert(tetris_info.figure.coords[1].y == 1);
  ck_assert(tetris_info.figure.coords[1].x == 3);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 5);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_j_3) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(J);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == -1);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 3);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 4);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_l_1) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(L);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 1);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 2);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 2);
  ck_assert(tetris_info.figure.coords[3].x == 5);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_l_2) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(L);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 6);
  ck_assert(tetris_info.figure.coords[1].y == 1);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 5);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 6);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_l_3) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(L);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 5);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 5);
  ck_assert(tetris_info.figure.coords[3].y == 2);
  ck_assert(tetris_info.figure.coords[3].x == 5);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_s) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(S);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == 0);
  ck_assert(tetris_info.figure.coords[0].x == 3);
  ck_assert(tetris_info.figure.coords[1].y == 1);
  ck_assert(tetris_info.figure.coords[1].x == 3);
  ck_assert(tetris_info.figure.coords[2].y == 1);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 2);
  ck_assert(tetris_info.figure.coords[3].x == 4);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_z) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(Z);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == -1);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 3);
  ck_assert(tetris_info.figure.coords[2].y == 0);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 3);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_t_1) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(T);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == -1);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 4);
  ck_assert(tetris_info.figure.coords[2].y == 0);
  ck_assert(tetris_info.figure.coords[2].x == 5);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 4);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_t_2) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(T);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == -1);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 3);
  ck_assert(tetris_info.figure.coords[2].y == 0);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 0);
  ck_assert(tetris_info.figure.coords[3].x == 5);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_moving_enter_t_3) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(T);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  tetris_info.state = MOVING;
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);

  ck_assert(tetris_info.figure.coords[0].y == -1);
  ck_assert(tetris_info.figure.coords[0].x == 4);
  ck_assert(tetris_info.figure.coords[1].y == 0);
  ck_assert(tetris_info.figure.coords[1].x == 3);
  ck_assert(tetris_info.figure.coords[2].y == 0);
  ck_assert(tetris_info.figure.coords[2].x == 4);
  ck_assert(tetris_info.figure.coords[3].y == 1);
  ck_assert(tetris_info.figure.coords[3].x == 4);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_falling) {
  tetris_t tetris_info;
  figure_t figure;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  copy_figure(tetris_info.figure, &figure);
  tetris_info.state = FALLING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == MOVING);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.figure.coords[i].y == figure.coords[i].y + 1);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_attaching) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  tetris_info.signal = NOSIG;
  tetris_info.state = ATTACHING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == SPAWN);
  ck_assert(tetris_info.timer > 0);
  for (int i = 0; i < 4; ++i)
    ck_assert(tetris_info.field[tetris_info.figure.coords[i].y]
                               [tetris_info.figure.coords[i].x] == CONST);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_removing_100) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j)
    tetris_info.field[19][j] = j < 3 || j > 6 ? CONST : EMPTY;
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  tetris_info.signal = NOSIG;
  tetris_info.state = ATTACHING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == REMOVING);
  ck_assert(tetris_info.timer > 0);
  ck_assert(tetris_info.filled_lines[0] == 19);
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j)
    ck_assert(tetris_info.field[19][j] == EMPTY);
  ck_assert(tetris_info.game_info.score == 100);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_removing_300) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j) {
    tetris_info.field[19][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[18][j] = j != 4 ? CONST : EMPTY;
  }
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  tetris_info.signal = NOSIG;
  tetris_info.state = ATTACHING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == REMOVING);
  ck_assert(tetris_info.timer > 0);
  ck_assert(tetris_info.filled_lines[0] == 18);
  ck_assert(tetris_info.filled_lines[1] == 19);
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j) {
    ck_assert(j != 4 ? tetris_info.field[19][j] == EMPTY
                     : tetris_info.field[19][j] == CONST);
    ck_assert(j != 4 ? tetris_info.field[18][j] == EMPTY
                     : tetris_info.field[18][j] == CONST);
  }
  ck_assert(tetris_info.game_info.score == 300);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_removing_700) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j) {
    tetris_info.field[19][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[18][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[17][j] = j != 4 ? CONST : EMPTY;
  }
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  tetris_info.signal = NOSIG;
  tetris_info.state = ATTACHING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == REMOVING);
  ck_assert(tetris_info.timer > 0);
  ck_assert(tetris_info.filled_lines[0] == 17);
  ck_assert(tetris_info.filled_lines[1] == 18);
  ck_assert(tetris_info.filled_lines[2] == 19);
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j)
    ck_assert(j != 4 ? tetris_info.field[19][j] == EMPTY
                     : tetris_info.field[19][j] == CONST);
  ck_assert(tetris_info.game_info.score == 700);
  ck_assert(tetris_info.game_info.speed == 2);

  exitstate(&tetris_info);
}
END_TEST

START_TEST(process_signal_removing_1500) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  tetris_info.next_figure = init_figure(I);
  tetris_info.state = SPAWN;
  process_signal(&tetris_info);
  tetris_info.signal = ENTER;
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j) {
    tetris_info.field[19][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[18][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[17][j] = j != 4 ? CONST : EMPTY;
    tetris_info.field[16][j] = j != 4 ? CONST : EMPTY;
  }
  tetris_info.signal = UP;
  process_signal(&tetris_info);
  tetris_info.signal = NOSIG;
  tetris_info.state = ATTACHING;
  process_signal(&tetris_info);
  ck_assert(tetris_info.state == REMOVING);
  ck_assert(tetris_info.timer > 0);
  ck_assert(tetris_info.filled_lines[0] == 16);
  ck_assert(tetris_info.filled_lines[1] == 17);
  ck_assert(tetris_info.filled_lines[2] == 18);
  ck_assert(tetris_info.filled_lines[3] == 19);
  process_signal(&tetris_info);
  for (int j = 0; j < FIELD_COLS; ++j)
    ck_assert(tetris_info.field[19][j] == EMPTY);
  ck_assert(tetris_info.game_info.score == 1500);
  ck_assert(tetris_info.game_info.speed == 3);

  exitstate(&tetris_info);
}
END_TEST

Suite *process_signal_cases(void) {
  Suite *c = suite_create("process_signal_cases");
  TCase *tc = tcase_create("process_signal_tc");

  tcase_add_test(tc, process_signal_start_escape);
  tcase_add_test(tc, process_signal_start_enter);
  tcase_add_test(tc, process_signal_spawn);
  tcase_add_test(tc, process_signal_moving_left);
  tcase_add_test(tc, process_signal_moving_right);
  tcase_add_test(tc, process_signal_moving_down);
  tcase_add_test(tc, process_signal_moving_up);
  tcase_add_test(tc, process_signal_moving_enter_i);
  tcase_add_test(tc, process_signal_moving_enter_j_1);
  tcase_add_test(tc, process_signal_moving_enter_j_2);
  tcase_add_test(tc, process_signal_moving_enter_j_3);
  tcase_add_test(tc, process_signal_moving_enter_l_1);
  tcase_add_test(tc, process_signal_moving_enter_l_2);
  tcase_add_test(tc, process_signal_moving_enter_l_3);
  tcase_add_test(tc, process_signal_moving_enter_s);
  tcase_add_test(tc, process_signal_moving_enter_z);
  tcase_add_test(tc, process_signal_moving_enter_t_1);
  tcase_add_test(tc, process_signal_moving_enter_t_2);
  tcase_add_test(tc, process_signal_moving_enter_t_3);
  tcase_add_test(tc, process_signal_falling);
  tcase_add_test(tc, process_signal_attaching);
  tcase_add_test(tc, process_signal_removing_100);
  tcase_add_test(tc, process_signal_removing_300);
  tcase_add_test(tc, process_signal_removing_700);
  tcase_add_test(tc, process_signal_removing_1500);

  suite_add_tcase(c, tc);

  return c;
}