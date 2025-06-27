#ifndef COMMAND_H
#define COMMAND_H

class elevator;
class elevator_system;

class command 
{
public:
    
    virtual ~command() {}
   
    virtual void execute() = 0;
};

class floor_button_command : 
    public command 
{
private:
    
    elevator* _elevator;
    
    int _floor;

public:
   
    floor_button_command(
        elevator* elevator,
        int floor);
    
    void execute() override;
};

class call_elevator_command : 
    public command 
{
private:
    
    elevator_system* _system;
    
    int _floor;

public:
    
    call_elevator_command(
        elevator_system* system, 
        int floor);
    
    void execute() override;
};

#endif // COMMAND_H