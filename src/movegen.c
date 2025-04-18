#include <board.h>
#include <stdint.h>
#include <move.h>
#include <attacks.h>
#include <movegen.h>
#include <stdio.h>
#include <stdlib.h>

static int pop_lsb(uint64_t *bb) {
    int sq = __builtin_ctzll(*bb);
    *bb &= *bb - 1;
    return sq;
}

void generate_white_pawn_moves(Move moves[], int *count) {
   *count = 0; 

    uint64_t single_pushes = generate_pawn_pushes_white();
    uint64_t captures = generate_pawn_captures_white();

    uint64_t push_targets = single_pushes;
    while (push_targets) {
        int to = pop_lsb(&push_targets);
        int from = to - 8;

        moves[*count].from = from;
        moves[*count].to = to;
        moves[*count].flags = 0; // TODO: Support promotion and EN PASSANT
        (*count)++;
    }
    uint64_t capture_targets = captures;
    while (capture_targets) {
        int to = pop_lsb(&capture_targets);
        
        if ((pawns_white >> 7) && (1ULL << to)) {
            moves[*count].from = to -7;
            moves[*count].to = to;
            moves[*count].flags = 1; // if you can capture, only if !
            (*count)++;
        }
        
        if ((pawns_white >> 9) & (1ULL << to)) {
            moves[*count].from = to - 9;
            moves[*count].to = to;
            moves[*count].flags = 1;
            (*count)++;
        }
    }
}

void generate_black_knight_moves(Move moves[], int *count) {
    uint64_t knights = knights_black;
    uint64_t occupied = pawns_black | knights_black | bishops_black |
                        rooks_black | queens_black | king_black;

    while (knights) {
        int from = __builtin_ctzll(knights);
        knights &= knights - 1;

        uint64_t attacks = knight_attacks[from] & ~occupied;

        while (attacks) {
            int to = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            moves[*count].from = from;
            moves[*count].to = to;
            moves[*count].flags = (pawns_white | knights_white | bishops_white | rooks_white | queens_white | king_white) & (1ULL << to) ? 1 : 0;
            (*count)++;
        }
    }
}

void generate_rook_moves_black(Move moves[], int *count) {
    uint64_t rooks = rooks_black;
    uint64_t friendlies = pawns_black | knights_black | bishops_black |
                          rooks_black | queens_black | king_black;
    uint64_t enemies = pawns_white | knights_white | bishops_white |
                       rooks_white | queens_white | king_white;

    while (rooks) {
        int from = __builtin_ctzll(rooks);
        rooks &= rooks - 1;

        int directions[4] = {8, -8, 1, -1}; // N, S, E, W

        for (int i = 0; i < 4; i++) {
            int dir = directions[i];
            int to = from;

            while (1) {
                int old_rank = to / 8;
                int old_file = to % 8;
                
                to += dir;

                // Boundary check
                if (to < 0 || to >= 64) break;

                int new_rank = to / 8;
                int new_file = to % 8;

                /*───────────────────────────
                  CRITICAL FIX: Same logic as White rooks
                ───────────────────────────*/
                if (dir == 1 || dir == -1) { // Horizontal moves
                    if (new_rank != old_rank) break;
                } else { // Vertical moves
                    if (new_file != old_file) break;
                }

                uint64_t to_mask = 1ULL << to;

                // Blocked by friendly piece
                if (friendlies & to_mask) break;

                // Add valid move
                moves[*count].from = from;
                moves[*count].to = to;
                moves[*count].flags = (enemies & to_mask) ? 1 : 0;
                (*count)++;

                // Stop after capturing enemy
                if (enemies & to_mask) break;
            }
        }
    }
}

void generate_bishop_moves_white(Move moves[], int *count) {
    uint64_t bishops = bishops_white;
    uint64_t friendlies = pawns_white | knights_white | bishops_white |
                          rooks_white | queens_white | king_white;
    uint64_t enemies = pawns_black | knights_black | bishops_black |
                       rooks_black | queens_black | king_black;

    while (bishops) {
        int from = __builtin_ctzll(bishops);
        bishops &= bishops - 1;

        int directions[4] = {9, 7, -9, -7}; // NE, NW, SW, SE

        for (int i = 0; i < 4; i++) {
            int dir = directions[i];
            int to = from;

            while (1) {
                int file = to % 8;
                to += dir;

                if (to < 0 || to > 63)
                    break;

                int to_file = to % 8;

                // File wrap prevention for diagonals
                if (abs(to_file - file) != 1 && to != from + dir)
                    break;

                uint64_t to_mask = 1ULL << to;
                if (friendlies & to_mask)
                    break;

                moves[*count].from = from;
                moves[*count].to = to;
                moves[*count].flags = (enemies & to_mask) ? 1 : 0;
                (*count)++;

                if (enemies & to_mask)
                    break;
            }
        }
    }
}


void generate_bishop_moves_black(Move moves[], int *count) {
    uint64_t bishops = bishops_black;
    uint64_t friendlies = pawns_black | knights_black | bishops_black |
                          rooks_black | queens_black | king_black;
    uint64_t enemies = pawns_white | knights_white | bishops_white |
                       rooks_white | queens_white | king_white;

    while (bishops) {
        int from = __builtin_ctzll(bishops);
        bishops &= bishops - 1;

        int directions[4] = {9, 7, -9, -7}; // NE, NW, SW, SE

        for (int i = 0; i < 4; i++) {
            int dir = directions[i];
            int to = from;

            while (1) {
                int file = to % 8;
                to += dir;

                if (to < 0 || to > 63)
                    break;

                int to_file = to % 8;

                if (abs(to_file - file) != 1 && to != from + dir)
                    break;

                uint64_t to_mask = 1ULL << to;
                if (friendlies & to_mask)
                    break;

                moves[*count].from = from;
                moves[*count].to = to;
                moves[*count].flags = (enemies & to_mask) ? 1 : 0;
                (*count)++;

                if (enemies & to_mask)
                    break;
            }
        }
    }
}

void generate_rook_moves_white(Move moves[], int *count) {
    uint64_t rooks = rooks_white;
    uint64_t friendlies = pawns_white | knights_white | bishops_white |
                          rooks_white | queens_white | king_white;
    uint64_t enemies = pawns_black | knights_black | bishops_black |
                       rooks_black | queens_black | king_black;

    while (rooks) {
        int from = __builtin_ctzll(rooks);
        rooks &= rooks - 1;

        int directions[4] = {8, -8, 1, -1}; // N, S, E, W

        for (int i = 0; i < 4; i++) {
            int dir = directions[i];
            int to = from;

            while (1) {
                int old_rank = to / 8;
                int old_file = to % 8;
                
                to += dir; // Move one step

                // Boundary check
                if (to < 0 || to >= 64) break;

                int new_rank = to / 8;
                int new_file = to % 8;

                /*───────────────────────────
                  CRITICAL FIX: Prevent wrap-around
                  For horizontal moves (E/W), ensure same rank
                  For vertical moves (N/S), ensure same file
                ───────────────────────────*/
                if (dir == 1 || dir == -1) { // Horizontal moves
                    if (new_rank != old_rank) break; // Changed rank = invalid
                } else { // Vertical moves
                    if (new_file != old_file) break; // Changed file = invalid
                }

                uint64_t to_mask = 1ULL << to;

                // Blocked by friendly piece
                if (friendlies & to_mask) break;

                // Add valid move
                moves[*count].from = from;
                moves[*count].to = to;
                moves[*count].flags = (enemies & to_mask) ? 1 : 0;
                (*count)++;

                // Stop after capturing enemy
                if (enemies & to_mask) break;
            }
        }
    }
}

void generate_knight_moves_white(Move moves[], int *count) {
    uint64_t knights = knights_white;
    uint64_t occupied = pawns_white | knights_white | bishops_white | rooks_white |
                        queens_white | king_white;  // friendly pieces
    while (knights) {
        int from = __builtin_ctzll(knights);
        knights &= knights - 1;

        uint64_t attacks = knight_attacks[from] & ~occupied;

        while (attacks) {
            int to = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            moves[*count].from = from;
            moves[*count].to = to;
            moves[*count].flags = (pawns_black | knights_black | bishops_black | rooks_black | queens_black | king_black) & (1ULL << to) ? 1 : 0;
            (*count)++;
        }
    }
}

void generate_black_pawn_moves(Move moves[], int *count) {
    *count = 0;
    uint64_t pushes = generate_pawn_pushes_black();
    uint64_t captures = generate_pawn_captures_black();

    // Process pushes
    uint64_t push_targets = pushes;
    while (push_targets) {
        int to = __builtin_ctzll(push_targets);
        push_targets &= push_targets - 1;
        int from = to + 8;

        // Check for double push (from rank 6 to 4)
        if ((to / 8) == 4 && (pawns_black & (1ULL << (to + 8)))) {
            from = to + 16;
        }

        moves[*count].from = from;
        moves[*count].to = to;
        moves[*count].flags = 0;
        (*count)++;
    }

    // Process captures
    uint64_t capture_targets = captures;
    while (capture_targets) {
        int to = __builtin_ctzll(capture_targets);
        capture_targets &= capture_targets - 1;

        // Determine 'from' squares for captures
        int from_left = to + 9;  // Capturing right (from black's perspective)
        int from_right = to + 7;

        if (from_left < 64 && (pawns_black & (1ULL << from_left))) {
            moves[*count].from = from_left;
            moves[*count].to = to;
            moves[*count].flags = 1;
            (*count)++;
        }
        if (from_right < 64 && (pawns_black & (1ULL << from_right))) {
            moves[*count].from = from_right;
            moves[*count].to = to;
            moves[*count].flags = 1;
            (*count)++;
        }
    }
}

void print_move(Move m) {
    char files[] = "abcdefgh";

    int from_file = m.from % 8;
    int from_rank = m.from / 8;
    int to_file = m.to % 8;
    int to_rank = m.to / 8;

    printf("%c%d -> %c%d (flag: %d)\n",
        files[from_file], from_rank + 1,
files[to_file], to_rank + 1,
        m.flags);
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


