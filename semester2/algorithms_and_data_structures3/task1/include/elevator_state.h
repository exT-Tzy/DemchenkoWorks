#ifndef ELEVATOR_STATE_H
#define ELEVATOR_STATE_H

#include <string>

class elevator;

class elevator_state 
{
public:
    
    virtual ~elevator_state() {}
    
    virtual void handle(
        elevator* elevator,
        int currentTime) = 0;
   
    virtual std::string get_name() const = 0;
};

class idle_closed_state : 
    public elevator_state
{
public:
    
    void handle(
        elevator* elevator,
        int currentTime) override;
    
    std::string get_name() const override;
};

class idle_open_state : 
    public elevator_state 
{
public:
   
    void handle(
        elevator* elevator, 
        int currentTime) override;
   
    std::string get_name() const override;
};

class moving_up_state : 
    public elevator_state 
{
public:
   
    void handle(
        elevator* elevator,
        int currentTime) override;
    
    std::string get_name() const override;
};

class moving_down_state :
    public elevator_state 
{
public:
    
    void handle(
        elevator* elevator, 
        int currentTime) override;
    
    std::string get_name() const override;
};

#endif // ELEVATOR_STATE_H