#ifndef QUORIDOR_GRAPH_H
#define QUORIDOR_GRAPH

#include <_ansi.h>
#define NEIGHBOURS_SIZE 6  // graph nodes maximum number of neighbouring nodes. (aka squares on the board) should be five, but test with 6
#define FAKE_NEIGHBOUR 0xFF

#define NOT_VISITED 0
#define VISITED 1
#define NONE_NODE 0xFF
#define BYTE_INFINITY 0xFF

_BEGIN_STD_C
// static = only available from this file.
void graph_init();
// void graph_add_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
void graph_wall_add(uint8_t move_index);
void graph_wall_remove(uint8_t move_index);

static void disconnect_nodes(uint8_t node_1, uint8_t node_2);
static void connect_nodes(uint8_t node_1, uint8_t node_2);
uint8_t get_edge_exists(uint8_t node_start, uint8_t node_end);
static void delete_edge(uint8_t start_node, uint8_t node_to_be_disconnected);
static void add_edge(uint8_t start_node, uint8_t node_to_be_connected);
static void graph_node_get_neighbours_at_init(uint8_t row, uint8_t col, uint8_t *neighbours);
int8_t  graph_delta_of_distances(uint8_t start_node_target_to_north, uint8_t start_node_target_to_south);

uint8_t graph_get_distance_to_winning_square(uint8_t target_south_else_north, uint8_t start_node);
uint8_t graph_node_index_from_row_col(uint8_t row, uint8_t col);

uint8_t graph_get_orhtogonal_neighbour_node_if_not_blocked(uint8_t node_index, int8_t direction);

_END_STD_C

#endif // QUORIDOR_GRAPH