#include "snake.h"

using namespace s21;

void userInput(UserAction_t action, bool hold [[maybe_unused]]) {

	if (action == Start) {
		if (Snake::get_snake_info()->get_state() != GAMEOVER) Snake::get_snake_info()->set_pause(false);
		if (Snake::get_snake_info()->get_state() == START) Snake::get_snake_info()->set_signal(ENTER);
	} else if (action == Pause) Snake::get_snake_info()->set_pause(true);
	else if (action == Terminate) Snake::get_snake_info()->set_signal(ESCAPE);
	else if (Snake::get_snake_info()->get_state() == ROTATE) Snake::get_snake_info()->set_signal((snake_signal_t)(action - 2));

	Snake::get_snake_info()->process_signal();
}

GameInfo_t updateCurrentState() {
	Snake::get_snake_info()->process_signal();
	if (Snake::get_snake_info()->get_state() != EXIT) Snake::get_snake_info()->update_game_info();
	return Snake::get_snake_info()->get_game_info();
}

Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

bool Coordinate::operator==(const Coordinate &other) const { return x == other.x && y == other.y; }

Coordinate& Coordinate::operator=(const Coordinate& other) {
	if (this != &other) {
		x = other.x;
		y = other.y;
	}

	return *this;
}

Coordinate Coordinate::left() const { return Coordinate(x - 1, y); }

Coordinate Coordinate::right() const { return Coordinate(x + 1, y); }

Coordinate Coordinate::up() const { return Coordinate(x, y - 1); }

Coordinate Coordinate::down() const { return Coordinate(x, y + 1); }

bool Coordinate::out_of_field(int left, int right, int up, int down) const { return x < left || x > right || y < up || y > down; }

Snake::Snake() : curr_dir(left), new_dir(left), apple(-1, -1), state(START), signal(NOSIG), gen(rd()), distrib(uniform_int_distribution<>(1, 199)) {

	snake_body.push_front(Coordinate(7, 7));

	game_info.field = new int*[FIELD_ROWS];
	for (int i = 0; i < FIELD_ROWS; i++) {
		game_info.field[i] = new int[FIELD_COLS];
		for (int j = 0; j < FIELD_COLS; j++) game_info.field[i][j] = 0;
	}

	game_info.next = new int*[NEXT_ROWS];
	for (int i = 0; i < NEXT_ROWS; i++) {
		game_info.next[i] = new int[NEXT_COLS];
		for (int j = 0; j < NEXT_COLS; j++) game_info.next[i][j] = 0;
	}

	game_info.score = 0;
	game_info.high_score = 0;
	read_highscore();
	game_info.level = 1;
	game_info.speed = 1;
	game_info.pause = true;
}

Snake::~Snake() {
	snake_body.clear();

	for (int i = 0; i < FIELD_ROWS; i++)
		delete game_info.field[i];
	delete game_info.field;

	for (int i = 0; i < NEXT_ROWS; i++)
		delete game_info.next[i];
	delete game_info.next;
}

Snake* Snake::get_snake_info() {
  static Snake snake_info;
  return &snake_info;
}


void Snake::set_pause(bool value) { game_info.pause = value; }

snake_fsm_t Snake::get_state() const { return state; }

void Snake::set_signal(snake_signal_t value) { signal = value; }

void Snake::process_signal() {
	action fsm_matrix[7][7] = {
		{&Snake::exitstate, nullptr, nullptr, nullptr, nullptr, &Snake::spawn, nullptr},
		{&Snake::spawn, &Snake::spawn, &Snake::spawn, &Snake::spawn, &Snake::spawn, &Snake::spawn, &Snake::spawn},
		{&Snake::exitstate, &Snake::dir_left, &Snake::dir_right, &Snake::dir_up, &Snake::dir_down, &Snake::move, &Snake::check_timer},
		{&Snake::move, &Snake::move, &Snake::move, &Snake::move, &Snake::move, &Snake::move, &Snake::move},
		{&Snake::grow, &Snake::grow, &Snake::grow, &Snake::grow, &Snake::grow, &Snake::grow, &Snake::grow},
		{&Snake::exitstate, &Snake::gameover, &Snake::gameover, &Snake::gameover, &Snake::gameover, &Snake::gameover, &Snake::gameover},
		{&Snake::exitstate, &Snake::exitstate, &Snake::exitstate, &Snake::exitstate, &Snake::exitstate, &Snake::exitstate, &Snake::exitstate}};

	action act = fsm_matrix[state][signal];

	if (act) (this->*act)();

	signal = NOSIG;
}

void Snake::update_game_info() {

	for (int i = 0; i < FIELD_ROWS; i++) for (int j = 0; j < FIELD_COLS; j++) game_info.field[i][j] = 0;

	for (const auto &chain : snake_body)
		game_info.field[chain.y][chain.x] = 1;

	if (apple.x >= 0 && apple.y >= 0) game_info.field[apple.y][apple.x] = 1;
}

GameInfo_t Snake::get_game_info() const { return game_info; }

void Snake::spawn() {

	int cell_number = distrib(gen);

	int i = -1, j;
	while (cell_number && ++i < FIELD_ROWS) {
		j = -1;
		while (cell_number && ++j < FIELD_COLS)
			if (find(snake_body.begin(), snake_body.end(), Coordinate(j, i)) == snake_body.end()) --cell_number;
	}

	apple = Coordinate(j, i);
	state = ROTATE;
}

void Snake::dir_left() {
	new_dir = curr_dir != right ? left : curr_dir;
	check_timer();
}

void Snake::dir_right() {
	new_dir = curr_dir != left ? right : curr_dir;
	check_timer();
}

void Snake::dir_up() {
	new_dir = curr_dir != down ? up : curr_dir;
	check_timer();
}

void Snake::dir_down() {
	new_dir = curr_dir != up ? down : curr_dir;
	check_timer();
}

void Snake::check_timer() {
	if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - timer).count() >= FALL_DELAY) {
		timer = chrono::steady_clock::now();
		state = MOVE;
	}
}

void Snake::move() {
	curr_dir = new_dir;

	Coordinate head = curr_dir == left ? snake_body.front().left() : curr_dir == right ? snake_body.front().right() : curr_dir == up ? snake_body.front().up() : snake_body.front().down();

	if (find(snake_body.begin(), snake_body.end(), head) != snake_body.end() || head.out_of_field(0, FIELD_COLS - 1, 0, FIELD_ROWS - 1)) state = GAMEOVER;
	else {
		snake_body.push_front(head);
		if (head == apple) state = GROW;
		else {
			state = ROTATE;
			snake_body.pop_back();
		}
		timer = chrono::steady_clock::now();
	}
}


void Snake::grow() {
	++game_info.score;
	game_info.high_score = max(game_info.score, game_info.high_score);

	if (!(game_info.score % 5) && game_info.level < 10) {
		++game_info.level;
		++game_info.speed;
	}

	if (snake_body.size() == 200) state = GAMEOVER;
	else {
		state = SPAWN;
		distrib = uniform_int_distribution<>(0, FIELD_ROWS * FIELD_COLS - snake_body.size() - 1);
	}
}

void Snake::gameover() {
	write_highscore();
	game_info.pause = true;
}

void Snake::exitstate() {
	// clear
}

void Snake::read_highscore() {
	ifstream File(HIGHSCORE_FILE, ios::binary);

	if (File) {
		File.read(reinterpret_cast<char*>(&(game_info.high_score)), sizeof(game_info.high_score));
		File.close();
	}
}

void Snake::write_highscore() const {
	ofstream File(HIGHSCORE_FILE, ios::binary);

	if (File) {
		File.write(reinterpret_cast<const char*>(&(game_info.high_score)), sizeof(game_info.high_score));
		File.close();
	}
}