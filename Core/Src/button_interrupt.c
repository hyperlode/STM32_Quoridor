#include <string.h>
#include <stdio.h>
#include "button_interrupt.h"

uint8_t buttons_interrupt_edge_up_debounced; // byte with all buttons
int debounce_next_edge_possible_millis[8];

void byte_set_bit(uint8_t *the_byte, uint8_t mask, uint8_t value)
{
    if (value)
    {
        *(the_byte) |= mask;
    }
    else
    {
        *(the_byte) &= ~mask;
    }
}

uint8_t byte_get_bit(uint8_t *the_byte, uint8_t mask)
{
    return (*(the_byte)&mask) > 0;
}

void button_interrupt_init(uint8_t button_index){
    debounce_next_edge_possible_millis[button_index] = HAL_GetTick();
}

void button_interrupt_set(uint8_t button_index)
{

    // if ( HAL_GetTick() > debounce_next_edge_possible_millis[button_index]){
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15)){
            debounce_next_edge_possible_millis[button_index] = HAL_GetTick() + BUTTON_INTERRUPT_DEBOUNCE_MILLIS;
            byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 1);
        }else{
            byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 0);
        }
    // }
}
uint8_t button_interrupt_get_edge_up_single_readout(uint8_t button_index){
    // will reset after readout
    uint8_t value = byte_get_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index);
    byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 0);
    return value;
}
