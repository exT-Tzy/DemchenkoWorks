#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "observer.h"
#include "elevator_state.h"
#include "passenger.h"

#include <vector>

class elevator :
    public subject 
{
private:
    
    elevator_state* _current_state;
    
    int _id;
    
    int _current_floor;
   
    int _max_capacity;
    
    int _next_move_time;
   
    int _door_open_time;

public:
   
    std::vector<bool> _floor_buttons;
    
    std::vector<passenger*> _passengers;

   
    int _idle_time;
   
    int _travel_time;
   
    int _floors_traveled;
   
    double _total_cargo_weight;
   
    double _max_load_achieved;
    
    int _overloads_count;

    elevator(
        int id,
        int floors,
        int maxCapacity);
    
    ~elevator();

    void set_state(
        elevator_state* state);
   
    void update(
        int currentTime);


   
    int get_id() const;
    
    int get_current_floor() const;
    
    int get_max_capacity() const;
    
    elevator_state* get_state() const;
   
    int get_next_move_time() const;
   
    int get_door_open_time() const;


    
    void set_current_floor(
        int floor);
    
    void set_next_move_time(
        int time);
   
    void set_door_open_time(
        int time);

   
    double get_current_weight() const;
   
    double get_load_factor() const;
    
    int get_movement_time() const;
    
    bool has_destination() const;
    
    int get_next_destination() const;

   
    void press_floor_button(
        int floor);
   
    void release_floor_button(
        int floor);
   
    bool can_accommodate(
        double weight) const;
    
    void add_passenger(
        passenger* passenger);
   
    void remove_passenger(
        passenger* passenger);
   
    std::vector<passenger*> handle_overload();
};

#endif // ELEVATOR_H