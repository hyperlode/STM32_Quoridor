#ifndef QUORIDOR_GAMEPLAY_H
#define QUORIDOR_GAMEPLAY_H

#include <_ansi.h>

_BEGIN_STD_C


void game_init(void);
uint8_t get_move_counter();

void next_move_loaded_game(void);
void load_game_by_notation_one_string(moves_as_notation_one_string);
void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
void walls_get_all_positions(uint8_t* positions, uint8_t player);
void make_move_pawn(uint8_t player, uint8_t move_index);
void make_move(uint8_t move_index);

void get_winning_distances(uint8_t* distances);

uint8_t pawn_get_position_as_node_index(uint8_t player);
void pawn_get_position_as_row_col(uint8_t* position, uint8_t player);

static void raise_error(uint8_t error_code);

_END_STD_C

#endif // QUORIDOR_GAMEPLAY_H
