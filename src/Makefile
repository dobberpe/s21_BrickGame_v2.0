CC = gcc
CFLAGS = -std=c11
TETRIS_SRC = brick_game/tetris/tetris.c
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)
TETRIS_LIB = build/tetris.a
CLI_SRC = gui/cli/interface.c
CLI_OBJ = $(CLI_SRC:.c=.o)
EXEC = build/tetris_cli

all: install

install: $(CLI_OBJ) $(TETRIS_LIB)
	@$(CC) $(CFLAGS) $(CLI_OBJ) -o $(EXEC) -L. -l:$(TETRIS_LIB) -lm -lncursesw
	@rm -f $(CLI_OBJ)

uninstall:
	@rm -rf $(EXEC)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_OBJ)
	@mkdir build
	@ar rcs $(TETRIS_LIB) $(TETRIS_OBJ)
	@ranlib $(TETRIS_LIB)
	@rm -f $(TETRIS_OBJ)