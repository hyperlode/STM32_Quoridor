#ifndef QUORIDOR_CONFIG_H
#define QUORIDOR_CONFIG_H

#include <_ansi.h>

_BEGIN_STD_C
#define MAX_MOVES_COUNT 100
#define MOVES_STRING_LENGTH 100*3  // each move is max 2 chars and a divider

#define ERROR_NOTATION 0 
#define ERROR_DELETE_EDGE 1


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
    uint8_t distance_to_win;
    struct Wall walls [10];
    uint8_t walls_placed;
};

typedef struct Player Player;
#endif // QUORIDOR_CONFIG_H