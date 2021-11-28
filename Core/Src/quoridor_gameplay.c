#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"

static uint8_t move_counter;
static Player players[2];

uint8_t board_graph[81][NEIGHBOURS_SIZE];

char moves[MAX_MOVES_COUNT][2];

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

void game_init(void)
{
    players[0].pawn.row += 0;
    players[0].pawn.col += 4;
    players[0].walls_placed = 0;

    players[1].pawn.row += 8;
    players[1].pawn.col += 4;
    players[1].walls_placed = 0;

    for (uint8_t move = 0; move < MAX_MOVES_COUNT; move++)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            moves[move][i] = 0;
        }
    }

    for (uint8_t player = 0; player < 2; player++)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            players[player].walls[i].row = 0;
            players[player].walls[i].col = 0;
            players[player].walls[i].horizontal_else_vertical = 0;
        }
    }

    // build up the board graph
    uint8_t index;
    uint8_t square_index;

    for (uint8_t col = 0; col < 9; col++)
    {
        for (uint8_t row = 0; row < 9; row++)
        {
            square_index = row * 9 + col;
            init_graph_node_get_neighbours(row, col, board_graph[square_index]);
            index++;
        }
    }

    board_graph;
    move_counter = 0;
}

uint8_t get_move_counter()
{
    return move_counter;
}

void add_wall_to_board_graph(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
{
    // set_wall_by_row_col();
    uint8_t base_square;
    base_square = row_col_to_square_index(row, col);

    if (horizontal_else_vertical)
    {
        disconnect_nodes(base_square, base_square - 9);
        disconnect_nodes(base_square - 1, base_square - 1 - 9);
        // disconnect_nodes(base_square, base_square + 9);
        // disconnect_nodes(base_square - 1, base_square - 1 + 9);
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

void init_graph_node_get_neighbours(uint8_t row, uint8_t col, uint8_t *neighbours)
{
    uint8_t neighbours_counter = 0;

    // NORTH neighbour
    if (row < 8)
    {
        neighbours[neighbours_counter] = index_from_row_col(row + 1, col);
        neighbours_counter++;
    }

    // EAST neighbour
    if (col < 8)
    {
        neighbours[neighbours_counter] = index_from_row_col(row, col + 1);
        neighbours_counter++;
    }

    // SOUTH neighbour
    if (row != 0)
    {
        neighbours[neighbours_counter] = index_from_row_col(row - 1, col);
        neighbours_counter++;
    }

    // WEST neighbour
    if (col != 0)
    {
        neighbours[neighbours_counter] = index_from_row_col(row, col - 1);
        neighbours_counter++;
    }

    while (neighbours_counter < NEIGHBOURS_SIZE)
    {
        neighbours[neighbours_counter] = FAKE_NEIGHBOUR;
        neighbours_counter++;
    }
}

void get_winning_distances(uint8_t* distances){
    distances[0] = get_distance_to_winning_square(0, pawn_get_position_as_square_index(0));
    distances[1] = get_distance_to_winning_square(1, pawn_get_position_as_square_index(1));
}

uint8_t get_distance_to_winning_square(uint8_t playing_player, uint8_t start_node)
{
    // get distance to nearest winning square

    // for quoridor, we need to calculate a path to the nearest winning node (aka row)

    uint8_t target_nodes_player_0[9] = {72, 73, 74, 75, 76, 77, 78, 79, 80};
    uint8_t target_nodes_player_1[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // set the target nodes depending on the active player.
    uint8_t *target_nodes;
    if (playing_player == 0)
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

uint8_t index_from_row_col(uint8_t row, uint8_t col)
{
    return row * 9 + col;
}

void move_next_replay(void)
{
        char *move = moves[move_counter];
        move_by_notation(move);
    
};

void move_by_notation(char* move_as_notation){
     uint8_t player = move_counter % 2;

    
    if (move_as_notation[0] == 'X')
    {
        // end of game
        return;
    }
    else if (move_as_notation[0] == 'N' || move_as_notation[0] == 'E' || move_as_notation[0] == 'S' || move_as_notation[0] == 'W')
    {
        pawn_move_by_notation(player, move_as_notation);
    }
    else
    {
        wall_set_by_notation(player, move_as_notation);
    }
    move_counter++;
}

void moves_string_to_moves(char *moves_as_string)
{
    // string of moves (in Lode notation) to separate moves
    uint16_t counter = 0;

    uint8_t char_counter_in_move = 0;
    for (uint8_t i = 0; i < MOVES_STRING_LENGTH; i++)
    {
        char c = moves_as_string[i];

        if (c == ',')
        {
            // moves have a maximum of two characters. If only one charcter, fill the second character with a dummy. If foresee problems other wise. e.g. previous game was NW, now overwrite with N , but the W is still there...
            if (char_counter_in_move == 1)
            {
                moves[counter][char_counter_in_move] = ' ';
            }
            else if (char_counter_in_move == 0)
            {
                // ERROR EMPTY MOVE
                raise_error(ERROR_NOTATION);
            }
            else if (char_counter_in_move > 2)
            {
                // ERROR TOO MANY CHARS in move
                raise_error(ERROR_NOTATION);
            }
            char_counter_in_move = 0;
            counter++;
        }
        else if (c == '\0')
        {
            // if last move is single char move (e.g. N), fill up second position.
            if (char_counter_in_move == 1)
            {
                moves[counter][char_counter_in_move] = ' ';
            }
            moves[counter + 1][0] = 'X';
            moves[counter + 1][1] = ' ';
            return;
        }

        else
        {
            moves[counter][char_counter_in_move] = c;
            char_counter_in_move++;
        }
    }
}

void raise_error(uint8_t error_code)
{
    while (1)
    {
    };
}

void wall_set_by_notation(uint8_t player, char *wall_position)
{
    // direction always two chars.

    uint8_t first_char = 1;
    uint8_t col;
    uint8_t row;
    uint8_t horizontal_else_vertical;

    for (uint8_t i = 0; i < 2; i++)
    {
        char c = wall_position[i];
        if (c >= 97 && c <= 104)
        {
            // a->h
            // N-S component
            if (first_char)
            {
                horizontal_else_vertical = 0;
            }
            col = c - 96;
            first_char = 0;
        }
        else if (c >= 49 && c <= 56)
        {
            // 1->9
            // E-W component

            if (first_char)
            {
                horizontal_else_vertical = 1;
            }
            row = c - 48;
            first_char = 0;
        }
        else
        {
            // INVALID ERROR
            raise_error(ERROR_NOTATION);
        }
    }
    set_wall_by_row_col(player, row, col, horizontal_else_vertical);
    add_wall_to_board_graph(row, col, horizontal_else_vertical);
    players[player].walls_placed++;
};

void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
{
    players[player].walls[players[player].walls_placed].row = row;
    players[player].walls[players[player].walls_placed].col = col;
    players[player].walls[players[player].walls_placed].horizontal_else_vertical = horizontal_else_vertical;
}

void walls_get_all_positions(uint8_t *positions, uint8_t player)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        positions[i * 3 + 0] = players[player].walls[i].row;
        positions[i * 3 + 1] = players[player].walls[i].col;
        positions[i * 3 + 2] = players[player].walls[i].horizontal_else_vertical;
    }
}

void pawn_move_by_notation(uint8_t player, char *direction)
{
    // direction always two chars. second char should be ' ' if single char move   // (which is a bit risky. For single character moves, the second character is the divider character between moves....)

    // double character moves (NN NE ,...) are basically treated as two separated pawn moves.
    for (uint8_t i = 0; i < 2; i++)
    {
        switch (direction[i])
        {
        case 'N':
        {
            pawn_set_position(player, 0, 1);
            break;
        }
        case 'S':
        {
            pawn_set_position(player, 0, -1);
            break;
        }
        case 'E':
        {
            pawn_set_position(player, 1, 0);
            break;
        }
        case 'W':
        {
            pawn_set_position(player, -1, 0);
            break;
        }
        case ' ':
        {
            // second char in single char move
            break;
        }
        case 0:
        {
            // second char in single char move
            // illegal notation. (i.e. uncovered.)
            raise_error(ERROR_NOTATION);
            break;
        }
        default:
        {
            // for single char moves, the second char will be something else...
            raise_error(ERROR_NOTATION);
            break;
        }
        }
    }
};

void pawn_set_position(uint8_t player, int8_t delta_x, int8_t delta_y)
{
    // DO NOT CHECK validity here. In the text notation, double moves (NN , NE,... ) will come here twice. e.g. IF NN, the first north would be invalid as it hits the other player.
    players[player].pawn.row += delta_y;
    players[player].pawn.col += delta_x;
}

uint8_t row_col_to_square_index(uint8_t row, uint8_t col)
{
    return row * 9 + col;
}

uint8_t pawn_get_position_as_square_index(uint8_t player)
{
    // player = 0 or 1
    return row_col_to_square_index(
        players[player].pawn.row,
        players[player].pawn.col);
}
void pawn_get_position_as_row_col(uint8_t *position, uint8_t player)
{
    // player = 0 or 1

    position[0] = players[player].pawn.row;
    position[1] = players[player].pawn.col;
    return position;
}
