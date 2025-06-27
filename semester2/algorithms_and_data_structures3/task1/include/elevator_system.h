#ifndef ELEVATOR_SYSTEM_H
#define ELEVATOR_SYSTEM_H

#include "observer.h"
#include "elevator.h"
#include "elevator_scheduling_strategy.h"
#include "passenger.h"

#include <vector>
#include <string>
#include <climits>

class elevator_system :
    public observer
{
private:

    std::vector<elevator*> _elevators;

    std::vector<passenger*> _all_passengers;

    std::vector<std::vector<passenger*>> _waiting_passengers; // by floor

    std::vector<bool> _call_buttons; // by floor

    elevator_scheduling_strategy* _strategy;

    int _floors;

    int _current_time;

    std::string format_time(
        int seconds);

    bool is_direction_compatible(
        elevator* elev,
        passenger* passenger);

    std::string get_elevator_passenger_direction(
        elevator* elev);

    void call_elevator_for_specific_passenger(
        int floor,
        passenger* passenger);

public:

    elevator_system(
        int floors,
        const std::vector<int>& elevatorCapacities);

    ~elevator_system();


    void add_passenger(
        passenger* passenger);

    void call_elevator(
        int floor);

    void update(
        const std::string& event,
        int elevatorId,
        int floor,
        int time) override;

    void simulate(
        int maxTime);

    void generate_reports(
        const std::string& passengerReportFile,
        const std::string& elevatorReportFile);
};

#endif // ELEVATOR_SYSTEM_H