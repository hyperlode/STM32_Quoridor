#include <string.h>
#include <stdio.h>
#include "buttons.h"




uint8_t buttons_state; // byte with all buttons
uint8_t buttons_debounced; // byte with all buttons
uint8_t buttons_edge_up_debounced; // byte with all buttons
uint8_t buttons_edge_down_debounced; // byte with all buttons
uint8_t buttons_edge_debounced; // byte with all buttons
uint8_t buttons_state_memory; // byte with all buttons

int debounce_init_times[8];

void button_set_state(uint8_t button, uint8_t state){
    byte_set_bit(&buttons_state, button, state);
}

void byte_set_bit(uint8_t* the_byte, uint8_t mask, uint8_t value){
if (value){
        *(the_byte) |= mask; 
    }else{
        *(the_byte) &= ~mask; 
    }
}

uint8_t  byte_get_bit(uint8_t* the_byte, uint8_t mask){
    return (*(the_byte) & mask) > 0;
}

void button_set_state_debounced(uint8_t button, uint8_t state){
    byte_set_bit(&buttons_debounced, button, state);
}

// void button_set_state_memory(uint8_t button, uint8_t state){
//     byte_set_bit(&buttons_state_memory, button, state);
// }

uint8_t button_get_state(uint8_t button){
    return byte_get_bit(&buttons_state, button);
}

// uint8_t button_get_state_memory(uint8_t button){
//     return byte_get_bit(&buttons_state_memory, button);
// }
uint8_t button_get_state_debounced(uint8_t button){
    return byte_get_bit(&buttons_debounced, button);
}

void button_activate_edge_up(uint8_t button){
    byte_set_bit(&buttons_edge_up_debounced, button, 1);
}
void button_activate_edge(uint8_t button){
    byte_set_bit(&buttons_edge_debounced, button, 1);
}
void button_activate_edge_down(uint8_t button){
    byte_set_bit(&buttons_edge_down_debounced, button, 1);
}

uint8_t button_get_edge_up_single_readout(uint8_t button){
    // will set to zero after read out.
    uint8_t value = byte_get_bit(&buttons_edge_up_debounced, button);
    byte_set_bit(&buttons_edge_up_debounced, button, 0);
    return value;
}
uint8_t button_get_edge_down_single_readout(uint8_t button){
    // will set to zero after read out.
    uint8_t value = byte_get_bit(&buttons_edge_down_debounced, button);
    byte_set_bit(&buttons_edge_down_debounced, button, 0);
    return value;
}
uint8_t button_get_edge_single_readout(uint8_t button){
    // will set to zero after read out.
    uint8_t value = byte_get_bit(&buttons_edge_debounced, button);
    byte_set_bit(&buttons_edge_debounced, button, 0);
    return value;
}


void buttons_refresh(){
    uint8_t state;
    uint8_t state_memory;
    uint8_t button;
    for (uint8_t i=0;i<8;i++){
        // check if button state is like it was before. 
        // if not, set edge.
        button = (0x01<<i);
        state = button_get_state(button);
        state_memory = button_get_state_debounced(button);
        
        if (state == state_memory){
            debounce_init_times[i] = HAL_GetTick();
        }else if ((HAL_GetTick() - debounce_init_times[i]) > BUTTON_DEBOUNCE_MILLIS ){
            // button_set_state_memory(button, state);
            button_set_state_debounced(button, state);
            if (state){
                button_activate_edge_up(button);
            }else{
                button_activate_edge_down(button);
            }
            button_activate_edge(button);
        }
        
    }
}