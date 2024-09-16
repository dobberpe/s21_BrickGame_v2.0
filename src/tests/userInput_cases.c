#include "tetris_test.h"

START_TEST(userInput_Start) {
  GameInfo_t gameInfo = updateCurrentState();
  ck_assert(gameInfo.pause);
  userInput(Start, false);
  gameInfo = updateCurrentState();
  ck_assert(!gameInfo.pause);
  userInput(Terminate, false);
}
END_TEST

START_TEST(userInput_Pause) {
  GameInfo_t gameInfo = updateCurrentState();
  ck_assert(!gameInfo.pause);
  userInput(Pause, false);
  gameInfo = updateCurrentState();
  ck_assert(gameInfo.pause);
  userInput(Terminate, false);
}
END_TEST

START_TEST(userInput_Terminate) {
  GameInfo_t gameInfo = updateCurrentState();
  ck_assert(gameInfo.pause);
  userInput(Terminate, false);
  gameInfo = updateCurrentState();
  ck_assert(gameInfo.field == NULL);
  ck_assert(gameInfo.next == NULL);
}
END_TEST

Suite *userInput_cases(void) {
  Suite *c = suite_create("userInput_cases");
  TCase *tc = tcase_create("userInput_tc");

  tcase_add_test(tc, userInput_Start);
  tcase_add_test(tc, userInput_Pause);
  tcase_add_test(tc, userInput_Terminate);

  suite_add_tcase(c, tc);

  return c;
}