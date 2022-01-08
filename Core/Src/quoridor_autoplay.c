#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"

uint8_t autoplay_move_type;

//uint8_t volatile autoplay_L2_valid_move_expected_delta[MOVE_INDEX_COUNT];
void (*callback_display_time_function_pointer)(uint8_t);

uint8_t autoplay_paused;

void autoplay_get_cursor(uint8_t *cursor_pawn_row_col)
{
    pawn_get_position_as_row_col(cursor_pawn_row_col, get_playing_player());
}

void autoplay_display_time_callback(void (*functionPointer)(uint8_t))
{
    callback_display_time_function_pointer = functionPointer;
}

uint8_t autoplay_execute_next_move(uint8_t level)
{
    uint8_t best_move_index;
    autoplay_paused_set(0);
    best_move_index = auto_play_get_move_from_opening_database();
    autoplay_move_type = AUTOPLAY_MOVE_TYPE_OPENING_DATABASE;

    if (
        best_move_index == MOVE_INDEX_DUMMY ||
        !get_move_index_valid(best_move_index)) // database move cannot be trusted. Check for validity.
    {
        best_move_index = autoplay_get_best_next_move(get_playing_player(), level);
        if (level == 1)
        {
            autoplay_move_type = AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_1;
        }
        else if (level == 2)
        {
            autoplay_move_type = AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_2;
        }
    }

    if (autoplay_paused_get())
    {
        return 0;
    }
    else
    {
        return make_move_if_valid(best_move_index);
    }
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

void autoplay_paused_set(uint8_t paused)
{
    autoplay_paused = paused;
}

uint8_t autoplay_paused_get()
{
    return autoplay_paused;
}

uint8_t autoplay_get_best_next_move_L2(uint8_t player)
{
    // copy valid moves for this move. Cannot be done by reference, it will change during make and undo moves.
    uint8_t L2_move_count;
    uint8_t L2_valid_moves[MOVE_INDEX_COUNT];
    uint8_t L2_valid_moves_deltas[MOVE_INDEX_COUNT];

    get_all_valid_move_indeces(L2_valid_moves, L2_valid_moves_deltas, &L2_move_count);

    // // check for winning move. 
    // for (uint8_t i=0;i<L2_move_count;i++){

    //     if (L2_valid_moves_deltas[i] == DELTA_MAX_WINNING_MOVE_MAGNITUDE){
    //         return L2_valid_moves[i];
    //     }
    // }

    // l2 means, we're looking at the opponent. We're looking for the least bad move the opponent can make.
    // this is: at the opponents move: a good delta for it is as high as possible. We need that delta to be as small as possible. so +127 is worst case scenario

    uint8_t volatile L1_responses_move_indeces[MOVE_INDEX_COUNT];
    int8_t volatile L1_responses_deltas[MOVE_INDEX_COUNT];
    autoplay_L2_get_opponent_counter_moves(L2_valid_moves, L1_responses_move_indeces, L1_responses_deltas, L2_move_count);

    // eliminate hesitation moves (detect a repeating move and react to it. )
    filter_moves_for_repeating_moves(L2_valid_moves, L1_responses_move_indeces, L1_responses_deltas, &L2_move_count);

    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count;
    uint8_t equal_delta_pawn_moves_count;
    auto_play_select_moves_with_equal_delta(L2_valid_moves, L1_responses_deltas, L2_move_count,
                                            equal_delta_moves, &equal_delta_moves_count, &equal_delta_pawn_moves_count);

    uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL;
    return pick_move_from_move_indeces(equal_delta_moves, equal_delta_moves_count, equal_delta_pawn_moves_count, wall_aggressiveness);
}

void filter_moves_for_repeating_moves(uint8_t *L2_move_indeces, uint8_t *counter_move_indeces, uint8_t *counter_move_deltas, uint8_t *moves_count)
{
    // get rid of all repeat move sequences.

    // check for back and forth repetitive movement. With a limited search depth, the computer is vulnerable to hesitation. And going back and forth. So, if no wall is placed by the adversary, do not repeat.
    // check lasts moves
    // check for a current move that would repeat a second to last move.  i.e. E, x, W, x, E(not good)  / W, x, W, x, W (good)  / E, x, W, +, E (good)// where x is opponent move  and + is opponent wall
    //i.e. E, x, W, x, E(not good)  / W, x, W, x, W (good)  / E, x, W, +, E (good)// where x is opponent move  and + is opponent wall

    uint8_t moves_history[RECORD_MOVES_HISTORY_LENGTH];
    get_history_moves_indeces(moves_history);
    uint8_t move_index_one_moves_ago = moves_history[get_move_counter() - 1];
    uint8_t move_index_two_moves_ago = moves_history[get_move_counter() - 2];
    uint8_t move_index_four_moves_ago = moves_history[get_move_counter() - 4];
    // uint8_t *moves_history;
    // get_history_moves_indeces(moves_history);
    // uint8_t move_index_one_moves_ago = moves_history[get_move_counter() - 1];
    // uint8_t move_index_two_moves_ago = moves_history[get_move_counter() - 2];
    // uint8_t move_index_four_moves_ago = moves_history[get_move_counter() - 4];

    uint8_t filtered_move_indeces[MOVE_INDEX_COUNT];
    uint8_t filtered_counter_move_indeces[MOVE_INDEX_COUNT];
    uint8_t filtered_counter_move_deltas[MOVE_INDEX_COUNT];
    uint8_t filtered_moves_count;

    if (get_move_counter() > 3)
    {

        for (uint8_t i = 0; i < *moves_count; i++)
        {

            uint8_t move_index = L2_move_indeces[i];
            // get move
            if (move_index < MOVE_INDEX_FIRST_WALL)
            {

                uint8_t include_move = 1;
                uint8_t reverse_move_index = move_indeces_to_reversed[move_index];

                // check if previous move was a wall
                if (move_index_one_moves_ago < MOVE_INDEX_FIRST_WALL){

                    // check if two moves ago, it was opposite move (aka: would cancel it now)
                    if (reverse_move_index ==  move_index_two_moves_ago){
                        
                        // check if four moves ago, it was the same move. --> if not, no problem
                        if (move_index == move_index_four_moves_ago){
                            include_move = 0;    
                        }
                    }else{
                        include_move = 1;
                    }

                    // // if same pro
                    // if (move_index == move_index_four_moves_ago){
                    //     include_move = 0;
                    //     if (move_index == move_index_two_moves_ago){
                    //          include_move = 1;
                    //     }
                    // }else{
                    //     include_move = 1;
                    // }
                }

                if (include_move)
                {
                    filtered_move_indeces[filtered_moves_count] = move_index;
                    filtered_counter_move_indeces[filtered_moves_count] = counter_move_indeces[move_index];
                    filtered_counter_move_deltas[filtered_moves_count] = counter_move_deltas[move_index];
                    filtered_moves_count++;
                }
            }
            else
            {
                filtered_move_indeces[filtered_moves_count] = move_index;
                filtered_counter_move_indeces[filtered_moves_count] = counter_move_indeces[move_index];
                filtered_counter_move_deltas[filtered_moves_count] = counter_move_deltas[move_index];
                filtered_moves_count++;
            }
        }
    }
    L2_move_indeces = filtered_move_indeces;
    counter_move_indeces = filtered_counter_move_indeces;
    counter_move_deltas = filtered_counter_move_deltas;
    *moves_count = filtered_moves_count;
}

void autoplay_L2_get_opponent_counter_moves(uint8_t *valid_move_indeces, uint8_t *counter_move_indeces, int8_t *counter_move_deltas, uint8_t valid_move_count)
{
    for (uint8_t L2_i = 0; L2_i < valid_move_count; L2_i++)
    {

        // check for pause
        if (button_interrupt_get_edge_up_single_readout(0))
        {
            // show ingame menu: - give up, continue,force L2
            // return, change gamestate.
            autoplay_paused_set(1);
            break;
        }

        // display time indication
        float tmp = 1.0f * L2_i / valid_move_count;
        callback_display_time_function_pointer((uint8_t)(tmp * 100));

        uint8_t L2_move_index = valid_move_indeces[L2_i];
        make_move(L2_move_index);

        // this is the opponent playing
        int8_t best_opponent_delta;
        uint8_t best_L1_move = autoplay_get_best_next_move_L1_with_delta(get_playing_player(), &best_opponent_delta);

        // store all deltas of valid L2 moves
        counter_move_indeces[L2_i] = best_L1_move;
        counter_move_deltas[L2_i] = best_opponent_delta;

        undo_last_move();
    }
}

uint8_t auto_play_select_moves_with_equal_delta(uint8_t *L2_moves_indeces, int8_t *counter_move_deltas, uint8_t valid_move_count,
                                                uint8_t *equal_delta_moves, uint8_t *equal_delta_moves_count, uint8_t *equal_delta_moves_pawn_count)
{

    int8_t result_delta = 127;
    for (uint8_t i = 0; i < valid_move_count; i++)
    {
        uint8_t L2_move_index = L2_moves_indeces[i];
        int8_t best_opponent_delta = counter_move_deltas[i];

        // delta needs to be as small as possible because, we're looking for the opponent move with the least overal damage.
        // basically, the BEST response of the opponent needs to be as bad as possible
        if (best_opponent_delta < result_delta)
        {
            result_delta = best_opponent_delta;
            *equal_delta_moves_count = 0;
            equal_delta_moves[*equal_delta_moves_count] = L2_move_index;
            (*equal_delta_moves_count)++;
            if (L2_move_index < MOVE_INDEX_FIRST_WALL)
            {
                *equal_delta_moves_pawn_count = 1;
            }
            else
            {
                *equal_delta_moves_pawn_count = 0;
            }
        }
        else if (best_opponent_delta == result_delta)
        {
            equal_delta_moves[*equal_delta_moves_count] = L2_move_index;
            (*equal_delta_moves_count)++;
            if (L2_move_index < MOVE_INDEX_FIRST_WALL)
            {
                (*equal_delta_moves_pawn_count)++;
            }
        }
    }
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