#include <string.h>
#include <stdio.h>
#include "quoridor_config.h"
#include "quoridor_board.h"
#include "quoridor_gameplay.h"
#include "human_player.h"
#include "quoridor_animation.h"

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
uint8_t players_type[2];

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
    program_state = STATE_PROGRAM_FLASH_ANIMATION_INIT;
    auto_play_single_step_mode = 0;
    void (*function_pointer)(uint8_t);

    function_pointer = &quoridor_display_game_with_error_code;
    quoridor_error_set_callback(function_pointer);

    function_pointer = &quoridor_display_game_with_time_bar_percentage;
    autoplay_display_time_callback(function_pointer);

    players_type[0] = PLAYER_HUMAN;
    players_type[1] = PLAYER_COMPUTER_L1;
}

void program_state_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    switch (program_state)
    {
    case STATE_PROGRAM_FLASH_ANIMATION_INIT:
    {
        animation_init();
        program_state = STATE_PROGRAM_FLASH_ANIMATION;

        break;
    }
    case STATE_PROGRAM_FLASH_ANIMATION:
    {
        animation_next_step_timed();

        //    if (east){
        //        animation_next_step_manual();
        //    }
        if (toggle)
        {
            animation_set_step_counter(0);
        }
        // if (animation_ended() || enter)
        if (enter || north || south || east || west)
        {
            program_state = STATE_PROGRAM_MENU_START_INIT;
        }
        break;
    }
    case STATE_PROGRAM_MENU_START_INIT:
    {
        menu_display_update = 1;
        program_state = STATE_PROGRAM_MENU_START;
        break;
    }
    case STATE_PROGRAM_MENU_START:
    {
        quoridor_menu_start(north, east, south, west, enter, toggle);
        break;
    }
    case STATE_PROGRAM_MENU_GAMETYPE_INIT:
    {
        menu_display_update = 1;
        program_state = STATE_PROGRAM_MENU_GAMETYPE;
        break;
    }
    case STATE_PROGRAM_MENU_GAMETYPE:
    {
        quoridor_menu_choose_gametype(north, east, south, west, enter, toggle);
        break;
    }
    case STATE_PROGRAM_GAME_PLAYING:
    {
        quoridor_game_manager(north, east, south, west, enter, toggle);
        break;
    }

    default:
    {
        break;
    }
    }
}

void quoridor_menu_start(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    if (south)
    {
        menu_ingame_active_item++;
        if (menu_ingame_active_item > 2)
        {
            menu_ingame_active_item = 0;
        }
        menu_display_update = 1;
    }
    if (north)
    {
        if (menu_ingame_active_item == 0)
        {
            menu_ingame_active_item = 2;
        }
        else
        {
            menu_ingame_active_item--;
        }
        menu_display_update = 1;
    }

    if (toggle)
    {
        program_state = STATE_PROGRAM_FLASH_ANIMATION_INIT;
    }

    if (enter)
    {
        switch (menu_ingame_active_item)
        {
        case 0:
        {
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            break;
        }
        case 1:
        {
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            break;
        }
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

    if (menu_display_update)
    {
        char title[] = "Quoridor";
        char item_0[] = "Play";
        char item_1[] = "Review";
        char item_2[] = "Settings";

        board_menu_display_3_items(menu_ingame_active_item, title, item_0, item_1, item_2);
    }
    menu_display_update = 0;
}

void quoridor_menu_ingame_human_init()
{
    menu_ingame_active_item = 0;
    menu_ingame_display_update = 1;
}

void quoridor_menu_ingame_human(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    if (south)
    {
        menu_ingame_active_item++;
        if (menu_ingame_active_item > 4)
        {
            menu_ingame_active_item = 0;
        }
        menu_ingame_display_update = 1;
    }
    if (north)
    {
        if (menu_ingame_active_item == 0)
        {
            menu_ingame_active_item = 4;
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
            //autoplay_execute_next_move(2);
            // return to game
            human_back_to_game();

            break;
        }
        case 1:
        {
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
            break;
        }
        case 2:
        {
            quoridor_state = STATE_QUORIDOR_UNDO_TURN;
            break;
        }
        case 3:
        {
            quoridor_state = STATE_QUORIDOR_UNDO_TWICE_TURN;
            break;
        }
        case 4:
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN_INIT;
            // autoplay_execute_next_move(2);
            // quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_FINISHED;
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
        char item_0[] = "Back to game";
        char item_1[] = "Give up";
        char item_2[] = "Undo Last Move";
        char item_3[] = "Undo Two Moves";
        char item_4[] = "L2 move";

        board_menu_display_5_items(menu_ingame_active_item, item_0, item_1, item_2, item_3, item_4);
        // char title[] = "Back to game ";
        // char item_0[] = "L2 move";
        // char item_1[] = "Undo last move";
        // char item_2[] = "Give up";

        // board_menu_display_4_items(menu_ingame_active_item, title, item_0, item_1, item_2);
    }
    menu_ingame_display_update = 0;
}

void quoridor_menu_ingame_computer_init()
{
    menu_ingame_active_item = 2;
    menu_ingame_display_update = 1;
}

void quoridor_menu_ingame_computer(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    if (south)
    {
        menu_ingame_active_item++;
        if (menu_ingame_active_item > 2)
        {
            menu_ingame_active_item = 0;
        }
        menu_ingame_display_update = 1;
    }
    if (north)
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
            quoridor_state = STATE_QUORIDOR_TURN_INIT;
            break;
        }
        case 1:
        {
            // give up
            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;

            break;
        }
        case 2:
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_INIT;

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
        char title[] = "Auto paused ";
        char item_0[] = "Continue";
        char item_1[] = "Give up";
        char item_2[] = "Force move";

        board_menu_display_3_items(menu_ingame_active_item, title, item_0, item_1, item_2);
    }
    menu_ingame_display_update = 0;
}

void quoridor_menu_choose_gametype(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{

    if (north || west)
    {
        players_type[0]++;
        if (players_type[0] > 3)
        {
            players_type[0] = 1;
        }
        menu_display_update = 1;
    }

    if (south || east)
    {
        players_type[1]++;
        if (players_type[1] > 3)
        {
            players_type[1] = 1;
        }
        menu_display_update = 1;
    }

    if (enter)
    {
        quoridor_state = STATE_QUORIDOR_INIT;
        program_state = STATE_PROGRAM_GAME_PLAYING;
    }
    if (toggle)
    {
        program_state = STATE_PROGRAM_MENU_START_INIT;
    }

    if (menu_display_update)
    {
        char line_1[] = "       vs";

        char player_toNorth_type_text_0[] = "Invalid";
        char player_toNorth_type_text_1[] = "     Human      ";
        char player_toNorth_type_text_2[] = "    Auto Easy   ";
        char player_toNorth_type_text_3[] = "    Auto Hard   ";

        char player_toSouth_type_text_0[] = "Invalid";
        char player_toSouth_type_text_1[] = "     Human      ";
        char player_toSouth_type_text_2[] = "    Auto Easy   ";
        char player_toSouth_type_text_3[] = "    Auto Hard   ";

        char *player_toNorth_types_text[4] = {
            player_toNorth_type_text_0,
            player_toNorth_type_text_1,
            player_toNorth_type_text_2,
            player_toNorth_type_text_3};
        char *player_toSouth_types_text[4] = {
            player_toSouth_type_text_0,
            player_toSouth_type_text_1,
            player_toSouth_type_text_2,
            player_toSouth_type_text_3};

        char line_3[] = "Choose Game  [v,^]";

        board_menu_display_game_type(menu_active_item, line_3, player_toNorth_types_text[players_type[0]], line_1, player_toSouth_types_text[players_type[1]]);
    }
    menu_display_update = 0;
}

void quoridor_lode_load_game_history_as_one_string(char *moves_as_notation_one_string, uint8_t history_length)
{
    // provide as lode notation, will store as move_indeces
    moves_string_to_moves_indeces(moves_as_notation_one_string, loaded_game_history_moves_indeces);
    loaded_game_history_moves_indeces_length = history_length;
}

////////////////////////////////////////////

void quoridor_game_manager(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
{
    switch (quoridor_state)
    {
    case (STATE_QUORIDOR_INIT):
    {
        game_init(NULL, 0);
        //game_init(loaded_game_history_moves_indeces, loaded_game_history_moves_indeces_length);
        quoridor_state = STATE_QUORIDOR_TURN_INIT;
        break;
    }
    case (STATE_QUORIDOR_UNDO_TWICE_TURN):
    {
        // we want to undo the last HUMAN turn. which also undoes the computer's move
        undo_last_move();
        undo_last_move();

        quoridor_state = STATE_QUORIDOR_TURN_INIT;

        break;
    }
    case (STATE_QUORIDOR_UNDO_TURN):
    {
        undo_last_move();

        quoridor_state = STATE_QUORIDOR_TURN_INIT;

        break;
    }

    //////////////////// HUMAN
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
        hide_cursor();
        quoridor_state = STATE_QUORIDOR_TURN_FINISHED;

        break;
    }

    //////////////////// COMPUTER L1
    case (STATE_QUORIDOR_COMPUTER_L1_TURN_INIT):
    {
        quoridor_display_game_with_time_bar_percentage(100);
        computer_emphasize_pawn();

        autoplay_execute_next_move(1);

        quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN;

        break;
    }

    case (STATE_QUORIDOR_COMPUTER_L1_TURN):
    {
        if (autoplay_paused_get())
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED_INIT;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_FINISHED;
        }

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED_INIT):
    {
        quoridor_menu_ingame_computer_init();
        quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED):
    {
        quoridor_menu_ingame_computer(north, east, south, west, enter, toggle);
        break;
    }

    //////////////////// COMPUTER L2
    case (STATE_QUORIDOR_COMPUTER_L2_TURN_INIT):
    {
        quoridor_display_game_with_time_bar_percentage(100);

        computer_emphasize_pawn();

        if (!auto_play_single_step_mode || north || south || east)
        {
            uint8_t success;
            success = autoplay_execute_next_move(2);
            // if (north)
            // {
            //     success = autoplay_execute_next_move(1);
            // }
            // else
            // {
            //     success = autoplay_execute_next_move(2);
            // }
        }
        quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L2_TURN):
    {
        if (autoplay_paused_get())
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED_INIT;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_TURN_FINISHED;
        }

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED_INIT):
    {
        quoridor_menu_ingame_computer_init();
        quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED;

        break;
    }
    case (STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED):
    {
        quoridor_menu_ingame_computer(north, east, south, west, enter, toggle);
        break;
    }

    case (STATE_QUORIDOR_COMPUTER_TURN_FINISHED):
    {
        quoridor_state = STATE_QUORIDOR_TURN_FINISHED;
        board_set_move_type(autoplay_get_move_type());
        computer_deemphasize_pawn();
        display_game_state();
        break;
    }

    case (STATE_QUORIDOR_TURN_FINISHED):
    {
        if (get_winner_index() != NO_WINNER)
        {

            quoridor_state = STATE_QUORIDOR_FINISHED;
        }
        else
        {
            quoridor_state = STATE_QUORIDOR_TURN_INIT;
        }

        break;
    }

    ///////////////// admin
    case (STATE_QUORIDOR_TURN_INIT):
    {
        uint8_t player_type = players_type[get_playing_player()];

        switch (player_type)
        {
        case PLAYER_HUMAN:
        {
            quoridor_state = STATE_QUORIDOR_HUMAN_TURN_INIT;
            break;
        }
        case PLAYER_COMPUTER_L1:
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L1_TURN_INIT;
            break;
        }
        case PLAYER_COMPUTER_L2:
        {
            quoridor_state = STATE_QUORIDOR_COMPUTER_L2_TURN_INIT;
            break;
        }
        default:
        {
            raise_error(ERROR_NON_VALID_PLAYER);
            break;
        }
        }
        break;
    }
    case (STATE_QUORIDOR_FINISHED):
    {

        quoridor_display_game_with_time_bar_percentage(100);
        if (enter)
        {
            game_counter++;
            quoridor_state = STATE_QUORIDOR_INIT;
        }
        if (toggle)
        {

            program_state = STATE_PROGRAM_MENU_GAMETYPE_INIT;
        }

        break;
    }

    default:
    {
        raise_error(ERROR_INVALID_QUORIDOR_STATE);
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
            quoridor_menu_ingame_human_init(); // should be called only once at init todo
        }
        quoridor_menu_ingame_human(north, east, south, west, enter, toggle);
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

void hide_cursor()
{
    board_hide_cursor();
}
void computer_deemphasize_pawn()
{
    hide_cursor();
}
void computer_emphasize_pawn()
{
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