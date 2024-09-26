#include "tetris.h"

void userInput(UserAction_t action, bool hold) {
  (void)hold;

  tetris_t *tetris_info = get_tetris_info();

  if (action == Start) {
    if (tetris_info->state != GAMEOVER) tetris_info->game_info.pause = false;
    if (tetris_info->state == START) tetris_info->signal = ENTER;
  } else if (action == Pause)
    tetris_info->game_info.pause = true;
  else if (action == Terminate)
    tetris_info->signal = ESCAPE;
  else if (tetris_info->state == MOVING) {
    tetris_info->signal = action - 2;
  }

  process_signal(tetris_info);
}

GameInfo_t updateCurrentState() {
  tetris_t *tetris_info = get_tetris_info();
  process_signal(tetris_info);
  if (tetris_info->state != EXIT)
    update_game_info(tetris_info, tetris_info->next_figure.type);

  return tetris_info->game_info;
}

tetris_t *get_tetris_info() {
  static tetris_t tetris_info = {
      .timer = -1,
  };

  if (tetris_info.timer == -1) init_tetris(&tetris_info);

  return &tetris_info;
}

void update_game_info(tetris_t *tetris_info, type_t next_figure_type) {
  for (int i = 0; i < FIELD_ROWS; ++i)
    for (int j = 0; j < FIELD_COLS; ++j)
      tetris_info->game_info.field[i][j] =
          (int)(tetris_info->field[i][j] != EMPTY);

  tetris_info->game_info.next[0][0] =
      next_figure_type != L && next_figure_type != O && next_figure_type != S;
  tetris_info->game_info.next[0][1] = true;
  tetris_info->game_info.next[0][2] = next_figure_type != Z;
  tetris_info->game_info.next[0][3] =
      next_figure_type == I || next_figure_type == L;
  tetris_info->game_info.next[1][0] = next_figure_type == S;
  tetris_info->game_info.next[1][1] =
      next_figure_type != I && next_figure_type != J;
  tetris_info->game_info.next[1][2] =
      next_figure_type == J || next_figure_type == O || next_figure_type == Z;
}

void init_tetris(tetris_t *tetris_info) {
  srand(time(NULL));
  tetris_info->game_info = init_game_info();
  tetris_info->field = (field_t **)malloc(sizeof(field_t *) * FIELD_ROWS);
  for (int i = 0; i < FIELD_ROWS; ++i) {
    tetris_info->field[i] = (field_t *)malloc(sizeof(field_t) * FIELD_COLS);
    for (int j = 0; j < FIELD_COLS; ++j) tetris_info->field[i][j] = EMPTY;
  }
  tetris_info->next_figure = init_figure(rand() % 7);
  for (int i = 0; i < 4; ++i) tetris_info->filled_lines[i] = -1;
  tetris_info->timer = 0;
  tetris_info->state = START;
  tetris_info->signal = NOSIG;
}

GameInfo_t init_game_info() {
  GameInfo_t game_info;

  game_info.field = (int **)malloc(FIELD_ROWS * sizeof(int *));
  for (int i = 0; i < FIELD_ROWS; ++i) {
    game_info.field[i] = (int *)malloc(FIELD_COLS * sizeof(int));
    for (int j = 0; j < FIELD_COLS; ++j) game_info.field[i][j] = 0;
  }

  game_info.next = (int **)malloc(NEXT_ROWS * sizeof(int *));
  for (int i = 0; i < NEXT_ROWS; ++i) {
    game_info.next[i] = (int *)malloc(NEXT_COLS * sizeof(int));
    for (int j = 0; j < NEXT_COLS; ++j) game_info.next[i][j] = 0;
  }

  game_info.score = 0;
  game_info.high_score = read_highscore(HIGHSCORE_FILE);
  game_info.level = 1;
  game_info.speed = 1;
  game_info.pause = 1;
  return game_info;
}

int read_highscore(char *filename) {
  FILE *file = fopen(filename, "rb");
  int highscore = 0;

  if (file) {
    fread(&highscore, sizeof(int), 1, file);
    fclose(file);
  }

  return highscore;
}

void write_highscore(char *filename, int *highscore) {
  FILE *file = fopen(filename, "wb");
  if (file) {
    fwrite(highscore, sizeof(int), 1, file);
    fclose(file);
  }
}

figure_t init_figure(type_t type) {
  figure_t figure;
  figure.type = type;

  for (int i = 0; i < 4; ++i) {
    figure.coords[i].x = 0;
    figure.coords[i].y = 0;
  }
  figure.coords[0].x =
      figure.type == L || figure.type == O || figure.type == S ? 4 : 3;
  figure.coords[1].x =
      figure.type == L || figure.type == O || figure.type == S ? 5 : 4;
  figure.coords[2].x = figure.type == L                       ? L_2_X
                       : figure.type == S                     ? S_2_X
                       : figure.type == O || figure.type == Z ? 4
                                                              : 5;
  figure.coords[2].y = figure.type == O || figure.type == S || figure.type == Z
                           ? 1
                           : figure.coords[2].y;
  figure.coords[3].x =
      figure.type == I                                           ? I_3_X
      : figure.type == J || figure.type == O || figure.type == Z ? 5
                                                                 : 4;
  figure.coords[3].y = figure.type == I ? figure.coords[3].y : 1;

  figure.rotation = normal;

  return figure;
}

void process_signal(tetris_t *tetris_info) {
  action fsm_table[8][7] = {
      {exitstate, NULL, NULL, NULL, NULL, spawn, NULL},
      {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
      {exitstate, moveleft, moveright, put_down, movedown, rotate, check_timer},
      {falling, falling, falling, falling, falling, falling, falling},
      {attaching, attaching, attaching, attaching, attaching, attaching,
       attaching},
      {remove_line, remove_line, remove_line, remove_line, remove_line,
       remove_line, remove_line},
      {exitstate, gameover, gameover, gameover, gameover, gameover, gameover},
      {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
       exitstate}};

  action act = fsm_table[tetris_info->state][tetris_info->signal];

  if (act) act(tetris_info);

  tetris_info->signal =
      tetris_info->state == SPAWN || tetris_info->state == FALLING
          ? tetris_info->signal
          : NOSIG;
}

void spawn(tetris_t *tetris_info) {
  copy_figure(tetris_info->next_figure, &(tetris_info->figure));
  tetris_info->next_figure = init_figure(rand() % 7);
  tetris_info->timer = get_time_ms();
  update_field(tetris_info->field, tetris_info->figure, TMP);
  tetris_info->state = MOVING;
}

void moveleft(tetris_t *tetris_info) {
  move_figure(tetris_info->field, &(tetris_info->figure), Left);
  check_timer(tetris_info);
}

void moveright(tetris_t *tetris_info) {
  move_figure(tetris_info->field, &(tetris_info->figure), Right);
  check_timer(tetris_info);
}

void put_down(tetris_t *tetris_info) {
  while (!fall(tetris_info->field, &(tetris_info->figure)))
    ;
  check_timer(tetris_info);
}

void movedown(tetris_t *tetris_info) {
  fall(tetris_info->field, &(tetris_info->figure));
  check_timer(tetris_info);
}

void rotate(tetris_t *tetris_info) {
  figure_t rotated;
  copy_figure(tetris_info->figure, &rotated);

  if (tetris_info->figure.type == I) {
    rotate_i(&(tetris_info->figure), &rotated);
  } else if (tetris_info->figure.type == J) {
    rotate_j(&(tetris_info->figure), &rotated);
  } else if (tetris_info->figure.type == L) {
    rotate_l(&(tetris_info->figure), &rotated);
  } else if (tetris_info->figure.type == S) {
    rotate_s(&(tetris_info->figure), &rotated);
  } else if (tetris_info->figure.type == Z) {
    rotate_z(&(tetris_info->figure), &rotated);
  } else {  // T
    rotate_t(&(tetris_info->figure), &rotated);
  }

  int left_pos = min(min(rotated.coords[0].x, rotated.coords[1].x),
                     min(rotated.coords[2].x, rotated.coords[3].x));
  int right_pos = max(max(rotated.coords[0].x, rotated.coords[1].x),
                      max(rotated.coords[2].x, rotated.coords[3].x));
  while (++left_pos <= 0) move_figure(tetris_info->field, &rotated, Right);
  while (--right_pos >= 9) move_figure(tetris_info->field, &rotated, Left);

  if (check_intersection(
          tetris_info->field,
          rotated)) {  // проверка на пересечение с постоянными фигурами
    for (int i = 0; i < 4; ++i) {
      tetris_info->figure.coords[i].x = rotated.coords[i].x;
      tetris_info->figure.coords[i].y = rotated.coords[i].y;
    }
    tetris_info->figure.rotation = (tetris_info->figure.rotation + 1) % 4;
  }

  check_timer(tetris_info);
}

void check_timer(tetris_t *tetris_info) {
  if (timer(&(tetris_info->timer), get_time_ms(), FALL_DELAY))
    tetris_info->state = FALLING;
}

void falling(tetris_t *tetris_info) {
  if (fall(tetris_info->field, &(tetris_info->figure)))
    tetris_info->state = ATTACHING;
  else {
    update_field(tetris_info->field, tetris_info->figure, TMP);
    tetris_info->state = MOVING;
  }
}

void attaching(tetris_t *tetris_info) {
  update_field(tetris_info->field, tetris_info->figure, CONST);
  tetris_info->state =
      check_loss(tetris_info->field)                              ? GAMEOVER
      : full_lines(tetris_info->field, tetris_info->filled_lines) ? REMOVING
                                                                  : SPAWN;
}

void remove_line(tetris_t *tetris_info) {
  int line_counter = -1;
  while (++line_counter < 4 && tetris_info->filled_lines[line_counter] != -1)
    replace_line(tetris_info->field, tetris_info->filled_lines[line_counter]);

  tetris_info->game_info.score += line_counter == 1   ? 100
                                  : line_counter == 2 ? 300
                                  : line_counter == 3 ? 700
                                  : line_counter == 4 ? 1500
                                                      : 0;

  tetris_info->game_info.high_score =
      max(tetris_info->game_info.score, tetris_info->game_info.high_score);

  if (tetris_info->game_info.score < 6000) {
    tetris_info->game_info.level = tetris_info->game_info.score / 600 + 1;
    tetris_info->game_info.speed = tetris_info->game_info.score / 600 + 1;
  }
  tetris_info->state = SPAWN;
}

void gameover(tetris_t *tetris_info) {
  tetris_info->game_info.pause = true;
}

void exitstate(tetris_t *tetris_info) {
  write_highscore(HIGHSCORE_FILE, &(tetris_info->game_info.high_score));

  if (tetris_info->game_info.field) {
    for (int i = 0; i < FIELD_ROWS; ++i) free(tetris_info->game_info.field[i]);
    free(tetris_info->game_info.field);
    tetris_info->game_info.field = NULL;
  }

  if (tetris_info->game_info.next) {
    for (int i = 0; i < NEXT_ROWS; ++i) free(tetris_info->game_info.next[i]);
    free(tetris_info->game_info.next);
    tetris_info->game_info.next = NULL;
  }

  if (tetris_info->field) {
    for (int i = 0; i < FIELD_ROWS; ++i) free(tetris_info->field[i]);
    free(tetris_info->field);
    tetris_info->field = NULL;
  }

  tetris_info->state = EXIT;
}

long long get_time_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool timer(long long *prev_time, long long curr_time, int delay) {
  bool res = false;
  if (curr_time - *prev_time >= delay) {
    *prev_time = curr_time;
    res = true;
  }
  return res;
}

void update_field(field_t **field, figure_t figure, field_t value) {
  remove_temp(field);
  add_figure(field, figure, value);
}

void remove_temp(field_t **field) {
  for (int i = 0; i < FIELD_ROWS; ++i)
    for (int j = 0; j < FIELD_COLS; ++j) {
      field[i][j] = field[i][j] == TMP ? EMPTY : field[i][j];
    }
}

void add_figure(field_t **field, figure_t figure, field_t value) {
  for (int i = 0; i < 4; ++i)
    if (figure.coords[i].y >= 0)
      field[figure.coords[i].y][figure.coords[i].x] = value;
}

void rotate_i(figure_t *figure, figure_t *rotated) {
  int x = figure->coords[0].x - 2;
  int y = figure->coords[0].y - 1;
  for (int i = 0; i < 4; ++i) {
    rotated->coords[i].x =
        figure->rotation == normal || figure->rotation == inverted
            ? figure->coords[1].x
            : ++x;
    rotated->coords[i].y =
        figure->rotation == normal || figure->rotation == inverted
            ? ++y
            : figure->coords[0].y;
  }
}

void rotate_j(figure_t *figure, figure_t *rotated) {
  rotated->coords[0].y += figure->rotation == inverted    ? -1
                          : figure->rotation == clockwise ? 1
                                                          : 0;
  rotated->coords[0].x += figure->rotation == inverted    ? 1
                          : figure->rotation == clockwise ? -1
                                                          : 0;
  rotated->coords[1].y += figure->rotation == counterclockwise ? 1
                          : figure->rotation == inverted       ? -1
                                                               : 0;
  rotated->coords[1].x += figure->rotation == counterclockwise ? -1
                          : figure->rotation == inverted       ? 1
                                                               : 0;
  rotated->coords[2].y += figure->rotation == normal      ? 1
                          : figure->rotation == clockwise ? -1
                                                          : 0;
  rotated->coords[2].x += figure->rotation == normal             ? -2
                          : figure->rotation == counterclockwise ? 1
                          : figure->rotation == inverted         ? -1
                                                                 : 2;
  rotated->coords[3].y += figure->rotation == normal             ? 1
                          : figure->rotation == counterclockwise ? -1
                                                                 : 0;
  rotated->coords[3].x += figure->rotation == normal             ? -2
                          : figure->rotation == counterclockwise ? 2
                          : figure->rotation == inverted         ? -1
                                                                 : 1;
}

void rotate_l(figure_t *figure, figure_t *rotated) {
  rotated->coords[0].x += figure->rotation == counterclockwise ? 2
                          : figure->rotation == inverted       ? -2
                                                               : 0;
  rotated->coords[1].y += figure->rotation == normal     ? 1
                          : figure->rotation == inverted ? -1
                                                         : 0;
  rotated->coords[1].x += figure->rotation == normal     ? -1
                          : figure->rotation == inverted ? 1
                                                         : 0;
  rotated->coords[2].y +=
      figure->rotation == normal ? 2
      : figure->rotation == counterclockwise || figure->rotation == clockwise
          ? -1
          : 0;
  rotated->coords[2].x +=
      figure->rotation == normal ? -2
      : figure->rotation == counterclockwise || figure->rotation == clockwise
          ? 1
          : 0;
  rotated->coords[3].y +=
      figure->rotation == normal || figure->rotation == inverted ? 1
      : figure->rotation == counterclockwise || figure->rotation == clockwise
          ? -1
          : 0;
  rotated->coords[3].x +=
      figure->rotation == normal || figure->rotation == counterclockwise ? 1
      : figure->rotation == inverted || figure->rotation == clockwise    ? -1
                                                                         : 0;
}

void rotate_s(figure_t *figure, figure_t *rotated) {
  rotated->coords[0].x +=
      figure->rotation == normal || figure->rotation == inverted ? -1 : 1;
  rotated->coords[1].y +=
      figure->rotation == normal || figure->rotation == inverted ? 1 : -1;
  rotated->coords[1].x +=
      figure->rotation == normal || figure->rotation == inverted ? -2 : 2;
  rotated->coords[2].x +=
      figure->rotation == normal || figure->rotation == inverted ? 1 : -1;
  rotated->coords[3].y +=
      figure->rotation == normal || figure->rotation == inverted ? 1 : -1;
}

void rotate_z(figure_t *figure, figure_t *rotated) {
  rotated->coords[0].y +=
      figure->rotation == normal || figure->rotation == inverted ? -1 : 1;
  rotated->coords[0].x +=
      figure->rotation == normal || figure->rotation == inverted ? 1 : -1;
  rotated->coords[1].x +=
      figure->rotation == normal || figure->rotation == inverted ? -1 : 1;
  rotated->coords[2].y +=
      figure->rotation == normal || figure->rotation == inverted ? -1 : 1;
  rotated->coords[3].x +=
      figure->rotation == normal || figure->rotation == inverted ? -2 : 2;
}

void rotate_t(figure_t *figure, figure_t *rotated) {
  rotated->coords[0].y += figure->rotation == normal      ? -1
                          : figure->rotation == clockwise ? 1
                                                          : 0;
  rotated->coords[0].x += figure->rotation == normal      ? 1
                          : figure->rotation == clockwise ? -1
                                                          : 0;
  rotated->coords[1].x += figure->rotation == counterclockwise ? -1
                          : figure->rotation == clockwise      ? 1
                                                               : 0;
  rotated->coords[2].x += figure->rotation == counterclockwise ? -1
                          : figure->rotation == clockwise      ? 1
                                                               : 0;
  rotated->coords[3].y += figure->rotation == counterclockwise ? -1
                          : figure->rotation == inverted       ? 1
                                                               : 0;
  rotated->coords[3].x += figure->rotation == counterclockwise ? 1
                          : figure->rotation == inverted       ? -1
                                                               : 0;
}

void copy_figure(figure_t src, figure_t *dst) {
  dst->type = src.type;
  dst->coords[0].x = src.coords[0].x;
  dst->coords[0].y = src.coords[0].y;
  dst->coords[1].x = src.coords[1].x;
  dst->coords[1].y = src.coords[1].y;
  dst->coords[2].x = src.coords[2].x;
  dst->coords[2].y = src.coords[2].y;
  dst->coords[3].x = src.coords[3].x;
  dst->coords[3].y = src.coords[3].y;
  dst->rotation = src.rotation;
}

bool check_intersection(field_t **field, figure_t figure) {
  bool res = true;

  int i = -1;
  while (++i < 4 && res)
    if (figure.coords[i].y >= 0)
      res = figure.coords[i].y >= 19 ||
                    field[figure.coords[i].y][figure.coords[i].x] == CONST
                ? false
                : res;

  return res;
}

bool move_figure(field_t **field, figure_t *figure, UserAction_t user_action) {
  bool moved = user_action == Left
                   ? min(min(figure->coords[0].x, figure->coords[1].x),
                         min(figure->coords[2].x, figure->coords[3].x))
                   : max(max(figure->coords[0].x, figure->coords[1].x),
                         max(figure->coords[2].x, figure->coords[3].x)) < 9;

  for (int i = 0; i < 4 && moved; ++i)
    moved = figure->coords[i].y < 0 ||
            field[figure->coords[i].y]
                 [figure->coords[i].x + (user_action == Right ? 1 : -1)] !=
                CONST;

  if (user_action == Left && moved)
    for (int i = 0; i < 4; ++i) --figure->coords[i].x;
  else if (user_action == Right && moved)
    for (int i = 0; i < 4; ++i) ++figure->coords[i].x;

  return moved;
}

bool fall(field_t **field, figure_t *figure) {
  bool figure_fell = fallen(field, *figure);
  if (!figure_fell)
    for (int i = 0; i < 4; ++i) ++figure->coords[i].y;

  return figure_fell;
}

bool fallen(field_t **field, figure_t figure) {
  bool res = false;

  int i = -1;
  while (++i < 4 && !res) res = figure.coords[i].y == 19;
  i = -1;
  while (++i < 4 && !res)
    res = field[figure.coords[i].y + 1][figure.coords[i].x] == CONST;

  return res;
}

bool check_loss(field_t **field) {
  bool loss = false;

  for (int j = 0; j < 10 && !loss; ++j) loss = field[0][j] == CONST;

  return loss;
}

bool full_lines(field_t **field, int *lines) {
  int line_counter = -1;
  for (int i = 0; i < FIELD_ROWS; ++i) {
    int j = -1;
    while (++j < FIELD_COLS && field[i][j] == CONST)
      ;
    if (j == FIELD_COLS) lines[++line_counter] = i;
  }

  return line_counter >= 0;
}

void replace_line(field_t **field, int i) {
  for (int k = i; k >= 0; --k)
    for (int l = 0; l < FIELD_COLS; ++l)
      field[k][l] = k == 0 ? EMPTY : field[k - 1][l];
}

// void debug_log(const char *message) {
//   FILE *f = fopen("debug.log", "a");
//   if (f) {
//     fprintf(f, "%s\n", message);
//     fclose(f);
//   }
// }