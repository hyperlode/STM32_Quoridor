#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"
#include "quoridor_notation.h"

static uint8_t move_counter;
static Player players[2];

uint8_t moves_indeces[MAX_MOVES_COUNT];

int8_t moves_delta[140];                           // store all deltas of moves. Combine with the "move_indeces_valid_for_current_board" to only check the relevant indeces.
uint8_t move_indeces_valid_for_current_board[140]; // move possible=1 , not possible = 0; The pawn moves have to be revisited at every move. Walls are easier. Once placed, they're fixed.

void game_init(void)
{
    players[0].pawn.row = 0;
    players[0].pawn.col = 4;
    players[0].walls_placed = 0;

    players[1].pawn.row = 8;
    players[1].pawn.col = 4;
    players[1].walls_placed = 0;

    for (uint8_t player = 0; player < 2; player++)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            players[player].walls[i].row = 0;
            players[player].walls[i].col = 0;
            players[player].walls[i].horizontal_else_vertical = 0;
        }
    }
    for (uint8_t i = 0; i < MOVE_INDEX_COUNT; i++)
    {
        move_indeces_valid_for_current_board[i] = 1;
        // moves_delta[i] = 0;
    }
    graph_init();

    analyse_possible_moves(get_playing_player()); // player is 0 at init.

    move_counter = 0;
}

uint8_t get_walls_placed(uint8_t player){
    return players[player].walls_placed;
}

int8_t get_delta_of_move_index(uint8_t move_index)
{

    return moves_delta[move_index];
}

void analyse_possible_moves(uint8_t player)
{
    analyse_possible_moves_pawn(player);
    analyse_possible_moves_walls();
}

void analyse_possible_moves_walls()
{
    for (uint8_t i = MOVE_INDEX_FIRST_WALL; i < MOVE_INDEX_COUNT; i++)
    {
        if (move_indeces_valid_for_current_board[i])
        {
            graph_wall_add(i);
            int8_t delta;
            delta = graph_delta_of_distances(
                pawn_get_position_as_node_index(0),
                pawn_get_position_as_node_index(1));

            moves_delta[i] = delta;
            move_indeces_valid_for_current_board[i] = (delta != PAWN_TARGET_NOT_REACHABLE);

            graph_wall_remove(i);
        }
    }

    // break
    ;
}

void analyse_possible_moves_pawn(uint8_t player)
{
    // player = moving player index (0(toNorth) or 1)
    for (uint8_t move_index = 0; move_index < MOVE_INDEX_FIRST_WALL; move_index++)
    {
        uint8_t valid;
        valid = check_move_possible_pawn(move_index, get_playing_player());
        move_indeces_valid_for_current_board[move_index] = valid;

        if (valid)
        {
            // calculate delta for the given move.

            // the pawn is not moved. This is just looking at the nodes.

            uint8_t player_node = pawn_get_position_as_node_index(player);
            uint8_t pawn_moved_node = graph_get_pawn_move_destination_node(player_node, move_index);

            int8_t delta=0;
            if (player)
            {
                // to south (neg delta is better)
                for (uint8_t winning_node=0;winning_node<9;winning_node++){
                    if (pawn_moved_node == winning_node){
                        delta = -1 * DELTA_WINNING_MOVE_MAGNITUDE;
                    }
                }
                if (delta !=  -1 * DELTA_WINNING_MOVE_MAGNITUDE){
                    delta = graph_delta_of_distances(
                        pawn_get_position_as_node_index(0),
                        pawn_moved_node);
                }
            }
            else
            {
                for (uint8_t winning_node=72;winning_node<81;winning_node++){
                    if (pawn_moved_node == winning_node){
                        delta = DELTA_WINNING_MOVE_MAGNITUDE;
                    }
                }
                if (delta != DELTA_WINNING_MOVE_MAGNITUDE){
                    delta = graph_delta_of_distances(
                        pawn_moved_node,
                        pawn_get_position_as_node_index(1));
                }
            }
            moves_delta[move_index] = delta;
        }
        else
        {
            moves_delta[move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
        }
    }
}

uint8_t get_move_counter()
{
    return move_counter;
}

void load_game_by_notation_one_string(moves_as_notation_one_string)
{
    moves_string_to_moves_indeces(moves_as_notation_one_string, moves_indeces);
}

void next_move_loaded_game(void)
{
    uint8_t move_index = moves_indeces[move_counter];
    make_move(move_index);
};

void make_move(uint8_t move_index)
{

    // check if move is in the possible moves list
    if (!move_indeces_valid_for_current_board[move_index])
    {
        raise_error(ERROR_NOT_A_VALID_MOVE_ON_THIS_BOARD);
    }

    // execute move

    if (move_index == MOVE_INDEX_END_OF_GAME)
    {
        // end of game
        return;
    }
    else if (move_index < MOVE_INDEX_FIRST_WALL)
    {
        make_move_pawn(get_playing_player(), move_index);
    }
    else
    {
        make_move_wall(get_playing_player(), move_index);
    }

    move_counter++;

    // prepare for next move
    analyse_possible_moves(get_playing_player());

    return;
}

uint8_t get_playing_player()
{
    return move_counter % 2;
}
uint8_t get_opponent(uint8_t player)
{
    return !player;
}

uint8_t check_move_possible_pawn(uint8_t move_index, uint8_t player)
{

    // Only pawn moves allowed

    if (move_index < 4)
    {
        return check_move_possible_pawn_orthogonal(move_index, player);
    }
    else if (move_index < 8)
    {
        return check_move_possible_pawn_orthogonal_jump(move_index, player);
    }
    else if (move_index < 12)
    {
        return check_move_possible_pawn_diagonal(move_index, player);
    }
    else
    {
        raise_error(ERROR_NOTATION_NOT_A_PAWN_MOVE);
    }
}

uint8_t check_move_possible_pawn_orthogonal(uint8_t move_index, uint8_t player)
{

    // Only pawn moves allowed
    // if (move_index >= 4)
    // {
    //     raise_error(ERROR_NOTATION_NOT_AN_ORTHO_PAWN_MOVE);
    // }

    uint8_t player_node = pawn_get_position_as_node_index(player);
    uint8_t neighbour_node;
    uint8_t opponent_node;

    // get direct neighbour from move_index
    neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(player_node, move_index);

    // if a wall is detected, return right away
    if (neighbour_node == FAKE_NEIGHBOUR)
    {
        return 0;
    }

    // check if the other pawn is on adjecent square
    opponent_node = pawn_get_position_as_node_index(get_opponent(player));

    // no opponent allowed on neighbouring square
    if (neighbour_node == opponent_node)
    {
        return 0;
    }
    return 1;
}

uint8_t check_move_possible_pawn_orthogonal_jump(uint8_t move_index, uint8_t player)
{

    // Only pawn moves allowed
    // if (move_index >= 4)
    // {
    //     raise_error(ERROR_NOTATION_NOT_AN_ORTHO_PAWN_MOVE_JUMP);
    // }

    uint8_t player_node = pawn_get_position_as_node_index(player);
    uint8_t neighbour_node;
    uint8_t opponent_node;
    uint8_t neighbour_neighbour_node;
    uint8_t direction;

    // get direct neighbour from move_index
    direction = move_index - 4; // normal case
    neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(player_node, direction);

    // check if other pawn is on adjecent square
    opponent_node = pawn_get_position_as_node_index(get_opponent(player));

    // we need a neighbour there
    if (neighbour_node != opponent_node)
    {
        return 0;
    }

    // get node behind opponent
    neighbour_neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(opponent_node, direction);

    if (neighbour_neighbour_node == FAKE_NEIGHBOUR)
    {
        return 0;
    }

    return 1;
}
uint8_t check_move_possible_pawn_diagonal(uint8_t move_index, uint8_t player)
{
    // diagonals can be reached on two sides. ugh. so if one direct is not working another might.
    uint8_t player_node = pawn_get_position_as_node_index(player);

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

    if (check_move_possible_pawn_L_jump(player, player_node, direction_0, direction_1))
    {
        return 1;
    }

    return check_move_possible_pawn_L_jump(player, player_node, direction_1, direction_0);
}

uint8_t check_move_possible_pawn_L_jump(player, start_node, direction_1, direction_2)
{
    // a diagonal jump is successful if it succeeds in one of the two possible l jumps. e.g. SE --> first south, then east, OR first east, then south.

    // direction 1 and direction 2 are ortho directions 0,1,2,3 fro N,E,S,W

    uint8_t neighbour_node;
    uint8_t opponent_node;
    uint8_t neighbour_neighbour_node;
    uint8_t direction;

    // get direct neighbour from move_index
    neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(start_node, direction_1);

    // check if other pawn is on adjecent square
    opponent_node = pawn_get_position_as_node_index(get_opponent(player));

    // we need a neighbour there
    if (neighbour_node != opponent_node)
    {
        return 0;
    }

    // get node behind opponent
    neighbour_neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(opponent_node, direction_1);

    // ortho jump MUST fail
    // the node needs to be blocked for an ortho jump to be feasable
    if (neighbour_neighbour_node != FAKE_NEIGHBOUR)
    {
        return 0;
    }

    // no wall are allowed to block
    neighbour_neighbour_node = graph_get_orhtogonal_neighbour_node_if_not_blocked(opponent_node, direction_2);
    if (neighbour_neighbour_node == FAKE_NEIGHBOUR)
    {
        return 0;
    }
    return 1;
}

void make_move_wall(uint8_t player, uint8_t move_index)
{
    uint8_t wall_row_col_dir[3];

    if (players[player].walls_placed >= 10){
        raise_error(ERROR_NO_MORE_WALLS_LEFT);
    }

    move_index_to_row_col_dir(move_index, wall_row_col_dir);
    set_wall_by_row_col(player, wall_row_col_dir[0], wall_row_col_dir[1], wall_row_col_dir[2]);
    graph_wall_add(move_index);

    // delete invalid moves from the valid moves list.
    move_indeces_valid_for_current_board[move_index] = 0;
    moves_delta[move_index] = FAKE_DELTA_FOR_INVALID_MOVE;

    uint8_t r_c_d[3];

    uint8_t invalid_move_index;

    if (wall_row_col_dir[2])
    {
        // horizonal

        if (wall_row_col_dir[1] < 8)
        {
            r_c_d[0] = wall_row_col_dir[0];
            r_c_d[1] = wall_row_col_dir[1] + 1;
            r_c_d[2] = 1;
            invalid_move_index = row_col_dir_to_move_index(r_c_d);
            move_indeces_valid_for_current_board[invalid_move_index] = 0;
            moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
        }
        if (wall_row_col_dir[1] > 1)
        {
            r_c_d[0] = wall_row_col_dir[0];
            r_c_d[1] = wall_row_col_dir[1] - 1;
            r_c_d[2] = 1;
            invalid_move_index = row_col_dir_to_move_index(r_c_d);
            move_indeces_valid_for_current_board[invalid_move_index] = 0;
            moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
        }
        r_c_d[0] = wall_row_col_dir[0];
        r_c_d[1] = wall_row_col_dir[1];
        r_c_d[2] = 0;
        invalid_move_index = row_col_dir_to_move_index(r_c_d);
        move_indeces_valid_for_current_board[invalid_move_index] = 0;
        moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
    }
    else
    {
        // vertical

        if (wall_row_col_dir[0] < 8)
        {
            r_c_d[0] = wall_row_col_dir[0] + 1;
            r_c_d[1] = wall_row_col_dir[1];
            r_c_d[2] = 0;
            invalid_move_index = row_col_dir_to_move_index(r_c_d);
            move_indeces_valid_for_current_board[invalid_move_index] = 0;
            moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
        }
        if (wall_row_col_dir[0] > 1)
        {
            r_c_d[0] = wall_row_col_dir[0] - 1;
            r_c_d[1] = wall_row_col_dir[1];
            r_c_d[2] = 0;
            invalid_move_index = row_col_dir_to_move_index(r_c_d);
            move_indeces_valid_for_current_board[invalid_move_index] = 0;
            moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
        }
        r_c_d[0] = wall_row_col_dir[0];
        r_c_d[1] = wall_row_col_dir[1];
        r_c_d[2] = 1;
        invalid_move_index = row_col_dir_to_move_index(r_c_d);
        move_indeces_valid_for_current_board[invalid_move_index] = 0;
        moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
    }

    players[player].walls_placed++;
};

uint8_t row_col_dir_to_move_index(uint8_t *row_col_dir)
{
    uint8_t offset;
    if (row_col_dir[2])
    { // horizontal
        offset = MOVE_INDEX_FIRST_WALL;
    }
    else
    {
        offset = MOVE_INDEX_FIRST_VERTICAL_WALL;
    }

    return ((row_col_dir[0] - 1) * 8) + (row_col_dir[1] - 1) + offset;
}

void move_index_to_row_col_dir(uint8_t move_index, uint8_t *row_col_dir)
{
    if (move_index < MOVE_INDEX_FIRST_WALL)
    {
        raise_error(ERROR_NOT_A_WALL);
        return;
    }

    uint8_t offset;
    if (move_index < MOVE_INDEX_FIRST_VERTICAL_WALL)
    { // horizontal
        offset = MOVE_INDEX_FIRST_WALL;
        row_col_dir[2] = 1;
    }
    else
    {
        offset = MOVE_INDEX_FIRST_VERTICAL_WALL;
        row_col_dir[2] = 0;
    }

    move_index -= offset;

    row_col_dir[0] = (move_index / 8) + 1;
    row_col_dir[1] = (move_index % 8) + 1;
}

void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
{
    players[player].walls[players[player].walls_placed].row = row;
    players[player].walls[players[player].walls_placed].col = col;
    players[player].walls[players[player].walls_placed].horizontal_else_vertical = horizontal_else_vertical;
}

void walls_get_all_positions(uint8_t *positions, uint8_t player)
{
    // mainly for an output situation to the graphics display
    for (uint8_t i = 0; i < 10; i++)
    {
        positions[i * 3 + 0] = players[player].walls[i].row;
        positions[i * 3 + 1] = players[player].walls[i].col;
        positions[i * 3 + 2] = players[player].walls[i].horizontal_else_vertical;
    }
}

void make_move_pawn(uint8_t player, uint8_t move_index)
{
    uint8_t deltas_row_col[2];
    pawn_move_index_to_row_col_deltas(move_index, deltas_row_col);

    players[player].pawn.row += deltas_row_col[0];
    players[player].pawn.col += deltas_row_col[1];
};

void pawn_move_index_to_row_col_deltas(uint8_t move_index, int8_t *deltas_row_col)
{
    uint8_t delta_row;
    uint8_t delta_col;
    switch (move_index)
    {
    case 0:
    {
        delta_row = 1;
        delta_col = 0;
        break;
    }
    case 1:
    {
        delta_row = 0;
        delta_col = 1;
        break;
    }
    case 2:
    {
        delta_row = -1;
        delta_col = 0;
        break;
    }
    case 3:
    {
        delta_row = 0;
        delta_col = -1;
        break;
    }
    // JUMPS
    case 4:
    {
        delta_row = 2;
        delta_col = 0;
        break;
    }
    case 5:
    {
        delta_row = 0;
        delta_col = 2;
        break;
    }
    case 6:
    {
        delta_row = -2;
        delta_col = 0;
        break;
    }
    case 7:
    {
        delta_row = 0;
        delta_col = -2;
        break;
    }
    // DIAGONALS
    case 8:
    {
        delta_row = 1;
        delta_col = 1;
        break;
    }
    case 9:
    {
        delta_row = 1;
        delta_col = -1;
        break;
    }
    case 10:
    {
        delta_row = -1;
        delta_col = 1;
        break;
    }
    case 11:
    {
        delta_row = -1;
        delta_col = -1;
        break;
    }
    default:
    {
        // for single char moves, the second char will be something else...
        raise_error(ERROR_NOTATION);
        break;
    }
    }
    deltas_row_col[0] = delta_row;
    deltas_row_col[1] = delta_col;
}

void get_winning_distances(uint8_t *distances)
{
    distances[0] = graph_get_distance_to_winning_square(0, pawn_get_position_as_node_index(0));
    distances[1] = graph_get_distance_to_winning_square(1, pawn_get_position_as_node_index(1));
}

uint8_t pawn_get_position_as_node_index(uint8_t player)
{
    // player = 0 or 1
    return graph_node_index_from_row_col(
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
