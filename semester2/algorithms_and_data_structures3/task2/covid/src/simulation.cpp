#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <random>

enum class disease_state
{
    healthy,
    incubating,
    sick,
    recovering,
    recovered,
    chronic
};

class person final
{
private:
    int _id;
    std::string _first_name;
    std::string _last_name;
    std::vector<int> _connections;
    disease_state _state;
    int _days_in_current_state;
    int _recovery_duration;
    int _infection_count;
    bool _immune;

public:
    person(
        int id,
        const std::string &first_name,
        const std::string &last_name)
        : _id(id),
          _first_name(first_name),
          _last_name(last_name),
          _state(disease_state::healthy),
          _days_in_current_state(0),
          _recovery_duration(0),
          _infection_count(0),
          _immune(false)
    {
    }

    void add_connection(
        int other_id)
    {
        _connections.push_back(other_id);
    }

    const std::vector<int> &get_connections() const
    {
        return _connections;
    }

    int get_id() const
    {
        return _id;
    }

    std::string get_first_name() const
    {
        return _first_name;
    }

    std::string get_last_name() const
    {
        return _last_name;
    }

    disease_state get_state() const
    {
        return _state;
    }

    void set_state(
        disease_state state)
    {
        _state = state;
        _days_in_current_state = 0;
    }

    int get_days_in_current_state() const
    {
        return _days_in_current_state;
    }

    void increment_days_in_state()
    {
        _days_in_current_state++;
    }

    void set_recovery_duration(
        int duration)
    {
        _recovery_duration = duration;
    }

    int get_recovery_duration() const
    {
        return _recovery_duration;
    }

    void increment_infection_count()
    {
        _infection_count++;
    }

    int get_infection_count() const
    {
        return _infection_count;
    }

    bool is_immune() const
    {
        return _immune;
    }

    void set_immune(
        bool immune)
    {
        _immune = immune;
    }

    bool is_contagious() const
    {
        return _state == disease_state::sick;
    }

    bool is_isolated() const
    {
        return _state == disease_state::recovering;
    }
};

class simulation_parameters
{
private:
    double _p1;
    double _p2;
    int _days;
    bool _r;
    int _t;
    int _incubation;

public:
    simulation_parameters(
        double p1,
        double p2,
        int days,
        bool r,
        int t,
        int incubation)
        : _p1(p1),
          _p2(p2),
          _days(days),
          _r(r),
          _t(t),
          _incubation(incubation)
    {
    }

    double get_p1() const { return _p1; }

    double get_p2() const { return _p2; }

    int get_days() const { return _days; }

    bool get_r() const { return _r; }

    int get_t() const { return _t; }

    int get_incubation() const { return _incubation; }
};

class disease_simulator
{
private:
    std::vector<person> _people;
    simulation_parameters _params;
    std::mt19937 _rng;
    std::uniform_real_distribution<double> _prob_dist;

    void load_data_from_file(
        std::string const &filename)
    {
        std::ifstream in(filename);

        if (!in.is_open())
        {
            throw std::runtime_error("cannot open input file: " + filename);
        }

        std::string line;

        while (std::getline(in, line))
        {
            if (line.find(',') == std::string::npos)
            {
                break;
            }

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ','))
            {
                tokens.push_back(token);
            }

            if (tokens.size() < 3)
            {
                continue;
            }

            int id = std::stoi(tokens[0]);
            std::string first_name = tokens[1];
            std::string last_name = tokens[2];

            _people.emplace_back(id, first_name, last_name);

            for (size_t i = 3; i < tokens.size(); ++i)
            {
                _people.back().add_connection(std::stoi(tokens[i]));
            }
        }

        std::cout << "loaded " << _people.size() << " people" << std::endl;
    }

    void initial_outbreak()
    {
        std::uniform_int_distribution<size_t> dist(0, _people.size() - 1);
        size_t patient_zero = dist(_rng);
        _people[patient_zero].set_state(disease_state::incubating);
        _people[patient_zero].increment_infection_count();
        std::cout << "patient zero: " << _people[patient_zero].get_first_name()
                  << " " << _people[patient_zero].get_last_name() << std::endl;
    }

    void process_infections()
    {
        std::vector<int> newly_infected;

        for (auto &infected : _people)
        {
            if (!infected.is_contagious())
            {
                continue;
            }

            for (int conn_id : infected.get_connections())
            {
                person &contact = _people[conn_id - 1];

                if (contact.get_state() == disease_state::healthy && !contact.is_immune())
                {
                    double roll = _prob_dist(_rng);

                    if (roll < _params.get_p1())
                    {
                        newly_infected.push_back(conn_id - 1);
                    }
                }
            }
        }

        for (int idx : newly_infected)
        {
            _people[idx].set_state(disease_state::incubating);
            _people[idx].increment_infection_count();
        }
    }

    void update_disease_states()
    {
        for (auto &p : _people)
        {
            p.increment_days_in_state();

            if (p.get_state() == disease_state::incubating)
            {
                if (p.get_days_in_current_state() >= _params.get_incubation())
                {
                    p.set_state(disease_state::sick);
                }
            }
            else if (p.get_state() == disease_state::sick)
            {
                if (p.get_days_in_current_state() >= 1)
                {
                    std::uniform_int_distribution<int> recovery_dist(_params.get_t(), 3 * _params.get_t());
                    p.set_recovery_duration(recovery_dist(_rng));
                    p.set_state(disease_state::recovering);
                }
            }
            else if (p.get_state() == disease_state::recovering)
            {
                if (p.get_days_in_current_state() >= p.get_recovery_duration())
                {
                    double roll = _prob_dist(_rng);

                    if (roll < _params.get_p2())
                    {
                        p.set_state(disease_state::recovered);

                        if (_params.get_r())
                        {
                            p.set_immune(true);
                        }
                    }
                    else
                    {
                        p.set_state(disease_state::chronic);
                    }
                }
            }
        }
    }

public:
    disease_simulator(
        const std::string &filename,
        const simulation_parameters &params)
        : _params(params),
          _rng(std::random_device{}()),
          _prob_dist(0.0, 1.0)
    {
        load_data_from_file(filename);
    }

    void run_simulation()
    {
        std::cout << "starting simulation for " << _params.get_days() << " days..." << std::endl;

        initial_outbreak();

        for (int day = 1; day <= _params.get_days(); ++day)
        {
            process_infections();
            update_disease_states();

            if (day % 10 == 0)
            {
                std::cout << "day " << day << " completed" << std::endl;
            }
        }
    }

    void generate_report(
        std::string const &output_filename)
    {
        std::ofstream out(output_filename);
        if (!out.is_open())
        {
            throw std::runtime_error("cannot open output file: " + output_filename);
        }

        std::vector<person *> never_infected;
        std::vector<person *> recovered_people;
        std::vector<person *> recovered_with_sick_contacts;
        std::vector<person *> healthy_all_contacts_infected_multiple;
        std::vector<person *> infected_multiple_times;
        std::vector<person *> chronic_people;

        for (auto &p : _people)
        {
            if (p.get_infection_count() == 0)
            {
                never_infected.push_back(&p);
            }

            if (p.get_state() == disease_state::recovered)
            {
                recovered_people.push_back(&p);

                bool any_contact_not_recovered = false;

                for (int conn_id : p.get_connections())
                {
                    const person &contact = _people[conn_id - 1];

                    if (contact.get_infection_count() > 0 && contact.get_state() != disease_state::recovered)
                    {
                        any_contact_not_recovered = true;

                        break;
                    }
                }

                if (any_contact_not_recovered)
                {
                    recovered_with_sick_contacts.push_back(&p);
                }
            }

            if (p.get_state() == disease_state::chronic)
            {
                chronic_people.push_back(&p);
            }

            if (p.get_infection_count() > 1)
            {
                infected_multiple_times.push_back(&p);
            }

            if (p.get_infection_count() == 0)
            {
                bool all_infected_multiple = true;

                if (p.get_connections().empty())
                {
                    all_infected_multiple = false;
                }

                for (int conn_id : p.get_connections())
                {
                    const person &contact = _people[conn_id - 1];

                    if (contact.get_infection_count() <= 1)
                    {
                        all_infected_multiple = false;

                        break;
                    }
                }

                if (all_infected_multiple && !p.get_connections().empty())
                {
                    healthy_all_contacts_infected_multiple.push_back(&p);
                }
            }
        }

        out << "1. NOT INFECTED DURING THE SIMULATION (" << never_infected.size() << "):\n";
        for (const auto *p : never_infected)
        {
            out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
        }
        out << "\n";

        out << "2. HEALED DURING THE SIMULATION (" << recovered_people.size() << "):\n";
        for (const auto *p : recovered_people)
        {
            out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
        }
        out << "\n";

        out << "3. HEALED WHOSE ENVIRONMENT WERE NOT HEALED (" << recovered_with_sick_contacts.size() << "):\n";
        for (const auto *p : recovered_with_sick_contacts)
        {
            out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
        }
        out << "\n";

        out << "4. PEOPLE WITH A CHRONIC FORM OF THE DISEASE (" << chronic_people.size() << "):\n";
        for (const auto *p : chronic_people)
        {
            out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
        }
        out << "\n";

        if (_params.get_r() == false)
        {
            out << "5. NOT INFECTED, WHOSE ENTIRE ENVIRONMENT HAS BEEN INFECTED >1 TIME ("
                << healthy_all_contacts_infected_multiple.size() << "):\n";
            for (const auto *p : healthy_all_contacts_infected_multiple)
            {
                out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
            }
            out << "\n";

            out << "6. THOSE WHO HAVE BEEN ILL MORE THAN ONCE (" << infected_multiple_times.size() << "):\n";
            for (const auto *p : infected_multiple_times)
            {
                out << p->get_id() << " " << p->get_last_name() << " " << p->get_first_name() << "\n";
            }
            out << "\n";
        }

        out.close();
        std::cout << "report generated: " << output_filename << std::endl;
    }
};

int main(
    int argc,
    char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;

        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    try
    {
        std::ifstream in(input_file);
        if (!in.is_open())
        {
            throw std::runtime_error("cannot open input file");
        }

        std::string line;
        double p1, p2;
        int days, t, incubation;
        bool r;

        while (std::getline(in, line))
        {
            if (line.find(',') == std::string::npos)
            {
                std::stringstream ss(line);
                std::string r_str;
                ss >> p1 >> p2 >> days >> r_str >> t >> incubation;
                r = (r_str == "true");

                break;
            }
        }
        in.close();

        simulation_parameters params(p1, p2, days, r, t, incubation);
        disease_simulator sim(input_file, params);

        sim.run_simulation();
        sim.generate_report(output_file);

        std::cout << "ыimulation completed" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "уrror: " << e.what() << std::endl;

        return 1;
    }

    return 0;
}