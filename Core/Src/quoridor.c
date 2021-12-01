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
    autoplay_init();
    display_game_state();
}


void move_cursor(){

}

void autoplay_game_next_move()
{
    if (get_game_done()){
        autoplay_game_init();
        return;
    }
    
    autoplay_execute_next_move();
    display_game_state();
}

void replay_game_init(moves_lode_notation)
{
    // provide string of moves "N,S,4e"
    game_init();
    load_game_by_notation_one_string(moves_lode_notation);
    display_game_state();
}

void replay_game_next_move()
{
    next_move_loaded_game();
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
