#include <string.h>
#include <stdio.h>
#include "quoridor_config.h"
#include "quoridor_board.h"
#include "quoridor_gameplay.h"

uint8_t distances_to_end[2];
uint8_t players_positions[2][2];
uint8_t players_walls[2][30];

void replay_game_init(moves_lode_notation)
{
    // provide string of moves "N,S,4e"
    game_init();
    moves_string_to_moves(moves_lode_notation);
    display_game_state();
}

void replay_game_next_move()
{
    move_next();
    display_game_state();
}

void display_game_state()
{

    pawn_get_position_as_row_col(players_positions[0], 0);
    pawn_get_position_as_row_col(players_positions[1], 1);

    distances_to_end[0] = dijkstra(pawn_get_position_as_square_index(0), 0);
    distances_to_end[1] = dijkstra(pawn_get_position_as_square_index(1), 1);

    walls_get_all_positions(players_walls[0], 0);
    walls_get_all_positions(players_walls[1], 1);
 
    board_state_update(players_positions[0], players_positions[1], players_walls[0], players_walls[1], distances_to_end, get_move_counter());
    board_state_draw();
}
