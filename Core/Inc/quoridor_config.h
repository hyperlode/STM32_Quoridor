#ifndef QUORIDOR_CONFIG_H
#define QUORIDOR_CONFIG_H

#include <_ansi.h>

_BEGIN_STD_C
#define MAX_MOVES_COUNT 100
#define MOVES_STRING_LENGTH 100*3  // each move is max 2 chars and a divider

#define MOVE_INDEX_FIRST_WALL 12
#define MOVE_INDEX_FIRST_HORIZONTAL_WALL MOVE_INDEX_FIRST_WALL
#define MOVE_INDEX_FIRST_VERTICAL_WALL 76
#define MOVE_INDEX_END_OF_GAME 255

#define ERROR_NOTATION 0 
#define ERROR_DELETE_EDGE 1
#define ERROR_NOT_YET_IMPLEMENTED 2
#define ERROR_NOTATION_INVALID_DIRECTION_SECOND_CHAR 3
#define ERROR_NOTATION_INVALID_DIRECTION 4
#define ERROR_NOTATION_WALL_NOTATION 5
#define ERROR_NOT_A_WALL 6
#define ERROR_NOTATION_EMPTY_MOVE 7
#define ERROR_NOTATION_TOO_MANY_CHARS 8


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