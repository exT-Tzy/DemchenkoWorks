#include "../include/simulation.h"
#include "../include/client_logger_builder.h"

#include <random>
#include <sstream>
#include <iostream>

hospital_simulation::hospital_simulation(
    int room_capacity,
    int num_doctors,
    int max_service_time)
    : _room_capacity(room_capacity),
      _num_doctors(num_doctors),
      _max_service_time(max_service_time),
      _next_patient_id(1),
      _current_time(0)
{
    client_logger_builder builder;

    _logger_patients = builder
                           .add_file_stream("patients_log.txt", logger::severity::information)
                           ->add_file_stream("patients_log.txt", logger::severity::warning)
                           ->build();

    builder.clear();
    _logger_doctors = builder
                          .add_file_stream("doctors_log.txt", logger::severity::information)
                          ->build();

    builder.clear();
    _logger_service = builder
                          .add_file_stream("service_log.txt", logger::severity::information)
                          ->build();

    builder.clear();
    _logger_queue = builder
                        .add_file_stream("queue_log.txt", logger::severity::information)
                        ->add_file_stream("queue_log.txt", logger::severity::warning)
                        ->build();

    _room = new waiting_room(_room_capacity, _logger_patients, _logger_queue);

    for (int i = 0; i < _num_doctors; ++i)
    {
        _doctors.push_back(new doctor(i + 1, _logger_doctors, _logger_service, _max_service_time));
    }

    std::ostringstream msg;
    msg << "simulation started";
    _logger_patients->information(msg.str());
    _logger_doctors->information(msg.str());
    _logger_service->information(msg.str());
    _logger_queue->information(msg.str());
}

hospital_simulation::~hospital_simulation()
{
    for (doctor *d : _doctors)
    {
        delete d;
    }

    delete _room;
    delete _logger_patients;
    delete _logger_doctors;
    delete _logger_service;
    delete _logger_queue;
}

doctor *hospital_simulation::find_available_doctor()
{
    for (doctor *d : _doctors)
    {
        if (!d->is_busy())
        {
            return d;
        }
    }

    return nullptr;
}

doctor *hospital_simulation::find_doctor_for_help()
{
    for (doctor *d : _doctors)
    {
        if (d->can_provide_help())
        {
            return d;
        }
    }

    return nullptr;
}

void hospital_simulation::process_doctor_work(
    int current_time)
{
    std::mt19937 rng(current_time);
    std::uniform_int_distribution<int> service_time_dist(1, _max_service_time);
    std::uniform_real_distribution<double> help_prob_dist(0.0, 1.0);

    for (doctor *d : _doctors)
    {
        d->update(current_time);
    }

    for (doctor *d : _doctors)
    {
        patient *finished = d->finish_service(current_time);
        if (finished != nullptr)
        {
            delete finished;
        }
    }

    for (doctor *d : _doctors)
    {
        if (!d->is_busy())
        {
            patient *p = _room->get_next_patient(current_time);

            if (p != nullptr)
            {
                int service_duration = service_time_dist(rng);
                d->start_service(p, current_time, service_duration);

                if (help_prob_dist(rng) < 0.15)
                {
                    int help_duration = service_time_dist(rng);
                    d->request_help(current_time, help_duration);

                    doctor *helper = find_doctor_for_help();

                    if (helper != nullptr)
                    {
                        helper->provide_help(current_time, help_duration);
                    }
                    else
                    {
                        std::ostringstream msg;
                        msg << "[Time " << current_time << "] WARNING: doctor " << d->get_id()
                            << " requested help but no doctor available!";
                        _logger_doctors->warning(msg.str());
                    }
                }
            }
        }
    }
}

void hospital_simulation::run(
    int simulation_duration,
    int min_arrival_interval,
    int max_arrival_interval)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> interval_dist(min_arrival_interval, max_arrival_interval);
    std::uniform_real_distribution<double> state_dist(0.0, 1.0);

    int next_arrival_time = interval_dist(rng);

    int total_patients_arrived = 0;
    int total_sick_arrived = 0;
    int total_healthy_arrived = 0;

    while (_current_time <= simulation_duration || _room->has_patients())
    {
        if (_current_time == next_arrival_time && _current_time <= simulation_duration)
        {
            patient_state state = (state_dist(rng) < 0.4)
                                      ? patient_state::sick
                                      : patient_state::healthy;

            patient *p = new patient(_next_patient_id++, state, _current_time);

            total_patients_arrived++;

            if (state == patient_state::sick)
            {
                total_sick_arrived++;
            }
            else
            {
                total_healthy_arrived++;
            }

            std::ostringstream msg;
            msg << "[Time " << _current_time << "] *** NEW ARRIVAL ***: patient " << p->get_id() << " (";
            msg << (state == patient_state::healthy ? "healthy" : "sick");
            msg << ") arrived at the facility";
            _logger_patients->information(msg.str());

            if (!_room->try_enter(p, _current_time))
            {
                // Asdsjnasisdahfiuoqhie
            }

            next_arrival_time = _current_time + interval_dist(rng);
        }

        _room->check_queue_infection(_current_time);
        process_doctor_work(_current_time);
        _room->process_queue(_current_time);

        _current_time++;

        if (_current_time > simulation_duration + 10000)
        {
            std::ostringstream msg;
            msg << "[Time " << _current_time << "] WARNING: simulation exceeded maximum duration.";
            _logger_patients->warning(msg.str());
            _logger_doctors->warning(msg.str());
            _logger_service->warning(msg.str());
            _logger_queue->warning(msg.str());

            break;
        }
    }

    std::string final_str = "\nsimulation completed\n";
    _logger_patients->information(final_str);
    _logger_doctors->information(final_str);
    _logger_service->information(final_str);
    _logger_queue->information(final_str);

    std::cout << final_str << std::endl;
}