#ifndef QUORIDOR_AUTOPLAY_H
#define QUORIDOR_AUTOPLAY_H

#include <_ansi.h>
#include <stdint.h>


uint8_t autoplay_next_move();
uint8_t auto_play_get_move_from_opening_database();
uint8_t autoplay_get_best_next_move(uint8_t player, uint8_t level);
uint8_t autoplay_get_best_next_move_L1(uint8_t player);
uint8_t autoplay_get_best_next_move_L1_with_delta(uint8_t player, int8_t* r_best_delta);
uint8_t autoplay_get_best_next_move_L2(uint8_t player);
uint8_t pick_move_from_move_indeces(uint8_t* move_indeces, uint8_t move_indeces_count, uint8_t pawn_moves_count, uint8_t wall_aggressiveness);
uint8_t autoplay_paused_get();
uint8_t auto_play_select_moves_with_equal_delta(uint8_t *best_counter_move_indeces, int8_t *best_counter_move_deltas, uint8_t valid_move_count,
                                     uint8_t *equal_delta_counter_moves, uint8_t* equal_delta_counter_moves_count, uint8_t* equal_delta_counter_moves_pawn_count);
// void autoplay_analyse_move_wall(uint8_t move_index, int8_t* delta);

#endif //QUORIDOR_AUTOPLAY_H
