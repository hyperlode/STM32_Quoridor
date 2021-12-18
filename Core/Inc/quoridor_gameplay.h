#ifndef QUORIDOR_GAMEPLAY_H
#define QUORIDOR_GAMEPLAY_H

#include <_ansi.h>

_BEGIN_STD_C

void game_init(char* load_game_history, uint8_t load_game_moves_count);

uint8_t get_winner_index();
int8_t *get_move_history_deltas_without_jumps();

uint8_t get_player_won(uint8_t player);
uint8_t get_playing_player();
uint8_t get_opponent(uint8_t player);

uint8_t get_move_index_valid(uint8_t move_index);
uint8_t get_walls_placed(uint8_t player);
int8_t get_delta_of_move_index(uint8_t move_index);
int8_t get_delta_of_move_index_normalized(uint8_t move_index);
uint8_t get_best_pawn_move();
uint8_t get_move_counter();

void analyse_possible_moves(uint8_t player);
void analyse_possible_moves_walls(uint8_t player);
void analyse_possible_moves_pawn(uint8_t player);

//void load_game_by_notation_one_string(moves_as_notation_one_string);
// void next_move_loaded_game(void);

uint8_t make_move_if_valid(uint8_t move_index);
void make_move(uint8_t move_index);

uint8_t check_move_possible_pawn(uint8_t move_index, uint8_t player);
uint8_t check_move_possible_pawn_orthogonal(uint8_t move_index, uint8_t player);
uint8_t check_move_possible_pawn_orthogonal_jump(uint8_t move_index, uint8_t player);
uint8_t check_move_possible_pawn_diagonal(uint8_t move_index, uint8_t player);
uint8_t check_move_possible_pawn_L_jump(player, start_node, direction_1, direction_2);
uint8_t make_move_wall(uint8_t player, uint8_t move_index);
uint8_t row_col_dir_to_move_index(uint8_t *row_col_dir);
void move_index_to_row_col_dir(uint8_t move_index, uint8_t *row_col_dir);
void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
void walls_get_all_positions(uint8_t *positions, uint8_t player);
void make_move_pawn(uint8_t player, uint8_t move_index);
void pawn_move_index_to_row_col_deltas(uint8_t move_index, int8_t *deltas_row_col);
void get_winning_distances(uint8_t *distances);
uint8_t pawn_get_position_as_node_index(uint8_t player);
void pawn_get_position_as_row_col(uint8_t *position, uint8_t player);
_END_STD_C

#endif // QUORIDOR_GAMEPLAY_H
