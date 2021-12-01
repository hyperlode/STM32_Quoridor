#include <string.h>
#include <stdio.h>
#include "quoridor_gameplay.h"
#include "human_player.h"

uint8_t cursor_row_col[2];
void human_init(void){
    human_turn_init();
}

void human_turn_init(){
    pawn_get_position_as_row_col(cursor_row_col, 0);

}

void human_button_press(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle, uint8_t* cursor_position){

    if (north){
        cursor_row_col[0] += 1;
    }
    if (east){
        cursor_row_col[1] += 1;
    }
    if (west){
        cursor_row_col[1] -= 1;
    }
    if (south){
        cursor_row_col[0] -= 1;
    }
    cursor_position[0] = cursor_row_col[0];
    cursor_position[1] = cursor_row_col[1];

}


