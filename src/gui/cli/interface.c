#include "interface.h"

int main() {
  WINDOW *game_window = init_ncurses();
  GameInfo_t game_info = updateCurrentState();
  UserAction_t action = Start;

  print_display(game_window);
  update_display(game_window, &game_info);
  while (action != Terminate) {
    if (!game_info.pause) {
      game_info = updateCurrentState();
      debug_log("back in cycle");
      for (int i = 0; i < FIELD_ROWS; i++) for (int j = 0; j < FIELD_COLS; j++) {
          char buffer[100];
      	  sprintf(buffer, "%d", game_info.field[i][j]);
      	  debug_log(buffer);
      }
      update_display(game_window, &game_info);
    }
    if (get_user_input(game_window, &action, game_info.pause)) userInput(action, false);
  }
  game_info = updateCurrentState(); // очистка

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

  for (int i = 3; i <= 23; i += 5)
    mvwprintw(brick_game_window, i, 44,
              i == 3    ? "SCORE"
              : i == 8  ? "LEVEL"
              : i == 13 ? "SPEED"
              : i == 18 ? "NEXT"
                        : "SPEED UP");
  mvwprintw(brick_game_window, 3, 63, "0");
  mvwprintw(brick_game_window, 8, 63, "0");
  mvwprintw(brick_game_window, 13, 63, "0");
  mvwprintw(brick_game_window, 23, 62, "0");
  mvwprintw(brick_game_window, 27, 44, "CONTROL:");
  for (int i = 29; i < 36; ++i)
    mvwprintw(brick_game_window, i, 47, "%s",
              i == 29   ? "SPACE - START"
              : i == 30 ? "SPACE - PAUSE"
              : i == 31 ? ">     - RIGHT"
              : i == 32 ? "<     - LEFT"
              : i == 33 ? "v     - PUT DOWN"
              : i == 34 ? "ENTER - ROTATE"
                        : "ESC   - QUIT");

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
  mvwprintw(brick_game_window, 3, 60, "0000");
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
  debug_log("update display");
  reset_info(brick_game_window);
  printf_info(brick_game_window, game_info);
  print_field(brick_game_window, game_info->field);
  wrefresh(brick_game_window);
}

void printf_info(WINDOW *brick_game_window, GameInfo_t *game_info) {
  mvwprintw(brick_game_window, 3,
            game_info->score / 1000  ? 60
            : game_info->score / 100 ? 61
            : game_info->score / 10  ? 62
                                     : 63,
            "%d", game_info->score);
  mvwprintw(brick_game_window, 8, game_info->level / 10 ? 62 : 63, "%d",
            game_info->level);
  mvwprintw(brick_game_window, 13, game_info->speed / 10 ? 62 : 63, "%d",
            game_info->speed);
  int speed_frac = ((game_info->score % 600) * 100) / 600;
  mvwprintw(brick_game_window, 23, speed_frac / 10 ? 61 : 62, "%d", speed_frac);

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

bool get_user_input(WINDOW *game_window, UserAction_t *user_action, bool pause) {

//  debug_log("get user input");

  bool pressed = true;
  int c = wgetch(game_window);

  if (c == ' ' && pause) *user_action = Start;
  else if (c == ' ' && !pause) *user_action = Pause;
  else if (c == KEY_ESC) *user_action = Terminate;
  else if (c == KEY_LEFT) *user_action = Left;
  else if (c == KEY_RIGHT) *user_action = Right;
  else if (c == KEY_UP) *user_action = Up;
  else if (c == KEY_DOWN) *user_action = Down;
  else if (c == KEY_ENTER) *user_action = Action;
  else pressed = false;

  return pressed;
}

void print_result(WINDOW *brick_game_window, GameInfo_t *game_info) {
  for (int i = 15; i < 27; ++i)
    for (int j = 10; j < 33; ++j)
      mvwprintw(brick_game_window, i, j,
                j == 10 || j == 32 ? " " :
                i == 15 || i == 23   ? j == 11   ? "╔"
                                       : j == 31 ? "╗"
                                                 : "═"
                : i == 22 || i == 26 ? j == 11   ? "╚"
                                       : j == 31 ? "╝"
                                                 : "═"
                : j == 11 || j == 31 ? "║"
                                     : " ");

  mvwprintw(brick_game_window, 16, 13, "╔═╗ ╔═╗ ╔═╦═╗ ╔═╗");
  mvwprintw(brick_game_window, 17, 13, "║ ╗ ╠═╣ ║ ║ ║ ╠═");
  mvwprintw(brick_game_window, 18, 13, "╚═╝ ╝ ╚ ╝   ╚ ╚═╝");
  mvwprintw(brick_game_window, 19, 13, "╔═╗ ╗  ╔ ╔═╗ ╔═╗");
  mvwprintw(brick_game_window, 20, 13, "║ ║ ╚╗╔╝ ╠═  ╠═╩╗");
  mvwprintw(brick_game_window, 21, 13, "╚═╝  ╚╝  ╚═╝ ╝  ╚");
  mvwprintw(brick_game_window, 24, 14, "YOUR SCORE %6d", game_info->score);
  mvwprintw(brick_game_window, 25, 14, "HIGHSCORE  %6d",
            max(game_info->high_score, game_info->score));
  wrefresh(brick_game_window);
}

void end_ncurses(WINDOW *brick_game_window) {
  wattroff(brick_game_window, COLOR_PAIR(DEFAULT));
  endwin();
}