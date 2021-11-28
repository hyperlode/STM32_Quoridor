#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "quoridor_config.h"
#include "quoridor_gameplay.h"
#include "quoridor_graph.h"
#include "quoridor_notation.h"

static uint8_t move_counter;
static Player players[2];

//char moves[MAX_MOVES_COUNT][2];
uint8_t moves_indeces[MAX_MOVES_COUNT];

void game_init(void)
{
    players[0].pawn.row += 0;
    players[0].pawn.col += 4;
    players[0].walls_placed = 0;

    players[1].pawn.row += 8;
    players[1].pawn.col += 4;
    players[1].walls_placed = 0;

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

void load_game_by_notation_one_string(moves_as_notation_one_string){
     moves_string_to_moves_indeces(moves_as_notation_one_string, moves_indeces);

}
// void replay_game_init(char *moves_as_notation_one_string)
// {
//     moves_string_to_moves_indeces(moves_as_notation_one_string, moves_indeces);
// }

void next_move_loaded_game(void)
{
    // char *move = moves[move_counter];
    // move_by_notation(move);

    uint8_t move_index = moves_indeces[move_counter];
    make_move(move_index);
};

void make_move(uint8_t move_index)
{
    uint8_t player = move_counter % 2;

    if (move_index == MOVE_INDEX_END_OF_GAME)
    {
        // end of game
        return;
    }
    else if (move_index < MOVE_INDEX_FIRST_WALL)
    {
        // pawn_move_by_notation(player, move_as_notation);
        make_move_pawn(player, move_index);
    }
    else
    {
        make_move_wall(player, move_index);
        // wall_set_by_notation(player, move_as_notation);
    }
    move_counter++;
}

void make_move_wall(uint8_t player, uint8_t move_index)
{
    uint8_t wall_row_col_dir[3];
    move_index_to_row_col_dir(move_index, wall_row_col_dir);
    set_wall_by_row_col(player, wall_row_col_dir[0], wall_row_col_dir[1], wall_row_col_dir[2]);
    graph_add_wall(wall_row_col_dir[0], wall_row_col_dir[1], wall_row_col_dir[2]);
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

void make_move_pawn(uint8_t player, uint8_t move_index)
{
    uint8_t delta_row;
    uint8_t delta_col;
    switch (move_index)
    {
    case 0:
    {
        delta_row = 1;
        delta_col = 0;
        break;
    }
    case 1:
    {
        delta_row = 0;
        delta_col = 1;
        break;
    }
    case 2:
    {
        delta_row = -1;
        delta_col = 0;
        break;
    }
    case 3:
    {
        delta_row = 0;
        delta_col = -1;
        break;
    }
    // JUMPS
    case 4:
    {
        delta_row = 2;
        delta_col = 0;
        break;
    }
    case 5:
    {
        delta_row = 0;
        delta_col = 2;
        break;
    }
    case 6:
    {
        delta_row = -2;
        delta_col = 0;
        break;
    }
    case 7:
    {
        delta_row = 0;
        delta_col = -2;
        break;
    }
    // DIAGONALS
    case 8:
    {
        delta_row = 1;
        delta_col = 1;
        break;
    }
    case 9:
    {
        delta_row = 1;
        delta_col = -1;
        break;
    }
    case 10:
    {
        delta_row = -1;
        delta_col = 1;
        break;
    }
    case 11:
    {
        delta_row = -1;
        delta_col = -1;
        break;
    }
    default:
    {
        // for single char moves, the second char will be something else...
        raise_error(ERROR_NOTATION);
        break;
    }
    }

    players[player].pawn.row += delta_row;
    players[player].pawn.col += delta_col;
};

void get_winning_distances(uint8_t *distances)
{
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

void raise_error(uint8_t error_code)
{
    while (1)
    {
    };
}
