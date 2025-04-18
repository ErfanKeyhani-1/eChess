#include "board.h"
#include "movegen.h"
#include "move.h"
#include "attacks.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    //──────────────────────────────
    //          INIT BLOCK
    //──────────────────────────────

    printf("            █████████  █████                              \n");
    printf("           ███░░░░░███░░███                               \n");
    printf("  ██████  ███     ░░░  ░███████    ██████   █████   █████ \n");
    printf(" ███░░███░███          ░███░░███  ███░░███ ███░░   ███░░  \n");
    printf("░███████ ░███          ░███ ░███ ░███████ ░░█████ ░░█████ \n");
    printf("░███░░░  ░░███     ███ ░███ ░███ ░███░░░   ░░░░███ ░░░░███\n");
    printf("░░██████  ░░█████████  ████ █████░░██████  ██████  ██████ \n");
    printf(" ░░░░░░    ░░░░░░░░░  ░░░░ ░░░░░  ░░░░░░  ░░░░░░  ░░░░░░  \n");
    printf("\n");
    printf("eChess Booted. Ready to Clap Stockfish's cheeks.\n\n");

    //──────────────────────────────
    //         FEN LOAD
    //──────────────────────────────
    printf("┌──────────────────────────────────────────┐\n");
    printf("│           📥 LOADING FEN STRING          │\n");
    printf("└──────────────────────────────────────────┘\n");
    load_fen("r2qkb1r/5p2/1pnp4/pbp1Bnp1/3P2p1/4NN2/PPPBPPP1/R2QK2R");
    if (!validate_kings()) {
        exit(1);
    }
    printf("[+] Done\n");

    //──────────────────────────────
    //          BOARD RENDER
    //──────────────────────────────
    printf("┌──────────────────────────────────────────┐\n");
    printf("│             ♟️ ASCII BOARD VIEW             │\n");
    printf("└──────────────────────────────────────────┘\n");
    print_board();
    printf("[+] Done.\n\n");

    //──────────────────────────────
    //         INIT TABLES
    //──────────────────────────────
    printf("┌──────────────────────────────────────────┐\n");
    printf("│      📊 Initializing Attack Tables       │\n");
    printf("└──────────────────────────────────────────┘\n");
    init_attack_tables();
    printf("[+] Done\n");

    //──────────────────────────────
    //        PAWN MOVEGEN
    //──────────────────────────────
    Move moves[256];
    int move_count = 0;
    printf("┌──────────────────────────────────────────┐\n");
    printf("│           🧠 WHITE PAWN MOVES            │\n");
    printf("└──────────────────────────────────────────┘\n");
    print_bitboard(generate_pawn_pushes_white());

    printf("┌──────────────────────────────────────────┐\n");
    printf("│           🧠 BLACK PAWN MOVES            │\n");
    printf("└──────────────────────────────────────────┘\n");
    print_bitboard(generate_pawn_pushes_black());

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ⚔️ WHITE PAWN CAPTURES           │\n");
    printf("└──────────────────────────────────────────┘\n");
    print_bitboard(generate_pawn_captures_white());

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ⚔️ BLACK PAWN CAPTURES           │\n");
    printf("└──────────────────────────────────────────┘\n");
    print_bitboard(generate_pawn_captures_black());

    generate_white_pawn_moves(moves, &move_count);
    printf("White PAWN moves GENERATED. Move count: %d\n", move_count);
    for (int i = 0; i < move_count; i++) {
        print_move(moves[i]);
    }
    
    //────────────────────────────────────────────
    //        BLACK PAWN MOVE LIST
    //────────────────────────────────────────────
    Move black_pawn_moves[256];
    int black_pawn_move_count = 0;

    generate_black_pawn_moves(black_pawn_moves, &black_pawn_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        🧠 BLACK PAWN MOVES GENERATED      │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("Move count: %d\n", black_pawn_move_count);

    for (int i = 0; i < black_pawn_move_count; i++) {
        print_move(black_pawn_moves[i]);
    }

    //──────────────────────────────
    //        KNIGHT MOVEGEN
    //──────────────────────────────
    Move knight_moves[64];
    int knight_move_count = 0;
    generate_knight_moves_white(knight_moves, &knight_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♘ WHITE KNIGHT MOVES ♘           │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 White Knight Moves: %d\n", knight_move_count);
    for (int i = 0; i < knight_move_count; i++) {
        print_move(knight_moves[i]);
    }

    Move black_knight_moves[64];
    int black_knight_move_count = 0;
    generate_black_knight_moves(black_knight_moves, &black_knight_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♞ BLACK KNIGHT MOVES ♞           │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 Black Knight Moves: %d\n", black_knight_move_count);
    for (int i = 0; i < black_knight_move_count; i++) {
        print_move(black_knight_moves[i]);
    }

    //────────────────────────────────────────────
    //        WHITE ROOK MOVE LIST
    //────────────────────────────────────────────
    Move rook_moves[64];
    int rook_move_count = 0;

    generate_rook_moves_white(rook_moves, &rook_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♖ WHITE ROOK MOVES ♖             │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 White Rook Moves: %d\n", rook_move_count);
    for (int i = 0; i < rook_move_count; i++) {
        print_move(rook_moves[i]);
    }

    //────────────────────────────────────────────
    //        BLACK ROOK MOVE LIST
    //────────────────────────────────────────────
    Move black_rook_moves[64];
    int black_rook_move_count = 0;

    generate_rook_moves_black(black_rook_moves, &black_rook_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♜ BLACK ROOK MOVES ♜             │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 Black Rook Moves: %d\n", black_rook_move_count);
    for (int i = 0; i < black_rook_move_count; i++) {
        print_move(black_rook_moves[i]);
    }

    //────────────────────────────────────────────
    //        WHITE BISHOP MOVE LIST
    //────────────────────────────────────────────
    Move white_bishop_moves[64];
    int white_bishop_move_count = 0;

    generate_bishop_moves_white(white_bishop_moves, &white_bishop_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♗ WHITE BISHOP MOVES ♗           │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 White Bishop Moves: %d\n", white_bishop_move_count);
    for (int i = 0; i < white_bishop_move_count; i++) {
        print_move(white_bishop_moves[i]);
    }

    //────────────────────────────────────────────
    //        BLACK BISHOP MOVE LIST
    //────────────────────────────────────────────
    Move black_bishop_moves[64];
    int black_bishop_move_count = 0;

    generate_bishop_moves_black(black_bishop_moves, &black_bishop_move_count);

    printf("┌──────────────────────────────────────────┐\n");
    printf("│        ♝ BLACK BISHOP MOVES ♝           │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("🧠 Black Bishop Moves: %d\n", black_bishop_move_count);
    for (int i = 0; i < black_bishop_move_count; i++) {
        print_move(black_bishop_moves[i]);
    }


    // TODO: Add queen/king movegen here

    return 0;
}

