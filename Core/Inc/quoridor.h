#ifndef QUORIDOR_H
#define QUORIDOR_H
#include <_ansi.h>

_BEGIN_STD_C
#define MAX_MOVES_COUNT 100
#define MOVES_STRING_LENGTH 100*3  // each move is max 2 chars and a divider




void replay_game_init(moves_lode_notation);
void replay_game_next_move();
void display_game_state();


_END_STD_C

#endif // QUORIDOR_H
