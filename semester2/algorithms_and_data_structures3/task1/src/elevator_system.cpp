#include "elevator_system.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

elevator_system::elevator_system(
    int floors,
    const std::vector<int>& elevatorCapacities)
    : _floors(floors), _current_time(0), _waiting_passengers(floors + 1),
    _call_buttons(floors + 1, false)
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

        if (selectedElevator >= 0)
        {
            _elevators[selectedElevator]->press_floor_button(floor);
            std::cout << "Elevator " << selectedElevator << " assigned to floor " << floor << std::endl;
        }
    }
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

        for (passenger* passenger : compatible_passengers)
        {
            if (elev->can_accommodate(passenger->_weight))
            {
                elev->add_passenger(passenger);
                elev->press_floor_button(passenger->_target_floor);
                passenger->_has_boarded = true;
                passenger->_boarding_time = time;

                for (typename passenger::passenger* companion : elev->_passengers)
                {
                    if (companion != passenger)
                    {
                        passenger->_companion_passengers.push_back(companion->_id);
                    }
                }

                std::cout << "Passenger " << passenger->_id << " boarded elevator " << elevatorId << std::endl;
            }
            else
            {
                waiting.push_back(passenger);
                std::cout << "Passenger " << passenger->_id << " cannot board - weight limit" << std::endl;
            }
        }

        for (passenger* passenger : incompatible_passengers)
        {
            waiting.push_back(passenger);
            call_elevator_for_specific_passenger(floor, passenger);
        }

        std::vector<passenger*> ejected = elev->handle_overload();

        for (passenger* ejectedPassenger : ejected)
        {
            waiting.push_back(ejectedPassenger);
            std::cout << "Passenger " << ejectedPassenger->_id << " ejected due to overweight" << std::endl;

            call_elevator(floor);
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

        for (passenger* passenger : _all_passengers)
        {
            if (!passenger->_has_reached_destination)
            {
                allPassengersDelivered = false;

                break;
            }
        }

        if (allPassengersDelivered)
        {
            std::cout << "All passengers delivered at time " << _current_time << std::endl;

            break;
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
        passengerFile << "Boarding time: " << (passenger->_boarding_time >= 0 ? format_time(passenger->_boarding_time) : "Not boarded") << std::endl;
        passengerFile << "Total travel time: " << passenger->_total_travel_time << " seconds" << std::endl;
        passengerFile << "Companions: ";
        for (int companionId : passenger->_companion_passengers)
        {
            passengerFile << companionId << " ";
        }
        passengerFile << std::endl;
        passengerFile << "Ejected due to overweight: " << (passenger->_was_ejected_due_to_overweight ? "Yes" : "No") << std::endl;
        passengerFile << std::endl;
    }

    passengerFile.close();

    std::ofstream elevatorFile(elevatorReportFile);

    for (elevator* elevator : _elevators)
    {
        elevatorFile << "Elevator ID: " << elevator->get_id() << std::endl;
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