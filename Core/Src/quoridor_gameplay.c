#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor.h"
#include "quoridor_gameplay.h"

static uint8_t move_counter;
static Player players [2];

char moves[MAX_MOVES_COUNT][2];

void game_init(void){
    players[0].pawn.row += 0;
    players[0].pawn.col += 4;
    players[1].pawn.row += 8;
    players[1].pawn.col += 4 ;

    move_counter = 0;
}

void move_next(void){
    uint8_t player = move_counter %2;
    pawn_set_position_by_direction(player, moves[move_counter]);
    move_counter++;
};

void moves_string_to_moves(char* moves_as_string){
    uint16_t counter = 0;

    uint8_t char_counter_in_move =0;
    for (uint8_t i=0; i<MAX_MOVES_COUNT; i++){
        char c = moves_as_string[i];

        if ( c == ","){
            
            char_counter_in_move = 0;
            counter ++;
        }else{
            moves[counter][char_counter_in_move] = c;
            char_counter_in_move ++;
        }
    }
}



void pawn_set_position_by_direction(uint8_t player, char* direction){
    // direction always two chars.

    for (uint8_t i=0;i<2;i++){
        switch (direction[i]){
            case 'N':{
                pawn_set_position(player, 0, 1);
                break;
            }
            case 'S':{
                pawn_set_position(player, 0, -1);
                break;
            }
            case 'E':{
                pawn_set_position(player, 1 ,0 );
                break;
            }
            case 'W':{
                pawn_set_position(player, -1, 0);
                break;
            }
        }
    }
};

void pawn_set_position(uint8_t player, int8_t delta_x, int8_t delta_y){
    players[player].pawn.row += delta_y;
    players[player].pawn.col += delta_x;
}

void pawn_get_position(uint8_t* position, uint8_t player){
    // player = 0 or 1
    
    position[0] = players[player].pawn.row;
    position[1] = players[player].pawn.col;
    return position;
}

