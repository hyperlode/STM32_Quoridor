#ifndef BUTTON_INTERRUPT_H
#define BUTTON_H
_BEGIN_STD_C
#include <_ansi.h>

#define BUTTON_INTERRUPT_DEBOUNCE_MILLIS 50


static void byte_set_bit(uint8_t *the_byte, uint8_t mask, uint8_t value);
static uint8_t byte_get_bit(uint8_t *the_byte, uint8_t mask);

void button_interrupt_set(uint8_t button_index);
uint8_t button_interrupt_get_edge_up_single_readout(uint8_t button_index);

#endif

