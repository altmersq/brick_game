CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -Wpedantic

OBJS := out/tetris.o out/backend.o out/frontend.o
LDFLAGS := -lncurses
INC := -I/usr/include/ncursesw

all: clean tetris

tetris:
	$(CC) $(CFLAGS) $(INC) *.c -o tetris $(LDFLAGS)
	./tetris

rebuild: clean tetris

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) out/*.o tetris