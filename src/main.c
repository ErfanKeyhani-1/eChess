#include "board.h"
#include <stdio.h>

int main() {
    printf("eChess Engine Booted. Ready to Clap Stockfish's cheeks.\n\n");

    // Classic starting position
    load_fen("rnb1k2r/ppp1qppp/3p1b2/3Q4/2B1P1n1/5N2/PPPP1PPP/RNB1K2R");

    print_board();

    return 0;
}
