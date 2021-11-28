#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"

uint8_t moves_delta [];


/* all moves have an index
    


*/
char move_index_to_notation [140][2]= {"N ", "E ", "S ", "W ", "NN", "EE", "SS", "WW", "NE", "NW", "SE", "SW", "1a", "1b", "1c", "1d", "1e", "1f", "1g", "1h", "2a", "2b", "2c", "2d", "2e", "2f", "2g", "2h", "3a", "3b", "3c", "3d", "3e", "3f", "3g", "3h", "4a", "4b", "4c", "4d", "4e", "4f", "4g", "4h", "5a", "5b", "5c", "5d", "5e", "5f", "5g", "5h", "6a", "6b", "6c", "6d", "6e", "6f", "6g", "6h", "7a", "7b", "7c", "7d", "7e", "7f", "7g", "7h", "8a", "8b", "8c", "8d", "8e", "8f", "8g", "8h", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};


void get_move_index_to_notation(uint8_t index, char* notation){
    notation = move_index_to_notation[index];
}

void get_notation_to_index(char* notation, uint8_t index){
    
}



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