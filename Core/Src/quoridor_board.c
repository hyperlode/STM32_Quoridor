#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_board.h"
  

#define BOARD_CELL_SPACING 7
#define BOARD_OFFSET_X 0
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
static uint16_t game_counter_b;

static cursor_pawn_col = CURSOR_NOT_SHOWN;
static cursor_pawn_row = CURSOR_NOT_SHOWN;

static cursor_wall_row_col_dir[3];
static uint8_t move_type;
static uint8_t error_type;
int8_t* move_history_deltas;

void oled_init(){
    ssd1306_Init();
    error_type = ERROR_NO_ERROR;
}

// void menu_display_ingame(uint8_t active_item){

//         // max 
    
//     ssd1306_Fill(White);    
//     ssd1306_SetCursor(5, 4);
//     ssd1306_WriteString("Back with Toggle ", Font_7x10, Black);


//     ssd1306_SetCursor(15, 20);
//     ssd1306_WriteString("Back to game", Font_7x10, Black);
  
//     ssd1306_SetCursor(15, 34);
//     ssd1306_WriteString("Undo last move", Font_7x10, Black);
  
//     ssd1306_SetCursor(15, 48);
//     ssd1306_WriteString("Give up", Font_7x10, Black);
    
//     board_draw_pawn_row_col(active_item * 2 + 1, 0 , 1);

//     ssd1306_UpdateScreen();

// }

void menu_display_3_items(uint8_t active_item, char* title, char* item_0, char* item_1, char* item_2){
    // max 
    ssd1306_Fill(White);    
    ssd1306_SetCursor(5, 4);
    // ssd1306_WriteString("Pick your fight", Font_7x10, Black);
    ssd1306_WriteString(title, Font_7x10, Black);

    
    ssd1306_SetCursor(15, 20);
    // ssd1306_WriteString("Human vs Machine", Font_7x10, Black);
    ssd1306_WriteString(item_0, Font_7x10, Black);
  
    ssd1306_SetCursor(15, 34);
    ssd1306_WriteString(item_1, Font_7x10, Black);
    // ssd1306_WriteString("Human battle", Font_7x10, Black);
  
    ssd1306_SetCursor(15, 48);
    // ssd1306_WriteString("Machine battle", Font_7x10, Black);
    ssd1306_WriteString(item_2, Font_7x10, Black);
    
    board_draw_pawn_row_col(active_item * 2 + 1, 0 , 1);

    ssd1306_UpdateScreen();

    
}

void board_set_cursor_wall(uint8_t* row_col_dir){
    
    cursor_wall_row_col_dir[0] = row_col_dir[0];
    cursor_wall_row_col_dir[1] = row_col_dir[1];
    cursor_wall_row_col_dir[2] = row_col_dir[2];
}

void board_set_move_type(uint8_t given_move_type){
    
    move_type = given_move_type;
}

void board_set_error(uint8_t error_typerrr){
    
    error_type = error_typerrr;
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

void board_draw_cursor_pawn(){

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

void board_state_update(uint8_t* player_1_pos, uint8_t* player_2_pos, uint8_t player_1_wall_placed, uint8_t player_2_wall_placed, uint8_t* player_1_walls, uint8_t* player_2_walls, uint8_t* distances_to_win, uint8_t move_counter_a, int8_t* move_deltas, uint16_t game_counter){
   
    uint8_t* walls;
    uint8_t* input_walls;
    move_history_deltas = move_deltas;

    move_counter_b = move_counter_a;
    game_counter_b = game_counter;
    for (uint8_t player=0;player<2;player++){
        if (player == 0){
            players[0].pawn.row = player_1_pos[0];
            players[0].pawn.col = player_1_pos[1];
            players[0].pawn.fill = 0;
            input_walls = player_1_walls;
            players[0].distance_to_win = distances_to_win[0];
            players[0].walls_placed = player_1_wall_placed;
                      
        }else{
            players[1].pawn.row = player_2_pos[0];
            players[1].pawn.col = player_2_pos[1];
            players[1].pawn.fill = 1;
            input_walls = player_2_walls;
            players[1].distance_to_win = distances_to_win[1];
            players[1].walls_placed = player_2_wall_placed;
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

void board_info_draw(){

    // delta history graph can be overwritten by other items (because of it's unboundedness.)
    for (uint8_t i=0;i<RECORD_MOVES_HISTORY_LENGTH;i++){
        
        int8_t deltaOffset = move_history_deltas[i];
        // if (deltaOffset > 0){
            ssd1306_Line(
                BOARD_MENU_X_OFFSET + i, 
                BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 2*BOARD_MENU_ROW_HEIGHT + 5, 
                BOARD_MENU_X_OFFSET + i, 
                BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 2*BOARD_MENU_ROW_HEIGHT + 5 + deltaOffset ,
                Black);   

        // }
    }

    // vertical thick board border
    for (uint8_t i=0;i<2;i++){
        ssd1306_Line(
            BOARD_OFFSET_X + i + 9 * BOARD_CELL_SPACING, 
            BOARD_OFFSET_Y, 
            BOARD_OFFSET_X + i + 9 * BOARD_CELL_SPACING, 
            BOARD_OFFSET_Y + BOARD_CELL_SPACING*9 ,
            Black);   
    }

    board_draw_pawn (BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + 2, BOARD_MENU_Y_OFFSET, 0);
    board_draw_pawn (BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + 19, BOARD_MENU_Y_OFFSET, 1);

    // items
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT);
    ssd1306_WriteString("Wall", Font_7x10, Black);

    ssd1306_SetCursor(BOARD_MENU_X_OFFSET, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT + BOARD_MENU_ROW_HEIGHT);
    ssd1306_WriteString("Path", Font_7x10, Black);

    // values

    // Walls
    char* values_as_char[2];
    uint8_t x_offset_align_center=0;
    x_offset_align_center = ( (10 - players[0].walls_placed) < 10) * BOARD_MENU_COL_ALIGN_CENTER;
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + x_offset_align_center, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT);
    snprintf(values_as_char, sizeof(values_as_char), "%d", 10 - players[0].walls_placed);
    ssd1306_WriteString(values_as_char, Font_7x10, Black);
    
    x_offset_align_center = ((10 - players[1].walls_placed) < 10) * BOARD_MENU_COL_ALIGN_CENTER;
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + BOARD_MENU_COL_WIDTH + x_offset_align_center, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT);
    snprintf(values_as_char, sizeof(values_as_char), "%d", 10 - players[1].walls_placed);
    ssd1306_WriteString(values_as_char, Font_7x10, Black);

    // Distances to win
    x_offset_align_center = (players[0].distance_to_win < 10) * BOARD_MENU_COL_ALIGN_CENTER;
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + x_offset_align_center, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ BOARD_MENU_ROW_HEIGHT);
    snprintf(values_as_char, sizeof(values_as_char), "%d", players[0].distance_to_win);
    ssd1306_WriteString(values_as_char, Font_7x10, Black);

    x_offset_align_center = (players[1].distance_to_win < 10) * BOARD_MENU_COL_ALIGN_CENTER;
    snprintf(values_as_char, sizeof(values_as_char), "%d", players[1].distance_to_win);
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + BOARD_MENU_COL_WIDTH + x_offset_align_center, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT + BOARD_MENU_ROW_HEIGHT);
    ssd1306_WriteString(values_as_char, Font_7x10, Black);
    
    // move counter 
    snprintf(values_as_char, sizeof(values_as_char), "%d%d", game_counter_b, move_counter_b);
    ssd1306_SetCursor(BOARD_MENU_X_OFFSET, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+  3 * BOARD_MENU_ROW_HEIGHT);
    ssd1306_WriteString(values_as_char, Font_7x10, Black);
}

void board_draw_error_type(){
    char* values_as_char[2];
   
   if (error_type != ERROR_NO_ERROR){

        ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + BOARD_MENU_COL_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 3 * BOARD_MENU_ROW_HEIGHT);
        // ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH + BOARD_MENU_COL_ITEMS_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 2 * BOARD_MENU_ROW_HEIGHT);
        // ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 3 * BOARD_MENU_ROW_HEIGHT);
        snprintf(values_as_char, sizeof(values_as_char), "%d", error_type);
        ssd1306_WriteString(values_as_char, Font_7x10, Black);
    }

}

void board_draw_move_type(){
    switch(move_type){
        // ssd1306_SetCursor(90, 30); //100,10
        //ssd1306_SetCursor(100, 50); //100,10
        case AUTOPLAY_MOVE_TYPE_OPENING_DATABASE:
        {
            // ssd1306_SetCursor(103, 50); //100,10
            ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 3 * BOARD_MENU_ROW_HEIGHT);
            ssd1306_WriteString("DB", Font_7x10, Black);
            break;
        }
        case AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_1:
        {
            ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 3*BOARD_MENU_ROW_HEIGHT);
            ssd1306_WriteString("L1", Font_7x10, Black);
            break;
        }
        default:
        {
            ssd1306_SetCursor(BOARD_MENU_X_OFFSET + BOARD_MENU_COL_ITEMS_WIDTH, BOARD_MENU_Y_OFFSET + BOARD_MENU_ROW_TITLE_HEIGHT+ 3*BOARD_MENU_ROW_HEIGHT);
            ssd1306_WriteString("C", Font_7x10, Black);
            break;
        }
    }
}

void board_state_draw(){
    ssd1306_Fill(White);    
    board_draw_dots();

    for (uint8_t player=0;player<2;player++){
        board_draw_pawn_row_col(players[player].pawn.row,players[player].pawn.col,players[player].pawn.fill);

        for (uint8_t wall=0;wall<10;wall++){
            board_draw_wall(
                players[player].walls[wall].row,
                players[player].walls[wall].col,
                players[player].walls[wall].horizontal_else_vertical,
                0);
        }
    }
    board_info_draw();
    board_draw_cursor_pawn();
    board_draw_cursor_wall();
    board_draw_move_type();
    board_draw_error_type();
     
    board_draw_outline();  // save for last to overwrite wall placement white dots.

    ssd1306_UpdateScreen();
}

void board_draw_test(){
    ssd1306_Fill(White);    
    board_draw_dots();
    board_draw_pawn_row_col(0,4,0);
    board_draw_pawn_row_col(8,4,1);
    board_draw_wall(3,3,1,1);
    board_draw_wall(5,3,1,1);
    board_draw_wall(1,4,0,1);
    board_draw_wall(1,1,0,1);
    board_draw_wall(5,3,0,1);

    board_draw_outline();  // save for last to overwrite wall placement white dots.
    ssd1306_UpdateScreen();
}

void board_draw_outline(){

    // horizontal lines
    for (uint8_t border_row=0;border_row<10;border_row+=9){
        // SSD1306_HEIGHT
        ssd1306_Line(
            BOARD_OFFSET_X, 
            BOARD_OFFSET_Y + border_row*BOARD_CELL_SPACING, 
            BOARD_OFFSET_X + BOARD_CELL_SPACING*9, 
            BOARD_OFFSET_Y + border_row*BOARD_CELL_SPACING ,
            Black);   
    }

    // vertical lines
    for (uint8_t border_col=0;border_col<10;border_col+=9){
        ssd1306_Line(
            BOARD_OFFSET_X + border_col*BOARD_CELL_SPACING, 
            BOARD_OFFSET_Y, 
            BOARD_OFFSET_X + border_col*BOARD_CELL_SPACING, 
            BOARD_OFFSET_Y + BOARD_CELL_SPACING*9 ,
            Black);   
    }
}

void board_draw_dots(){
    for (uint8_t rows=0;rows<10;rows++){
        for (uint8_t cols=0;cols<10;cols++){
            ssd1306_DrawPixel(BOARD_OFFSET_X + cols*BOARD_CELL_SPACING, BOARD_OFFSET_Y + rows*BOARD_CELL_SPACING, Black);
        }
    }
}

void board_draw_pawn(uint8_t x, uint8_t y, uint8_t fill){
    ssd1306_DrawRectangle(
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + x, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + y, 
        PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + x + PAWN_WIDTH, 
        PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + y + PAWN_HEIGHT,
        Black
        );

    if (fill){
        ssd1306_DrawRectangle(
            PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + x + 1, 
            PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + y + 1, 
            PAWN_CELL_OFFSET_X + BOARD_OFFSET_X + x + PAWN_WIDTH - 1, 
            PAWN_CELL_OFFSET_Y + BOARD_OFFSET_Y + y + PAWN_HEIGHT - 1,
            Black
            );
    }
}
void board_draw_pawn_row_col(uint8_t row, uint8_t col, uint8_t fill){
    // row, col : 0 to 8
    // fill vs nofill for player distinction
    row = 8 - row;
    board_draw_pawn(col*BOARD_CELL_SPACING, row*BOARD_CELL_SPACING, fill);
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