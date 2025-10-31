#include "../include/patient.h"

patient::patient(
    int id,
    patient_state state,
    int arrival_time)
    : _id(id),
      _state(state),
      _arrival_time(arrival_time),
      _service_start_time(0),
      _service_end_time(0)
{
}

int patient::get_id() const
{
    return _id;
}

patient_state patient::get_state() const
{
    return _state;
}

void patient::set_state(
    patient_state state)
{
    _state = state;
}

int patient::get_arrival_time() const
{
    return _arrival_time;
}

void patient::set_service_start_time(
    int time)
{
    _service_start_time = time;
}

void patient::set_service_end_time(
    int time)
{
    _service_end_time = time;
}

int patient::get_waiting_time() const
{
    return _service_start_time - _arrival_time;
}

int patient::get_service_time() const
{
    return _service_end_time - _service_start_time;
}

int patient::get_total_time() const
{
    return _service_end_time - _arrival_time;
}