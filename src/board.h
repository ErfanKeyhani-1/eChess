#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Piece ID enum
typedef enum {
    EMPTY,
    W_PAWN, B_PAWN,
    W_KNIGHT, B_KNIGHT,
    W_ROOK, B_ROOK,
    W_BISHOP, B_BISHOP,
    W_QUEEN, B_QUEEN,
    W_KING, B_KING
} Piece;

// Piece-at-square checker
Piece get_piece_at_square(int sq);


void load_fen(const char *fen);
void print_bitboard(uint64_t bb);

/*
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *-----------------------PIECES----------------------------
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

// Bitboards for white and black pawns
extern uint64_t pawns_white;
extern uint64_t pawns_black;

// Knight bitboards
extern uint64_t knights_white;
extern uint64_t knights_black;

// Rook bitboards
extern uint64_t rooks_white;
extern uint64_t rooks_black;

// Bishop bitboards
extern uint64_t bishops_white;
extern uint64_t bishops_black;

// Queen bitboards
extern uint64_t queens_white;
extern uint64_t queens_black;

// King bitboards
extern uint64_t king_white;
extern uint64_t king_black;


//=============================PAWN MOVEMENT=================================
uint64_t generate_pawn_pushes_white();
uint64_t generate_pawn_pushes_black();


uint64_t generate_pawn_captures_white();
uint64_t generate_pawn_captures_black();

// Function declarations
void init_board();
void print_board();

#endif // BOARD_H
