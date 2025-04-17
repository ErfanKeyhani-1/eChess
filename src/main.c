#include "board.h"
#include <stdio.h>

int main() {
    printf("eChess Engine Booted. Ready to Clap Stockfish's cheeks.\n\n");

    // Classic starting position
    load_fen("rnbqkbnr/pppp1p1p/2P3p1/3Np2Q/2B4B/3P1N2/PPP2PPP/R3K2R");

    print_board();

    printf("🧠 Legal White Pawn Pushes:\n");
    print_bitboard(generate_pawn_pushes_white());

    printf("🧠 Legal Black Pawn Pushes:\n");
    print_bitboard(generate_pawn_pushes_black());

    printf("⚔️  White Pawn Captures:\n");
    print_bitboard(generate_pawn_captures_white());
    
    printf("⚔️  Black Pawn Captures:\n");
    print_bitboard(generate_pawn_captures_black());
 
    return 0;
}
