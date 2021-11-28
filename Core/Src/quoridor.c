#include <string.h>
#include <stdio.h>
#include "quoridor_config.h"
#include "quoridor_board.h"
#include "quoridor_gameplay.h"

uint8_t distances_to_end[2];
uint8_t players_positions[2][2];
uint8_t players_walls[2][30];

void autoplay_game_init()
{
    // 2 computer players
    game_init();
    display_game_state();
}

void autoplay_game_next_move()
{
    char next_move[2];
    autoplay_next_move(next_move);
    move_by_notation(next_move);
    display_game_state();
}

void replay_game_init(moves_lode_notation)
{
    // provide string of moves "N,S,4e"
    game_init();
    moves_string_to_moves(moves_lode_notation);
    display_game_state();
}

void replay_game_next_move()
{
    move_next_replay();
    display_game_state();
}

void display_game_state()
{
    pawn_get_position_as_row_col(players_positions[0], 0);
    pawn_get_position_as_row_col(players_positions[1], 1);

    get_winning_distances(distances_to_end);

    walls_get_all_positions(players_walls[0], 0);
    walls_get_all_positions(players_walls[1], 1);

    board_state_update(players_positions[0], players_positions[1], players_walls[0], players_walls[1], distances_to_end, get_move_counter());
    board_state_draw();
}
