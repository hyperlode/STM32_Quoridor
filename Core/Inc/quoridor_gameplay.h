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

void game_init(void);
uint8_t get_move_counter();
void add_wall_to_board_graph(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
void disconnect_nodes(uint8_t node_1, uint8_t node_2);
void delete_edge(uint8_t start_node, uint8_t node_to_be_disconnected);
void init_graph_node_get_neighbours(uint8_t row, uint8_t col, uint8_t* neighbours);

uint8_t get_distance_to_winning_square(uint8_t playing_player, uint8_t start_node);

void move_next_replay(void);
void moves_string_to_moves(char* moves_as_string);

void wall_set_by_notation(uint8_t player, char* wall_position);
void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
void walls_get_all_positions(uint8_t* positions, uint8_t player);
void move_by_notation(char* move_as_notation);
void pawn_move_by_notation(uint8_t player, char* direction);
void pawn_set_position(uint8_t player, int8_t delta_x, int8_t delta_y);

uint8_t index_from_row_col(uint8_t row, uint8_t col);
uint8_t row_col_to_square_index(uint8_t row, uint8_t col);
uint8_t pawn_get_position_as_square_index(uint8_t player);
void pawn_get_position_as_row_col(uint8_t* position, uint8_t player);

void raise_error(uint8_t error_code);

_END_STD_C

#endif // QUORIDOR_GAMEPLAY_H
