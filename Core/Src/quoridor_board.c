#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_board.h"
  
void oled_init(){
    ssd1306_Init();
}

#define BOARD_CELL_SPACING 7
#define BOARD_OFFSET_X 7
#define BOARD_OFFSET_Y 0


#define WALL_CELL_OFFSET_Y 2
#define WALL_CELL_OFFSET_X 2

#define WALL_CELL_OFFSET_Y 0
#define WALL_CELL_OFFSET_X 0
#define WALL_THICKNESS 3


#define PAWN_CELL_OFFSET_X 2
#define PAWN_CELL_OFFSET_Y 2
#define PAWN_WIDTH 3
#define PAWN_HEIGHT 3 


static Player players [2];
static uint8_t move_counter_b;

static cursor_pawn_col = CURSOR_NOT_SHOWN;
static cursor_pawn_row = CURSOR_NOT_SHOWN;

static cursor_wall_row_col_dir[3];


void board_set_cursor_wall(uint8_t* row_col_dir){
    
    cursor_wall_row_col_dir[0] = row_col_dir[0];
    cursor_wall_row_col_dir[1] = row_col_dir[1];
    cursor_wall_row_col_dir[2] = row_col_dir[2];
}

void board_set_cursor(uint8_t* row_col){
    cursor_pawn_row = row_col[0];  
    cursor_pawn_col = row_col[1];
}

void board_draw_cursor_wall(){
     if (cursor_wall_row_col_dir[0] == CURSOR_NOT_SHOWN || cursor_wall_row_col_dir[1] == CURSOR_NOT_SHOWN){
        return;
    }

    board_draw_wall( cursor_wall_row_col_dir[0], cursor_wall_row_col_dir[1], cursor_wall_row_col_dir[2], 1 );
}

void oard_draw_cursor_pawn(){

    if (cursor_pawn_row == CURSOR_NOT_SHOWN || cursor_pawn_col == CURSOR_NOT_SHOWN){
        return;
    }

    uint8_t row = 8 - cursor_pawn_row;
    uint8_t col = cursor_pawn_col;



    ssd1306_DrawRectangle(
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING - 1, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING - 1, 
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING + PAWN_WIDTH + 1, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING + PAWN_HEIGHT + 1,
        Black
        );
}

void board_state_update(uint8_t* player_1_pos, uint8_t* player_2_pos, uint8_t* player_1_walls, uint8_t* player_2_walls, uint8_t* distances_to_win, uint8_t move_counter_a){
   
    uint8_t* walls;
    uint8_t* input_walls;

    move_counter_b = move_counter_a;
    for (uint8_t player=0;player<2;player++){
        if (player == 0){
            players[0].pawn.row = player_1_pos[0];
            players[0].pawn.col = player_1_pos[1];
            players[0].pawn.fill = 0;
            input_walls = player_1_walls;
            players[0].distance_to_win = distances_to_win[0];
                      
        }else{
            players[1].pawn.row = player_2_pos[0];
            players[1].pawn.col = player_2_pos[1];
            players[1].pawn.fill = 1;
            input_walls = player_2_walls;
            players[1].distance_to_win = distances_to_win[1];
        }

        struct Wall* tmp;
        for(uint8_t i=0;i<10;i++){
            tmp = &(players[player].walls[i]);
            tmp->row = input_walls[i*3 + 0];
            tmp->col = input_walls[i*3 + 1];
            tmp->horizontal_else_vertical = input_walls[i*3 + 2];
        }
    }
}

void board_state_draw(){
    ssd1306_Fill(White);    
    board_draw_dots();

    for (uint8_t player=0;player<2;player++){
        board_draw_pawn(players[player].pawn.row,players[player].pawn.col,players[player].pawn.fill);

        for (uint8_t wall=0;wall<10;wall++){
            board_draw_wall(
                players[player].walls[wall].row,
                players[player].walls[wall].col,
                players[player].walls[wall].horizontal_else_vertical,
                0);
        }
    }
    ssd1306_SetCursor(70, 1);

    ssd1306_WriteString("Dist 1:", Font_7x10, Black);

    char* distance_player_1 [2];
    snprintf(distance_player_1, sizeof(distance_player_1), "%d", players[0].distance_to_win);
    
    ssd1306_SetCursor(70, 12);
    ssd1306_WriteString(distance_player_1, Font_7x10, Black);

    char* distance_player_2 [2];
    snprintf(distance_player_2, sizeof(distance_player_2), "%d", players[1].distance_to_win);

    ssd1306_SetCursor(70, 26);
    ssd1306_WriteString("Dist 2:", Font_7x10, Black);
    ssd1306_SetCursor(70, 37);
    ssd1306_WriteString(distance_player_2, Font_7x10, Black);

    char* move_counter_text [2];
    snprintf(move_counter_text, sizeof(move_counter_text), "Mo: %d", move_counter_b);

    ssd1306_SetCursor(70, 48);
    ssd1306_WriteString(move_counter_text, Font_7x10, Black);

    oard_draw_cursor_pawn();
    board_draw_cursor_wall();
     
    board_draw_outline();  // save for last to overwrite wall placement white dots.



    ssd1306_UpdateScreen();
}

void board_draw_test(){
    ssd1306_Fill(White);    
    board_draw_dots();
    board_draw_pawn(0,4,0);
    board_draw_pawn(8,4,1);
    board_draw_wall(3,3,1,1);
    board_draw_wall(5,3,1,1);
    board_draw_wall(1,4,0,1);
    board_draw_wall(1,1,0,1);
    board_draw_wall(5,3,0,1);

    board_draw_outline();  // save for last to overwrite wall placement white dots.
    ssd1306_UpdateScreen();
}

void board_draw_outline(){
    for (uint8_t border_row=0;border_row<10;border_row+=9){
        // SSD1306_HEIGHT
        ssd1306_Line(BOARD_OFFSET_X, BOARD_OFFSET_Y + border_row*BOARD_CELL_SPACING, BOARD_OFFSET_X + BOARD_CELL_SPACING*9, BOARD_OFFSET_Y + border_row*BOARD_CELL_SPACING ,Black);   
    }
    for (uint8_t border_col=0;border_col<10;border_col+=9){
        ssd1306_Line(BOARD_OFFSET_X + border_col*BOARD_CELL_SPACING, BOARD_OFFSET_Y, BOARD_OFFSET_X + border_col*BOARD_CELL_SPACING, BOARD_OFFSET_Y + BOARD_CELL_SPACING*9 ,Black);   
    }
}

void board_draw_dots(){
    for (uint8_t rows=0;rows<10;rows++){
        for (uint8_t cols=0;cols<10;cols++){
            ssd1306_DrawPixel(BOARD_OFFSET_X + cols*BOARD_CELL_SPACING, BOARD_OFFSET_Y + rows*BOARD_CELL_SPACING, Black);
        }
    }
}

void board_draw_pawn(uint8_t row, uint8_t col, uint8_t fill){
    // row, col : 0 to 8
    // fill vs nofill for player distinction
    row = 8 - row;
    ssd1306_DrawRectangle(
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING, 
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING + PAWN_WIDTH, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING + PAWN_HEIGHT,
        Black
        );

    if (fill){
        ssd1306_DrawRectangle(
            PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING + 1, 
            PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING + 1, 
            PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + col*BOARD_CELL_SPACING + PAWN_WIDTH - 1, 
            PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + row*BOARD_CELL_SPACING + PAWN_HEIGHT - 1,
            Black
            );
    }
}

void board_draw_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical, uint8_t fat_wall ){
    // row, col: from 1 to 8
    // horizontal: 1 (E-W), vertical: 0 (N-S)
    // wall_thickness: 1,3,5

    if (row ==0 || col == 0){
        return;
    }
    uint8_t wall_thickness = 0;
    row = 9 - row;

    
    
   
    // return ;
    // standard one line thick wall
    if (horizontal_else_vertical){
        ssd1306_DrawRectangle(

            WALL_CELL_OFFSET_X + BOARD_OFFSET_X + 1 + (col-1) * BOARD_CELL_SPACING, 
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y  + row * BOARD_CELL_SPACING, 
            WALL_CELL_OFFSET_X + BOARD_OFFSET_X - 1 + (col+1) * BOARD_CELL_SPACING, 
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y  + row * BOARD_CELL_SPACING + wall_thickness,
            Black);

            ssd1306_DrawPixel(
                    WALL_CELL_OFFSET_X + BOARD_OFFSET_X  + (col -1) * BOARD_CELL_SPACING,
                    WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y  + row * BOARD_CELL_SPACING,
                    White);

            ssd1306_DrawPixel(
                WALL_CELL_OFFSET_X + BOARD_OFFSET_X  + (col+1) * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y  + row * BOARD_CELL_SPACING,
                White);


    }else{
        ssd1306_DrawRectangle(
            WALL_CELL_OFFSET_X + BOARD_OFFSET_X  + col * BOARD_CELL_SPACING, 
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y + 1 + (row - 1) * BOARD_CELL_SPACING, 
            WALL_CELL_OFFSET_X + BOARD_OFFSET_X  + col * BOARD_CELL_SPACING + wall_thickness, 
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y - 1 + ((row - 1) +2) * BOARD_CELL_SPACING,
            Black);


        ssd1306_DrawPixel(
            WALL_CELL_OFFSET_X + BOARD_OFFSET_X  + col * BOARD_CELL_SPACING,
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y + (row + 1) * BOARD_CELL_SPACING,
            White);

        ssd1306_DrawPixel(
            WALL_CELL_OFFSET_X + BOARD_OFFSET_X + col * BOARD_CELL_SPACING , 
            WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y + ((row ) - 1) * BOARD_CELL_SPACING,
            White);
    }
    
    // draw rectangle around the wall for fat walls
    if (fat_wall){
        wall_thickness = 2;
        if (horizontal_else_vertical){
            ssd1306_DrawRectangle(
                WALL_CELL_OFFSET_X + BOARD_OFFSET_X + 1 + (col - 1) * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y - 1 + row * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_X + BOARD_OFFSET_X - 1 + (col + 1) * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y - 1 + row * BOARD_CELL_SPACING + wall_thickness,
                Black);
        }else{
            ssd1306_DrawRectangle(
                WALL_CELL_OFFSET_X + BOARD_OFFSET_X - 1 + col * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y + 1 + (row - 1) * BOARD_CELL_SPACING, 
                WALL_CELL_OFFSET_X + BOARD_OFFSET_X - 1 + col * BOARD_CELL_SPACING + wall_thickness, 
                WALL_CELL_OFFSET_Y + BOARD_OFFSET_Y - 1 + ((row - 1) +2) * BOARD_CELL_SPACING,
                Black);
        }
    }
}

void board_draw_lines(){
    ssd1306_Fill(White);
    for (uint8_t rows=0;rows<10;rows++){
        // SSD1306_HEIGHT
        ssd1306_Line(BOARD_OFFSET_X, BOARD_OFFSET_Y + rows*BOARD_CELL_SPACING, BOARD_OFFSET_X + BOARD_CELL_SPACING*9, BOARD_OFFSET_Y + rows*BOARD_CELL_SPACING ,Black);   
    }
    for (uint8_t cols=0;cols<10;cols++){
        ssd1306_Line(BOARD_OFFSET_X + cols*BOARD_CELL_SPACING, BOARD_OFFSET_Y, BOARD_OFFSET_X + cols*BOARD_CELL_SPACING, BOARD_OFFSET_Y + BOARD_CELL_SPACING*9 ,Black);   

    }
    ssd1306_UpdateScreen();
}