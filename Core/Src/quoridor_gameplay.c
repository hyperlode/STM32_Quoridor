#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"

static uint8_t move_counter;
static Player players[2];

char moves[MAX_MOVES_COUNT][2];

// char move_index_to_notation [140][2]= {"N ", "E ", "S ", "W ", "NN", "EE", "SS", "WW", "NE", "NW", "SE", "SW", "1a", "1b", "1c", "1d", "1e", "1f", "1g", "1h", "2a", "2b", "2c", "2d", "2e", "2f", "2g", "2h", "3a", "3b", "3c", "3d", "3e", "3f", "3g", "3h", "4a", "4b", "4c", "4d", "4e", "4f", "4g", "4h", "5a", "5b", "5c", "5d", "5e", "5f", "5g", "5h", "6a", "6b", "6c", "6d", "6e", "6f", "6g", "6h", "7a", "7b", "7c", "7d", "7e", "7f", "7g", "7h", "8a", "8b", "8c", "8d", "8e", "8f", "8g", "8h", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};

void game_init(void)
{
    players[0].pawn.row += 0;
    players[0].pawn.col += 4;
    players[0].walls_placed = 0;

    players[1].pawn.row += 8;
    players[1].pawn.col += 4;
    players[1].walls_placed = 0;

    for (uint8_t move = 0; move < MAX_MOVES_COUNT; move++)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            moves[move][i] = 0;
        }
    }

    for (uint8_t player = 0; player < 2; player++)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            players[player].walls[i].row = 0;
            players[player].walls[i].col = 0;
            players[player].walls[i].horizontal_else_vertical = 0;
        }
    }

    graph_init();

    move_counter = 0;
}

uint8_t get_move_counter()
{
    return move_counter;
}

// void get_move_index_to_notation(uint8_t index, char* notation){
//     notation = move_index_to_notation[index];
// }

// void get_notation_to_index(char* notation, uint8_t index){

// }

uint8_t index_from_row_col(uint8_t row, uint8_t col)
{
    return row * 9 + col;
}

void move_next_replay(void)
{
    char *move = moves[move_counter];
    move_by_notation(move);
};

void move_by_notation(char *move_as_notation)
{
    uint8_t player = move_counter % 2;

    if (move_as_notation[0] == 'X')
    {
        // end of game
        return;
    }
    else if (move_as_notation[0] == 'N' || move_as_notation[0] == 'E' || move_as_notation[0] == 'S' || move_as_notation[0] == 'W')
    {
        pawn_move_by_notation(player, move_as_notation);
    }
    else
    {
        wall_set_by_notation(player, move_as_notation);
    }
    move_counter++;
}

void moves_string_to_moves(char *moves_as_string)
{
    // string of moves (in Lode notation) to separate moves
    uint16_t counter = 0;

    uint8_t char_counter_in_move = 0;
    for (uint8_t i = 0; i < MOVES_STRING_LENGTH; i++)
    {
        char c = moves_as_string[i];

        if (c == ',')
        {
            // moves have a maximum of two characters. If only one charcter, fill the second character with a dummy. If foresee problems other wise. e.g. previous game was NW, now overwrite with N , but the W is still there...
            if (char_counter_in_move == 1)
            {
                moves[counter][char_counter_in_move] = ' ';
            }
            else if (char_counter_in_move == 0)
            {
                // ERROR EMPTY MOVE
                raise_error(ERROR_NOTATION);
            }
            else if (char_counter_in_move > 2)
            {
                // ERROR TOO MANY CHARS in move
                raise_error(ERROR_NOTATION);
            }
            char_counter_in_move = 0;
            counter++;
        }
        else if (c == '\0')
        {
            // if last move is single char move (e.g. N), fill up second position.
            if (char_counter_in_move == 1)
            {
                moves[counter][char_counter_in_move] = ' ';
            }
            moves[counter + 1][0] = 'X';
            moves[counter + 1][1] = ' ';
            return;
        }

        else
        {
            moves[counter][char_counter_in_move] = c;
            char_counter_in_move++;
        }
    }
}

void raise_error(uint8_t error_code)
{
    while (1)
    {
    };
}

void wall_set_by_notation(uint8_t player, char *wall_position)
{
    // direction always two chars.

    uint8_t first_char = 1;
    uint8_t col;
    uint8_t row;
    uint8_t horizontal_else_vertical;

    for (uint8_t i = 0; i < 2; i++)
    {
        char c = wall_position[i];
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
            raise_error(ERROR_NOTATION);
        }
    }
    set_wall_by_row_col(player, row, col, horizontal_else_vertical);
    graph_add_wall(row, col, horizontal_else_vertical);
    players[player].walls_placed++;
};

void set_wall_by_row_col(uint8_t player, uint8_t row, uint8_t col, uint8_t horizontal_else_vertical)
{
    players[player].walls[players[player].walls_placed].row = row;
    players[player].walls[players[player].walls_placed].col = col;
    players[player].walls[players[player].walls_placed].horizontal_else_vertical = horizontal_else_vertical;
}

void walls_get_all_positions(uint8_t *positions, uint8_t player)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        positions[i * 3 + 0] = players[player].walls[i].row;
        positions[i * 3 + 1] = players[player].walls[i].col;
        positions[i * 3 + 2] = players[player].walls[i].horizontal_else_vertical;
    }
}

void pawn_move_by_notation(uint8_t player, char *direction)
{
    // direction always two chars. second char should be ' ' if single char move   // (which is a bit risky. For single character moves, the second character is the divider character between moves....)

    // double character moves (NN NE ,...) are basically treated as two separated pawn moves.
    for (uint8_t i = 0; i < 2; i++)
    {
        switch (direction[i])
        {
        case 'N':
        {
            pawn_set_position(player, 0, 1);
            break;
        }
        case 'S':
        {
            pawn_set_position(player, 0, -1);
            break;
        }
        case 'E':
        {
            pawn_set_position(player, 1, 0);
            break;
        }
        case 'W':
        {
            pawn_set_position(player, -1, 0);
            break;
        }
        case ' ':
        {
            // second char in single char move
            break;
        }
        case 0:
        {
            // second char in single char move
            // illegal notation. (i.e. uncovered.)
            raise_error(ERROR_NOTATION);
            break;
        }
        default:
        {
            // for single char moves, the second char will be something else...
            raise_error(ERROR_NOTATION);
            break;
        }
        }
    }
};

void pawn_set_position(uint8_t player, int8_t delta_x, int8_t delta_y)
{
    // DO NOT CHECK validity here. In the text notation, double moves (NN , NE,... ) will come here twice. e.g. IF NN, the first north would be invalid as it hits the other player.
    players[player].pawn.row += delta_y;
    players[player].pawn.col += delta_x;
}


void get_winning_distances(uint8_t* distances){
    distances[0] = graph_get_distance_to_winning_square(0, pawn_get_position_as_node_index(0));
    distances[1] = graph_get_distance_to_winning_square(1, pawn_get_position_as_node_index(1));
}

uint8_t pawn_get_position_as_node_index(uint8_t player)
{
    // player = 0 or 1
    return graph_node_index_from_row_col(
        players[player].pawn.row,
        players[player].pawn.col);
}
void pawn_get_position_as_row_col(uint8_t *position, uint8_t player)
{
    // player = 0 or 1

    position[0] = players[player].pawn.row;
    position[1] = players[player].pawn.col;
    return position;
}
