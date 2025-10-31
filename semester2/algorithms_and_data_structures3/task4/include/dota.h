#ifndef DOTA_H
#define DOTA_H

#include <vector>
#include <random>

#include "characters.h"

class game final
{
private:
    std::vector<character *> _characters;
    std::mt19937 _rng;

    character *find_character_by_name(
        const std::string &name);

    equipment_item generate_random_equipment();

    std::string get_random_equipment_name(
        equipment_slot slot);

    void display_character_list();

    int select_character_from_list();

    void display_fighters_list();

    void display_healers_list();

    int select_fighter_from_list();

    int select_healer_from_list(
        const std::vector<int> &excluded_indices = {});

    bool attack_hits(
        const character &attacker,
        const character &defender);

    bool is_critical_hit(
        const character &attacker);

    double calculate_actual_damage(
        double base_damage,
        const character &defender,
        bool is_critical);

public:
    game();
    ~game();

    void create_character();

    void generate_equipment_for_character();

    void display_all_characters();

    void start_battle();

    void restore_all_characters();

    void run();
};

#endif // DOTA_H