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
    players[0].walls_placed = 0;

    players[1].pawn.row += 8;
    players[1].pawn.col += 4 ;
    players[1].walls_placed = 0;

    for (uint8_t move=0;move<MAX_MOVES_COUNT;move++){
        for (uint8_t i=0;i<2;i++){
            moves[move][i] = 0;
        }
    }

    for (uint8_t player=0;player<2;player++){
        for (uint8_t i=0;i<10;i++){
            players[player].walls[i].row = 0;
            players[player].walls[i].col = 0;
            players[player].walls[i].horizontal_else_vertical = 0;
        }
    }

    move_counter = 0;
}

void move_next(void){
    uint8_t player = move_counter %2;
    
    uint8_t* move = moves[move_counter];
    if (move[0] == 'N' || move[0] == 'E' || move[0] == 'S' || move[0] == 'W'  ){
        pawn_move_by_notation(player, move);

    }else{
        wall_set_by_notation(player, move);

    }
    move_counter++;
};

void moves_string_to_moves(char* moves_as_string){
    uint16_t counter = 0;

    uint8_t char_counter_in_move =0;
    for (uint8_t i=0; i<MAX_MOVES_COUNT; i++){
        char c = moves_as_string[i];

        if ( c == ','){
            
            char_counter_in_move = 0;
            counter ++;
        } else if (c == '\0'){
            return;
        }else{
            moves[counter][char_counter_in_move] = c;
            char_counter_in_move ++;
        }
    }
}

void wall_set_by_notation(uint8_t player, char* wall_position){
    // direction always two chars.

    uint8_t first_char = 1;
    uint8_t col;
    uint8_t row;
    uint8_t horizontal_else_vertical;

    for (uint8_t i=0;i<2;i++){
        char c = wall_position[i];
        if ( c >= 97 && c <=104){
            // a->h
            // N-S component
            if (first_char){
                horizontal_else_vertical = 0;
                col = c - 96;
            }else{
                col = c - 96;

            }
            first_char = 0;


        }else if (c >= 49 && c <= 56){
            // 1->9
            // E-W component

            if (first_char){
                horizontal_else_vertical = 1;
                row = c - 48;

            }else{
                row = c - 48;

            }
            first_char = 0;



        }else{

            // ERROR
            return;
        }
        
        players[player].walls[players[player].walls_placed].row = row;
        players[player].walls[players[player].walls_placed].col = col;
        players[player].walls[players[player].walls_placed].horizontal_else_vertical = horizontal_else_vertical;

    }
    players[player].walls_placed ++;
      
    
};

void pawn_move_by_notation(uint8_t player, char* direction){
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

void walls_get_positions(uint8_t* positions, uint8_t player){
    
    for (uint8_t i=0; i<10; i++){
        positions[i*3 + 0 ] = players[player].walls[i].row;
        positions[i*3 + 1 ] = players[player].walls[i].col;
        positions[i*3 + 2 ] = players[player].walls[i].horizontal_else_vertical;
    }
}

void pawn_get_position(uint8_t* position, uint8_t player){
    // player = 0 or 1
    
    position[0] = players[player].pawn.row;
    position[1] = players[player].pawn.col;
    return position;
}

