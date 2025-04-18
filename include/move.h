#ifndef MOVE_H
#define MOVE_H
#include <stdio.h>
typedef struct {
    int from;   // Square index 0–63
    int to;     // Square index 0–63
    int flags;  // 0 = quiet, 1 = capture, 2 = double push, etc.
} Move;

#endif // MOVE_H

