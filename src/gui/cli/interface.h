#ifndef INTERFACE_H
#define INTERFACE_H

#define KEY_ESC 27
#define DEFAULT 1
#define UNACTIVE 2
#define PAUSE 3

#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../brick_game/library_specification.h"

WINDOW *init_ncurses();
void print_display(WINDOW *brick_game_window);
void print_box(WINDOW *brick_game_window);
void reset_info(WINDOW *brick_game_window);
void print_pause(WINDOW *brick_game_window, bool pause);
void update_display(WINDOW *brick_game_window, GameInfo_t *game_info);
void printf_info(WINDOW *brick_game_window, GameInfo_t *game_info);
void print_field(WINDOW *brick_game_window, int **field);
bool get_user_input(WINDOW *game_window, UserAction_t *user_action, bool pause);
void print_result(WINDOW *brick_game_window, GameInfo_t *game_info);
void end_ncurses(WINDOW *game_window);
void debug_log(const char *message);

#endif  // INTERFACE_H
