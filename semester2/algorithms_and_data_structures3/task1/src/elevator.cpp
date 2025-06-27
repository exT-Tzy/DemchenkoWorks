#include "elevator.h"

#include <cmath>
#include <algorithm>

elevator::elevator(
    int id, 
    int floors, 
    int maxCapacity)
    : _id(id), _current_floor(1), _max_capacity(maxCapacity), _next_move_time(0), _door_open_time(0),
    _floor_buttons(floors + 1, false), _idle_time(0), _travel_time(0), _floors_traveled(0),
    _total_cargo_weight(0), _max_load_achieved(0), _overloads_count(0) 
{
    _current_state = new idle_closed_state();
}

elevator::~elevator() 
{
    delete _current_state;
}

void elevator::set_state(
    elevator_state* state) 
{
    delete _current_state;
    _current_state = state;
}

void elevator::update(
    int currentTime) 
{
    _current_state->handle(this, currentTime);
}

int elevator::get_id() const 
{ 
    return _id; 
}

int elevator::get_current_floor() const 
{ 
    return _current_floor; 
}

int elevator::get_max_capacity() const 
{ 
    return _max_capacity; 
}

elevator_state* elevator::get_state() const 
{ 
    return _current_state; 
}

int elevator::get_next_move_time() const
{
    return _next_move_time; 
}

int elevator::get_door_open_time() const 
{ 
    return _door_open_time;
}

void elevator::set_current_floor(
    int floor) 
{
    _current_floor = floor;
}

void elevator::set_next_move_time(
    int time) 
{ 
    _next_move_time = time; 
}

void elevator::set_door_open_time(
    int time) 
{
    _door_open_time = time;
}

double elevator::get_current_weight() const 
{
    double weight = 0;
    
    for (const passenger* p : _passengers)
    {
        weight += p->_weight;
    }
    
    return weight;
}

double elevator::get_load_factor() const 
{
    return get_current_weight() / _max_capacity;
}

int elevator::get_movement_time() const 
{
    double loadFactor = get_load_factor();
    
    return static_cast<int>(std::ceil(3 + 5 * loadFactor));
}

bool elevator::has_destination() const 
{
    for (bool pressed : _floor_buttons) 
    {
        if (pressed) 
        {
            return true;
        }
    }
   
    return false;
}

int elevator::get_next_destination() const 
{
    if (dynamic_cast<moving_up_state*>(_current_state) ||
        dynamic_cast<idle_closed_state*>(_current_state)) 
    {
        for (int i = _current_floor + 1; i < _floor_buttons.size(); i++) 
        {
            if (_floor_buttons[i]) 
            {
                return i;
            }
        }

        for (int i = _current_floor - 1; i >= 1; i--) 
        {
            if (_floor_buttons[i]) return i;
        }
    }
    else 
    {
        for (int i = _current_floor - 1; i >= 1; i--) 
        {
            if (_floor_buttons[i]) return i;
        }

        for (int i = _current_floor + 1; i < _floor_buttons.size(); i++) 
        {
            if (_floor_buttons[i]) return i;
        }
    }
   
    return -1;
}

void elevator::press_floor_button(
    int floor) 
{
    if (floor >= 1 && floor < _floor_buttons.size()) 
    {
        _floor_buttons[floor] = true;
    }
}

void elevator::release_floor_button(
    int floor) 
{
    if (floor >= 1 && floor < _floor_buttons.size()) 
    {
        _floor_buttons[floor] = false;
    }
}

bool elevator::can_accommodate(
    double weight) const 
{
    return get_current_weight() + weight <= _max_capacity;
}

void elevator::add_passenger(
    passenger* passenger) 
{
    _passengers.push_back(passenger);
    
    double currentWeight = get_current_weight();
    
    if (currentWeight > _max_load_achieved) 
    {
        _max_load_achieved = currentWeight;
    }
    
    _total_cargo_weight += passenger->_weight;
}

void elevator::remove_passenger(
    passenger* passenger) 
{
    _passengers.erase(std::remove(_passengers.begin(), _passengers.end(), passenger),
        _passengers.end());
}

std::vector<passenger*> elevator::handle_overload() 
{
    std::vector<passenger*> ejected;
    
    while (get_current_weight() > _max_capacity && !_passengers.empty()) 
    {
        passenger* lastBoarded = _passengers.back();
        
        _passengers.pop_back();
        ejected.push_back(lastBoarded);
        lastBoarded->_was_ejected_due_to_overweight = true;
        lastBoarded->_has_boarded = false;
        
        _overloads_count++;
        _total_cargo_weight -= lastBoarded->_weight;
    }
    
    return ejected;
}