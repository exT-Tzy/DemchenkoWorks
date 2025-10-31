#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "waiting_room.h"
#include "doctor.h"
#include "logger.h"

class hospital_simulation
{
private:
    int _room_capacity;
    int _num_doctors;
    int _max_service_time;
    waiting_room *_room;
    std::vector<doctor *> _doctors;
    logger *_logger_patients;
    logger *_logger_doctors;
    logger *_logger_service;
    logger *_logger_queue;
    int _next_patient_id;
    int _current_time;

public:
    hospital_simulation(
        int room_capacity,
        int num_doctors,
        int max_service_time);

    ~hospital_simulation();

    void run(
        int simulation_duration,
        int min_arrival_interval,
        int max_arrival_interval);

private:
    void process_doctor_work(
        int current_time);

    doctor *find_available_doctor();

    doctor *find_doctor_for_help();
};

#endif // SIMULATION_H