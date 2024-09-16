#ifndef TETRIS_TEST_H
#define TETRIS_TEST_H

#include <check.h>

#include "../brick_game/tetris/tetris.h"

Suite *userInput_cases(void);
Suite *init_cases(void);
Suite *process_signal_cases(void);

#endif  // TETRIS_TEST_H
