#include "command.h"
#include "elevator.h"
#include "elevator_system.h"

floor_button_command::floor_button_command(
    elevator* elevator, 
    int floor)
    : _elevator(elevator), _floor(floor) 
{}

void floor_button_command::execute() 
{
    _elevator->press_floor_button(_floor);
}

call_elevator_command::call_elevator_command(
    elevator_system* system, 
    int floor)
    : _system(system), _floor(floor) 
{}

void call_elevator_command::execute() 
{
    _system->call_elevator(_floor);
}