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

uint8_t menu_active_item;
uint8_t menu_display_update;

uint8_t menu_ingame_active_item;
uint8_t menu_ingame_display_update;
uint8_t menu_ingame_active_memory;

uint8_t program_state;

uint8_t auto_play_pause = 0;
void quoridor_init()
{
    quoridor_state = STATE_QUORIDOR_INIT;
    program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
}

void program_state_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    switch (program_state)
    {

    case STATE_PROGRAM_MENU_GAMETYPE_INIT:
    {

        menu_active_item = 2;
        menu_display_update = 1;
        program_state = STATE_PROGRAM_MENU_GAMETYPE;
        break;
    }

    case STATE_PROGRAM_MENU_GAMETYPE:
    {
        quoridor_menu_gameplay(north, east, south, west, enter, toggle);
        break;
    }
    case STATE_PROGRAM_HUMAN_VS_COMPUTER:
    {
        quoridor_human_vs_computer_manager(north, east, south, west, enter, toggle);
        break;
    }
    case STATE_PROGRAM_HUMAN_VS_HUMAN:
    {
        quoridor_human_vs_human_manager(north, east, south, west, enter, toggle);
        break;
    }
    case STATE_PROGRAM_COMPUTER_VS_COMPUTER:
    {
        if (toggle)
        {
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
        }
        quoridor_computer_vs_computer_manager(north, east, south, west, enter, toggle);
        break;
    }
    default:
    {
        break;
    }
    }
}
void quoridor_menu_ingame_init()
{
    menu_ingame_active_item = 2;
    menu_ingame_display_update = 1;
}

void quoridor_menu_ingame(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    if (north)
    {
        menu_ingame_active_item++;
        if (menu_ingame_active_item > 2)
        {
            menu_ingame_active_item = 0;
        }
        menu_ingame_display_update = 1;
    }
    if (south)
    {
        if (menu_ingame_active_item == 0)
        {
            menu_ingame_active_item = 2;
        }
        else
        {
            menu_ingame_active_item--;
        }
        menu_ingame_display_update = 1;
    }

    if (enter)
    {
        switch (menu_ingame_active_item)
        {
        case 2:
        {
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            break;
        }
        default:
        {
            break;
        }
        }
    }

    if (menu_ingame_display_update)
    {
        menu_display_ingame(menu_ingame_active_item);
    }
    menu_ingame_display_update = 0;
}

void quoridor_menu_gameplay(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    if (north)
    {
        menu_active_item++;
        if (menu_active_item > 2)
        {
            menu_active_item = 0;
        }
        menu_display_update = 1;
    }
    if (south)
    {
        if (menu_active_item == 0)
        {
            menu_active_item = 2;
        }
        else
        {
            menu_active_item--;
        }
        menu_display_update = 1;
    }
    if (enter)
    {
        switch (menu_active_item)
        {
        case 2:
        {
            quoridor_state = STATE_QUORIDOR_INIT;
            program_state = STATE_PROGRAM_HUMAN_VS_COMPUTER;
            break;
        }
        case 1:
        {
            quoridor_state = STATE_QUORIDOR_INIT;
            quoridor_state = STATE_QUORIDOR_INIT;
            program_state = STATE_PROGRAM_HUMAN_VS_HUMAN;
            break;
        }
        case 0:
        {
            quoridor_state = STATE_QUORIDOR_INIT;
            program_state = STATE_PROGRAM_COMPUTER_VS_COMPUTER;
            break;
        }
        }
    }

    if (menu_display_update)
    {
        menu_display(menu_active_item);
    }
    menu_display_update = 0;
}

void quoridor_human_vs_human_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init();
        display_game_state();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_INIT):
    {
        human_turn_init();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_END):
    {
        if (get_winner_index() != NO_WINNER)
        {
            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        }

        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN):
    {
        quoridor_human_interaction(north, east, south, west, enter, toggle);

        break;
    }

    case (STATE_QUORIDOR_FINISHED):
    {
        if (enter)
        {
            quoridor_state = STATE_QUORIDOR_INIT;
        }

        break;
    }
    default:
    {
    }
    }
}

void quoridor_computer_vs_computer_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init();
        autoplay_init();
        display_game_state();
        quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_INIT;
        break;
    }

    case (STATE_QUORIDOR_COMPUTER_TURN_INIT):
    {

        quoridor_state = STATE_QUORIDOR_COMPUTER_TURN;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_TURN):
    {

        if (west){
            if (auto_play_pause){
                auto_play_pause = 0;
            }else{
                auto_play_pause = 1;
            }
        }
        if (!auto_play_pause){
                
            autoplay_execute_next_move();
            if (get_winner_index() != NO_WINNER)
            {
                quoridor_state = STATE_QUORIDOR_FINISHED;
            }
            else
            {
                quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_INIT;
            }
            board_set_move_type(autoplay_get_move_type());
            display_game_state();
        }
        break;
    }
    case (STATE_QUORIDOR_FINISHED):
    {
        // if (enter)
        // {
            quoridor_state = STATE_QUORIDOR_INIT;
        // }

        break;
    }
    default:
    {
    }
    }
}

void quoridor_human_vs_computer_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init();
        display_game_state();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_INIT):
    {
        human_turn_init();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_END):
    {
        if (get_winner_index() != NO_WINNER)
        {
            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_INIT;
        }

        break;
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
    
        autoplay_execute_next_move();
        if (get_winner_index() != NO_WINNER)
        {
            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        }
        board_set_move_type(autoplay_get_move_type());
        display_game_state();
       
        break;
    }
    case (STATE_QUORIDOR_FINISHED):
    {
        if (enter)
        {
            quoridor_state = STATE_QUORIDOR_INIT;
        }

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
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_END;
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

    if (human_is_in_ingame_menu())
    {
        if (!menu_ingame_active_memory)
        {
            quoridor_menu_ingame_init(); // should be called only once at init todo
        }
        quoridor_menu_ingame(north, east, south, west, enter, toggle);
        menu_ingame_active_memory = 1;
    }
    else
    {
        menu_ingame_active_memory = 0;
        display_game_state();
    }
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
