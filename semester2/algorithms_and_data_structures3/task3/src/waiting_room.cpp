#include "../include/waiting_room.h"

#include <sstream>

waiting_room::waiting_room(
    int capacity,
    logger *logger_patients,
    logger *logger_queue)
    : _capacity(capacity),
      _room_empty(true),
      _logger_patients(logger_patients),
      _logger_queue(logger_queue),
      _last_infection_check_time(0),
      _infection_check_interval(100)
{
}

waiting_room::~waiting_room()
{
}

bool waiting_room::try_enter(
    patient *p,
    int current_time)
{
    if (_patients_inside.size() >= static_cast<size_t>(_capacity))
    {
        _waiting_queue.push(p);

        std::ostringstream msg;
        msg << "[time " << current_time << "] patient " << p->get_id() << " (";
        msg << (p->get_state() == patient_state::healthy ? "healthy" : "sick");
        msg << ") joined the queue (room is full). queue size: " << _waiting_queue.size();
        _logger_queue->information(msg.str());

        return false;
    }

    if (_room_empty)
    {
        _patients_inside.push_back(p);
        _current_room_state = p->get_state();
        _room_empty = false;

        std::ostringstream msg;
        msg << "[time " << current_time << "] patient " << p->get_id() << " (";
        msg << (p->get_state() == patient_state::healthy ? "healthy" : "sick");
        msg << ") entered waiting room (first patient). room now has " << _patients_inside.size()
            << "/" << _capacity << " patients";
        _logger_patients->information(msg.str());

        return true;
    }

    if (_current_room_state == p->get_state())
    {
        _patients_inside.push_back(p);

        std::ostringstream msg;
        msg << "[time " << current_time << "] patient " << p->get_id() << " (";
        msg << (p->get_state() == patient_state::healthy ? "healthy" : "sick");
        msg << ") entered waiting room. room now has " << _patients_inside.size()
            << "/" << _capacity << " patients";
        _logger_patients->information(msg.str());

        return true;
    }

    _waiting_queue.push(p);

    std::ostringstream msg;
    msg << "[time " << current_time << "] patient " << p->get_id() << " (";
    msg << (p->get_state() == patient_state::healthy ? "healthy" : "sick");
    msg << ") joined the queue. queue size: " << _waiting_queue.size();
    _logger_queue->information(msg.str());

    return false;
}

void waiting_room::process_queue(
    int current_time)
{
    while (!_waiting_queue.empty() && _patients_inside.size() < static_cast<size_t>(_capacity))
    {
        patient *p = _waiting_queue.front();

        if (_room_empty || _current_room_state == p->get_state())
        {
            _waiting_queue.pop();
            _patients_inside.push_back(p);

            if (_room_empty)
            {
                _current_room_state = p->get_state();
                _room_empty = false;
            }

            std::ostringstream msg;
            msg << "[time " << current_time << "] patient " << p->get_id() << " (";
            msg << (p->get_state() == patient_state::healthy ? "healthy" : "sick");
            msg << ") moved from queue to waiting room. room: " << _patients_inside.size()
                << "/" << _capacity << ", queue: " << _waiting_queue.size();
            _logger_queue->information(msg.str());
        }
        else
        {
            break;
        }
    }
}

patient *waiting_room::get_next_patient(
    int current_time)
{
    if (_patients_inside.empty())
    {
        return nullptr;
    }

    patient *earliest = _patients_inside[0];
    size_t earliest_index = 0;

    for (size_t i = 1; i < _patients_inside.size(); ++i)
    {
        if (_patients_inside[i]->get_arrival_time() < earliest->get_arrival_time())
        {
            earliest = _patients_inside[i];
            earliest_index = i;
        }
    }

    _patients_inside.erase(_patients_inside.begin() + earliest_index);

    std::ostringstream msg;
    msg << "[time " << current_time << "] patient " << earliest->get_id()
        << " taken from waiting room for service. room now has " << _patients_inside.size()
        << " patients";
    _logger_patients->information(msg.str());

    if (_patients_inside.empty())
    {
        _room_empty = true;

        std::ostringstream msg2;
        msg2 << "[Time " << current_time << "] waiting room is now EMPTY.";
        _logger_patients->information(msg2.str());
    }

    return earliest;
}

void waiting_room::check_queue_infection(
    int current_time)
{
    if (current_time - _last_infection_check_time < _infection_check_interval)
    {
        return;
    }

    _last_infection_check_time = current_time;

    if (_waiting_queue.size() <= 1)
    {
        return;
    }

    bool has_sick = false;
    int healthy_count = 0;

    std::queue<patient *> temp_queue = _waiting_queue;
    while (!temp_queue.empty())
    {
        patient *p = temp_queue.front();
        temp_queue.pop();

        if (p->get_state() == patient_state::sick)
        {
            has_sick = true;
        }
        else
        {
            healthy_count++;
        }
    }

    if (has_sick && healthy_count > 0)
    {
        std::queue<patient *> new_queue;
        int infected_count = 0;

        while (!_waiting_queue.empty())
        {
            patient *p = _waiting_queue.front();
            _waiting_queue.pop();

            if (p->get_state() == patient_state::healthy)
            {
                p->set_state(patient_state::sick);
                infected_count++;

                std::ostringstream msg;
                msg << "[time " << current_time << "] QUEUE INFECTION: patient "
                    << p->get_id() << " got infected while waiting in queue";
                _logger_queue->warning(msg.str());
            }

            new_queue.push(p);
        }

        _waiting_queue = new_queue;

        if (infected_count > 0)
        {
            std::ostringstream msg;
            msg << "[Time " << current_time << "] *** QUEUE INFECTION ***: "
                << infected_count << " patients got infected in the queue! total in queue: "
                << _waiting_queue.size();
            _logger_queue->warning(msg.str());
        }
    }
}

size_t waiting_room::get_queue_size() const
{
    return _waiting_queue.size();
}

size_t waiting_room::get_room_size() const
{
    return _patients_inside.size();
}

bool waiting_room::has_patients() const
{
    return !_patients_inside.empty() || !_waiting_queue.empty();
}