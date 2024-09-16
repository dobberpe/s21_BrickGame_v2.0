#include "tetris_test.h"

START_TEST(init_tetris_test) {
  tetris_t tetris_info;
  init_tetris(&tetris_info);
  ck_assert(tetris_info.field != NULL);
  for (int i = 0; i < FIELD_ROWS; ++i)
    for (int j = 0; j < FIELD_COLS; ++j)
      ck_assert(tetris_info.field[i][j] == EMPTY);
  for (int i = 0; i < 4; ++i) ck_assert(tetris_info.filled_lines[i] == -1);
  ck_assert(tetris_info.timer == 0);
  ck_assert(tetris_info.state == START);
  ck_assert(tetris_info.signal == NOSIG);
  ck_assert(tetris_info.game_info.field != NULL);
  for (int i = 0; i < FIELD_ROWS; ++i)
    for (int j = 0; j < FIELD_COLS; ++j)
      ck_assert(tetris_info.game_info.field[i][j] == 0);
  ck_assert(tetris_info.game_info.next != NULL);
  for (int i = 0; i < NEXT_ROWS; ++i)
    for (int j = 0; j < NEXT_COLS; ++j)
      ck_assert(tetris_info.game_info.next[i][j] == 0);
  ck_assert(tetris_info.game_info.score == 0);
  ck_assert(tetris_info.game_info.high_score == 4900);
  ck_assert(tetris_info.game_info.level == 1);
  ck_assert(tetris_info.game_info.speed == 1);
  ck_assert(tetris_info.game_info.pause == 1);

  ck_assert(tetris_info.next_figure.coords[0].y == 0);
  ck_assert(tetris_info.next_figure.coords[0].x ==
            (tetris_info.next_figure.type == L ||
                     tetris_info.next_figure.type == O ||
                     tetris_info.next_figure.type == S
                 ? 4
                 : 3));
  ck_assert(tetris_info.next_figure.coords[1].y == 0);
  ck_assert(tetris_info.next_figure.coords[1].x ==
            (tetris_info.next_figure.type == L ||
                     tetris_info.next_figure.type == O ||
                     tetris_info.next_figure.type == S
                 ? 5
                 : 4));
  ck_assert(tetris_info.next_figure.coords[2].y ==
            (tetris_info.next_figure.type == O ||
                     tetris_info.next_figure.type == S ||
                     tetris_info.next_figure.type == Z
                 ? 1
                 : 0));
  ck_assert(
      tetris_info.next_figure.coords[2].x ==
      (tetris_info.next_figure.type == L   ? 6
       : tetris_info.next_figure.type == S ? 3
       : tetris_info.next_figure.type == O || tetris_info.next_figure.type == Z
           ? 4
           : 5));
  ck_assert(tetris_info.next_figure.coords[3].y ==
            (tetris_info.next_figure.type == I ? 0 : 1));
  ck_assert(tetris_info.next_figure.coords[3].x ==
            (tetris_info.next_figure.type == I    ? 6
             : (tetris_info.next_figure.type & 1) ? 5
                                                  : 4));
  exitstate(&tetris_info);
}
END_TEST

Suite *init_cases(void) {
  Suite *c = suite_create("init_cases");
  TCase *tc = tcase_create("init_tc");

  tcase_add_test(tc, init_tetris_test);

  suite_add_tcase(c, tc);

  return c;
}