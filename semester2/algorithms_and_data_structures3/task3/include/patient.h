#ifndef PATIENT_H
#define PATIENT_H

enum class patient_state
{
    healthy,
    sick
};

class patient
{
private:
    int _id;
    patient_state _state;
    int _arrival_time;
    int _service_start_time;
    int _service_end_time;

public:
    patient(
        int id,
        patient_state state,
        int arrival_time);

    int get_id() const;

    patient_state get_state() const;

    void set_state(
        patient_state state);

    int get_arrival_time() const;

    void set_service_start_time(
        int time);

    void set_service_end_time(
        int time);

    int get_waiting_time() const;

    int get_service_time() const;

    int get_total_time() const;
};

#endif // PATIENT_H