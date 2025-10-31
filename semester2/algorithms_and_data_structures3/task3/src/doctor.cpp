#include "../include/doctor.h"

#include <sstream>

doctor::doctor(
    int id,
    logger *logger_doctors,
    logger *logger_service,
    int max_service_time)
    : _id(id),
      _logger_doctors(logger_doctors),
      _logger_service(logger_service),
      _max_service_time(max_service_time),
      _current_patient(nullptr),
      _service_end_time(0),
      _busy(false),
      _needs_help(false),
      _help_end_time(0),
      _providing_help(false),
      _help_provide_end_time(0)
{
}

doctor::~doctor()
{
}

bool doctor::is_busy() const
{
    return _busy || _providing_help;
}

bool doctor::can_provide_help() const
{
    return !_busy && !_needs_help && !_providing_help;
}

void doctor::start_service(
    patient *p,
    int current_time,
    int service_duration)
{
    _current_patient = p;
    _busy = true;
    _service_end_time = current_time + service_duration;

    p->set_service_start_time(current_time);

    std::ostringstream msg;
    msg << "[time " << current_time << "] doctor " << _id
        << " started serving patient " << p->get_id()
        << " (estimated end: " << _service_end_time << ")";
    _logger_doctors->information(msg.str());
}

void doctor::request_help(
    int current_time,
    int help_duration)
{
    _needs_help = true;
    _help_end_time = current_time + help_duration;

    std::ostringstream msg;
    msg << "[time " << current_time << "] doctor " << _id
        << " requests help";

    if (_current_patient != nullptr)
    {
        msg << " for patient " << _current_patient->get_id();
    }

    msg << " (help duration: " << help_duration << ")";
    _logger_doctors->information(msg.str());
}

void doctor::provide_help(
    int current_time,
    int help_duration)
{
    _providing_help = true;
    _help_provide_end_time = current_time + help_duration;

    std::ostringstream msg;
    msg << "[time " << current_time << "] doctor " << _id
        << " is providing help (duration: " << help_duration << ")";
    _logger_doctors->information(msg.str());
}

patient *doctor::finish_service(
    int current_time)
{
    if (!_busy || _current_patient == nullptr)
    {
        return nullptr;
    }

    if (current_time < _service_end_time)
    {
        return nullptr;
    }

    _current_patient->set_service_end_time(current_time);

    std::ostringstream msg;
    msg << "[Time " << current_time << "] doctor " << _id
        << " finished serving patient " << _current_patient->get_id();
    msg << " | service time: " << _current_patient->get_service_time() << " units";
    msg << " | waiting time: " << _current_patient->get_waiting_time() << " units";
    msg << " | total time in facility: " << _current_patient->get_total_time() << " units";
    _logger_service->information(msg.str());

    patient *finished_patient = _current_patient;
    _current_patient = nullptr;
    _busy = false;

    return finished_patient;
}

void doctor::update(
    int current_time)
{
    if (_needs_help && current_time >= _help_end_time)
    {
        _needs_help = false;

        std::ostringstream msg;
        msg << "[time " << current_time << "] doctor " << _id
            << " finished receiving help";

        if (_current_patient != nullptr)
        {
            msg << " for patient " << _current_patient->get_id();
        }

        _logger_doctors->information(msg.str());
    }

    if (_providing_help && current_time >= _help_provide_end_time)
    {
        _providing_help = false;

        std::ostringstream msg;
        msg << "[time " << current_time << "] doctor " << _id
            << " finished providing help";
        _logger_doctors->information(msg.str());
    }
}

int doctor::get_id() const
{
    return _id;
}