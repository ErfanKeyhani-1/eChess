#include "attacks.h"
#include <stdio.h>
uint64_t knight_attacks[64];

void init_attack_tables() {
    for (int sq = 0; sq < 64; sq++) {
        uint64_t attack = 0ULL;
        int rank = sq / 8;
        int file = sq % 8;

        int directions[8][2] = {
            {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
            {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
        };

        for (int i = 0; i < 8; i++) {
            int dr = directions[i][0];
            int df = directions[i][1];

            int r = rank + dr;
            int f = file + df;

            if (r >= 0 && r < 8 && f >= 0 && f < 8) {
                int target_sq = r * 8 + f;
                attack |= (1ULL << target_sq);
            }
        }

        knight_attacks[sq] = attack;
    }
}
