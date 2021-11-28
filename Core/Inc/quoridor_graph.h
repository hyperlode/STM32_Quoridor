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
void graph_add_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical);
static void disconnect_nodes(uint8_t node_1, uint8_t node_2);
static void delete_edge(uint8_t start_node, uint8_t node_to_be_disconnected);
static void graph_node_get_neighbours_at_init(uint8_t row, uint8_t col, uint8_t *neighbours);

uint8_t graph_get_distance_to_winning_square(uint8_t target_south_else_north, uint8_t start_node);
uint8_t graph_node_index_from_row_col(uint8_t row, uint8_t col);
_END_STD_C

#endif // QUORIDOR_GRAPH
