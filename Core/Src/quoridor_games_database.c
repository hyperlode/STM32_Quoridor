
#include <string.h>
#include <stdio.h>
#include "quoridor_games_database.h"
#include "quoridor_config.h"
#include "quoridor_error.h"

uint8_t move_counter; // keep track of position
uint8_t game_indeces_still_in_the_running[2][STORED_GAMES_COUNT];
uint16_t game_indeces_still_in_the_running_count[2];

uint8_t suggested_next_move_indeces[STORED_GAMES_COUNT];
uint16_t suggested_next_moves_count;

uint8_t game_in_opening_database;




uint8_t opening_initiate_next_move_handler()
{
    //  we need to keep track of all the games that are "still in the running " when a game is played, in order not to analyse everything all the time

    move_counter = 0;
    for (uint16_t i = 0; i < STORED_GAMES_COUNT; i++)
    {
        game_indeces_still_in_the_running[0][i] = i;
        game_indeces_still_in_the_running[1][i] = i;
    }

    game_indeces_still_in_the_running_count[0] = STORED_GAMES_COUNT;
    game_indeces_still_in_the_running_count[1] = STORED_GAMES_COUNT;

    suggested_next_moves_count = 0;
    game_in_opening_database = 1;
}

void opening_initiate_next_move_handler_from_game_moves(uint8_t* moves_indeces, uint8_t moves_indeces_length){
    opening_initiate_next_move_handler();
    for (uint8_t i=0;i<moves_indeces_length;i++){
        opening_update_game_move(moves_indeces[i], i+1);
    }
}

uint8_t opening_update_game_move(uint8_t last_played_move_index, uint8_t move_counter_in_game)
{
    // move_counter in game, is already thinking of the move that will be played next. if history = N,S,  move_counter in game = 2 (which is for the next move )
    if (move_counter + 1 != move_counter_in_game)
    {
        raise_error(ERROR_OPENING_MOVES_MOVE_COUNTER_MISMATCH);
    }

    // both player games need to be updated.
    for (uint8_t player_index = 0; player_index < 2; player_index++)
    {
        // check if still relevant
        if (game_indeces_still_in_the_running_count[player_index] <= 0)
        {

            // no games were in the running at the previous cycle. so, no use to check.
            if (game_indeces_still_in_the_running_count[0] == 0 && game_indeces_still_in_the_running_count[1] == 0)
            {
                game_in_opening_database = 0;
            }
        }
        else
        {
            // keep tally of number of games still in the running.

            uint16_t valid_games_count = 0;

            // go through all games to check if current move can be found
            for (uint16_t i = 0; i < game_indeces_still_in_the_running_count[player_index]; i++)
            {
                uint16_t game_index = game_indeces_still_in_the_running[player_index][i];

                if (allGames_to_north_won_and_lost[player_index][game_index][move_counter] == last_played_move_index)
                {
                    // game represents already played game.
                    game_indeces_still_in_the_running[player_index][valid_games_count] = game_index;
                    valid_games_count++;
                }
            }
            game_indeces_still_in_the_running_count[player_index] = valid_games_count;
        }
    }

    move_counter++;
}

uint8_t opening_next_move_suggestion()
{

    if (!game_in_opening_database)
    {
        return MOVE_INDEX_DUMMY;
    }

    suggested_next_moves_count = 0;
    uint8_t player_index = move_counter % 2;
    // save the next move as a suggestion

    // go through all games to check if current move can be found
    for (uint16_t i = 0; i < game_indeces_still_in_the_running_count[player_index]; i++)
    {
        uint16_t game_index = game_indeces_still_in_the_running[player_index][i];
        suggested_next_move_indeces[suggested_next_moves_count] = allGames_to_north_won_and_lost[player_index][game_index][move_counter];
        suggested_next_moves_count++;
    }

    // pick a move_index from the suggested move_indeces
    if (suggested_next_moves_count == 0)
    {
        return MOVE_INDEX_DUMMY;
    }
    else
    {
        uint16_t rand_index = rand() % suggested_next_moves_count;
        return suggested_next_move_indeces[rand_index];
    }
    return MOVE_INDEX_DUMMY;
}
