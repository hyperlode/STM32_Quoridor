#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"


/* all moves have an index
    


*/
uint8_t autoplay_execute_next_move()
{
    uint8_t best_move_index;
    best_move_index = autoplay_get_best_next_move(get_playing_player());

    make_move(best_move_index);
}

uint8_t autoplay_get_best_next_move(uint8_t player)
{
    // from board state, get distances for all possible moves.

    // best_delta: for the 0 plyaer (=toNorth player) as "positive" as possible , for 1 player (to south), delta more negative is better
    int8_t best_delta;
    int8_t delta_to_absolute_value;

    if (player){
        delta_to_absolute_value = -1;
    }else{
        delta_to_absolute_value = 1;
    }
    
    best_delta = -127; // worst as init

    // draws
    // index of draws
    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count = 0;
    //uint8_t number_of_pawn_moves;

    // get best moves 
    for (uint8_t move_index = 0; move_index<MOVE_INDEX_COUNT; move_index++){
        int8_t delta;
        delta = get_delta_of_move_index(move_index) ;
        if (delta != FAKE_DELTA_FOR_INVALID_MOVE){
            if (delta * delta_to_absolute_value > best_delta){
                best_delta = delta * delta_to_absolute_value;
                equal_delta_moves_count = 0;
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count ++;
            }else if (delta * delta_to_absolute_value == best_delta){
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count ++;
            }
        }
    }

    // if winning move is in moves: do it! oh no! we don't know it that easily...


    // choose a move
    uint8_t rand_index;
    rand_index = rand() % equal_delta_moves_count;
    return equal_delta_moves[rand_index];
    // execute

    // repeat



    // return move with best path delta

    

    // return;
}
