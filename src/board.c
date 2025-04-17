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
