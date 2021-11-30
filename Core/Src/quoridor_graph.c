#include <string.h>
#include <stdio.h>
#include "quoridor_graph.h"
#include "quoridor_config.h"
/*
board graph nodes indeces to quoridor board squares

  N
  ^
W + E
  S

72 73 74 75 76 77 78 79 80
63 64 65 66 67 68 69 70 71
54 55 56 57 58 59 60 61 62 
45 46 47 48 49 50 51 52 53
36 37 38 39 40 41 42 43 44 
27 28 29 30 31 32 33 34 35 
18 19 20 21 22 23 24 25 26
9  10 11 12 13 14 15 16 17
0  1  2  3  4  5  6  7  8 

*/
uint8_t board_graph[81][NEIGHBOURS_SIZE];

void graph_init()
{
    // build up the board graph
    uint8_t index;
    uint8_t square_index;

    for (uint8_t col = 0; col < 9; col++)
    {
        for (uint8_t row = 0; row < 9; row++)
        {
            square_index = row * 9 + col;
            graph_node_get_neighbours_at_init(row, col, board_graph[square_index]);
            index++;
        }
    }
}

void move_index_to_node_indeces(uint8_t move_index, uint8_t *node_indeces)
{
    if (move_index < MOVE_INDEX_FIRST_WALL)
    {
        raise_error(ERROR_NOT_A_WALL);
    }
    uint8_t offset;
    uint8_t row;
    uint8_t col;
    uint8_t horizontal_else_vertical;

    if (move_index < MOVE_INDEX_FIRST_VERTICAL_WALL)
    {
        offset = MOVE_INDEX_FIRST_HORIZONTAL_WALL;
        horizontal_else_vertical = 1;
    }
    else
    {
        offset = MOVE_INDEX_FIRST_VERTICAL_WALL;
        horizontal_else_vertical = 0;
    }

    move_index -= offset;
    row = (move_index / 8) + 1;
    col = (move_index % 8) + 1;

    uint8_t base_node_index = graph_node_index_from_row_col(row, col);
    node_indeces[0] = base_node_index;
    if (horizontal_else_vertical)
    {
        node_indeces[1] = base_node_index - 9;
        node_indeces[2] = base_node_index - 1;
        node_indeces[3] = base_node_index - 9 - 1;
    }
    else
    {
        node_indeces[1] = base_node_index - 1;
        node_indeces[2] = base_node_index - 9;
        node_indeces[3] = base_node_index - 1 - 9;
        // node_indeces[1] = base_node_index + 1;
        // node_indeces[2] = base_node_index - 9;
        // node_indeces[3] = base_node_index + 1 - 9;
    }
}

void graph_wall_add(uint8_t move_index)
{
    uint8_t node_indeces[4];

    move_index_to_node_indeces(move_index, node_indeces);
    disconnect_nodes(node_indeces[0], node_indeces[1]);
    disconnect_nodes(node_indeces[2], node_indeces[3]);
}

void graph_wall_remove(uint8_t move_index)
{
    uint8_t node_indeces[4];

    move_index_to_node_indeces(move_index, node_indeces);
    connect_nodes(node_indeces[0], node_indeces[1]);
    connect_nodes(node_indeces[2], node_indeces[3]);
}

// void graph_add_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
// {
//     // set_wall_by_row_col();
//     uint8_t base_square;
//     base_square = graph_node_index_from_row_col(row, col);

//     if (horizontal_else_vertical)
//     {
//         disconnect_nodes(base_square, base_square - 9);
//         disconnect_nodes(base_square - 1, base_square - 1 - 9);
//     }
//     else
//     {
//         disconnect_nodes(base_square, base_square + 1);
//         disconnect_nodes(base_square - 9, base_square + 1 - 9);
//     }
// }

uint8_t get_edge_exists(uint8_t node_start, uint8_t node_end)
{

    uint8_t i = 0;
    uint8_t node_neighbour;
    do
    {
        node_neighbour = board_graph[node_start][i];
        i++;

    } while (node_neighbour != node_end && node_neighbour != FAKE_NEIGHBOUR);

    return node_neighbour == node_end;
}

void disconnect_nodes(uint8_t node_1, uint8_t node_2)
{
    delete_edge(node_1, node_2);
    delete_edge(node_2, node_1);
}

void connect_nodes(uint8_t node_1, uint8_t node_2)
{
    add_edge(node_1, node_2);
    add_edge(node_2, node_1);
}

void add_edge(uint8_t start_node, uint8_t node_to_be_connected)
{
#define NOT_FOUND_INDEX 66
    uint8_t i = 0;
    uint8_t delete_node_index = NOT_FOUND_INDEX;
    uint8_t check_node;

    // search for end
    while (board_graph[start_node][i] != FAKE_NEIGHBOUR)
    {
        i++;

        if (i >= NEIGHBOURS_SIZE)
        {
            raise_error(NEIGHBOURS_OUT_OF_BOUNDS);
        }
    }

    // add node
    board_graph[start_node][i] = node_to_be_connected;
}

void delete_edge(uint8_t start_node, uint8_t node_to_be_disconnected)
{

#define NOT_FOUND_INDEX 66
    uint8_t i = 0;
    uint8_t delete_node_index = NOT_FOUND_INDEX;
    uint8_t check_node;

    do
    {
        check_node = board_graph[start_node][i];
        if (check_node == node_to_be_disconnected)
        {
            delete_node_index = i;
        }
        i++;
    } while (check_node != FAKE_NEIGHBOUR);

    if (delete_node_index == NOT_FOUND_INDEX)
    {
        // NOT FOUND
        raise_error(ERROR_DELETE_EDGE);
        return;
    }

    i--; // get last i right
    i--; // get index of last valid node

    if (i == delete_node_index)
    {
        // only one valid node available
    }
    else
    {
        board_graph[start_node][delete_node_index] = board_graph[start_node][i];
    }
    board_graph[start_node][i] = FAKE_NEIGHBOUR;
}

void graph_node_get_neighbours_at_init(uint8_t row, uint8_t col, uint8_t *neighbours)
{
    uint8_t neighbours_counter = 0;

    // NORTH neighbour
    if (row < 8)
    {
        neighbours[neighbours_counter] = graph_node_index_from_row_col(row + 1, col);
        neighbours_counter++;
    }

    // EAST neighbour
    if (col < 8)
    {
        neighbours[neighbours_counter] = graph_node_index_from_row_col(row, col + 1);
        neighbours_counter++;
    }

    // SOUTH neighbour
    if (row != 0)
    {
        neighbours[neighbours_counter] = graph_node_index_from_row_col(row - 1, col);
        neighbours_counter++;
    }

    // WEST neighbour
    if (col != 0)
    {
        neighbours[neighbours_counter] = graph_node_index_from_row_col(row, col - 1);
        neighbours_counter++;
    }

    while (neighbours_counter < NEIGHBOURS_SIZE)
    {
        neighbours[neighbours_counter] = FAKE_NEIGHBOUR;
        neighbours_counter++;
    }
}

uint8_t graph_node_index_from_row_col(uint8_t row, uint8_t col)
{
    return row * 9 + col;
}

uint8_t graph_get_pawn_move_destination_node(uint8_t node_index, uint8_t move_index)
{

    // Only pawn moves allowed

    if (move_index < 4)
    {
        return graph_get_orhtogonal_neighbour_node(node_index, move_index);
    }
    else if (move_index < 8)
    {
        return graph_get_orhtogonal_neighbour_node_jump(node_index, move_index);
    }
    else if (move_index < 12)
    {
        return graph_get_diagonal_neighbour_node(node_index, move_index);
    }
    else
    {
        raise_error(ERROR_NOTATION_NOT_A_PAWN_MOVE);
    }
}

uint8_t graph_get_diagonal_neighbour_node(uint8_t node_index, uint8_t move_index)
{
    // does not check for reachability!!!

    uint8_t neighbour_index;
    uint8_t direction_0;
    uint8_t direction_1;

    switch (move_index)
    {
    case (8):
    {
        direction_0 = 0;
        direction_1 = 1;
        break;
    }
    case (9):
    {
        direction_0 = 0;
        direction_1 = 3;
        break;
    }
    case (10):
    {
        direction_0 = 2;
        direction_1 = 1;
        break;
    }
    case (11):
    {
        direction_0 = 2;
        direction_1 = 3;
        break;
    }
    default:
    {
        raise_error(ERROR_NOTATION_NOT_A_PAWN_MOVE);
        break;
    }
    }

    neighbour_index = graph_get_orhtogonal_neighbour_node(node_index, direction_0);
    return graph_get_orhtogonal_neighbour_node(neighbour_index, direction_1);
}

uint8_t graph_get_orhtogonal_neighbour_node_jump(uint8_t node_index, uint8_t move_index)
{
    // does not check for reachability!!!
    uint8_t neighbour_index;
    neighbour_index = graph_get_orhtogonal_neighbour_node(node_index, move_index - 4);
    return graph_get_orhtogonal_neighbour_node(neighbour_index, move_index - 4);
}

uint8_t graph_get_orhtogonal_neighbour_node(uint8_t node_index, uint8_t direction)
{
    // does not check for reachability!!!
    // direct neighbour N,E,S,W = 0,1,3,4

    if (node_index == FAKE_NEIGHBOUR)
    {
        raise_error(ERROR_FAKE_NODE);
    }
    uint8_t neighbour_index;

    switch (direction)
    {
    case (0):
    {
        if (node_index > 71)
        {
            return FAKE_NEIGHBOUR;
        }
        else
        {
            neighbour_index = node_index + 9;
        }
        break;
    }
    case (1):
    {
        if ((node_index + 1) % 9 == 0)
        {
            return FAKE_NEIGHBOUR;
        }
        else
        {
            neighbour_index = node_index + 1;
        }
        break;
    }
    case (2):
    {
        if (node_index < 9)
        {
            return FAKE_NEIGHBOUR;
        }
        else
        {
            neighbour_index = node_index - 9;
        }
        break;
    }
    case (3):
    {
        if ((node_index) % 9 == 0)
        {
            return FAKE_NEIGHBOUR;
        }
        else
        {
            neighbour_index = node_index - 1;
        }
        break;
    }
    default:
    {
        raise_error(ERROR_NOT_AN_ORTHOGONAL_DIRECTION);
    }
    }
    return neighbour_index;
}
uint8_t graph_get_orhtogonal_neighbour_node_if_not_blocked(uint8_t node_index, uint8_t direction)
{
    // if not accessible, will return FAKE_NEIGHBOUR

    uint8_t neighbour_index;
    neighbour_index = graph_get_orhtogonal_neighbour_node(node_index, direction);
    // neighbour is identified. But, are they connected?
    if (get_edge_exists(node_index, neighbour_index))
    {
        return neighbour_index;
    }
    else
    {
        return FAKE_NEIGHBOUR;
    }
}

int8_t graph_delta_of_distances(uint8_t start_node_target_to_north, uint8_t start_node_target_to_south)
{
    //delta is the difference in path length between two paths. in python : delta = toSouthPathLength - toNorthPathLength

    // delta = target_south - target_north
    // --> delta negative: to south player is winning
    // --> delta positive: to north player is winning
    uint8_t dist_to_south = graph_get_distance_to_winning_square(1, start_node_target_to_south);

    if (dist_to_south == PAWN_TARGET_NOT_REACHABLE)
    {
        return PAWN_TARGET_NOT_REACHABLE;
    }
    uint8_t dist_to_north = graph_get_distance_to_winning_square(0, start_node_target_to_north);
    if (dist_to_north == PAWN_TARGET_NOT_REACHABLE)
    {
        return PAWN_TARGET_NOT_REACHABLE;
    }
    return dist_to_south - dist_to_north;
}

uint8_t graph_get_distance_to_winning_square(uint8_t target_south_else_north, uint8_t start_node)
{
    // get distance to nearest winning square

    // for quoridor, we need to calculate a path to the nearest winning node (aka row)

    uint8_t target_nodes_player_0[9] = {72, 73, 74, 75, 76, 77, 78, 79, 80};
    uint8_t target_nodes_player_1[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // set the target nodes depending on the active player.
    uint8_t *target_nodes;
    if (target_south_else_north == 0)
    {
        target_nodes = target_nodes_player_0;
    }
    else
    {
        target_nodes = target_nodes_player_1;
    }

    uint8_t visited[81];        // nodes as index
    uint8_t nodes_to_visit[81]; // contains nodes to be visited
    uint8_t nodes_to_visit_count = 0;
    uint8_t distance_to_start_node[81]; // dijkstra scores here nodes as index
    uint8_t target_reached = 0;

    uint8_t active_node = start_node;
    for (uint8_t i = 0; i < 81; i++)
    {
        visited[i] = NOT_VISITED;
        nodes_to_visit[i] = NONE_NODE;
        distance_to_start_node[i] = BYTE_INFINITY;
    }

    // initialize
    nodes_to_visit[nodes_to_visit_count] = start_node;
    nodes_to_visit_count = 1;
    distance_to_start_node[start_node] = 0; // distance to itself = 0

    //while there are still nodes to visit and end no target nodes reached.
    while (nodes_to_visit_count > 0 && target_reached == 0)
    {
        // as long as nodes_to_visit_count > 0: there are still nodes that need to be visited.

        // search for potential active node (= node with shortest distance)
        uint8_t active_node;
        uint8_t active_node_distance = BYTE_INFINITY;
        uint8_t index_of_active_node_in_nodes_to_visit;
        for (uint8_t i = 0; i < nodes_to_visit_count; i++)
        {
            if (distance_to_start_node[nodes_to_visit[i]] <= active_node_distance)
            {
                active_node = nodes_to_visit[i];
                active_node_distance = distance_to_start_node[nodes_to_visit[i]];
                index_of_active_node_in_nodes_to_visit = i;
            }
        }

        // check all neighbours of active node
        uint8_t neighbour_counter = 0;
        uint8_t neighbour_node = board_graph[active_node][neighbour_counter];
        while (neighbour_node != FAKE_NEIGHBOUR && neighbour_counter < NEIGHBOURS_SIZE)
        {

            // check if neighbour node is target node. If so, goal reached. report.
            for (uint8_t i = 0; i < 9; i++)
            {
                if (neighbour_node == target_nodes[i])
                {
                    // goal reached
                    return active_node_distance + 1;
                }
            }

            // handle neighbour
            if (visited[neighbour_node] == NOT_VISITED)
            {
                // check if distance to neighbour node is shorter than what the neighbour node already has stored. If so, replace by new shorter distance (yippeee)
                if ((active_node_distance + 1) < distance_to_start_node[neighbour_node])
                {
                    distance_to_start_node[neighbour_node] = active_node_distance + 1;
                }

                // add neighbour to nodes to visit. take care: It might already be in the list. (in that case, do not add...)
                uint8_t neighbour_node_was_already_added_for_being_visited = 0;
                for (uint8_t i = 0; i < nodes_to_visit_count; i++)
                {
                    if (nodes_to_visit[i] == neighbour_node)
                    {
                        neighbour_node_was_already_added_for_being_visited = 1;
                    }
                }

                if (!neighbour_node_was_already_added_for_being_visited)
                {
                    nodes_to_visit[nodes_to_visit_count] = neighbour_node;
                    nodes_to_visit_count++;
                }
            }

            neighbour_counter++;
            neighbour_node = board_graph[active_node][neighbour_counter];

            // next neighbour
        }
        // delete node from nodes to visit (replace it by the last added node)
        nodes_to_visit[index_of_active_node_in_nodes_to_visit] = nodes_to_visit[nodes_to_visit_count - 1];
        nodes_to_visit[nodes_to_visit_count - 1] = FAKE_NEIGHBOUR;
        nodes_to_visit_count--;

        // add node as DONE (so ti )
        visited[active_node] = VISITED;
    }

    return PAWN_TARGET_NOT_REACHABLE;
}
