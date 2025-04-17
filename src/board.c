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

uint64_t generate_pawn_pushes_white() {
    uint64_t empty = ~(pawns_white | pawns_black | knights_white | knights_black |
                       rooks_white | rooks_black | bishops_white | bishops_black |
                       queens_white | queens_black | king_white | king_black);

    // Single push: move white pawns 1 rank forward if square is empty
    uint64_t single_push = (pawns_white << 8) & empty;

    // Double push: only for pawns on rank 2 (bits 8-15) if both squares are empty
    uint64_t rank2 = pawns_white & 0x000000000000FF00ULL;
    uint64_t double_push = ((rank2 << 8) & empty) << 8 & empty;

    return single_push | double_push;
}

uint64_t generate_pawn_pushes_black() {
    uint64_t occupied =
        pawns_white | pawns_black |
        knights_white | knights_black |
        rooks_white | rooks_black |
        bishops_white | bishops_black |
        queens_white | queens_black |
        king_white | king_black;

    uint64_t empty = ~occupied;

    // Single push (one square forward = down the board)
    uint64_t single_push = (pawns_black >> 8) & empty;

    // Double push (rank 7 = bits 48–55)
    uint64_t rank7 = pawns_black & 0x00FF000000000000ULL;
    uint64_t double_push = ((rank7 >> 8) & empty) >> 8 & empty;

    return single_push | double_push;
}

uint64_t generate_pawn_captures_white() {
    uint64_t enemy =
        pawns_black | knights_black | rooks_black |
        bishops_black | queens_black | king_black;

    // Mask to avoid wrapping from A-file to H-file
    uint64_t not_a_file = 0xfefefefefefefefeULL;
    uint64_t not_h_file = 0x7f7f7f7f7f7f7f7fULL;

    // Capture left (diagonal): shift left by 7 (unless on A-file)
    uint64_t left_attacks = (pawns_white << 7) & enemy & not_a_file;

    // Capture right (diagonal): shift left by 9 (unless on H-file)
    uint64_t right_attacks = (pawns_white << 9) & enemy & not_h_file;

    return left_attacks | right_attacks;
}

uint64_t generate_pawn_captures_black() {
    uint64_t enemy =
        pawns_white | knights_white | rooks_white |
        bishops_white | queens_white | king_white;

    // Prevent A- and H-file wraparounds
    uint64_t not_a_file = 0xfefefefefefefefeULL;
    uint64_t not_h_file = 0x7f7f7f7f7f7f7f7fULL;

    // Diag-left (down-left from black POV)
    uint64_t left_attacks = (pawns_black >> 9) & enemy & not_h_file;

    // Diag-right (down-right from black POV)
    uint64_t right_attacks = (pawns_black >> 7) & enemy & not_a_file;

    return left_attacks | right_attacks;
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


