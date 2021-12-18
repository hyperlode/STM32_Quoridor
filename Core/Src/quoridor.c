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

uint8_t game_counter;

uint8_t auto_play_single_step_mode;

uint8_t loaded_game_history_moves_indeces[RECORD_MOVES_HISTORY_LENGTH];
uint8_t loaded_game_history_moves_indeces_length;

void quoridor_display_game_with_error_code(uint8_t error_code)
{
    board_set_error(error_code);
    // board_state_draw();
    display_game_state();
}

void quoridor_display_game_with_time_bar_percentage(uint8_t percentage_completed)
{
    board_set_time_bar_percentage_completed(percentage_completed);
    display_game_state();
}

void quoridor_init()
{
    quoridor_state = STATE_QUORIDOR_INIT;
    program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
    auto_play_single_step_mode = 0;
    void (*function_pointer)(uint8_t);

    function_pointer = &quoridor_display_game_with_error_code;
    quoridor_error_set_callback(function_pointer);

    function_pointer = &quoridor_display_game_with_time_bar_percentage;
    autoplay_display_time_callback(function_pointer);
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
        case 0:
        {
            //program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            // give up
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            break;
        }
        case 1:
        {
            // undo
            quoridor_state = STATE_QUORIDOR_UNDO_TURN;
            break;
        }
        case 2:
        {
            // return to game
            //human_set_state(STATE_MOVE_PAWN);
            autoplay_execute_next_move(2);
            human_set_state(STATE_MOVE_PAWN);

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
        char title[] = "Back with Toggle ";
        char item_0[] = "L2 move";
        char item_1[] = "Undo last move";
        char item_2[] = "Give up";

        menu_display_3_items(menu_ingame_active_item, title, item_0, item_1, item_2);
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

        char title[] = "Pick your fight";
        char item_0[] = "Human vs Machine";
        char item_1[] = "Human battle";
        char item_2[] = "Machine battle";

        menu_display_3_items(menu_active_item, title, item_0, item_1, item_2);
    }
    menu_display_update = 0;
}

void quoridor_lode_load_game_history_as_one_string(char *moves_as_notation_one_string, uint8_t history_length)
{
    // provide as lode notation, will store as move_indeces
    moves_string_to_moves_indeces(moves_as_notation_one_string, loaded_game_history_moves_indeces);
    loaded_game_history_moves_indeces_length = history_length;
}

void quoridor_human_vs_human_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init(loaded_game_history_moves_indeces, loaded_game_history_moves_indeces_length);
        display_game_state();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        break;
    }
    case (STATE_QUORIDOR_UNDO_TURN):
    {
        undo_last_move();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        // quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
        human_set_state(STATE_MOVE_PAWN);
        break;
    }

    case (STATE_QUORIDOR_HUMAN_TURN):
    {
        quoridor_human_interaction(north, east, south, west, enter, toggle);

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
        board_hide_cursor();
        if (get_winner_index() != NO_WINNER)
        {
            quoridor_state = STATE_QUORIDOR_FINISHED;
            game_counter++;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        }

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
    if (enter)
    {
        auto_play_single_step_mode = !auto_play_single_step_mode;
    }

    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init(NULL, 0);
        autoplay_init();
        display_game_state();
        quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_INIT;
        break;
    }

    case (STATE_QUORIDOR_COMPUTER_L1_TURN_INIT):
    {

        quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L1_TURN):
    {

        if (!auto_play_single_step_mode || north || south || east)
        {
            if (north)
            {
                autoplay_execute_next_move(2);
            }
            else
            {
                autoplay_execute_next_move(1);
            }
            if (get_winner_index() != NO_WINNER)
            {
                game_counter++;
                quoridor_state = STATE_QUORIDOR_FINISHED;
            }
            else
            {
                quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN_INIT;
            }
            board_set_move_type(autoplay_get_move_type());
        }

        if (west)
        {
            undo_last_move();
        }

        display_game_state();
        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L2_TURN_INIT):
    {

        quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L2_TURN):
    {

        computer_emphasize_pawn();

        if (!auto_play_single_step_mode || north || south || east)
        {
            if (north)
            {
                autoplay_execute_next_move(2);
            }
            else
            {
                autoplay_execute_next_move(2);
            }
            if (get_winner_index() != NO_WINNER)
            {
                game_counter++;
                quoridor_state = STATE_QUORIDOR_FINISHED;
            }
            else
            {
                quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_INIT;
            }
            board_set_move_type(autoplay_get_move_type());
        }

        computer_deemphasize_pawn();

        if (west)
        {
            undo_last_move();
        }

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

void quoridor_human_vs_computer_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init(NULL, 0);
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        break;
    }
    case (STATE_QUORIDOR_UNDO_TURN):
    {
        // we want to undo the last HUMAN turn. which also undoes the computer's move
        undo_last_move();
        undo_last_move();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        // quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
        human_set_state(STATE_MOVE_PAWN);
        quoridor_display_game_with_time_bar_percentage(100);
        break;
    }

    case (STATE_QUORIDOR_HUMAN_TURN_INIT):
    {
        human_turn_init();
        quoridor_state = STATE_QUORIDOR_HUMAN_TURN;
        quoridor_display_game_with_time_bar_percentage(100);
        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN):
    {
        quoridor_human_interaction(north, east, south, west, enter, toggle);

        break;
    }
    case (STATE_QUORIDOR_HUMAN_TURN_END):
    {
        board_hide_cursor();
        if (get_winner_index() != NO_WINNER)
        {
            game_counter++;
            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_INIT;
        }

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_TURN_INIT):
    {
        quoridor_display_game_with_time_bar_percentage(100);
        quoridor_state = STATE_QUORIDOR_COMPUTER_TURN;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_TURN):
    {
        computer_emphasize_pawn();
        autoplay_execute_next_move(2);
        if (get_winner_index() != NO_WINNER)
        {
            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
        }
        board_set_move_type(autoplay_get_move_type());
        // display_game_state();
        computer_deemphasize_pawn();

        break;
    }
    case (STATE_QUORIDOR_FINISHED):
    {
        quoridor_display_game_with_time_bar_percentage(100);
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

// void replay_game_init(char* moves_lode_notation, uint8_t moves_history_length)
// {
//     // provide string of moves "N,S,4e"
//     game_init(moves_lode_notation, moves_history_length);

//     display_game_state();
// }

// void replay_game_next_move()
// {
//     next_move_loaded_game();
//     display_game_state();
// }
void computer_deemphasize_pawn(){
    board_hide_cursor();

}
void computer_emphasize_pawn(){
    uint8_t cursor_position[2];
    autoplay_get_cursor(cursor_position);
    board_set_cursor(cursor_position);
}

void display_game_state()
{
    pawn_get_position_as_row_col(players_positions[0], 0);
    pawn_get_position_as_row_col(players_positions[1], 1);

    get_winning_distances(distances_to_end);

    walls_get_all_positions(players_walls[0], 0);
    walls_get_all_positions(players_walls[1], 1);

    //board_set_error(4);

    board_state_update(
        players_positions[0],
        players_positions[1],
        get_walls_placed(0),
        get_walls_placed(1),
        players_walls[0],
        players_walls[1],
        distances_to_end,
        get_move_counter(),
        get_move_history_deltas_without_jumps(),
        game_counter);

    board_state_draw();
}