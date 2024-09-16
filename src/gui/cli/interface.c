#include "interface.h"

int main() {
  WINDOW *game_window = init_ncurses();
  GameInfo_t game_info = updateCurrentState();
  UserAction_t action = Start;

  print_display(game_window);
  update_display(game_window, &game_info);
  while (action != Terminate) {
    game_info = updateCurrentState();
    if (!game_info.pause) update_display(game_window, &game_info);
    if (get_user_input(game_window, &action, game_info.pause))
      userInput(action, false);
  }

  end_ncurses(game_window);

  return 0;
}

WINDOW *init_ncurses() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(false);
  WINDOW *brick_game_window = newwin(42, 70, 0, 0);
  if (brick_game_window) {
    keypad(brick_game_window, TRUE);
    nodelay(brick_game_window, TRUE);
    start_color();
    init_pair(DEFAULT, COLOR_GREEN, COLOR_BLACK);
    init_pair(UNACTIVE, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAUSE, COLOR_RED, COLOR_BLACK);
    wattrset(brick_game_window, COLOR_PAIR(DEFAULT));
  }

  return brick_game_window;
}

void print_display(WINDOW *brick_game_window) {
  print_box(brick_game_window);

  wattrset(brick_game_window, COLOR_PAIR(UNACTIVE));
  for (int i = 1; i < 41; ++i)
    for (int j = 2; j < 41; ++j)
      mvwprintw(brick_game_window, i, j,
                !((j + 1) % 4)   ? "═"
                : !((j + 2) % 4) ? i & 1 ? "╔" : "╚"
                : !(j % 4)       ? i & 1 ? "╗" : "╝"
                                 : "");
  for (int i = 18; i < 22; ++i)
    for (int j = 51; j < 66; ++j)
      mvwprintw(brick_game_window, i, j,
                !(j % 4)         ? "═"
                : !((j + 1) % 4) ? i & 1 ? "╚" : "╔"
                : !((j - 1) % 4) ? i & 1 ? "╝" : "╗"
                                 : "");
  reset_info(brick_game_window);
  wattrset(brick_game_window, COLOR_PAIR(DEFAULT));

  mvwprintw(brick_game_window, 3, 44, "SCORE");
  mvwprintw(brick_game_window, 4, 44, "HIGHSCORE");
  mvwprintw(brick_game_window, 8, 44, "LEVEL");
  mvwprintw(brick_game_window, 13, 44, "SPEED");
  mvwprintw(brick_game_window, 18, 44, "NEXT");
  mvwprintw(brick_game_window, 23, 44, "SPEED UP");
  mvwprintw(brick_game_window, 3, 63, "0");
  mvwprintw(brick_game_window, 8, 63, "0");
  mvwprintw(brick_game_window, 13, 63, "0");
  mvwprintw(brick_game_window, 23, 62, "0");
  mvwprintw(brick_game_window, 27, 44, "CONTROL:");
  mvwprintw(brick_game_window, 29, 45, "ENTER - START / PAUSE");
  mvwprintw(brick_game_window, 30, 45, "←     - LEFT");
  mvwprintw(brick_game_window, 31, 45, "→     - RIGHT");
  mvwprintw(brick_game_window, 32, 45, "↓     - MOVE DOWN");
  mvwprintw(brick_game_window, 33, 45, "↑     - PUT DOWN");
  mvwprintw(brick_game_window, 34, 45, "SPACE - ROTATE");
  mvwprintw(brick_game_window, 35, 45, "ESC   - QUIT");

  wrefresh(brick_game_window);
}

void print_box(WINDOW *brick_game_window) {
  for (int i = 0; i < 70; ++i) {
    mvwprintw(brick_game_window, 0, i,
              !i        ? "╔"
              : i == 42 ? "╦"
              : i == 69 ? "╗"
                        : "═");
    mvwprintw(brick_game_window, 41, i,
              !i        ? "╚"
              : i == 42 ? "╩"
              : i == 69 ? "╝"
                        : "═");
  }
  for (int i = 1; i < 41; ++i) {
    mvwprintw(brick_game_window, i, 0, "║");
    mvwprintw(brick_game_window, i, 42, "║");
    mvwprintw(brick_game_window, i, 69, "║");
  }
}

void reset_info(WINDOW *brick_game_window) {
  wattrset(brick_game_window, COLOR_PAIR(UNACTIVE));
  mvwprintw(brick_game_window, 3, 58, "000000");
  mvwprintw(brick_game_window, 4, 58, "000000");
  mvwprintw(brick_game_window, 8, 62, "00");
  mvwprintw(brick_game_window, 13, 62, "00");
  mvwprintw(brick_game_window, 23, 61, "00 %%");
  for (int i = 18; i < 22; ++i)
    for (int j = 51; j < 66; ++j)
      mvwprintw(brick_game_window, i, j,
                !(j % 4)         ? "═"
                : !((j + 1) % 4) ? i & 1 ? "╚" : "╔"
                : !((j - 1) % 4) ? i & 1 ? "╝" : "╗"
                                 : "");
  print_pause(brick_game_window, false);
  wattrset(brick_game_window, COLOR_PAIR(DEFAULT));
}

void print_pause(WINDOW *brick_game_window, bool pause) {
  wattrset(brick_game_window, COLOR_PAIR(pause ? PAUSE : UNACTIVE));
  mvwprintw(brick_game_window, 37, 46, "╔═╗ ╔═╗ ╗ ╔ ╔═╗ ╔═╗");
  mvwprintw(brick_game_window, 38, 46, "╠═╝ ╠═╣ ║ ║ ╚═╗ ╠═");
  mvwprintw(brick_game_window, 39, 46, "╝   ╝ ╚ ╚═╝ ╚═╝ ╚═╝");
  wattrset(brick_game_window, COLOR_PAIR(DEFAULT));
}

void update_display(WINDOW *brick_game_window, GameInfo_t *game_info) {
  reset_info(brick_game_window);
  printf_info(brick_game_window, game_info);
  print_field(brick_game_window, game_info->field);
  wrefresh(brick_game_window);
}

void printf_info(WINDOW *brick_game_window, GameInfo_t *game_info) {
  mvwprintw(brick_game_window, 3,
            game_info->score / 100000  ? 58
            : game_info->score / 10000 ? 59
            : game_info->score / 1000  ? 60
            : game_info->score / 100   ? 61
            : game_info->score / 10    ? 62
                                       : 63,
            "%d", game_info->score);
  mvwprintw(brick_game_window, 4,
            game_info->high_score / 100000  ? 58
            : game_info->high_score / 10000 ? 59
            : game_info->high_score / 1000  ? 60
            : game_info->high_score / 100   ? 61
            : game_info->high_score / 10    ? 62
                                            : 63,
            "%d", game_info->high_score);
  mvwprintw(brick_game_window, 8, game_info->level / 10 ? 62 : 63, "%d",
            game_info->level);
  mvwprintw(brick_game_window, 13, game_info->speed / 10 ? 62 : 63, "%d",
            game_info->speed);
  int speed_frac =
      game_info->speed < 10 ? ((game_info->score % 600) * 100) / 600 : 100;
  mvwprintw(brick_game_window, 23,
            speed_frac == 100 ? 60
            : speed_frac / 10 ? 61
                              : 62,
            "%d", speed_frac);

  for (int i = 0; i < NEXT_ROWS; ++i) {
    for (int j = 0; j < NEXT_COLS; ++j) {
      if (!game_info->next[i][j])
        wattrset(brick_game_window, COLOR_PAIR(UNACTIVE));
      mvwprintw(brick_game_window, i * 2 + 18, j * 4 + 51, "╔═╗");
      mvwprintw(brick_game_window, i * 2 + 19, j * 4 + 51, "╚═╝");
      wattrset(brick_game_window, COLOR_PAIR(DEFAULT));
    }
  }

  print_pause(brick_game_window, game_info->pause);
}

void print_field(WINDOW *brick_game_window, int **field) {
  for (int i = 0; i < FIELD_ROWS; ++i) {
    for (int j = 0; j < FIELD_COLS; ++j) {
      if (!field[i][j]) wattrset(brick_game_window, COLOR_PAIR(UNACTIVE));
      mvwprintw(brick_game_window, i * 2 + 1, j * 4 + 2, "╔═╗");
      mvwprintw(brick_game_window, i * 2 + 2, j * 4 + 2, "╚═╝");
      wattrset(brick_game_window, COLOR_PAIR(DEFAULT));
    }
  }
}

bool get_user_input(WINDOW *game_window, UserAction_t *user_action,
                    bool pause) {
  bool pressed = true;
  int c = wgetch(game_window);

  if (c == '\n' && pause)
    *user_action = Start;
  else if (c == '\n' && !pause)
    *user_action = Pause;
  else if (c == KEY_ESC)
    *user_action = Terminate;
  else if (c == KEY_LEFT)
    *user_action = Left;
  else if (c == KEY_RIGHT)
    *user_action = Right;
  else if (c == KEY_UP)
    *user_action = Up;
  else if (c == KEY_DOWN)
    *user_action = Down;
  else if (c == ' ')
    *user_action = Action;
  else
    pressed = false;

  return pressed;
}

void end_ncurses(WINDOW *brick_game_window) {
  wattroff(brick_game_window, COLOR_PAIR(DEFAULT));
  endwin();
}