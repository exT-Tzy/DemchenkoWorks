#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <set>

class person_generator final
{
private:
    std::vector<std::string> _first_names;
    std::vector<std::string> _last_names;
    std::mt19937 _rng;

    void initialize_names()
    {
        _first_names =
            {
                "Иван", "Петр", "Сергей", "Андрей", "Алексей", "Дмитрий", "Михаил", "Александр",
                "Владимир", "Николай", "Евгений", "Максим", "Артем", "Денис", "Павел", "Роман",
                "Виктор", "Игорь", "Антон", "Олег", "Юрий", "Константин", "Илья", "Степан",
                "Мария", "Анна", "Елена", "Ольга", "Наталья", "Татьяна", "Ирина", "Светлана",
                "Екатерина", "Людмила", "Галина", "Нина", "Вера", "Любовь", "Валентина", "Зоя"};

        _last_names =
            {
                "Иванов(а)", "Петров(а)", "Сидоров(а)", "Смирнов(а)", "Козлов(а)", "Попов(а)", "Соколов(а)", "Лебедев(а)",
                "Новиков(а)", "Морозов(а)", "Волков(а)", "Соловьев(а)", "Васильев(а)", "Зайцев(а)", "Павлов(а)", "Семенов(а)",
                "Голубев(а)", "Виноградов(а)", "Богданов(а)", "Воробьев(а)", "Федоров(а)", "Михайлов(а)", "Беляев(а)", "Тарасов(а)",
                "Белов(а)", "Комаров(а)", "Орлов(а)", "Киселев(а)", "Макаров(а)", "Андреев(а)", "Ковалев(а)", "Ильин(а)",
                "Гусев(а)", "Титов(а)", "Кузьмин(а)", "Кудрявцев(а)", "Баранов(а)", "Куликов(а)", "Алексеев(а)", "Степанов(а)"};
    }

public:
    person_generator(
        unsigned int seed) : _rng(seed)
    {
        initialize_names();
    }

    std::string generate_first_name()
    {
        std::uniform_int_distribution<size_t> dist(0, _first_names.size() - 1);

        return _first_names[dist(_rng)];
    }

    std::string generate_last_name()
    {
        std::uniform_int_distribution<size_t> dist(0, _last_names.size() - 1);

        return _last_names[dist(_rng)];
    }

    std::mt19937 &get_rng()
    {

        return _rng;
    }
};

class graph_generator final
{
private:
    int _num_people;
    std::vector<std::set<int>> _adjacency_list;
    std::mt19937 &_rng;

    void ensure_connected_graph()
    {
        std::vector<int> component(static_cast<size_t>(_num_people));

        for (int i = 0; i < _num_people; ++i)
        {
            component[i] = i;
        }

        std::uniform_int_distribution<int> dist(0, _num_people - 1);

        for (int i = 1; i < _num_people; ++i)
        {
            int prev = dist(_rng) % i;
            add_edge(i, prev);

            int old_comp = component[i];
            int new_comp = component[prev];

            for (int j = 0; j < _num_people; ++j)
            {
                if (component[j] == old_comp)
                {
                    component[j] = new_comp;
                }
            }
        }
    }

    void add_random_edges(
        int min_edges,
        int max_edges)
    {
        std::uniform_int_distribution<int> person_dist(0, _num_people - 1);
        std::uniform_int_distribution<int> edge_count_dist(min_edges, max_edges);

        for (int i = 0; i < _num_people; ++i)
        {
            int target_edges = edge_count_dist(_rng);
            int attempts = 0;

            while (_adjacency_list[i].size() < static_cast<size_t>(target_edges) && attempts < target_edges * 3)
            {
                int other = person_dist(_rng);

                if (other != i && _adjacency_list[i].find(other) == _adjacency_list[i].end())
                {
                    add_edge(i, other);
                }

                attempts++;
            }
        }
    }

    void add_edge(
        int a,
        int b)
    {
        _adjacency_list[a].insert(b);
        _adjacency_list[b].insert(a);
    }

public:
    graph_generator(
        int num_people,
        std::mt19937 &rng) : _num_people(num_people),
                             _adjacency_list(static_cast<size_t>(num_people)),
                             _rng(rng)
    {
    }

    void generate(
        int min_connections,
        int max_connections)
    {
        ensure_connected_graph();
        add_random_edges(min_connections, max_connections);
    }

    const std::set<int> &get_connections(
        int person_id) const
    {
        return _adjacency_list[person_id];
    }
};

void generate_input_file(
    std::string const &filename,
    int num_people,
    double p1,
    double p2,
    int days,
    bool r,
    int t,
    int incubation)
{
    std::random_device rd;
    person_generator gen(rd());
    graph_generator graph(num_people, gen.get_rng());

    std::cout << "generating graph with " << num_people << " people..." << std::endl;
    graph.generate(3, 15);

    std::ofstream out(filename);

    if (!out.is_open())
    {
        std::cerr << "error opening file: " << filename << std::endl;

        return;
    }

    std::cout << "writing people data..." << std::endl;

    for (int i = 1; i <= num_people; ++i)
    {
        std::string first_name = gen.generate_first_name();
        std::string last_name = gen.generate_last_name();

        out << i << "," << first_name << "," << last_name;

        const std::set<int> &connections = graph.get_connections(i - 1);

        for (int conn : connections)
        {
            out << "," << (conn + 1);
        }

        out << "\n";

        if (i % 10000 == 0)
        {
            std::cout << "processed " << i << " people..." << std::endl;
        }
    }

    out << p1 << " " << p2 << " " << days << " " << (r ? "true" : "false") << " " << t << " " << incubation << "\n";
    out.close();

    std::cout << "file generated: " << filename << std::endl;
}

int main()
{
    int num_people;
    std::string filename;
    double p1, p2;
    int days, t, incubation;
    bool r;

    std::cout << "enter number of people (10000-1000000): ";
    std::cin >> num_people;

    if (num_people < 10000 || num_people > 1000000)
    {
        std::cerr << "number of people must be between 10000 and 1000000" << std::endl;

        return 1;
    }

    std::cout << "enter output filename: ";
    std::cin >> filename;

    std::cout << "enter p1 (infection probability, 0-1): ";
    std::cin >> p1;

    std::cout << "enter p2 (recovery probability, 0-1): ";
    std::cin >> p2;

    std::cout << "enter number of days to simulate: ";
    std::cin >> days;

    std::cout << "enter r (immunity after recovery, true/false): ";
    std::string r_str;
    std::cin >> r_str;
    r = (r_str == "true");

    std::cout << "enter T (base recovery time, >3): ";
    std::cin >> t;

    std::cout << "enter I (incubation period in days): ";
    std::cin >> incubation;

    generate_input_file(filename, num_people, p1, p2, days, r, t, incubation);

    return 0;
}