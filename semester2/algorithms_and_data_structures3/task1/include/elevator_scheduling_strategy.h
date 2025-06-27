#ifndef ELEVATOR_SCHEDULING_STRATEGY_H
#define ELEVATOR_SCHEDULING_STRATEGY_H

#include <vector>

class elevator;

class elevator_scheduling_strategy 
{
public:
    
    virtual ~elevator_scheduling_strategy() {}
    
    virtual int select_elevator(
        const std::vector<elevator*>& elevators,
        int floor) = 0;
};

class nearest_available_strategy : 
    public elevator_scheduling_strategy 
{
public:
   
    int select_elevator(
        const std::vector<elevator*>& elevators,
        int floor) override;
};

#endif // ELEVATOR_SCHEDULING_STRATEGY_H