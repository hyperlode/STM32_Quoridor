#ifndef QUORIDOR_GAMEPLAY_H
#define QUORIDOR_GAMEPLAY_H

#include <_ansi.h>

_BEGIN_STD_C
//typedef struct Player Player;
#define NEIGHBOURS_SIZE 6  // graph nodes maximum number of neighbouring nodes. (aka squares on the board) should be five, but test with 6
#define FAKE_NEIGHBOUR 0xFF

#define NOT_VISITED 0
#define VISITED 1
#define NONE_NODE 0xFF
#define BYTE_INFINITY 0xFF

uint8_t index_from_row_col(uint8_t row, uint8_t col);
uint8_t row_col_to_square_index(uint8_t row, uint8_t col);

void game_init(void);
_END_STD_C

#endif // QUORIDOR_GAMEPLAY_H
