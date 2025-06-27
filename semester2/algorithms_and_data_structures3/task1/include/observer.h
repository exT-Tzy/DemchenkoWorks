#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <string>
#include <algorithm>

class observer 
{
public:
    
    virtual ~observer() {}
    
    virtual void update(
        const std::string& event,
        int elevatorId,
        int floor, 
        int time) = 0;
};

class subject 
{
private:
    
    std::vector<observer*> _observers;

public:
    
    virtual ~subject() {}


    void add_observer(
        observer* observer);
   
    void remove_observer(
        observer* observer);
   
    void notify_observers(
        const std::string& event,
        int elevatorId,
        int floor, 
        int time);
};

#endif // OBSERVER_H