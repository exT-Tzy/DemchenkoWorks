#ifndef PASSENGER_H
#define PASSENGER_H

#include <vector>

class passenger 
{
public:
    
    int _id;
    
    double _weight;
    
    int _source_floor;
    
    int _target_floor;
    
    int _appearance_time;

   
    int _boarding_time;
    
    int _total_travel_time;
   
    std::vector<int> _companion_passengers;
   
    bool _was_ejected_due_to_overweight;
   
    bool _has_boarded;
   
    bool _has_reached_destination;

   
    passenger(
        int id, 
        double weight,
        int sourceFloor,
        int targetFloor, 
        int appearanceTime);
};

#endif // PASSENGER_H