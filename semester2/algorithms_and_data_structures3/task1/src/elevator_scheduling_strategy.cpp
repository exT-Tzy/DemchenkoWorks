#include "elevator_scheduling_strategy.h"
#include "elevator.h"
#include "elevator_state.h"

#include <iostream>
#include <climits>
#include <cmath>

int nearest_available_strategy::select_elevator(
    const std::vector<elevator*>& elevators,
    int floor)
{
    int bestElevator = -1;
    int bestScore = INT_MAX;

    std::cout << "Selecting elevator for floor " << floor << std::endl;

    for (int i = 0; i < elevators.size(); i++)
    {
        elevator* elev = elevators[i];
        int currentFloor = elev->get_current_floor();
        int distance = std::abs(currentFloor - floor);

        elevator_state* state = elev->get_state();
        bool isIdle = dynamic_cast<idle_closed_state*>(state) ||
            dynamic_cast<idle_open_state*>(state);
        bool isMovingUp = dynamic_cast<moving_up_state*>(state);
        bool isMovingDown = dynamic_cast<moving_down_state*>(state);

        int directionPenalty = 0;
        bool needsDirectionChange = false;

        if (!isIdle) 
        {
            int upwardDestinations = 0;
            int downwardDestinations = 0;

            for (passenger* p : elev->_passengers)
            {
                if (p->_target_floor > currentFloor) 
                {
                    upwardDestinations++;
                }
                else if (p->_target_floor < currentFloor) 
                {
                    downwardDestinations++;
                }
            }

            for (int f = currentFloor + 1; f < elev->_floor_buttons.size(); f++) 
            {
                if (elev->_floor_buttons[f]) 
                {
                    upwardDestinations++;
                }
            }
            
            for (int f = 1; f < currentFloor; f++) 
            {
                if (elev->_floor_buttons[f]) 
                {
                    downwardDestinations++;
                }
            }

            if (isMovingUp) 
            {
                if (floor < currentFloor && upwardDestinations == 0) 
                {
                    needsDirectionChange = false;
                }
                else if (floor < currentFloor && upwardDestinations > 0) 
                {
                    needsDirectionChange = true;
                    directionPenalty = 50; 
                }
            }
            else if (isMovingDown) 
            {
                if (floor > currentFloor && downwardDestinations == 0) 
                {
                    needsDirectionChange = false;
                }
                else if (floor > currentFloor && downwardDestinations > 0) 
                {
                    needsDirectionChange = true;
                    directionPenalty = 50;
                }
            }
        }

        int loadPenalty = static_cast<int>(elev->get_load_factor() * 20);
        int stopsPenalty = elev->_passengers.size() * 5;
        int idleBonus = isIdle ? -30 : 0;
        int score = distance + directionPenalty + loadPenalty + stopsPenalty + idleBonus;

        std::cout << "Elevator " << i << " at floor " << currentFloor
            << ", distance: " << distance
            << ", direction penalty: " << directionPenalty
            << ", load penalty: " << loadPenalty
            << ", stops penalty: " << stopsPenalty
            << ", idle bonus: " << idleBonus
            << ", total score: " << score
            << ", state: " << state->get_name() << std::endl;

        if (score < bestScore)
        {
            bestScore = score;
            bestElevator = i;
        }
    }

    std::cout << "Selected elevator " << bestElevator << " for floor " << floor
        << " with score " << bestScore << std::endl;

    return bestElevator;
}