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

void animation()
{
#define LINES 20
    if (HAL_GetTick() > animation_step_next_millis)
    {
        #define GRID_DISAPPEAR_START_STEP 13
        animation_step_next_millis = HAL_GetTick() + ANIMATION_STEP_MILLIS;
        animation_step_counter++;
        ssd1306_Fill(Black);

        // display grid

        uint8_t first_vertical=0;
        if (animation_step_counter > GRID_DISAPPEAR_START_STEP){
            first_vertical = animation_step_counter - GRID_DISAPPEAR_START_STEP;
            if (first_vertical > 19){
                first_vertical = 19;
            }
        }

        uint16_t verticals = animation_step_counter;
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
                White);
        }

        uint8_t first_horizontal=0;
        if (animation_step_counter > GRID_DISAPPEAR_START_STEP+5){
            first_horizontal = animation_step_counter - GRID_DISAPPEAR_START_STEP-5;
            if (first_horizontal > 10){
                first_horizontal = 10;
            }
        }

        int16_t horizontals = animation_step_counter - 3;
        if (horizontals > 10)
        {
            horizontals = 10;
        }

        if (animation_step_counter > 3)
        {
            for (uint8_t hori = 0; hori < horizontals; hori++)
            {
                ssd1306_Line(
                    0,
                    hori * BOARD_CELL_SPACING,
                    BOARD_X_MAX,
                    hori * BOARD_CELL_SPACING,
                    White);
            }
        }


        // display dots

        for (uint8_t fill_col=0;fill_col< first_vertical;fill_col++)
        {
            for (uint8_t fill_line=0;fill_line<BOARD_CELL_SPACING;fill_line++)
             ssd1306_Line(
                    fill_line + fill_col*BOARD_CELL_SPACING,
                    0,
                    fill_line + fill_col*BOARD_CELL_SPACING,
                    first_horizontal * BOARD_CELL_SPACING,
                    White);
        }
            
        for (uint8_t rows = 0; rows < first_horizontal; rows++)
        {
            for (uint8_t cols = 0; cols < first_vertical; cols++)
            {
                ssd1306_DrawPixel(BOARD_OFFSET_X + cols * BOARD_CELL_SPACING, BOARD_OFFSET_Y + rows * BOARD_CELL_SPACING, Black);
            }
        }

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
    }
    ssd1306_UpdateScreen();
}

// void animationAddRaster(step){

// }