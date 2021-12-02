#include <string.h>
#include <stdio.h>
#include "quoridor_gameplay.h"
#include "human_player.h"
#include "quoridor_config.h"

uint8_t player;
uint8_t cursor_row_col[2];
int8_t row_col_cursor_distances_from_pawn[2];

void human_init(void)
{
    human_turn_init();
}

void human_turn_init()
{
    player = get_playing_player();
    pawn_get_position_as_row_col(cursor_row_col, player);
    row_col_cursor_distances_from_pawn[0] = 0;
    row_col_cursor_distances_from_pawn[1] = 0;
}

uint8_t human_commit_move()
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
        if (delta_col == 0)
        {
            move_index = EAST_EAST;
        }
    }
    else if (delta_row == -2)
    {
        if (delta_col == 0)
        {
            move_index = WEST_WEST;
        }
    }
    
    if (move_index == MOVE_INDEX_DUMMY)
    {
        // invalid move.
        return 0;
    }

    if (get_move_index_valid(move_index))
    {
        make_move(move_index);
        return 1;
    }

    return 0;
}

void human_button_press(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle, uint8_t *cursor_position)
{

    if (north)
    {
        cursor_row_col[0] += 1;
        row_col_cursor_distances_from_pawn[0] += 1;
    }
    if (east)
    {
        cursor_row_col[1] += 1;
        row_col_cursor_distances_from_pawn[1] += 1;
    }
    if (south)
    {
        cursor_row_col[0] -= 1;
        row_col_cursor_distances_from_pawn[0] -= 1;
    }
    if (west)
    {
        cursor_row_col[1] -= 1;
        row_col_cursor_distances_from_pawn[1] -= 1;
    }
    cursor_position[0] = cursor_row_col[0];
    cursor_position[1] = cursor_row_col[1];
}