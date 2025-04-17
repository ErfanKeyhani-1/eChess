#include <stdio.h>
#include "board.h"

int main() {
    printf("eChess Engine Booted. Ready to Clap Stockfish.\n");
    init_board();
    print_board();
    return 0;
}
