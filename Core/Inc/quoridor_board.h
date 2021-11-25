#ifndef QUORIDOR_BOARD_H
#define QUORIDOR_BOARD_H

#include <_ansi.h>

_BEGIN_STD_C
struct Pawn{
    uint8_t row;
    uint8_t col;
    uint8_t fill;    
};

struct Wall{
    uint8_t row;
    uint8_t col;
    uint8_t horizontal_else_vertical;
};

struct Player{
    struct Pawn pawn;
    struct Wall walls [10];
};


typedef struct Player Player;




void board_draw_refresh();
void board_draw_lines(void);
void board_draw_dots(void);
void board_draw_pawn(uint8_t row, uint8_t col, uint8_t fill);
void oled_init(void);
void board_state_update(uint8_t* player_1_pos, uint8_t* player_2_pos, uint8_t* player_1_walls, uint8_t* player_2_walls);
void board_state_draw();

_END_STD_C

#endif // QUORIDOR_BOARD_H
