#include "passenger.h"

passenger::passenger(
    int id,
    double weight, 
    int sourceFloor,
    int targetFloor, 
    int appearanceTime)
    : _id(id), _weight(weight), _source_floor(sourceFloor), _target_floor(targetFloor),
    _appearance_time(appearanceTime), _boarding_time(-1), _total_travel_time(0),
    _was_ejected_due_to_overweight(false), _has_boarded(false), _has_reached_destination(false) 
{}