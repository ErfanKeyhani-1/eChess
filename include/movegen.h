#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdint.h>
#include "move.h"
#include <stdio.h>

void generate_white_pawn_moves(Move moves[], int *count);
void generate_black_pawn_moves(Move moves[], int *count);
void generate_knight_moves_white(Move moves[], int *count);
void generate_black_knight_moves(Move moves[], int *count);
void generate_rook_moves_white(Move moves[], int *count);
void generate_rook_moves_black(Move moves[], int *count);
void generate_bishop_moves_white(Move moves[], int *count);
void generate_bishop_moves_black(Move moves[], int *count);

uint64_t generate_pawn_pushes_white();
uint64_t generate_pawn_pushes_black();
uint64_t generate_pawn_captures_white();
uint64_t generate_pawn_captures_black();

void print_move(Move m);


#endif // MOVEGEN_H
