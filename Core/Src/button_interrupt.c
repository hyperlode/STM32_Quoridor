#include <string.h>
#include <stdio.h>
#include "button_interrupt.h"

uint8_t buttons_interrupt_edge_up_debounced; // byte with all buttons
int debounce_next_edge_possible_millis[8];


GPIO_TypeDef*  buttons_port [8];
uint16_t buttons_pin [8];

void button_interupt_set_button(uint8_t button_index, GPIO_TypeDef* port, uint16_t pin){
    // 0, GPIOE, GPIO_PIN_15
    buttons_port[button_index] = port;
    buttons_pin[button_index] = pin;
}

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

// void button_interrupt_init(uint8_t button_index){
//     debounce_next_edge_possible_millis[button_index] = HAL_GetTick();
// }

void button_interrupt_set(uint8_t button_index)
{
    // debouncing should have been done by this point
    if (HAL_GPIO_ReadPin(buttons_port[button_index], buttons_pin[button_index])){
        byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 1);
    }else{
        byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 0);
    }
}
uint8_t button_interrupt_get_edge_up_single_readout(uint8_t button_index){
    // will reset after readout
    uint8_t value = byte_get_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index);
    byte_set_bit(&buttons_interrupt_edge_up_debounced, 1<<button_index, 0);
    return value;
}
