#ifndef QUORIDOR_AUTOPLAY_H
#define QUORIDOR_AUTOPLAY_H

#include <_ansi.h>
#include <stdint.h>


uint8_t autoplay_next_move();
uint8_t autoplay_get_best_next_move(uint8_t player);
// void autoplay_analyse_move_wall(uint8_t move_index, int8_t* delta);

#endif //QUORIDOR_AUTOPLAY_H
