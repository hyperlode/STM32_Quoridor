#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H
_BEGIN_STD_C
#include <_ansi.h>

#define STATE_MOVE_PAWN 0
#define STATE_MOVE_WALL_HORIZONTAL 1
#define STATE_MOVE_WALL_VERTICAL 2
#define STATE_INGAME_MENU 3
#define STATE_INVALID 4

uint8_t human_commit_move();
uint8_t human_commit_move_pawn();
uint8_t human_commit_move_wall();
uint8_t human_is_in_ingame_menu();
uint8_t human_get_state();


// struct Human_play_mode{
//     uint8_t move_pawn;
//     uint8_t move_wall_horizontal;
//     uint8_t move_wall_vertical;
// }

// typdef struct Human_play_mode  Human_play_mode;

_END_STD_C
#endif //HUMAN_PLAYER_H