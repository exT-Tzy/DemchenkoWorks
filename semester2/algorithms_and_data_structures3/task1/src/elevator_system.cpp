#include "elevator_system.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

elevator_system::elevator_system(
    int floors,
    const std::vector<int>& elevatorCapacities)
    : _floors(floors), _current_time(0), _waiting_passengers(floors + 1),
    _call_buttons(floors + 1, false), _attempted_elevators_per_floor(floors + 1),
    _floor_attempt_count(floors + 1, 0)
{
    _strategy = new nearest_available_strategy();

    for (int i = 0; i < elevatorCapacities.size(); i++)
    {
        elevator* elevator = new typename elevator::elevator(i, floors, elevatorCapacities[i]);

        elevator->add_observer(this);
        _elevators.push_back(elevator);
    }
}

elevator_system::~elevator_system()
{
    for (elevator* elevator : _elevators)
    {
        delete elevator;
    }

    for (passenger* passenger : _all_passengers)
    {
        delete passenger;
    }

    delete _strategy;
}

void elevator_system::add_passenger(
    passenger* passenger)
{
    _all_passengers.push_back(passenger);
}

void elevator_system::call_elevator(
    int floor)
{
    if (floor >= 1 && floor <= _floors)
    {
        if (_call_buttons[floor])
        {
            return;
        }

        _call_buttons[floor] = true;

        int selectedElevator = _strategy->select_elevator(_elevators, floor);

        if (selectedElevator >= 0 && should_try_elevator(selectedElevator, floor))
        {
            _elevators[selectedElevator]->press_floor_button(floor);
            mark_elevator_attempted(selectedElevator, floor);
            std::cout << "Elevator " << selectedElevator << " assigned to floor " << floor << std::endl;
        }
        else
        {
            std::cout << "No suitable elevator available for floor " << floor << std::endl;
        }
    }
}

bool elevator_system::should_try_elevator(int elevatorId, int floor)
{
    return _attempted_elevators_per_floor[floor].find(elevatorId) == _attempted_elevators_per_floor[floor].end();
}

void elevator_system::mark_elevator_attempted(int elevatorId, int floor)
{
    _attempted_elevators_per_floor[floor].insert(elevatorId);
    _floor_attempt_count[floor]++;
}

void elevator_system::reset_floor_attempts(int floor)
{
    _attempted_elevators_per_floor[floor].clear();
    _floor_attempt_count[floor] = 0;
}

int elevator_system::find_capable_elevator(const std::vector<passenger*>& passengers, int floor)
{
    if (passengers.empty()) return -1;

    double totalWeight = 0;
    for (passenger* p : passengers)
    {
        totalWeight += p->_weight;
    }

    int bestElevator = -1;
    int bestDistance = INT_MAX;

    for (int i = 0; i < _elevators.size(); i++)
    {
        elevator* elev = _elevators[i];

        if (elev->get_max_capacity() >= totalWeight)
        {
            if (should_try_elevator(i, floor))
            {
                int distance = std::abs(elev->get_current_floor() - floor);

                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    bestElevator = i;
                }
            }
        }
    }

    return bestElevator;
}

std::string elevator_system::get_elevator_passenger_direction(elevator* elev)
{
    if (elev->_passengers.empty())
    {
        return "none";
    }

    int currentFloor = elev->get_current_floor();
    bool hasUpward = false;
    bool hasDownward = false;

    for (passenger* p : elev->_passengers)
    {
        if (p->_target_floor > currentFloor)
        {
            hasUpward = true;
        }
        else if (p->_target_floor < currentFloor)
        {
            hasDownward = true;
        }
    }

    if (hasUpward && !hasDownward)
    {
        return "up";
    }
    else if (hasDownward && !hasUpward)
    {
        return "down";
    }
    else
    {
        return "mixed";
    }
}

bool elevator_system::is_direction_compatible(elevator* elev, passenger* passenger)
{
    int currentFloor = elev->get_current_floor();
    int sourceFloor = passenger->_source_floor;
    int targetFloor = passenger->_target_floor;

    std::string passengerDirection;
    if (targetFloor > sourceFloor)
    {
        passengerDirection = "up";
    }
    else
    {
        passengerDirection = "down";
    }

    if (dynamic_cast<idle_closed_state*>(elev->get_state()) ||
        dynamic_cast<idle_open_state*>(elev->get_state()))
    {
        return true;
    }

    std::string elevatorPassengerDirection = get_elevator_passenger_direction(elev);

    if (elevatorPassengerDirection == "none")
    {
        return true;
    }

    if (elevatorPassengerDirection == passengerDirection)
    {
        return true;
    }

    if (elevatorPassengerDirection == "mixed")
    {
        if (dynamic_cast<moving_up_state*>(elev->get_state()))
        {
            return passengerDirection == "up";
        }
        else if (dynamic_cast<moving_down_state*>(elev->get_state()))
        {
            return passengerDirection == "down";
        }
    }

    return false;
}

void elevator_system::update(
    const std::string& event,
    int elevatorId,
    int floor,
    int time)
{
    if (event == "stopped" && _call_buttons[floor])
    {
        _call_buttons[floor] = false;

        elevator* elev = _elevators[elevatorId];
        auto& waiting = _waiting_passengers[floor];

        std::cout << "Elevator " << elevatorId << " stopped at floor " << floor
            << " with " << waiting.size() << " waiting passengers" << std::endl;

        std::vector<passenger*> compatible_passengers;
        std::vector<passenger*> incompatible_passengers;

        for (passenger* passenger : waiting)
        {
            if (is_direction_compatible(elev, passenger))
            {
                compatible_passengers.push_back(passenger);
                std::cout << "Passenger " << passenger->_id << " is compatible with elevator direction" << std::endl;
            }
            else
            {
                incompatible_passengers.push_back(passenger);
                std::cout << "Passenger " << passenger->_id << " skipped - incompatible direction" << std::endl;
            }
        }

        waiting.clear();

        std::vector<passenger*> passengers_who_couldnt_board;

        std::vector<int> current_passengers_ids;
        for (passenger* existing_passenger : elev->_passengers)
        {
            current_passengers_ids.push_back(existing_passenger->_id);
        }

        for (passenger* passenger : compatible_passengers)
        {
            if (elev->can_accommodate(passenger->_weight))
            {
                elev->add_passenger(passenger);
                elev->press_floor_button(passenger->_target_floor);
                passenger->_has_boarded = true;
                passenger->_boarding_time = time;

                for (int existing_id : current_passengers_ids)
                {
                    passenger->_companion_passengers.push_back(existing_id);
                }

                for (typename passenger::passenger* other_new : compatible_passengers)
                {
                    if (other_new != passenger && other_new->_has_boarded)
                    {
                        passenger->_companion_passengers.push_back(other_new->_id);
                        other_new->_companion_passengers.push_back(passenger->_id);
                    }
                }

                for (typename passenger::passenger* existing_passenger : elev->_passengers)
                {
                    if (existing_passenger != passenger)
                    {
                        existing_passenger->_companion_passengers.push_back(passenger->_id);
                    }
                }

                std::cout << "Passenger " << passenger->_id << " boarded elevator " << elevatorId << std::endl;
            }
            else
            {
                passenger->_was_ejected_due_to_overweight = true;
                passengers_who_couldnt_board.push_back(passenger);
                std::cout << "Passenger " << passenger->_id << " ejected due to overweight - cannot board" << std::endl;
            }
        }

        std::vector<passenger*> ejected = elev->handle_overload();

        for (passenger* ejectedPassenger : ejected)
        {
            ejectedPassenger->_was_ejected_due_to_overweight = true;
            ejectedPassenger->_has_boarded = false;

            for (passenger* remaining : elev->_passengers)
            {
                remaining->_companion_passengers.erase(
                    std::remove(remaining->_companion_passengers.begin(),
                        remaining->_companion_passengers.end(),
                        ejectedPassenger->_id),
                    remaining->_companion_passengers.end());
            }

            ejectedPassenger->_companion_passengers.clear();

            std::cout << "Passenger " << ejectedPassenger->_id << " ejected due to overweight" << std::endl;
        }

        std::vector<passenger*> passengers_needing_new_elevator;

        for (passenger* passenger : passengers_who_couldnt_board)
        {
            passengers_needing_new_elevator.push_back(passenger);
        }

        for (passenger* ejectedPassenger : ejected)
        {
            passengers_needing_new_elevator.push_back(ejectedPassenger);
        }

        for (passenger* passenger : incompatible_passengers)
        {
            passengers_needing_new_elevator.push_back(passenger);
        }

        if (!passengers_needing_new_elevator.empty())
        {
            for (passenger* passenger : passengers_needing_new_elevator)
            {
                waiting.push_back(passenger);
            }

            int capableElevator = find_capable_elevator(passengers_needing_new_elevator, floor);

            if (capableElevator != -1)
            {
                std::cout << "Found capable elevator " << capableElevator
                    << " for " << passengers_needing_new_elevator.size()
                    << " passengers at floor " << floor << std::endl;
                force_call_elevator(floor);
            }
            else if (_floor_attempt_count[floor] < MAX_ATTEMPTS_PER_FLOOR)
            {
                std::cout << "Retrying with different elevator for floor " << floor
                    << " (attempt " << _floor_attempt_count[floor] + 1 << ")" << std::endl;
                force_call_elevator(floor);
            }
            else
            {
                std::cout << "WARNING: Maximum attempts reached for floor " << floor
                    << ". " << passengers_needing_new_elevator.size()
                    << " passengers cannot be served by any elevator!" << std::endl;

                for (passenger* passenger : passengers_needing_new_elevator)
                {
                    passenger->_has_reached_destination = false;
                    std::cout << "Passenger " << passenger->_id
                        << " cannot be served - no suitable elevator available" << std::endl;
                }

                reset_floor_attempts(floor);
            }
        }
        else
        {
            reset_floor_attempts(floor);
        }
    }

    if (event == "passenger_delivered")
    {
        elevator* elev = _elevators[elevatorId];

        for (auto it = elev->_passengers.begin(); it != elev->_passengers.end();)
        {
            passenger* passenger = *it;

            if (passenger->_target_floor == floor)
            {
                passenger->_has_reached_destination = true;
                passenger->_total_travel_time = time - passenger->_boarding_time;

                for (typename passenger::passenger* remaining : elev->_passengers)
                {
                    if (remaining != passenger)
                    {
                        remaining->_companion_passengers.erase(
                            std::remove(remaining->_companion_passengers.begin(),
                                remaining->_companion_passengers.end(),
                                passenger->_id),
                            remaining->_companion_passengers.end());
                    }
                }

                std::cout << "Passenger " << passenger->_id << " delivered to floor " << floor << std::endl;
                it = elev->_passengers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void elevator_system::call_elevator_for_specific_passenger(int floor, passenger* passenger)
{
    std::string passengerDirection;
    if (passenger->_target_floor > passenger->_source_floor)
    {
        passengerDirection = "up";
    }
    else
    {
        passengerDirection = "down";
    }

    int bestElevator = -1;
    int minDistance = INT_MAX;

    std::cout << "Looking for suitable elevator for passenger " << passenger->_id
        << " going " << passengerDirection << " from floor " << floor << std::endl;

    for (int i = 0; i < _elevators.size(); i++)
    {
        elevator* elev = _elevators[i];

        if (!elev->can_accommodate(passenger->_weight))
        {
            continue;
        }

        if (!should_try_elevator(i, floor))
        {
            continue;
        }

        bool canServe = false;
        int distance = std::abs(elev->get_current_floor() - floor);

        if (dynamic_cast<idle_closed_state*>(elev->get_state()) ||
            dynamic_cast<idle_open_state*>(elev->get_state()))
        {
            canServe = true;
        }
        else if (elev->_passengers.empty())
        {
            canServe = true;
        }
        else
        {
            std::string elevatorPassengerDirection = get_elevator_passenger_direction(elev);

            if (elevatorPassengerDirection == passengerDirection ||
                elevatorPassengerDirection == "mixed")
            {
                if (dynamic_cast<moving_up_state*>(elev->get_state()) && passengerDirection == "up")
                {
                    canServe = (floor >= elev->get_current_floor());
                }
                else if (dynamic_cast<moving_down_state*>(elev->get_state()) && passengerDirection == "down")
                {
                    canServe = (floor <= elev->get_current_floor());
                }
            }
        }

        if (canServe && distance < minDistance)
        {
            minDistance = distance;
            bestElevator = i;
        }
    }

    if (bestElevator != -1)
    {
        _elevators[bestElevator]->press_floor_button(floor);
        mark_elevator_attempted(bestElevator, floor);
        std::cout << "Elevator " << bestElevator << " specifically assigned to passenger "
            << passenger->_id << " at floor " << floor << std::endl;
    }
    else
    {
        call_elevator(floor);
        std::cout << "No specific elevator found, calling nearest for passenger "
            << passenger->_id << std::endl;
    }
}

void elevator_system::simulate(
    int maxTime)
{
    std::cout << "Starting simulation with " << _elevators.size() << " elevators" << std::endl;

    for (_current_time = 0; _current_time <= maxTime; _current_time++)
    {
        for (passenger* passenger : _all_passengers)
        {
            if (passenger->_appearance_time == _current_time)
            {
                _waiting_passengers[passenger->_source_floor].push_back(passenger);
                std::cout << "Time " << _current_time << ": Passenger " << passenger->_id
                    << " appeared on floor " << passenger->_source_floor << std::endl;

                call_elevator(passenger->_source_floor);
            }
        }

        for (elevator* elev : _elevators)
        {
            elev->update(_current_time);
        }

        bool allPassengersDelivered = true;
        bool hasUnservablePassengers = false;

        for (passenger* passenger : _all_passengers)
        {
            if (!passenger->_has_reached_destination)
            {
                allPassengersDelivered = false;

                bool canBeServed = false;
                for (elevator* elev : _elevators)
                {
                    if (elev->get_max_capacity() >= passenger->_weight)
                    {
                        canBeServed = true;
                        break;
                    }
                }

                if (!canBeServed)
                {
                    hasUnservablePassengers = true;
                    std::cout << "WARNING: Passenger " << passenger->_id
                        << " (weight: " << passenger->_weight
                        << ") cannot be served by any elevator!" << std::endl;
                }
            }
        }

        if (allPassengersDelivered)
        {
            std::cout << "All passengers delivered at time " << _current_time << std::endl;
            break;
        }

        if (hasUnservablePassengers)
        {
            std::cout << "Some passengers cannot be served due to weight limitations." << std::endl;
        }
    }
}

void elevator_system::generate_reports(
    const std::string& passengerReportFile,
    const std::string& elevatorReportFile)
{
    std::ofstream passengerFile(passengerReportFile);

    for (passenger* passenger : _all_passengers)
    {
        passengerFile << "Passenger ID: " << passenger->_id << std::endl;
        passengerFile << "Appearance time: " << format_time(passenger->_appearance_time) << std::endl;
        passengerFile << "Source floor: " << passenger->_source_floor << std::endl;
        passengerFile << "Target floor: " << passenger->_target_floor << std::endl;
        passengerFile << "Weight: " << passenger->_weight << " kg" << std::endl;
        passengerFile << "Boarding time: " << (passenger->_boarding_time >= 0 ? format_time(passenger->_boarding_time) : "Not boarded") << std::endl;
        passengerFile << "Total travel time: " << passenger->_total_travel_time << " seconds" << std::endl;
        passengerFile << "Companions: ";
        for (int companionId : passenger->_companion_passengers)
        {
            passengerFile << companionId << " ";
        }
        if (passenger->_companion_passengers.empty())
        {
            passengerFile << "None";
        }
        passengerFile << std::endl;
        passengerFile << "Ejected due to overweight: " << (passenger->_was_ejected_due_to_overweight ? "Yes" : "No") << std::endl;
        passengerFile << "Reached destination: " << (passenger->_has_reached_destination ? "Yes" : "No") << std::endl;
        passengerFile << std::endl;
    }

    passengerFile.close();

    std::ofstream elevatorFile(elevatorReportFile);

    for (elevator* elevator : _elevators)
    {
        elevatorFile << "Elevator ID: " << elevator->get_id() << std::endl;
        elevatorFile << "Max capacity: " << elevator->get_max_capacity() << " kg" << std::endl;
        elevatorFile << "Idle time: " << elevator->_idle_time << " seconds" << std::endl;
        elevatorFile << "Travel time: " << elevator->_travel_time << " seconds" << std::endl;
        elevatorFile << "Floors traveled: " << elevator->_floors_traveled << std::endl;
        elevatorFile << "Total cargo weight: " << std::fixed << std::setprecision(2) << elevator->_total_cargo_weight << " kg" << std::endl;
        elevatorFile << "Maximum load achieved: " << std::fixed << std::setprecision(2) << elevator->_max_load_achieved << " kg" << std::endl;
        elevatorFile << "Overload count: " << elevator->_overloads_count << std::endl;
        elevatorFile << std::endl;
    }

    elevatorFile.close();
}

std::string elevator_system::format_time(
    int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;
    std::ostringstream oss;

    oss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << secs;

    return oss.str();
}

void elevator_system::force_call_elevator(int floor)
{
    if (floor >= 1 && floor <= _floors)
    {
        int selectedElevator = -1;
        int bestScore = INT_MAX;

        for (int i = 0; i < _elevators.size(); i++)
        {
            if (should_try_elevator(i, floor))
            {
                elevator* elev = _elevators[i];

                auto& waiting = _waiting_passengers[floor];
                bool canServeAll = true;
                double totalWeight = 0;

                for (passenger* p : waiting)
                {
                    totalWeight += p->_weight;
                }

                if (elev->get_max_capacity() < totalWeight)
                {
                    canServeAll = false;
                }

                if (canServeAll)
                {
                    int distance = std::abs(elev->get_current_floor() - floor);
                    if (distance < bestScore)
                    {
                        bestScore = distance;
                        selectedElevator = i;
                    }
                }
            }
        }

        if (selectedElevator >= 0)
        {
            _elevators[selectedElevator]->press_floor_button(floor);
            _call_buttons[floor] = true;
            mark_elevator_attempted(selectedElevator, floor);
            std::cout << "Force called elevator " << selectedElevator
                << " for passengers at floor " << floor << std::endl;
        }
        else
        {
            std::cout << "No suitable elevator available for force call at floor " << floor << std::endl;
        }
    }
}