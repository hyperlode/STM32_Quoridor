#include "quoridor_autoplay.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"

uint8_t end_of_game;

uint8_t autoplay_move_type;

uint8_t autoplay_init()
{
    end_of_game = 0;
}

uint8_t autoplay_execute_next_move()
{
    if (end_of_game)
    {
        raise_error(111);
    }

    uint8_t best_move_index;
    best_move_index = auto_play_get_move_from_opening_database();
    autoplay_move_type = AUTOPLAY_MOVE_TYPE_OPENING_DATABASE;
    
    if (
        best_move_index == MOVE_INDEX_DUMMY ||
        !get_move_index_valid()) // database move cannot be trusted. Check for validity.
    {
        best_move_index = autoplay_get_best_next_move(get_playing_player());
        autoplay_move_type = AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_1;
    }

    make_move(best_move_index);
}

uint8_t autoplay_get_move_type()
{
    return autoplay_move_type;
}

uint8_t auto_play_get_move_from_opening_database()
{

    opening_next_move_suggestion();
}

uint8_t autoplay_get_best_next_move(uint8_t player)
{
    // from board state, get distances for all possible moves.

    // deltas are normalized for the current player. Meaning: higher delta is better. positive delta = winning.

    int8_t best_delta;

    best_delta = -127; // worst as init

    // draws
    // index of draws
    uint8_t equal_delta_moves[MOVE_INDEX_COUNT];
    uint8_t equal_delta_moves_count = 0;
    uint8_t number_of_pawn_moves = 0;
    uint8_t moves_to_investigate_count;

    // get best moves
    if (get_walls_placed(player) >= 10)
    {
        moves_to_investigate_count = MOVE_INDEX_FIRST_WALL;
    }
    else
    {
        moves_to_investigate_count = MOVE_INDEX_COUNT;
    }

    for (uint8_t move_index = 0; move_index < moves_to_investigate_count; move_index++)
    {
        int8_t delta;
        delta = get_delta_of_move_index_normalized(move_index);

        if (delta == (DELTA_WINNING_MOVE_MAGNITUDE))
        {
            end_of_game = 1;
        }

        if (delta != FAKE_DELTA_FOR_INVALID_MOVE)
        {
            if (delta > best_delta)
            {
                best_delta = delta;
                equal_delta_moves_count = 0;
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count++;

                if (move_index < MOVE_INDEX_FIRST_WALL)
                {
                    number_of_pawn_moves = 1;
                }
                else
                {
                    number_of_pawn_moves = 0;
                }
            }
            else if (delta == best_delta)
            {
                equal_delta_moves[equal_delta_moves_count] = move_index;
                equal_delta_moves_count++;
                if (move_index < MOVE_INDEX_FIRST_WALL)
                {
                    number_of_pawn_moves++;
                }
            }
        }
    }

    // choose a move
    uint8_t rand_index;

#define COMPUTER_PLAYER_AGRESSIVENESS 20
    uint8_t wall_aggressiveness = COMPUTER_PLAYER_AGRESSIVENESS;

    uint8_t prioritizes_wall_over_pawn;

    prioritizes_wall_over_pawn = rand() % 100 < wall_aggressiveness;

    if (number_of_pawn_moves > 0 && !prioritizes_wall_over_pawn)
    {
        // prioritize pawn moves over wall placement if equal delta.
        rand_index = rand() % number_of_pawn_moves;
    }
    else
    {
        rand_index = rand() % equal_delta_moves_count;
    }

    return equal_delta_moves[rand_index];
}
