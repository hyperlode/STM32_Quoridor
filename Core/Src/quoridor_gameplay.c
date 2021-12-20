#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"
#include "quoridor_notation.h"
#include "quoridor_games_database.h"

static Player players[2];

int8_t moves_delta[MOVE_INDEX_COUNT];                  // store all deltas of moves. Combine with the "move_index_invalidity_score" to only check the relevant indeces.
uint8_t move_index_invalidity_score[MOVE_INDEX_COUNT]; // move possible=0 , not possible>0; The pawn moves have to be revisited at every move. Walls are easier. Once placed, they're fixed.
uint8_t move_index_invalid_noExit_or_outOfWalls[MOVE_INDEX_COUNT];
uint8_t player_winner_index;

int8_t move_history_deltas_without_jumps[RECORD_MOVES_HISTORY_LENGTH]; // contains the delta change for every move

uint8_t game_history_moves_indeces[RECORD_MOVES_HISTORY_LENGTH]; // contains all moves of a game by move_index
static uint8_t move_counter;

uint8_t move_indeces_to_reversed[12] = {2, 3, 0, 1, 6, 7, 4, 5, 11, 10, 9, 8};

void game_init(char *load_game_history_moves_indeces, uint8_t load_game_moves_count)
{
    // load game in lode notation

    players[0].pawn.row = 0;
    players[0].pawn.col = 4;
    players[0].walls_placed = 0;

    players[1].pawn.row = 8;
    players[1].pawn.col = 4;
    players[1].walls_placed = 0;

    for (uint8_t player = 0; player < 2; player++)
    {
        for (uint8_t i = 0; i < PLAYER_MAX_WALL_COUNT; i++)
        {
            players[player].walls[i].row = WALL_NOT_PLACED;
            players[player].walls[i].col = WALL_NOT_PLACED;
            players[player].walls[i].horizontal_else_vertical = WALL_NOT_PLACED;
        }
    }

    for (uint8_t i = 0; i < RECORD_MOVES_HISTORY_LENGTH; i++)
    {
        move_history_deltas_without_jumps[i] = 0;
    }

    for (uint8_t i = 0; i < MOVE_INDEX_COUNT; i++)
    {
        move_index_invalidity_score[i] = MOVE_INDEX_VALID;
        move_index_invalid_noExit_or_outOfWalls[i] = MOVE_INDEX_VALID;
        // move_index_invalid_since_move[i] = MOVE_INDEX_VALID_MOVE_INFINITY;
    }
    graph_init();

    move_counter = 0;
    opening_initiate_next_move_handler();
    analyse_possible_moves(get_playing_player()); // player is 0 at init.
    player_winner_index = NO_WINNER;

    // load game
    for (uint8_t i = 0; i < load_game_moves_count; i++)
    {
        make_move(load_game_history_moves_indeces[i]);
    }
}

uint8_t get_winner_index()
{
    // will return NO_WINNER if nobody won
    return player_winner_index;
}

int8_t *get_move_history_deltas_without_jumps()
{
    return move_history_deltas_without_jumps;
}

// uint8_t check_pawn_on_winning_square(player){
//     uint8_t player_node = pawn_get_position_as_node_index(player);

//     if (player)
//     {
//         return player_node <= 9;
//     }
//     else
//     {
//         return player_node >=72;
//     }
// }

uint8_t get_playing_player()
{
    return move_counter % 2;
}
uint8_t get_opponent(uint8_t player)
{
    return !player;
}

uint8_t get_player_won(uint8_t player)
{
    // winner / end of game
    uint8_t row_col[2];
    pawn_get_position_as_row_col(row_col, player);
    if (player)
    {
        return row_col[0] == 0;
    }
    else
    {
        return row_col[0] == 8;
    }
    return 0;
}

void get_all_valid_move_indeces(uint8_t *valid_move_indeces, uint8_t* valid_move_indeces_counter_arg)
{
    // provide an array of MOVE_INDEX_COUNT length

    uint8_t valid_move_indeces_counter = 0;
    for (uint8_t i = 0; i < MOVE_INDEX_COUNT; i++)
    {

        if (get_move_index_valid(i))
        {
            valid_move_indeces[valid_move_indeces_counter] = i;
            valid_move_indeces_counter++;
        }
    }

    for (uint8_t i = valid_move_indeces_counter; i < MOVE_INDEX_COUNT; i++)
    {
        valid_move_indeces[i] = MOVE_INDEX_DUMMY;
    }

    //return valid_move_indeces_counter;
    *valid_move_indeces_counter_arg = valid_move_indeces_counter;
}

uint8_t get_move_index_valid(uint8_t move_index)
{
    uint8_t move_physically_possible_on_board = move_index_invalidity_score[move_index] == MOVE_INDEX_VALID;
    uint8_t move_not_preventing_pawn_from_winning = move_index_invalid_noExit_or_outOfWalls[move_index] == MOVE_INDEX_VALID;

    return (move_physically_possible_on_board && move_not_preventing_pawn_from_winning);
}

uint8_t get_walls_placed(uint8_t player)
{
    return players[player].walls_placed;
}

int8_t get_delta_of_move_index(uint8_t move_index)
{
    return moves_delta[move_index];
}

int8_t get_delta_of_move_index_normalized(uint8_t move_index)
{
    // for each player equal: higher delta = better.  (positive delta = winning, negative=losing)
    uint8_t delta_to_absolute_value;
    if (get_playing_player())
    {
        delta_to_absolute_value = -1;
    }
    else
    {
        delta_to_absolute_value = 1;
    }
    return moves_delta[move_index] * delta_to_absolute_value;
}

uint8_t get_best_pawn_move()
{
    uint8_t best_move_index; // there will always be at least one valid move in the end.
    int8_t best_move_index_delta = -127;
    uint8_t equal_deltas_count = 0;

    for (uint8_t move_index = 0; move_index < MOVE_INDEX_FIRST_WALL; move_index++)
    {
        if (get_move_index_valid(move_index))
        {
            int8_t delta = get_delta_of_move_index_normalized(move_index);
            if (delta == best_move_index_delta)
            {
                equal_deltas_count++;
            }
            else if (delta > best_move_index_delta)
            {
                equal_deltas_count = 0;
                best_move_index_delta = delta;
                best_move_index = move_index;
            }
        }
    }

    if (equal_deltas_count != 0)
    {
        return MOVE_INDEX_DUMMY;
    }
    else
    {
        return best_move_index;
    }
}

void get_winning_distances(uint8_t *distances)
{
    distances[0] = graph_get_distance_to_winning_square(0, pawn_get_position_as_node_index(0));
    distances[1] = graph_get_distance_to_winning_square(1, pawn_get_position_as_node_index(1));
}




////////////////////////
//////////////// ANALYSE 
////////////////////////

void analyse_possible_moves(uint8_t player)
{
    analyse_possible_moves_pawn(player);
    analyse_possible_moves_walls(player);
}

void analyse_possible_moves_walls(uint8_t player)
{
    // does player still have walls?

    if (players[player].walls_placed >= PLAYER_MAX_WALL_COUNT)
    {
        

        for (uint8_t i = MOVE_INDEX_FIRST_WALL; i < MOVE_INDEX_COUNT; i++)
        {
            move_index_invalid_noExit_or_outOfWalls[i] = MOVE_INDEX_INVALID;
        }

        return;
    }

    // what is the path delta for both players a when playing this move?
    // are there routes for both pawns to a winning square?
    for (uint8_t i = MOVE_INDEX_FIRST_WALL; i < MOVE_INDEX_COUNT; i++)
    {
        if (move_index_invalidity_score[i] == MOVE_INDEX_VALID)
        {
            graph_wall_add(i);
            int8_t delta;
            delta = graph_delta_of_distances(
                pawn_get_position_as_node_index(0),
                pawn_get_position_as_node_index(1));

            moves_delta[i] = delta;

            if (delta == PAWN_TARGET_NOT_REACHABLE)
            {

                move_index_invalid_noExit_or_outOfWalls[i] = MOVE_INDEX_INVALID;
            }
            else
            {
                move_index_invalid_noExit_or_outOfWalls[i] = MOVE_INDEX_VALID;
            }

            graph_wall_remove(i);
        }
    }
}

void analyse_possible_moves_pawn(uint8_t player)
{
    // player = moving player index (0(toNorth) or 1)
    for (uint8_t move_index = 0; move_index < MOVE_INDEX_FIRST_WALL; move_index++)
    {
        uint8_t valid;
        valid = check_move_possible_pawn(move_index, get_playing_player());

        if (!valid)
        {
            moves_delta[move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
            move_index_invalidity_score[move_index] = 1;
        }
        else
        {
            move_index_invalidity_score[move_index] = MOVE_INDEX_VALID;

            // calculate delta for the given move.

            // the pawn is not moved. This is just looking at the nodes.

            uint8_t player_node = pawn_get_position_as_node_index(player);
            uint8_t pawn_moved_node = graph_get_pawn_move_destination_node(player_node, move_index);

            int8_t delta = 0;
            if (player == PLAYER_TO_SOUTH)
            {
                // pawn on winning node ?
                if (pawn_moved_node <= 9)
                {

                    delta = -1 * DELTA_MAX_WINNING_MOVE_MAGNITUDE;
                }
                else
                {
                    delta = graph_delta_of_distances(
                        pawn_get_position_as_node_index(0),
                        pawn_moved_node);
                }
            }
            else
            {
                // pawn on winning node?
                if (pawn_moved_node >= 72)
                {
                    delta = DELTA_MAX_WINNING_MOVE_MAGNITUDE;
                }
                else
                {
                    delta = graph_delta_of_distances(
                        pawn_moved_node,
                        pawn_get_position_as_node_index(1));
                }
            }
            moves_delta[move_index] = delta;
        }
    }
}

uint8_t get_move_counter()
{
    return move_counter;
}

// void next_move_loaded_game(void)
// {
//     uint8_t move_index = game_history_moves_indeces[move_counter];
//     make_move(move_index);
// };


////////////////////////
//////////////// MAKE MOVE
////////////////////////


uint8_t make_move_if_valid(uint8_t move_index)
{
    if (!get_move_index_valid(move_index))
    {
        return 0;
    }
    if (move_index >= MOVE_INDEX_FIRST_WALL)
    {
        if (players[get_playing_player()].walls_placed >= PLAYER_MAX_WALL_COUNT)
        {
            return 0;
        }
    }
    make_move(move_index);
    return 1;
}

void make_move(uint8_t move_index)
{
    // store delta in move history (this does not take into account the other pawn.)

    if (move_counter < RECORD_MOVES_HISTORY_LENGTH)
    {
        move_history_deltas_without_jumps[move_counter] = graph_delta_of_distances(
            pawn_get_position_as_node_index(0),
            pawn_get_position_as_node_index(1));
    }

    // use make_move_if_valid() if there is user input and need for feedback
    // do a validy check before calling this function.
    // it will ASSERT error at invalid moves.

    // check if move is in the possible moves list
    if (move_index_invalidity_score[move_index] != MOVE_INDEX_VALID)
    {
        raise_error(ERROR_NOT_A_VALID_MOVE_ON_THIS_BOARD);
    }

    // execute move

    if (move_index == MOVE_INDEX_END_OF_GAME)
    {
        // end of game indication by player (give up before winning square reached)
        return;
    }
    else if (move_index < MOVE_INDEX_FIRST_WALL)
    {
        make_move_pawn(get_playing_player(), move_index);
        if (get_player_won(get_playing_player()))
        {
            player_winner_index = get_playing_player();
        }
        else
        {
            player_winner_index = NO_WINNER;
        }
    }
    else
    {
        make_move_wall(get_playing_player(), move_index);
    }

    game_history_moves_indeces[move_counter] = move_index;
    move_counter++;

    // prepare for next move
    opening_update_game_move(move_index, move_counter);
    analyse_possible_moves(get_playing_player());
}

void undo_last_move()
{

    // do not undo if no moves yet
    if (move_counter == 0)
    {
        return;
    }

    // get move index
    uint8_t previous_move_counter = move_counter - 1;
    uint8_t previous_player = previous_move_counter % 2;
    uint8_t previous_move_index = game_history_moves_indeces[previous_move_counter];
    if (previous_move_index < MOVE_INDEX_FIRST_WALL)
    {

        // do inverted move if pawn
        uint8_t move_index_pawn_inverted = move_indeces_to_reversed[previous_move_index];
        // get inverted move
        // apply move without the need for checking
        //
        make_move_pawn(previous_player, move_index_pawn_inverted);
    }
    else
    {
        // remove from graph if wall

        uint8_t row_col_dir[3];
        pop_last_placed_wall(previous_player, row_col_dir);
        graph_wall_remove(previous_move_index);

        // set back validities of affected wall positions
        adjust_affected_walls_validity_scores(previous_move_index, 0);

        // // undo "invalidity due to complete block"
        // for (uint8_t i=0;i<MOVE_INDEX_COUNT;i++){
        //     if (move_counter == move_index_invalid_since_move[i] ){
        //         if (move_index_invalidity_score[i] >0){
        //             move_index_invalidity_score[i] --;

        //         }else{
        //             // ASSERT ERROR: how did a move index become invalid at a certain move if it's actually indicated as valid?
        //             raise_error(ERROR_INCONSISTENCY_IN_INVALID_MOVE_DUE_TO_NO_EXIT);
        //         }
        //         move_index_invalid_since_move[i] = MOVE_INDEX_VALID_MOVE_INFINITY;
        //     }
        // }
    }

    move_history_deltas_without_jumps[move_counter] = 0;
    move_counter--;

    // reset opening database searcher
    opening_initiate_next_move_handler_from_game_moves(game_history_moves_indeces, move_counter);
    analyse_possible_moves(get_playing_player());
}

////////////////////////
//////////////// PAWN ACTIONS
////////////////////////

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
        raise_error(ERROR_NOTATION_NOT_A_PAWN_MOVE_FOR_CHECKING);
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
    // if (move_index >= 8)
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
        raise_error(ERROR_NOTATION_NOT_A_DIAGONAL_PAWN_MOVE);
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

/////////////////////////
/////////// WALL ACTIONS 
////////////////////////

uint8_t make_move_wall(uint8_t player, uint8_t move_index)
{

    uint8_t wall_row_col_dir[3];
    if (players[player].walls_placed >= PLAYER_MAX_WALL_COUNT)
    {
        raise_error(ERROR_NO_MORE_WALLS_LEFT);
        //return 0;
    }

    wall_move_index_to_row_col_dir(move_index, wall_row_col_dir);
    set_wall_by_row_col(player, wall_row_col_dir[0], wall_row_col_dir[1], wall_row_col_dir[2]);
    graph_wall_add(move_index);

    adjust_affected_walls_validity_scores(move_index, 1);
}

void adjust_affected_walls_validity_scores(uint8_t move_index, uint8_t add_wall_else_delete)
{

    // void delete_all_affected_wall_moves_from_valid_moves(move_index){

    uint8_t wall_row_col_dir[3];
    wall_move_index_to_row_col_dir(move_index, wall_row_col_dir);
    adjust_wall_invalidity_score(
        wall_row_col_dir[0],
        wall_row_col_dir[1],
        wall_row_col_dir[2],
        add_wall_else_delete);

    if (wall_row_col_dir[2])
    {
        // horizonal
        // inline neighbour
        if (wall_row_col_dir[1] < 8)
        {
            adjust_wall_invalidity_score(
                wall_row_col_dir[0],
                wall_row_col_dir[1] + 1,
                1,
                add_wall_else_delete);
        }
        // inline neighbour
        if (wall_row_col_dir[1] > 1)
        {
            adjust_wall_invalidity_score(
                wall_row_col_dir[0],
                wall_row_col_dir[1] - 1,
                1,
                add_wall_else_delete);
        }
        // cut through neighbour
        adjust_wall_invalidity_score(
            wall_row_col_dir[0],
            wall_row_col_dir[1],
            0,
            add_wall_else_delete);
    }
    else
    {
        // vertical

        // inline neighbour
        if (wall_row_col_dir[0] < 8)
        {
            adjust_wall_invalidity_score(
                wall_row_col_dir[0] + 1,
                wall_row_col_dir[1],
                0,
                add_wall_else_delete);
        }

        // inline neighbour
        if (wall_row_col_dir[0] > 1)
        {
            adjust_wall_invalidity_score(
                wall_row_col_dir[0] - 1,
                wall_row_col_dir[1],
                0,
                add_wall_else_delete);
        }

        // cut through neighbour
        adjust_wall_invalidity_score(
            wall_row_col_dir[0],
            wall_row_col_dir[1],
            1,
            add_wall_else_delete);
    }
};

void adjust_wall_invalidity_score(uint8_t row, uint8_t col, uint8_t dir, uint8_t increase_invalidity_else_decrease)
{
    // increase_invalidity_else_decrease > 0: if a wall is added, it decreases the invalidity score
    // increase_invalidity_else_decrease == 0: if a wall is deleted (undo), it decreases the invalidity score (it might not be zero, because other walls might prevent neighbour move_indeces becoming valid)

    uint8_t row_col_dir[3];
    row_col_dir[0] = row;
    row_col_dir[1] = col;
    row_col_dir[2] = dir;

    uint8_t invalid_move_index;
    invalid_move_index = wall_row_col_dir_to_move_index(row_col_dir);

    if (increase_invalidity_else_decrease)
    {
        move_index_invalidity_score[invalid_move_index]++;
    }
    else
    {
        if (move_index_invalidity_score[invalid_move_index] > 0)
        {
            move_index_invalidity_score[invalid_move_index]--;
        }
    }
    moves_delta[invalid_move_index] = FAKE_DELTA_FOR_INVALID_MOVE;
}

uint8_t wall_row_col_dir_to_move_index(uint8_t *row_col_dir)
{
    // for walls
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

void wall_move_index_to_row_col_dir(uint8_t move_index, uint8_t *row_col_dir)
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

    players[player].walls_placed++;
}

void pop_last_placed_wall(uint8_t player, uint8_t *row_col_dir)
{
    // delete last placed wall
    players[player].walls_placed--;

    row_col_dir[0] = players[player].walls[players[player].walls_placed].row;
    row_col_dir[1] = players[player].walls[players[player].walls_placed].col;
    row_col_dir[2] = players[player].walls[players[player].walls_placed].horizontal_else_vertical;

    players[player].walls[players[player].walls_placed].row = WALL_NOT_PLACED;
    players[player].walls[players[player].walls_placed].col = WALL_NOT_PLACED;
    players[player].walls[players[player].walls_placed].horizontal_else_vertical = WALL_NOT_PLACED;
}

void walls_get_all_positions(uint8_t *positions, uint8_t player)
{
    // mainly for an output situation to the graphics display
    for (uint8_t i = 0; i < PLAYER_MAX_WALL_COUNT; i++)
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
