#ifndef BUTTON_H
#define BUTTON_H
_BEGIN_STD_C
#include <_ansi.h>


#define BUTTON_DEBOUNCE_MILLIS 20
void button_set_state(uint8_t button, uint8_t state);

static void byte_set_bit(uint8_t *the_byte, uint8_t mask, uint8_t value);

static uint8_t byte_get_bit(uint8_t *the_byte, uint8_t mask);

void button_set_state_debounced(uint8_t button, uint8_t state);

uint8_t button_get_state(uint8_t button);

uint8_t button_get_state_debounced(uint8_t button);

void button_activate_edge_up(uint8_t button);
void button_activate_edge(uint8_t button);
void button_activate_edge_down(uint8_t button);

uint8_t button_get_edge_up_single_readout(uint8_t button);
uint8_t button_get_edge_down_single_readout(uint8_t button);
uint8_t button_get_edge_single_readout(uint8_t button);

void buttons_refresh();


_END_STD_C
#endif //BUTTON