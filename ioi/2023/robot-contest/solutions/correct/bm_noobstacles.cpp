#include "robot.h"

void program_pulibot()
{
    set_instruction({0, -2, 0, 0, -2}, 1, 'S');
    set_instruction({0, -2, 0, 0, 1}, 1, 'S');
    set_instruction({0, -2, -2, 0, 1}, 1, 'E');
    set_instruction({0, 1, -2, 0, 0}, 1, 'E');
    set_instruction({0, 1, -2, -2, 0}, 1, 'T');
}