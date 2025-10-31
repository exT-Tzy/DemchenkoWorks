#ifndef DOCTOR_H
#define DOCTOR_H

#include "patient.h"
#include "logger.h"

class doctor
{
private:
    int _id;
    logger *_logger_doctors;
    logger *_logger_service;
    int _max_service_time;
    patient *_current_patient;
    int _service_end_time;
    bool _busy;
    bool _needs_help;
    int _help_end_time;
    bool _providing_help;
    int _help_provide_end_time;

public:
    doctor(
        int id,
        logger *logger_doctors,
        logger *logger_service,
        int max_service_time);

    ~doctor();

    bool is_busy() const;

    bool can_provide_help() const;

    void start_service(
        patient *p,
        int current_time,
        int service_duration);

    void request_help(
        int current_time,
        int help_duration);

    void provide_help(
        int current_time,
        int help_duration);

    patient *finish_service(
        int current_time);

    void update(
        int current_time);

    int get_id() const;
};

#endif // DOCTOR_H