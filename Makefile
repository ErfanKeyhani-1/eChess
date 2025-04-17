CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -O2 -Iinclude
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
OUT=build/echess

all: $(OUT)

$(OUT): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build/ *.o src/*.o

.PHONY: all clean
