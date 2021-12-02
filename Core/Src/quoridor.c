#include <string.h>
#include <stdio.h>
#include "quoridor_config.h"
#include "quoridor_board.h"
#include "quoridor_gameplay.h"
#include "human_player.h"

uint8_t distances_to_end[2];
uint8_t players_positions[2][2];
uint8_t players_walls[2][30];

uint8_t quoridor_state;
void human_vs_computer_init()
{
    game_init();
    display_game_state();
    quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
}

void quoridor_human_vs_computer_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        human_vs_computer_init();
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_INIT):
    {
        quoridor_human_turn_init();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
    }
    case (STATE_QUORIDOR_HUMAN_TURN):
    {
        quoridor_human_interaction(north, east, south, west, enter, toggle);
        break;
    }
    case (STATE_QUORIDOR_COMPUTER_TURN_INIT):
    {

        quoridor_state = STATE_QUORIDOR_COMPUTER_TURN;
        break;
    }
    case (STATE_QUORIDOR_COMPUTER_TURN):
    {
        quoridor_computer_turn_init();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;

        break;
    }
    case (STATE_QUORIDOR_FINISHED):
    {
        break;
    }
    default:
    {
    }
    }
}

void quoridor_human_interaction(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    uint8_t cursor_position[2];
    uint8_t cursor_wall_position[3];

    if (enter)
    {
        uint8_t success;
        success = human_commit_move();
        if (success)
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_INIT;
        }
    }
    else
    {
        human_button_press(north, east, south, west, enter, toggle);

        human_get_cursor_wall(cursor_wall_position);
        board_set_cursor_wall(cursor_wall_position);
        

        human_get_cursor_pawn(cursor_position);
        board_set_cursor(cursor_position);
    }
    display_game_state();
}

void quoridor_human_turn_init()
{
    human_turn_init();
}

void quoridor_computer_turn_init()
{
    autoplay_execute_next_move();
    quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
}

void autoplay_game_init()
{
    // 2 computer players
    game_init();
    autoplay_init();
    display_game_state();
}

void autoplay_game_next_move()
{
    if (get_game_done())
    {
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
