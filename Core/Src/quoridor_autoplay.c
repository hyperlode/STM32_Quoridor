#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"

// uint8_t end_of_game;

uint8_t autoplay_move_type;
uint8_t autoplay_L2_valid_move_indeces[MOVE_INDEX_COUNT];
uint8_t volatile autoplay_L2_valid_move_indeces_with_best_L1_move[MOVE_INDEX_COUNT];
uint8_t volatile autoplay_L2_valid_move_expected_delta[MOVE_INDEX_COUNT];
void (*callback_display_time_function_pointer)(uint8_t);

void autoplay_get_cursor(uint8_t *cursor_pawn_row_col)
{
    pawn_get_position_as_row_col(cursor_pawn_row_col, get_playing_player());
}

uint8_t autoplay_init()
{
    // end_of_game = 0;
}

void autoplay_display_time_callback(void (*functionPointer)(uint8_t))
{
    callback_display_time_function_pointer = functionPointer;
}

uint8_t autoplay_execute_next_move(uint8_t level)
{
    // if (end_of_game)
    // {
    //     raise_error(ERROR_MOVING_WHILE_END_OF_GAME);
    // }

    uint8_t best_move_index;
    best_move_index = auto_play_get_move_from_opening_database();
    autoplay_move_type = AUTOPLAY_MOVE_TYPE_OPENING_DATABASE;

    if (
        best_move_index == MOVE_INDEX_DUMMY ||
        !get_move_index_valid(best_move_index)) // database move cannot be trusted. Check for validity.
    {
        best_move_index = autoplay_get_best_next_move(get_playing_player(), level);
        autoplay_move_type = AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_1;
    }

    make_move(best_move_index);
}

uint8_t autoplay_get_move_type()
{
    return autoplay_move_type;
}

uint8_t auto_play_get_move_from_opening_database()
{

    opening_next_move_suggestion();
}

uint8_t autoplay_get_best_next_move(uint8_t player, uint8_t depth)
{

    switch (depth)
    {

    case 1:
    {

        return autoplay_get_best_next_move_L1(player);
        break;
    }
    case 2:
    {

        return autoplay_get_best_next_move_L2(player);
        break;
    }
    default:
    {
        return autoplay_get_best_next_move_L1(player);
        break;
    }
    }
}

uint8_t autoplay_get_best_next_move_L2(uint8_t player)
{
    // copy valid moves for this move. Cannot be done by reference, it will change during make and undo moves.
    uint8_t L2_move_count;
    get_all_valid_move_indeces(autoplay_L2_valid_move_indeces, &L2_move_count);

    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count = 0;
    // l2 means, we're looking at the opponent. We're looking for the least bad move the opponent can make.
    // this is: at the opponents move: a good delta for it is as high as possible. We need that delta to be as small as possible. so +127 is worst case scenario
    int8_t result_delta = 127;
    uint8_t pawn_moves_count = 0;

    for (uint8_t L2_i = 0; L2_i < L2_move_count; L2_i++)
    {

        float tmp = 1.0f * L2_i / L2_move_count;
        callback_display_time_function_pointer((uint8_t)(tmp * 100));

        uint8_t L2_move_index = autoplay_L2_valid_move_indeces[L2_i];
        make_move(L2_move_index);

        // this is the opponent playing
        int8_t best_opponent_delta;
        uint8_t best_L1_move = autoplay_get_best_next_move_L1_with_delta(get_playing_player(), &best_opponent_delta);
        
        autoplay_L2_valid_move_indeces_with_best_L1_move[L2_i] = best_L1_move;

        // delta needs to be as small as possible because, we're looking for the opponent move with the least overal damage.
        // basically, the BEST response of the opponent needs to be as bad as possible
        if (best_opponent_delta < result_delta)
        {
            result_delta = best_opponent_delta;
            equal_delta_moves_count = 0;
            equal_delta_moves[equal_delta_moves_count] = L2_move_index;
            equal_delta_moves_count++;
            if (L2_move_index < MOVE_INDEX_FIRST_WALL)
            {
                pawn_moves_count = 1;
            }
            else
            {
                pawn_moves_count = 0;
            }
        }
        else if (best_opponent_delta == result_delta)
        {
            equal_delta_moves[equal_delta_moves_count] = L2_move_index;
            equal_delta_moves_count++;
            if (L2_move_index < MOVE_INDEX_FIRST_WALL)
            {
                pawn_moves_count++;
            }
        }

        undo_last_move();
    }

    // for all valid moves:
    // make move
    // for all valid moves
    // make move
    // get delta and store if improved
    // undo move
    // undo move

    // get all best deltas and moves.

    // analyse L1 deltas
    // make move and display.
    // uint8_t rand_index;
    // rand_index = rand() % equal_delta_moves_count;

    uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL;
    return pick_move_from_move_indeces(equal_delta_moves, equal_delta_moves_count, pawn_moves_count, wall_aggressiveness);
}

void autoplay_get_next_move_equal_deltas_L1(uint8_t player, int8_t *equal_delta_moves, uint8_t *r_equal_delta_moves_count, uint8_t *r_pawn_moves_count, int8_t *r_best_delta)
{
    // from board state, get distances for all possible moves.

    // deltas are normalized for the current player. Meaning: higher delta is better. positive delta = winning.

    //int8_t best_delta;

    int8_t best_delta = -127; // worst as init

    // draws
    // index of draws
    // uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count = 0;
    uint8_t pawn_moves_count = 0;
    uint8_t moves_to_investigate_count;

    // get best moves
    if (get_walls_placed(player) >= 10)
    {
        moves_to_investigate_count = MOVE_INDEX_FIRST_WALL;
    }
    else
    {
        moves_to_investigate_count = MOVE_INDEX_COUNT;
    }

    for (uint8_t move_index = 0; move_index < moves_to_investigate_count; move_index++)
    {
        int8_t delta;
        delta = get_delta_of_move_index_normalized(move_index);

        if (delta != FAKE_DELTA_FOR_INVALID_MOVE)
        {

            if (delta > best_delta)
            {
                best_delta = delta;
                equal_delta_moves_count = 0;
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count++;

                if (move_index < MOVE_INDEX_FIRST_WALL)
                {
                    pawn_moves_count = 1;
                }
                else
                {
                    pawn_moves_count = 0;
                }
            }
            else if (delta == best_delta)
            {
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count++;
                if (move_index < MOVE_INDEX_FIRST_WALL)
                {
                    pawn_moves_count++;
                }
            }
        }
    }

    *r_equal_delta_moves_count = equal_delta_moves_count;
    *r_pawn_moves_count = pawn_moves_count;
    *r_best_delta = best_delta;
}

uint8_t autoplay_get_best_next_move_L1(uint8_t player)
{
    int8_t best_delta;
    return autoplay_get_best_next_move_L1_with_delta(player, best_delta);
}

uint8_t autoplay_get_best_next_move_L1_with_delta(uint8_t player, int8_t *r_best_delta)
{
    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count;
    uint8_t pawn_moves_count;
    int8_t delta;

    autoplay_get_next_move_equal_deltas_L1(player, equal_delta_moves, &equal_delta_moves_count, &pawn_moves_count, &delta);

    // choose a move
    // uint8_t rand_index;
    uint8_t picked_move_index;
    uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL;

    picked_move_index = pick_move_from_move_indeces(equal_delta_moves, equal_delta_moves_count, pawn_moves_count, wall_aggressiveness);

    *r_best_delta = delta;
    return picked_move_index;
}

uint8_t pick_move_from_move_indeces(uint8_t *move_indeces, uint8_t move_indeces_count, uint8_t pawn_moves_count, uint8_t wall_aggressiveness)
{
    uint8_t rand_index;

    // uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL;

    uint8_t prioritizes_wall_over_pawn;

    prioritizes_wall_over_pawn = rand() % 100 < wall_aggressiveness;

    if (pawn_moves_count > 0 && !prioritizes_wall_over_pawn)
    {
        // prioritize pawn moves over wall placement if equal delta.
        rand_index = rand() % pawn_moves_count;
    }
    else
    {
        rand_index = rand() % move_indeces_count;
    }

    return move_indeces[rand_index];
}