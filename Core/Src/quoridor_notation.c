
#include <string.h>
#include <stdio.h>
#include "quoridor_notation.h"
#include "quoridor_config.h"
char move_index_to_notation[140][2] = {"N ", "E ", "S ", "W ", "NN", "EE", "SS", "WW", "NE", "NW", "SE", "SW", "1a", "1b", "1c", "1d", "1e", "1f", "1g", "1h", "2a", "2b", "2c", "2d", "2e", "2f", "2g", "2h", "3a", "3b", "3c", "3d", "3e", "3f", "3g", "3h", "4a", "4b", "4c", "4d", "4e", "4f", "4g", "4h", "5a", "5b", "5c", "5d", "5e", "5f", "5g", "5h", "6a", "6b", "6c", "6d", "6e", "6f", "6g", "6h", "7a", "7b", "7c", "7d", "7e", "7f", "7g", "7h", "8a", "8b", "8c", "8d", "8e", "8f", "8g", "8h", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};

void moves_string_to_moves_indeces(char *moves_as_string, uint8_t *moves_as_indeces)
{
    // string of moves (in Lode notation) to separate moves
    uint16_t counter = 0;

    uint8_t char_counter_in_move = 0;

    char single_move[2];

    for (uint8_t i = 0; i < MOVES_STRING_LENGTH; i++)
    {
        char c = moves_as_string[i];

        if (c == ',')
        {
            // moves have a maximum of two characters. If only one charcter, fill the second character with a dummy. If foresee problems other wise. e.g. previous game was NW, now overwrite with N , but the W is still there...
            if (char_counter_in_move == 1)
            {
                single_move[char_counter_in_move] = ' ';
            }
            else if (char_counter_in_move == 0)
            {
                // ERROR EMPTY MOVE
                raise_error(ERROR_NOTATION_EMPTY_MOVE);
            }
            else if (char_counter_in_move > 2)
            {
                // ERROR TOO MANY CHARS in move
                raise_error(ERROR_NOTATION_TOO_MANY_CHARS);
            }
            moves_as_indeces[counter] = get_notation_to_move_index(single_move);
            char_counter_in_move = 0;
            counter++;
        }
        else if (c == '\0')
        {
            // if last move is single char move (e.g. N), fill up second position.
            if (char_counter_in_move == 1)
            {
                single_move[char_counter_in_move] = ' ';
            }
            moves_as_indeces[counter] = get_notation_to_move_index(single_move);

            // Add end of game identifier
            counter++;
            single_move[0] = 'X';
            single_move[1] = ' ';
            moves_as_indeces[counter] = get_notation_to_move_index(single_move);
            return;
        }

        else
        {
            single_move[char_counter_in_move] = c;
            char_counter_in_move++;
        }
    }
    return;
}

void get_move_index_to_notation(uint8_t index, char *notation)
{
    notation = move_index_to_notation[index];
}

uint8_t get_notation_to_move_index(char *move_as_notation)
{
    uint8_t index;
    if (move_as_notation[0] == 'X')
    {
        index = MOVE_INDEX_END_OF_GAME;
    }
    else if (move_as_notation[0] == 'N' || move_as_notation[0] == 'E' || move_as_notation[0] == 'S' || move_as_notation[0] == 'W')
    {
        index = notation_to_move_index_pawn(move_as_notation);
    }
    else
    {
        index = notation_to_move_index_wall(move_as_notation);
    }
    return index;
}

uint8_t notation_to_move_index_pawn(char *notation)
{
    uint8_t index;

    switch (notation[0])
    {
    case 'N':
    {
        index = 0;
        break;
    }
    case 'E':
    {
        index = 1;
        break;
    }
    case 'S':
    {
        index = 2;
        break;
    }
    case 'W':
    {
        index = 3;
        break;
    }

    default:
    {
        // for single char moves, the second char will be something else...
        raise_error(ERROR_NOTATION_INVALID_DIRECTION);
        break;
    }
    }

    if (notation[0] == notation[1])
    {
        index += 4;
    }
    else
    {
        switch (notation[1])
        {
        case 'E':
        {
            index += 8;
            break;
        }
        case 'W':
        {
            index += 9;
            break;
        }
        case ' ':
        {
            // not a double move
            break;
        }
        default:
        {
            // for single char moves, the second char will be something else...
            raise_error(ERROR_NOTATION_INVALID_DIRECTION_SECOND_CHAR);
            break;
        }
        }
    }
    return index;
}

uint8_t notation_to_move_index_wall(char *wall_position_as_notation)
{
    // direction always two chars.

    uint8_t first_char = 1;
    uint8_t col;
    uint8_t row;
    uint8_t horizontal_else_vertical;

    for (uint8_t i = 0; i < 2; i++)
    {
        char c = wall_position_as_notation[i];
        if (c >= 97 && c <= 104)
        {
            // a->h
            // N-S component
            if (first_char)
            {
                horizontal_else_vertical = 0;
            }
            col = c - 96;
            first_char = 0;
        }
        else if (c >= 49 && c <= 56)
        {
            // 1->9
            // E-W component

            if (first_char)
            {
                horizontal_else_vertical = 1;
            }
            row = c - 48;
            first_char = 0;
        }
        else
        {
            // INVALID ERROR
            raise_error(ERROR_NOTATION_WALL_NOTATION);
        }
    }

    uint8_t offset;
    if (horizontal_else_vertical)
    {
        offset = MOVE_INDEX_FIRST_HORIZONTAL_WALL;
    }
    else
    {
        offset = MOVE_INDEX_FIRST_VERTICAL_WALL;
    }
    return offset - 1 + 8 * (row - 1) + col;
}
