#ifndef QUORIDOR_H
#define QUORIDOR_H
#include <_ansi.h>

_BEGIN_STD_C
#define MAX_MOVES_COUNT 100
#define MOVES_STRING_LENGTH 100*3  // each move is max 2 chars and a divider

void quoridor_human_interaction(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle);


void replay_game_init(moves_lode_notation);
void replay_game_next_move();
void display_game_state();


_END_STD_C

#endif // QUORIDOR_H
