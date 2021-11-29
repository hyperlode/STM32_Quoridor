#ifndef QUORIDOR_NOTATION_H
#define QUORIDOR_NOTATION

#include <_ansi.h>

_BEGIN_STD_C
// static = only available from this file.
void get_move_index_to_notation(uint8_t index, char* notation);
void test(char* looode);
uint8_t get_notation_to_move_index(char* move_as_notationBBB);
static uint8_t notation_to_move_index_pawn(char* notation);
static uint8_t notation_to_move_index_wall(char* wall_position_as_notation);
void moves_string_to_moves_indeces(char *moves_as_string, uint8_t* moves_as_indeces);
_END_STD_C

#endif // QUORIDOR_NOTATION
