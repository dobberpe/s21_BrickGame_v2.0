CC = gcc
CXX = g++

CFLAGS = -Wall -Wextra -Werror -std=c11
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
TFLAGS = -lcheck -lsubunit -lm
GFLAGS = -fprofile-arcs -ftest-coverage

TETRIS_SRC = brick_game/tetris/tetris.c
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)
TETRIS_LIB = build/tetris.a

SNAKE_SRC = brick_game/snake/snake.cpp
SNAKE_OBJ = $(SNAKE_SRC:.cpp=.o)
SNAKE_LIB = build/snake.a

CLI_SRC = gui/cli/interface.c
CLI_OBJ = $(CLI_SRC:.c=.o)
TETRIS_CLI = build/tetris_cli
SNAKE_CLI = build/snake_cli
TETRIS_DSK = build/tetris_dsk
SNAKE_DSK = build/snake_dsk

DIST_NAME = brick_game.tar.gz
TEST_SRC = $(wildcard ./tests/*.c)
TEST_GCDA = $(TETRIS_SRC:.c=.gcda)
TEST_OBJ = $(TEST_SRC:.c=.o)

all: install

install: tetris_cli snake_cli tetris_dsk snake_dsk clean

uninstall:
	@rm -f $(TETRIS_CLI) $(SNAKE_CLI) $(TETRIS_DSK) $(SNAKE_DSK)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_OBJ)
	@mkdir -p build
	@ar rcs $(TETRIS_LIB) $(TETRIS_OBJ)
	@ranlib $(TETRIS_LIB)
	@rm -f $(TETRIS_OBJ)

$(SNAKE_LIB): $(SNAKE_OBJ)
	@mkdir -p build
	@ar rcs $(SNAKE_LIB) $(SNAKE_OBJ)
	@ranlib $(SNAKE_LIB)
	@rm -f $(SNAKE_OBJ)

tetris_cli: $(TETRIS_LIB) $(CLI_OBJ)
	@$(CC) $(CFLAGS) $(CLI_OBJ) -o $(TETRIS_CLI) -L. -l:$(TETRIS_LIB) -lm -lncursesw

snake_cli: $(SNAKE_LIB) $(CLI_OBJ)
	@$(CXX) $(CXXFLAGS) $(CLI_OBJ) -o $(SNAKE_CLI) -L. -l:$(SNAKE_LIB) -lm -lncursesw

tetris_dsk: $(TETRIS_LIB)
	@qmake -o Makefile.desktop LIBRARY=tetris
	@make -f Makefile.desktop
	@rm -f Makefile.desktop
	@rm -f controller.o gamewindow.o main.o moc_*

snake_dsk: $(SNAKE_LIB)
	@qmake -o Makefile.desktop LIBRARY=snake
	@make -f Makefile.desktop
	@rm -f Makefile.desktop
	@rm -f controller.o gamewindow.o main.o moc_*

dvi:
	@latex -output-directory=build build/documentation.tex

dist: install
	@tar -czvf $(DIST_NAME) Makefile brick_game.pro brick_game gui tests build

test: $(TEST_OBJ) $(TETRIS_LIB)
	@$(CC) $(CFLAGS) $(TEST_OBJ) -o test -L. -l:$(TETRIS_LIB) $(TFLAGS)
	@rm -f $(TEST_OBJ)
	@./test

gcov_report: clean add_coverage $(TETRIS_LIB)
	@$(CC) $(CFLAGS) -o gcov_r $(TEST_SRC) -L. -l:$(TETRIS_LIB) $(TFLAGS)
	@mkdir coverage_report
	@./gcov_r
	@python3 -m gcovr -r . --html --html-details -o coverage_report/index.html --exclude test
	@open coverage_report/index.html

fix_format:
	@cp ../materials/linters/.clang-format .
	@clang-format -i brick_game/*.* brick_game/*/* gui/*/* tests/*

format_check:
	@cp ../materials/linters/.clang-format .
	@clang-format -n brick_game/*.* brick_game/*/* gui/*/* tests/*
	@#cppcheck --enable=all --suppress=missingIncludeSystem brick_game/*.* brick_game/*/* gui/cli/* tests/*

leak_check:
	@valgrind --tool=memcheck --leak-check=yes ./test

clean:
	@rm -f $(CLI_OBJ) $(TETRIS_LIB) $(SNAKE_LIB) test gcov* coverage.info *.gcda *.gcno tests/*.gcno brick_game/tetris/*.gcda brick_game/tetris/*.gcno
	@rm -rf coverage_report
	@rm -rf .clang-format
	@rm -rf $(DIST_NAME)

clear: uninstall clean

add_coverage:
	$(eval CFLAGS:=$(CFLAGS) $(GFLAGS))

re: clear all

