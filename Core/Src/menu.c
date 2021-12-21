
// #include <_ansi.h>
// #include <string.h>
// #include <stdio.h>

// #include "menu.h"

// uint8_t menu_active_item;
// uint8_t menu_display_update;

// void quoridor_menu_gameplay(uint8_t north, uint8_t east, uint8_t south, uint8_t west, uint8_t enter, uint8_t toggle)
// {

//     if (north)
//     {
//         menu_active_item++;
//         if (menu_active_item > 2)
//         {
//             menu_active_item = 0;
//         }
//         menu_display_update = 1;
//     }
//     if (south)
//     {
//         if (menu_active_item == 0)
//         {
//             menu_active_item = 2;
//         }
//         else
//         {
//             menu_active_item--;
//         }
//         menu_display_update = 1;
//     }
//     if (enter)
//     {
//         switch (menu_active_item)
//         {
//         case 2:
//         {
//             quoridor_state = STATE_QUORIDOR_INIT;
//             program_state = STATE_PROGRAM_HUMAN_VS_COMPUTER;
//             break;
//         }
//         case 1:
//         {
//             quoridor_state = STATE_QUORIDOR_INIT;
//             quoridor_state = STATE_QUORIDOR_INIT;
//             program_state = STATE_PROGRAM_HUMAN_VS_HUMAN;
//             break;
//         }
//         case 0:
//         {
//             quoridor_state = STATE_QUORIDOR_INIT;
//             program_state = STATE_PROGRAM_COMPUTER_VS_COMPUTER;
//             break;
//         }
//         }
//     }

//     if (menu_display_update)
//     {

//         char title [] = "Pick your fight";
//         char item_0 []  = "Human vs Machine";
//         char item_1 [] = "Human battle";
//         char item_2 [] = "Machine battle";

//         menu_display_3_items(menu_active_item,title, item_0, item_1, item_2);
//     }
//     menu_display_update = 0;
// }
