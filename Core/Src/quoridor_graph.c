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

void graph_init(){
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

void graph_add_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
{
    // set_wall_by_row_col();
    uint8_t base_square;
    base_square = graph_node_index_from_row_col(row, col);

    if (horizontal_else_vertical)
    {
        disconnect_nodes(base_square, base_square - 9);
        disconnect_nodes(base_square - 1, base_square - 1 - 9);
    }
    else
    {
        disconnect_nodes(base_square, base_square + 1);
        disconnect_nodes(base_square - 9, base_square + 1 - 9);
    }
}

void disconnect_nodes(uint8_t node_1, uint8_t node_2)
{
    delete_edge(node_1, node_2);
    delete_edge(node_2, node_1);
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
}


void raise_error(uint8_t error_code)
{
    while (1)
    {
    };
}