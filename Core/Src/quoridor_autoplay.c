#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"

uint8_t end_of_game;

uint8_t autoplay_move_type;
uint8_t autoplay_L2_valid_move_indeces [MOVE_INDEX_COUNT];

uint8_t autoplay_init()
{
    end_of_game = 0;
}

uint8_t autoplay_execute_next_move()
{
    if (end_of_game)
    {
        raise_error(111);
    }

    uint8_t best_move_index;
    best_move_index = auto_play_get_move_from_opening_database();
    autoplay_move_type = AUTOPLAY_MOVE_TYPE_OPENING_DATABASE;
    
    if (
        best_move_index == MOVE_INDEX_DUMMY ||
        !get_move_index_valid(best_move_index)) // database move cannot be trusted. Check for validity.
    {
        best_move_index = autoplay_get_best_next_move(get_playing_player());
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

uint8_t autoplay_get_best_next_move(uint8_t player){
    
    return autoplay_get_best_next_move_L1(player);

}

// uint8_t autoplay_get_best_next_move_L2(uint8_t player){

//     // copy valid moves
//     uint8_t L2_move_count = get_all_valid_move_indeces(autoplay_L2_valid_move_indeces);

//     for (uint8_t L2_i = 0; L2_i<L2_move_count; L2_i++){
        
//         int8_t total_move_delta = get_delta_of_move_index_normalized(L2_i);

//         make_move(autoplay_L2_valid_move_indeces[L2_i]);
//         get_get
        
//         // this is the opponent playing
//         uint8_t best_L1_move = autoplay_get_best_next_move_L1(get_playing_player());


//     }
    
//     // for all valid moves:
//         // make move
//         // for all valid moves
//             // make move
//             // get delta and store if improved
//             // undo move
//         // undo move

//     // get all best deltas and moves. 

//     // analyse L1 deltas 
//     // make move and display.




// }


void autoplay_get_next_move_equal_deltas(uint8_t player, int8_t* equal_delta_moves, uint8_t* r_equal_delta_moves_count, uint8_t* r_pawn_moves_count, int8_t* r_best_delta)
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

        if (delta == (DELTA_WINNING_MOVE_MAGNITUDE))
        {
            end_of_game = 1;
        }

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

    *r_equal_delta_moves_count= equal_delta_moves_count;
    *r_pawn_moves_count = pawn_moves_count;
    *r_best_delta = best_delta;
}

uint8_t autoplay_get_best_next_move_L1(uint8_t player)
{
    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count;
    uint8_t  pawn_moves_count;
    int8_t delta;
    
    autoplay_get_next_move_equal_deltas(player, equal_delta_moves, &equal_delta_moves_count, &pawn_moves_count, &delta );

    // choose a move
    uint8_t rand_index;

    uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL;

    uint8_t prioritizes_wall_over_pawn;

    prioritizes_wall_over_pawn = rand() % 100 < wall_aggressiveness;

    if (pawn_moves_count > 0 && !prioritizes_wall_over_pawn)
    {
        // prioritize pawn moves over wall placement if equal delta.
        rand_index = rand() % pawn_moves_count;
    }
    else
    {
        rand_index = rand() % equal_delta_moves_count;
    }

    return equal_delta_moves[rand_index];
}
