#include <string.h>
#include <stdio.h>
#include "quoridor_gameplay.h"
#include "human_player.h"
#include "quoridor_config.h"

uint8_t player;
uint8_t cursor_pawn_row_col[2];
uint8_t cursor_wall_row_col_dir[3];

int8_t row_col_cursor_distances_from_pawn[2];

uint8_t state_play_mode;


void human_turn_init()
{
    player = get_playing_player();
    pawn_get_position_as_row_col(cursor_pawn_row_col, player);

    row_col_cursor_distances_from_pawn[0] = 0;
    row_col_cursor_distances_from_pawn[1] = 0;

    cursor_wall_row_col_dir[0] = 4;
    cursor_wall_row_col_dir[1] = 4;

    human_back_to_game();
}

void human_back_to_game(){
    // e.g. when returning from menu
    human_set_state(STATE_MOVE_PAWN);

}
uint8_t human_commit_move()
{

    if (state_play_mode == STATE_MOVE_PAWN)
    {
        return human_commit_move_pawn();
    }
    else if (state_play_mode == STATE_MOVE_WALL_HORIZONTAL || state_play_mode == STATE_MOVE_WALL_VERTICAL)
    {
        return human_commit_move_wall();
    }

    return 0;
}

uint8_t human_commit_move_wall()
{

    uint8_t wall_position[3];
    uint8_t move_index = MOVE_INDEX_DUMMY;
    human_get_cursor_wall(wall_position);

    move_index = row_col_dir_to_move_index(wall_position);

    return make_move_if_valid(move_index);
}

uint8_t human_commit_move_pawn()
{

    // get move_index from cursor
    // uint8_t pawn_row_col[2];
    // pawn_get_position_as_row_col(pawn_row_col, player);

    // check if move legal
    int8_t delta_row = row_col_cursor_distances_from_pawn[0];
    int8_t delta_col = row_col_cursor_distances_from_pawn[1];
    uint8_t move_index = MOVE_INDEX_DUMMY;

    if (delta_row == 1)
    {
        if (delta_col == 0)
        {
            move_index = NORTH;
        }
        if (delta_col == 1)
        {
            move_index = NORTH_EAST;
        }
        if (delta_col == -1)
        {
            move_index = NORTH_WEST;
        }
    }
    else if (delta_row == -1)
    {
        if (delta_col == 0)
        {
            move_index = SOUTH;
        }
        if (delta_col == 1)
        {
            move_index = SOUTH_EAST;
        }
        if (delta_col == -1)
        {
            move_index = SOUTH_WEST;
        }
    }
    else if (delta_col == 1)
    {
        if (delta_row == 0)
        {
            move_index = EAST;
        }
    }
    else if (delta_col == -1)
    {
        if (delta_row == 0)
        {
            move_index = WEST;
        }
    }
    else if (delta_row == 2)
    {
        if (delta_col == 0)
        {
            move_index = NORTH_NORTH;
        }
    }
    else if (delta_row == -2)
    {
        if (delta_col == 0)
        {
            move_index = SOUTH_SOUTH;
        }
    }
    else if (delta_col == 2)
    {
        if (delta_row == 0)
        {
            move_index = EAST_EAST;
        }
    }
    else if (delta_col == -2)
    {
        if (delta_row == 0)
        {
            move_index = WEST_WEST;
        }
    }
    else if (delta_col == 0 && delta_row == 0){
        // auto pawn move if available.
        move_index = get_best_pawn_move();
        
    }

    if (move_index == MOVE_INDEX_DUMMY)
    {
        // invalid move.
        return 0;
    }

    return make_move_if_valid(move_index);
}

void human_button_press_move_wall_horizontal(uint8_t north, uint8_t east, uint8_t south, uint8_t west)
{
    if (north)
    {
        cursor_wall_row_col_dir[0] += 1;
    }
    if (east)
    {
        cursor_wall_row_col_dir[1] += 1;
    }
    if (south)
    {
        cursor_wall_row_col_dir[0] -= 1;
    }
    if (west)
    {
        cursor_wall_row_col_dir[1] -= 1;
    }
}

void human_button_press_move_pawn(uint8_t north, uint8_t east, uint8_t south, uint8_t west)
{
    if (north)
    {
        cursor_pawn_row_col[0] += 1;
        row_col_cursor_distances_from_pawn[0] += 1;
    }
    if (east)
    {
        cursor_pawn_row_col[1] += 1;
        row_col_cursor_distances_from_pawn[1] += 1;
    }
    if (south)
    {
        cursor_pawn_row_col[0] -= 1;
        row_col_cursor_distances_from_pawn[0] -= 1;
    }
    if (west)
    {
        cursor_pawn_row_col[1] -= 1;
        row_col_cursor_distances_from_pawn[1] -= 1;
    }
}

void human_get_cursor_wall(uint8_t *cursor_wall_position)
{
    if (state_play_mode != STATE_MOVE_WALL_HORIZONTAL && state_play_mode != STATE_MOVE_WALL_VERTICAL)
    {
        cursor_wall_position[0] = CURSOR_NOT_SHOWN;
        cursor_wall_position[1] = CURSOR_NOT_SHOWN;
        return;
    }

    cursor_wall_position[0] = cursor_wall_row_col_dir[0];
    cursor_wall_position[1] = cursor_wall_row_col_dir[1];
    if (state_play_mode == STATE_MOVE_WALL_HORIZONTAL)
    {
        cursor_wall_position[2] = 1;
    }
    else
    {
        cursor_wall_position[2] = 0;
    }
}

void human_get_cursor_pawn(uint8_t *cursor_position)
{
    if (state_play_mode == STATE_MOVE_PAWN)
    {
        cursor_position[0] = cursor_pawn_row_col[0];
        cursor_position[1] = cursor_pawn_row_col[1];
    }
    else
    {
        cursor_position[0] = CURSOR_NOT_SHOWN;
        cursor_position[1] = CURSOR_NOT_SHOWN;
    }
}

void human_button_press_ingame_menu(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    
}
uint8_t human_is_in_ingame_menu()
{
    return human_get_state() == STATE_INGAME_MENU;
}
uint8_t human_get_state()
{
    return state_play_mode;
}
void human_set_state(uint8_t play_state)
{
    state_play_mode = play_state;
}

void human_button_press(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    if (toggle)
    {
        state_play_mode++;
        if (state_play_mode == STATE_INVALID)
        {
            state_play_mode = STATE_MOVE_PAWN;
        }
    }

    switch (state_play_mode)
    {

    case STATE_MOVE_PAWN:
    {
        human_button_press_move_pawn(north, east, south, west);
        break;
    }
    case STATE_MOVE_WALL_HORIZONTAL:
    {
        human_button_press_move_wall_horizontal(north, east, south, west);
        break;
    }
    case STATE_MOVE_WALL_VERTICAL:
    {
        human_button_press_move_wall_horizontal(north, east, south, west);
        break;
    }
    case STATE_INGAME_MENU:
    {
        human_button_press_ingame_menu(north, east, south, west, enter, toggle);
        break;
    }
    default:
    {
        break;
    }
    }
}