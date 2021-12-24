#include <string.h>
#include <stdio.h>

#include "quoridor_animation.h"

uint32_t animation_step_next_millis;
uint16_t animation_step_counter;

void animation_set_step_counter(uint16_t step)
{
    animation_step_counter = step;
}
uint16_t animation_get_steps()
{
    return animation_step_counter;
}
void animation_init()
{
    animation_step_next_millis = HAL_GetTick() + ANIMATION_STEP_MILLIS;
    animation_step_counter = 0;
}
uint8_t animation_ended()
{
    return animation_get_steps() > ANIMATION_LENGTH_STEPS;
}

void animation_next_step_timed()
{
    if (HAL_GetTick() > animation_step_next_millis)
    {
        animation_step_next_millis = HAL_GetTick() + ANIMATION_STEP_MILLIS;
        animation_step_counter++;

        animation(animation_step_counter);
    }
}

void animation_next_step_manual()
{
    animation_step_counter++;
    animation(animation_step_counter);
}

void animation(uint16_t step)
{
#define LINES 20

    ssd1306_Fill(White);

    if (animation_step_counter < ANIMATION_START_GRID_DISAPPEAR_STEP)
    {

        // display grid animation
        animation_display_grid(animation_step_counter);
    }
    else if (animation_step_counter < ANIMATION_START_MOVE_PAWNS_STEP)
    {

        // make grid transition to text
        animation_display_text(animation_step_counter - ANIMATION_START_GRID_DISAPPEAR_STEP);
    }
    else if (animation_step_counter < ANIMATION_START_SLIDE_PAWNS_STEP)
    {
        animation_display_text(animation_step_counter - ANIMATION_START_GRID_DISAPPEAR_STEP);
        animation_move_pawns(animation_step_counter - ANIMATION_START_MOVE_PAWNS_STEP);
    }
    else if (animation_step_counter < ANIMATION_START_DISPLAY_CREDITS)
    {
        animation_display_text(animation_step_counter - ANIMATION_START_GRID_DISAPPEAR_STEP);
        animation_move_pawns_freely(animation_step_counter - ANIMATION_START_SLIDE_PAWNS_STEP);
    }
    else
    {
        animation_display_text(animation_step_counter - ANIMATION_START_GRID_DISAPPEAR_STEP);
        animation_move_pawns_freely(animation_step_counter - ANIMATION_START_SLIDE_PAWNS_STEP);

        //set cursor
        ssd1306_SetCursor(53, 49);
        ssd1306_WriteString("ameije.com", Font_7x10, Black);
    }

    // display text

    //

    // filled up rectangle
    // for (uint8_t fill_col=0;fill_col< first_vertical;fill_col++)
    // {
    //     for (uint8_t fill_line=0;fill_line<BOARD_CELL_SPACING;fill_line++)
    //      ssd1306_Line(
    //             fill_line + fill_col*BOARD_CELL_SPACING,
    //             0,
    //             fill_line + fill_col*BOARD_CELL_SPACING,
    //             first_horizontal * BOARD_CELL_SPACING,
    //             White);
    // }

    // // display dots
    // for (uint8_t rows = 0; rows < first_horizontal; rows++)
    // {
    //     for (uint8_t cols = 0; cols < first_vertical; cols++)
    //     {
    //         ssd1306_DrawPixel(BOARD_OFFSET_X + cols * BOARD_CELL_SPACING, BOARD_OFFSET_Y + rows * BOARD_CELL_SPACING, Black);
    //     }
    // }

    // for (uint8_t i=0;i<animation_step_counter;i++){
    //    ssd1306_Line(
    //     // ((BOARD_X_MAX/2) - l/2) +  ,
    //     animation_step_counter,
    //     0,
    //     (BOARD_X_MAX/LINES)*i,
    //     BOARD_Y_MAX,
    //     Black);
    // }

    // ssd1306_Line(
    //     0,
    //     animation_step_counter,
    //     BOARD_X_MAX,
    //     animation_step_counter,
    //     Black);

    ssd1306_UpdateScreen();
}

void animation_move_pawns_freely(int16_t in_step)
{
    // move pawns freely

    uint8_t step = (in_step) / 2;

#define ANIMATION_NUMBER_OF_PAWN_SLIDE_POSITIONS 6

    if (step >= ANIMATION_NUMBER_OF_PAWN_SLIDE_POSITIONS)
    {
        step = ANIMATION_NUMBER_OF_PAWN_SLIDE_POSITIONS - 1;
    }

    int8_t pawn_moves[ANIMATION_NUMBER_OF_PAWN_SLIDE_POSITIONS][2][2] = {
        {{0, 0}, {0, 0}},
        {{-1, -1}, {1, 1}},
        {{-2, -2}, {2, 2}},
        {{-3, -3}, {3, 3}},
        {{-4, -4}, {4, 4}},
        {{-4, -4}, {4, 5}}};

    board_draw_pawn(21 + pawn_moves[step][0][0], 35 + pawn_moves[step][0][1], 0, 0);
    board_draw_pawn(63 + pawn_moves[step][1][0], 14 + pawn_moves[step][1][1], 1, 0);
    // board_draw_pawn(10,17,0,0);
    // board_draw_pawn(60,5,1,0);
}

void animation_move_pawns(int16_t in_step)
{
    // move pawns in row and col

    uint8_t step = (in_step) / 6;

#define ANIMATION_NUMBER_OF_PAWN_POSITIONS 9
    if (step >= ANIMATION_NUMBER_OF_PAWN_POSITIONS)
    {
        step = ANIMATION_NUMBER_OF_PAWN_POSITIONS - 1;
    }

    uint8_t pawn_moves[ANIMATION_NUMBER_OF_PAWN_POSITIONS][2][2] = {
        {{0, 4}, {8, 4}},
        {{0, 4}, {8, 4}},
        {{1, 4}, {7, 4}},
        {{2, 4}, {6, 4}},
        {{2, 3}, {6, 5}},
        {{3, 3}, {6, 6}},
        {{3, 3}, {6, 7}},
        {{3, 3}, {6, 8}},
        {{3, 3}, {6, 9}}};
    board_draw_pawn_row_col(pawn_moves[step][0][0], pawn_moves[step][0][1], 0);
    board_draw_pawn_row_col(pawn_moves[step][1][0], pawn_moves[step][1][1], 1);
}
void animation_display_text(int16_t in_step)
{
    uint8_t step = in_step / 4;
    int16_t grid_to_dot_step = step;
    if (grid_to_dot_step > 6)
    {
        grid_to_dot_step = 6;
    }

    // animation grid to dots
    for (int8_t row = 0; row < 10; row++)
    {
        for (int8_t col = 0; col < 19; col++)
        {

            int8_t left_part = BOARD_OFFSET_X + col * BOARD_CELL_SPACING - (3 - (grid_to_dot_step / 2));
            if (left_part < 0)
            {
                left_part = 0;
            }
            int8_t bottom_part = BOARD_OFFSET_Y + row * BOARD_CELL_SPACING - (3 - (grid_to_dot_step / 2));
            if (bottom_part < 0)
            {
                bottom_part = 0;
            }

            // ssd1306_DrawPixel(BOARD_OFFSET_X + col * BOARD_CELL_SPACING, BOARD_OFFSET_Y + row * BOARD_CELL_SPACING, Black);
            ssd1306_Line(
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING + (3 - (grid_to_dot_step / 2 + grid_to_dot_step % 2)),
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING,
                (uint8_t)left_part,
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING,
                Black);

            ssd1306_Line(
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING,
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING + (3 - (grid_to_dot_step / 2 + grid_to_dot_step % 2)),
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING,
                bottom_part,
                Black);
        }
    }

// display "quoridor"-text with fake walls (fake as in: only one cell wide)
// row-col-dir (dir:0=vertical, 1=horizontal)
#define SEGMENTS_COUNT 27
    uint8_t segments[SEGMENTS_COUNT][3] = {
        {1, 1, 0}, // Q
        {2, 1, 0},
        {1, 1, 1},
        {3, 1, 1},
        {1, 2, 0},
        {2, 2, 0},

        {2, 3, 0},
        {3, 3, 1},
        {2, 4, 0},

        {2, 5, 0},
        {2, 5, 1},
        {3, 5, 1},
        {2, 6, 0},

        {2, 7, 0}, // R
        {2, 7, 1},

        {2, 9, 0},

        {2, 10, 0},
        {2, 10, 1},
        {3, 10, 1},
        {1, 11, 0},
        {2, 11, 0},

        {2, 12, 0},
        {2, 12, 1},
        {3, 12, 1},
        {2, 13, 0},

        {2, 14, 0}, // R
        {2, 14, 1}

    };

    for (uint8_t segment_index = 0; segment_index < SEGMENTS_COUNT; segment_index++)
    {
        uint8_t row = 2 + segments[segment_index][0];
        uint8_t col = 1 + segments[segment_index][1];
        uint8_t horizontal_else_vertical = segments[segment_index][2];

        if (horizontal_else_vertical)
        {
            ssd1306_Line(
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING,
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING,
                BOARD_OFFSET_X + (col + 1) * BOARD_CELL_SPACING,
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING,
                Black);
        }
        else
        {
            ssd1306_Line(
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING,
                BOARD_OFFSET_Y + row * BOARD_CELL_SPACING,
                BOARD_OFFSET_X + col * BOARD_CELL_SPACING,
                BOARD_OFFSET_Y + (row + 1) * BOARD_CELL_SPACING,
                Black);
        }
    }
}
void animation_display_grid(int16_t step)
{

    // display grid

    uint8_t first_vertical = 0;
    if (step > ANIMATION_START_GRID_DISAPPEAR_STEP)
    {
        first_vertical = step - ANIMATION_START_GRID_DISAPPEAR_STEP;
        if (first_vertical > 19)
        {
            first_vertical = 19;
        }
    }

    uint16_t verticals = step;
    if (verticals > 19)
    {
        verticals = 19;
    }
    for (uint8_t vert = 0; vert < verticals; vert++)
    {
        ssd1306_Line(
            vert * BOARD_CELL_SPACING,
            0,
            vert * BOARD_CELL_SPACING,
            BOARD_Y_MAX,
            Black);
    }

    uint8_t first_horizontal = 0;
    if (step > ANIMATION_START_GRID_DISAPPEAR_STEP + 5)
    {
        first_horizontal = step - ANIMATION_START_GRID_DISAPPEAR_STEP - 5;
        if (first_horizontal > 10)
        {
            first_horizontal = 10;
        }
    }

    int16_t horizontals = step - 3;
    if (horizontals > 10)
    {
        horizontals = 10;
    }

    if (step > 3)
    {
        for (uint8_t hori = 0; hori < horizontals; hori++)
        {
            ssd1306_Line(
                0,
                hori * BOARD_CELL_SPACING,
                BOARD_X_MAX,
                hori * BOARD_CELL_SPACING,
                Black);
        }
    }
}