#include "observer.h"

void subject::add_observer(
    observer* observer) 
{
    _observers.push_back(observer);
}

void subject::remove_observer(
    observer* observer) 
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void subject::notify_observers(
    const std::string& event,
    int elevatorId,
    int floor, 
    int time) 
{
    for (observer* observer : _observers) 
    {
        observer->update(event, elevatorId, floor, time);
    }
}