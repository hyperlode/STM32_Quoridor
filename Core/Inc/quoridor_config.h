#ifndef QUORIDOR_CONFIG_H
#define QUORIDOR_CONFIG_H

#include <_ansi.h>

_BEGIN_STD_C


// from 0 to 100 if a pawn move or wall placement result in the same delta, what is prioritized?
#define COMPUTER_PLAYER_AGRESSIVENESS_EAGERNESS_TO_PLAY_WALL 20

#define MOVE_INDEX_VALID 0
#define MOVE_INDEX_INVALID 1

#define BUTTON_NORTH 1
#define BUTTON_EAST 2 
#define BUTTON_SOUTH 4
#define BUTTON_WEST 8 
#define BUTTON_ENTER 16
#define BUTTON_TOGGLE 32

#define CURSOR_NOT_SHOWN 255

#define WALL_NOT_PLACED 255
#define PLAYER_MAX_WALL_COUNT 10

#define PLAYER_TO_NORTH 0
#define PLAYER_TO_SOUTH 1

#define PLAYER_UNDEFINED 0
#define PLAYER_HUMAN 1
#define PLAYER_COMPUTER_L1 2
#define PLAYER_COMPUTER_L2 3

// move_index
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define NORTH_NORTH 4
#define EAST_EAST 5
#define SOUTH_SOUTH 6
#define WEST_WEST 7
#define NORTH_EAST 8 
#define NORTH_WEST 9 
#define SOUTH_EAST 10
#define SOUTH_WEST 11

#define MAX_MOVES_COUNT 100
#define MOVES_STRING_LENGTH 100*3  // each move is max 2 chars and a divider

#define MOVE_INDEX_FIRST_WALL 12
#define MOVE_INDEX_FIRST_HORIZONTAL_WALL MOVE_INDEX_FIRST_WALL
#define MOVE_INDEX_FIRST_VERTICAL_WALL 76
#define MOVE_INDEX_COUNT 140
#define MOVE_INDEX_END_OF_GAME 255
#define MOVE_INDEX_DUMMY 254
#define FAKE_DELTA_FOR_INVALID_MOVE 127
#define MOVE_INDEX_VALID_MOVE_INFINITY 255
#define PAWN_TARGET_NOT_REACHABLE FAKE_DELTA_FOR_INVALID_MOVE

#define STATE_QUORIDOR_INIT 0
#define STATE_QUORIDOR_TURN_INIT 18   
#define STATE_QUORIDOR_TURN_FINISHED 17

#define STATE_QUORIDOR_HUMAN_TURN_INIT 1
// #define STATE_QUORIDOR_COMPUTER_TURN_INIT 2
#define STATE_QUORIDOR_HUMAN_TURN 3
// #define STATE_QUORIDOR_COMPUTER_TURN 4
#define STATE_QUORIDOR_FINISHED 5
#define STATE_QUORIDOR_HUMAN_TURN_END 6
#define STATE_QUORIDOR_UNDO_TURN 7

#define STATE_QUORIDOR_COMPUTER_L1_TURN_INIT 8
#define STATE_QUORIDOR_COMPUTER_L1_TURN 9
#define STATE_QUORIDOR_COMPUTER_L1_TURN_FINISHED 10

#define STATE_QUORIDOR_COMPUTER_L2_TURN_INIT 11
#define STATE_QUORIDOR_COMPUTER_L2_TURN 12
#define STATE_QUORIDOR_COMPUTER_L2_TURN_FINISHED 13

#define STATE_QUORIDOR_COMPUTER_TURN_INIT 14
#define STATE_QUORIDOR_COMPUTER_TURN 15
#define STATE_QUORIDOR_COMPUTER_TURN_FINISHED 16

#define STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED_INIT 19   
#define STATE_QUORIDOR_COMPUTER_L2_TURN_PAUSED 20
#define STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED_INIT 21   
#define STATE_QUORIDOR_COMPUTER_L1_TURN_PAUSED 22
#define STATE_QUORIDOR_UNDO_TWICE_TURN 23

#define RECORD_MOVES_HISTORY_LENGTH 100

#define STATE_PROGRAM_MENU_START_INIT 0 
#define STATE_PROGRAM_MENU_GAMETYPE 1
#define STATE_PROGRAM_MENU_START 2
#define STATE_PROGRAM_MENU_GAMETYPE_INIT 3
#define STATE_PROGRAM_GAME_PLAYING 4
#define STATE_PROGRAM_FLASH_ANIMATION_INIT 5
#define STATE_PROGRAM_FLASH_ANIMATION 6

#define DELTA_MAX_WINNING_MOVE_MAGNITUDE 125
#define NO_WINNER 66

#define ERROR_NO_ERROR 0 
#define ERROR_DELETE_EDGE 1
#define ERROR_NOT_YET_IMPLEMENTED 2
#define ERROR_NOTATION_INVALID_DIRECTION_SECOND_CHAR 3
#define ERROR_NOTATION_INVALID_DIRECTION 4
#define ERROR_NOTATION_WALL_NOTATION 5
#define ERROR_NOT_A_WALL 6
#define ERROR_NOTATION_EMPTY_MOVE 7
#define ERROR_NOTATION_TOO_MANY_CHARS 8
#define NEIGHBOURS_OUT_OF_BOUNDS 9
#define ERROR_NOT_AN_ORTHOGONAL_DIRECTION 10
#define ERROR_NOTATION_NOT_A_PAWN_MOVE 11
#define ERROR_NOTATION_NOT_AN_ORTHO_PAWN_MOVE 12
#define ERROR_NOTATION_NOT_AN_ORTHO_PAWN_MOVE_JUMP 13
#define ERROR_NOTATION_NOT_A_DIAGONAL_PAWN_MOVE 14
#define ERROR_NOT_A_VALID_MOVE_ON_THIS_BOARD 15
#define ERROR_FAKE_NODE 16
#define ERROR_NO_MORE_WALLS_LEFT 17
#define ERROR_OPENING_MOVES_MOVE_COUNTER_MISMATCH 18
#define ERROR_NOTATION 19 
#define ERROR_INCONSISTENCY_IN_INVALID_MOVE_DUE_TO_NO_EXIT 20
#define ERROR_NOTATION_NOT_A_DIAGONAL_PAWN_MOVE 21
#define ERROR_NOTATION_NOT_A_PAWN_MOVE_FOR_DESTINATION_NODE 22
#define ERROR_NOTATION_NOT_A_PAWN_MOVE_FOR_CHECKING 23
#define ERROR_MOVING_WHILE_END_OF_GAME 24
#define ERROR_NEIGHBOUR_ALREADY_EXISTING 25
#define ERROR_NON_VALID_PLAYER 26
#define ERROR_INVALID_QUORIDOR_STATE 26


#define BOARD_MENU_X_OFFSET 66
#define BOARD_MENU_Y_OFFSET 2

#define BOARD_X_MAX 127
#define BOARD_Y_MAX 63
#define BOARD_CELL_SPACING 7
#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 0

#define WALL_CELL_OFFSET_Y 2
#define WALL_CELL_OFFSET_X 2

#define WALL_CELL_OFFSET_Y 0
#define WALL_CELL_OFFSET_X 0
#define WALL_THICKNESS 3

#define PAWN_CELL_OFFSET_X 2
#define PAWN_CELL_OFFSET_Y 2
#define PAWN_WIDTH 3
#define PAWN_HEIGHT 3

#define ANIMATION_STEP_MILLIS 10
#define ANIMATION_LENGTH_STEPS 150

#define ANIMATION_START_GRID_DISAPPEAR_STEP 30
// #define ANIMATION_START_GRID_DISAPPEAR_STEP 20
#define ANIMATION_START_MOVE_PAWNS_STEP (ANIMATION_START_GRID_DISAPPEAR_STEP + 32)
#define ANIMATION_START_SLIDE_PAWNS_STEP (ANIMATION_START_MOVE_PAWNS_STEP + 54)
#define ANIMATION_START_DISPLAY_CREDITS (ANIMATION_START_SLIDE_PAWNS_STEP + 12)

    


#define BOARD_MENU_COL_ITEMS_WIDTH 30
#define BOARD_MENU_ROW_TITLE_HEIGHT 12
#define BOARD_MENU_ROW_HEIGHT 13
#define BOARD_MENU_COL_WIDTH 16
#define BOARD_MENU_COL_ALIGN_CENTER 3

#define AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_2 2
#define AUTOPLAY_MOVE_TYPE_CALCULATED_LEVEL_1 1
#define AUTOPLAY_MOVE_TYPE_OPENING_DATABASE 0
struct Pawn{
    uint8_t row;
    uint8_t col;
    uint8_t fill;    
};

struct Wall{
    uint8_t row;
    uint8_t col;
    uint8_t horizontal_else_vertical;
};

struct Player{
    struct Pawn pawn;
    uint8_t distance_to_win;
    struct Wall walls [10];
    uint8_t walls_placed;
};

typedef struct Player Player;



_END_STD_C

#endif // QUORIDOR_CONFIG_H