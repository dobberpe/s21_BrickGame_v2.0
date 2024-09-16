CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TFLAGS = -lcheck -lsubunit -lm
GFLAGS = -fprofile-arcs -ftest-coverage
TETRIS_SRC = brick_game/tetris/tetris.c
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)
TETRIS_LIB = build/tetris.a
CLI_SRC = gui/cli/interface.c
CLI_OBJ = $(CLI_SRC:.c=.o)
EXEC = build/tetris_cli
DIST_NAME = brick_game.tar.gz
TEST_SRC = $(wildcard ./tests/*.c)
TEST_GCDA = $(TETRIS_SRC:.c=.gcda)
TEST_OBJ = $(TEST_SRC:.c=.o)

all: run

install: $(CLI_OBJ) $(TETRIS_LIB)
	@$(CC) $(CFLAGS) $(CLI_OBJ) -o $(EXEC) -L. -l:$(TETRIS_LIB) -lm -lncursesw
	@rm -f $(CLI_OBJ)

uninstall:
	@rm -rf $(EXEC)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_OBJ)
	@mkdir -p build
	@ar rcs $(TETRIS_LIB) $(TETRIS_OBJ)
	@ranlib $(TETRIS_LIB)
	@rm -f $(TETRIS_OBJ)

dvi:
	@latex -output-directory=build build/documentation.tex

dist: install
	@tar -czvf $(DIST_NAME)  Makefile brick_game gui tests

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

run: install
	@./$(EXEC)

fix_format:
	@cp ../materials/linters/.clang-format .
	@clang-format -i brick_game/*.* brick_game/*/* gui/*/* tests/*

format_check:
	@cp ../materials/linters/.clang-format .
	@clang-format -n brick_game/*.* brick_game/*/* gui/*/* tests/*
	@cppcheck --enable=all --suppress=missingIncludeSystem brick_game/*.* brick_game/*/* gui/cli/* tests/*

leak_check:
	@valgrind --tool=memcheck --leak-check=yes ./test

clean:
	@rm -f $(TETRIS_LIB) test gcov* coverage.info *.gcda *.gcno tests/*.gcno brick_game/tetris/*.gcda brick_game/tetris/*.gcno
	@rm -rf coverage_report
	@rm -rf .clang-format
	@rm -rf $(DIST_NAME)

add_coverage:
	$(eval CFLAGS:=$(CFLAGS) $(GFLAGS))

re: clean all

