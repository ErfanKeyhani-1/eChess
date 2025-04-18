#include "board.h"
#include <stdio.h>

// Initialize global bitboards
uint64_t pawns_white = 0ULL;
uint64_t pawns_black = 0ULL;

uint64_t knights_white = 0ULL;
uint64_t knights_black = 0ULL;

uint64_t rooks_white = 0ULL;
uint64_t rooks_black = 0ULL;
uint64_t bishops_white = 0ULL;
uint64_t bishops_black = 0ULL;

uint64_t queens_white = 0ULL;
uint64_t queens_black = 0ULL;

uint64_t king_white = 0ULL;
uint64_t king_black = 0ULL;

void init_board() {
    pawns_white = 0x000000000000FF00ULL;
    pawns_black = 0x00FF000000000000ULL;

    knights_white = 0x0000000000000042ULL;   // b1 & g1
    knights_black = 0x4200000000000000ULL;   // b8 & g8

    rooks_white = 0x0000000000000081ULL;   // a1 & h1
    rooks_black = 0x8100000000000000ULL;   // a8 & h8

    bishops_white = 0x0000000000000024ULL;  // c1 & f1
    bishops_black = 0x2400000000000000ULL;  // c8 & f8

    queens_white  = 0x0000000000000008ULL;  // d1
    queens_black  = 0x0800000000000000ULL;  // d8

    king_white    = 0x0000000000000010ULL;  // e1
    king_black    = 0x1000000000000000ULL;  // e8

}

void load_fen(const char *fen) {
    // Clear all bitboards
    pawns_white = pawns_black = 0ULL;
    knights_white = knights_black = 0ULL;
    rooks_white = rooks_black = 0ULL;
    bishops_white = bishops_black = 0ULL;
    queens_white = queens_black = 0ULL;
    king_white = king_black = 0ULL;

    int rank = 7;
    int file = 0;
    const char *c = fen;

    while (*c && rank >= 0) {
        if (*c == '/') {
            rank--;
            file = 0;
        } else if (*c >= '1' && *c <= '8') {
            file += *c - '0';
        } else {
            int sq = rank * 8 + file;
            uint64_t mask = 1ULL << sq;

            switch (*c) {
                case 'P': pawns_white   |= mask; break;
                case 'p': pawns_black   |= mask; break;
                case 'N': knights_white |= mask; break;
                case 'n': knights_black |= mask; break;
                case 'R': rooks_white   |= mask; break;
                case 'r': rooks_black   |= mask; break;
                case 'B': bishops_white |= mask; break;
                case 'b': bishops_black |= mask; break;
                case 'Q': queens_white  |= mask; break;
                case 'q': queens_black  |= mask; break;
                case 'K': king_white    |= mask; break;
                case 'k': king_black    |= mask; break;
            }

            file++;
        }

        c++;
    }
}

int validate_kings() {
    if (__builtin_popcountll(king_white) != 1 || __builtin_popcountll(king_black) != 1) {
        fprintf(stderr, "Invalid FEN: Each side must have exactly one king.\n");
        return 0;
    }
    return 1;
}

void print_bitboard(uint64_t bb) {
    printf("\n");

    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);

        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            uint64_t mask = 1ULL << sq;

            printf("%c ", (bb & mask) ? '1' : '.');
        }

        printf("\n");
    }

    printf("  a b c d e f g h\n\n");
}

void print_board() {
    printf("\n");

    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);

        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;

            switch (get_piece_at_square(sq)) {
                case W_PAWN:   printf("P "); break;
                case B_PAWN:   printf("p "); break;
                case W_KNIGHT: printf("N "); break;
                case B_KNIGHT: printf("n "); break;
                case W_ROOK:   printf("R "); break;
                case B_ROOK:   printf("r "); break;
                case W_BISHOP: printf("B "); break;
                case B_BISHOP: printf("b "); break;
                case W_QUEEN:  printf("Q "); break;
                case B_QUEEN:  printf("q "); break;
                case W_KING:   printf("K "); break;
                case B_KING:   printf("k "); break;
                case EMPTY:    printf(". "); break;
            }
        }

        printf("\n");
    }

    printf("  a b c d e f g h\n\n");
}

Piece get_piece_at_square(int sq) {
    uint64_t mask = 1ULL << sq;

    if (pawns_white   & mask) return W_PAWN;
    if (pawns_black   & mask) return B_PAWN;
    if (knights_white & mask) return W_KNIGHT;
    if (knights_black & mask) return B_KNIGHT;
    if (rooks_white   & mask) return W_ROOK;
    if (rooks_black   & mask) return B_ROOK;
    if (bishops_white & mask) return W_BISHOP;
    if (bishops_black & mask) return B_BISHOP;
    if (queens_white  & mask) return W_QUEEN;
    if (queens_black  & mask) return B_QUEEN;
    if (king_white    & mask) return W_KING;
    if (king_black    & mask) return B_KING;

    return EMPTY;
}


