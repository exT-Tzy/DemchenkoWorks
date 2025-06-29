#include "elevator_state.h"
#include "elevator.h"

std::string idle_closed_state::get_name() const 
{
    return "Idle Closed";
}

void idle_closed_state::handle(
    elevator* elevator, 
    int currentTime) 
{
    if (elevator->has_destination()) 
    {
        int nextFloor = elevator->get_next_destination();
        
        if (nextFloor > elevator->get_current_floor()) 
        {
            elevator->set_state(new moving_up_state());
        }
        else if (nextFloor < elevator->get_current_floor()) 
        {
            elevator->set_state(new moving_down_state());
        }
        else 
        {
            elevator->set_state(new idle_open_state());
            elevator->set_door_open_time(currentTime + 5);
            elevator->release_floor_button(elevator->get_current_floor());
            elevator->notify_observers("stopped", elevator->get_id(), elevator->get_current_floor(), currentTime);
        }
        
        elevator->set_next_move_time(currentTime + elevator->get_movement_time());
    }
    else 
    {
        elevator->_idle_time++;
    }
}

std::string idle_open_state::get_name() const 
{
    return "Idle Open";
}

void idle_open_state::handle(elevator* elevator, int currentTime) 
{
    if (currentTime >= elevator->get_door_open_time()) 
    {
        elevator->set_state(new idle_closed_state());
        elevator->notify_observers("passenger_delivered", elevator->get_id(), elevator->get_current_floor(), currentTime);
    }
}

std::string moving_up_state::get_name() const
{
    return "Moving Up";
}

void moving_up_state::handle(
    elevator* elevator,
    int currentTime)
{
    if (currentTime >= elevator->get_next_move_time())
    {
        int next_floor = elevator->get_current_floor() + 1;

        if (next_floor >= elevator->_floor_buttons.size())
        {
            elevator->set_state(new idle_closed_state());
            return;
        }

        elevator->set_current_floor(next_floor);
        elevator->_floors_traveled++;

        if (elevator->_floor_buttons[elevator->get_current_floor()])
        {
            elevator->set_state(new idle_open_state());
            elevator->set_door_open_time(currentTime + 5);
            elevator->release_floor_button(elevator->get_current_floor());
            elevator->notify_observers("stopped", elevator->get_id(), elevator->get_current_floor(), currentTime);
        }
        else if (elevator->has_destination())
        {
            elevator->set_next_move_time(currentTime + elevator->get_movement_time());
        }
        else
        {
            elevator->set_state(new idle_closed_state());
        }
    }
    else
    {
        elevator->_travel_time++;
    }
}

std::string moving_down_state::get_name() const 
{
    return "Moving Down";
}

void moving_down_state::handle(
    elevator* elevator,
    int currentTime)
{
    if (currentTime >= elevator->get_next_move_time())
    {
        int next_floor = elevator->get_current_floor() - 1;

        if (next_floor < 1)
        {
            elevator->set_state(new idle_closed_state());
            return;
        }

        elevator->set_current_floor(next_floor);
        elevator->_floors_traveled++;

        if (elevator->_floor_buttons[elevator->get_current_floor()])
        {
            elevator->set_state(new idle_open_state());
            elevator->set_door_open_time(currentTime + 5);
            elevator->release_floor_button(elevator->get_current_floor());
            elevator->notify_observers("stopped", elevator->get_id(), elevator->get_current_floor(), currentTime);
        }
        else if (elevator->has_destination())
        {
            elevator->set_next_move_time(currentTime + elevator->get_movement_time());
        }
        else
        {
            elevator->set_state(new idle_closed_state());
        }
    }
    else
    {
        elevator->_travel_time++;
    }
}