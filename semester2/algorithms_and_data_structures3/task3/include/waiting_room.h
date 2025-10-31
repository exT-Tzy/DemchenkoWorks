#ifndef WAITING_ROOM_H
#define WAITING_ROOM_H

#include <vector>
#include <queue>

#include "patient.h"
#include "logger.h"

class waiting_room
{
private:
    int _capacity;
    std::vector<patient *> _patients_inside;
    std::queue<patient *> _waiting_queue;
    patient_state _current_room_state;
    bool _room_empty;
    logger *_logger_patients;
    logger *_logger_queue;
    int _last_infection_check_time;
    int _infection_check_interval;

public:
    waiting_room(
        int capacity,
        logger *logger_patients,
        logger *logger_queue);

    ~waiting_room();

    bool try_enter(
        patient *p,
        int current_time);

    void process_queue(
        int current_time);

    patient *get_next_patient(
        int current_time);

    void check_queue_infection(
        int current_time);

    size_t get_queue_size() const;

    size_t get_room_size() const;

    bool has_patients() const;
};

#endif // WAITING_ROOM_H