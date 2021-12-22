#ifndef QUORIDOR_BOARD_H
#define QUORIDOR_BOARD_H

#include <_ansi.h>

_BEGIN_STD_C

void board_set_cursor(uint8_t* row_col);

void board_draw_cursor_pawn();
void oard_draw_cursor_wall();

void board_state_update(uint8_t* player_1_pos, uint8_t* player_2_pos, uint8_t player_1_wall_placed, uint8_t player_2_wall_placed, uint8_t* player_1_walls, uint8_t* player_2_walls, uint8_t* distances_to_win, uint8_t move_counter_a, int8_t* move_deltas, uint16_t game_counter);

void board_state_draw();
void menu_display_game_type(uint8_t active_item, char *item_0, char *item_1, char *item_2, char *item_3);
void board_draw_test();

void board_draw_outline();

void board_draw_dots();

void board_draw_pawn_row_col(uint8_t row, uint8_t col, uint8_t fill);

void board_draw_wall(uint8_t row, uint8_t col, uint8_t horizontal_else_vertical, uint8_t fat_wall );

void board_draw_lines();

_END_STD_C

#endif // QUORIDOR_BOARD_H
