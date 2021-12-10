#include <string.h>
#include <stdio.h>

#include "quoridor_error.h"

static error_code;

void(*callback_function_pointer)(uint8_t);



uint8_t get_error_code(){
    return error_code;
}

void quoridor_error_set_callback(void(*functionPointer)(uint8_t)){

    callback_function_pointer = functionPointer;
}

void raise_error(uint8_t error_code_parameter)
{
    error_code = error_code_parameter;
    callback_function_pointer(error_code);
    while (1)
    {
    };
}
