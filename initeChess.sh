#!/bin/bash

# === Safety Setup ===
set -e  # Exit on error
set -u  # Treat unset vars as error
set -o pipefail

echo "🔥 Welcome to the eChess Project Initializer 🔥"
echo "💬 Enter your first commit message:"
read -r commit_msg

# === Folder Structure ===
echo "📁 Creating folder structure..."
mkdir -p src include tests build

# === File Creation ===

# --- .gitignore ---
cat <<EOF > .gitignore
*.o
*.out
*.swp
build/
EOF

# --- README.md ---
cat <<EOF > README.md
# eChess ♟️

A hyper-optimized chess engine written in pure C.

Built for performance, clean design, and eventual neural network AI enhancements.

## Structure
- \`src/\` — core engine code
- \`include/\` — header files
- \`tests/\` — unit tests
- \`build/\` — compiled binaries

## Author
Erfan Keyhani — Cyber Warfare God™
EOF

# --- Makefile ---
cat <<'EOF' > Makefile
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
EOF

# --- src/main.c ---
cat <<'EOF' > src/main.c
#include <stdio.h>
#include "board.h"

int main() {
    printf("eChess Engine Booted. Ready to Clap Stockfish.\n");
    init_board();
    print_board();
    return 0;
}
EOF

# --- src/board.c ---
cat <<'EOF' > src/board.c
#include <stdio.h>
#include "board.h"

uint64_t pawns_white = 0x000000000000FF00;
uint64_t pawns_black = 0x00FF000000000000;

void init_board() {
    // future: initialize all bitboards
    printf("Board initialized.\n");
}

void print_board() {
    // future: render board ASCII
    printf("Board display not implemented yet.\n");
}
EOF

# --- src/board.h ---
cat <<'EOF' > src/board.h
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void init_board();
void print_board();

#endif // BOARD_H
EOF

# === Git Add, Commit, Push ===
echo "🧹 Staging all files..."
git add .

echo "📝 Making first commit..."
git commit -m "$commit_msg"

echo "🚀 Pushing to GitHub..."
git branch -M main
git push -u origin main

echo "✅ eChess Project Initialized Successfully!"
echo "Now go make that engine BITE, king 🫡"

