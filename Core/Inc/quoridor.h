#ifndef QUORIDOR_H
#define QUORIDOR_H

#include <_ansi.h>

_BEGIN_STD_C
#define MAX_MOVES_COUNT 100

struct Pawn{
    uint8_t row;
    uint8_t col;
    uint8_t fill;    
};

struct Wall{
    uint8_t row;
    uint8_t col;
    uint8_t horizontal_else_vertical;
};

struct Player{
    struct Pawn pawn;
    struct Wall walls [10];
    uint8_t walls_placed;
};




typedef struct Player Player;

_END_STD_C

#endif // QUORIDOR_H
