#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"

uint8_t moves_delta [];


/* all moves have an index
    


*/






void autoplay_next_move(char* move){
    // from board state, get distances for all possible moves.
    // return move with best path delta




    switch (get_move_counter()){
    case 0: {
        move [0] = 'N';
        move [1] = ' ';
        break;
    }
    case 1: {
        move [0] = 'S';
        move [1] = ' ';
        break;
    }
    default:{
        move [0] = 'E';
        move [1] = ' ';
        break;
    }

    }

}