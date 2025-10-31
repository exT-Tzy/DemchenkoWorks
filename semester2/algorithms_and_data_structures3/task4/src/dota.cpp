#include "../include/dota.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iomanip>
#include <limits>

game::game()
{
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());

    _rng.seed(seed);
}

game::~game()
{
    for (auto character : _characters)
    {
        delete character;
    }
}

character *game::find_character_by_name(
    const std::string &name)
{
    for (auto character : _characters)
    {
        if (character->get_name() == name)
        {
            return character;
        }
    }

    return nullptr;
}

std::string game::get_random_equipment_name(
    equipment_slot slot)
{
    std::vector<std::string> helmet_names = {"Iron Helmet", "Steel Helmet", "Golden Helmet", "Dragon Helmet", "Mythic Helmet"};
    std::vector<std::string> shoulder_names = {"Leather Shoulders", "Plate Shoulders", "Dragon Shoulders", "Titan Shoulders", "Epic Shoulders"};
    std::vector<std::string> chest_names = {"Chainmail", "Plate Armor", "Dragon Chestplate", "Legendary Armor", "Mythic Chestplate"};
    std::vector<std::string> gloves_names = {"Leather Gloves", "Steel Gauntlets", "Dragon Gloves", "Titan Gauntlets", "Epic Gloves"};
    std::vector<std::string> cloak_names = {"Simple Cloak", "Magic Cloak", "Shadow Cloak", "Phoenix Cloak", "Divine Cloak"};

    std::uniform_int_distribution<> dist(0, 4);

    switch (slot)
    {
    case equipment_slot::helmet:
        return helmet_names[dist(_rng)];

    case equipment_slot::shoulder:
        return shoulder_names[dist(_rng)];

    case equipment_slot::chest:
        return chest_names[dist(_rng)];

    case equipment_slot::gloves:
        return gloves_names[dist(_rng)];

    case equipment_slot::cloak:
        return cloak_names[dist(_rng)];

    default:
        return "Unknown Item";
    }
}

equipment_item game::generate_random_equipment()
{
    std::uniform_int_distribution<> slot_dist(0, 4);
    std::uniform_int_distribution<> stat_dist(5, 25);
    std::uniform_int_distribution<> secondary_dist(2, 12);

    equipment_slot slot = static_cast<equipment_slot>(slot_dist(_rng));
    std::string name = get_random_equipment_name(slot);

    int health_bonus = 0;
    int armor_bonus = 0;
    int accuracy_bonus = 0;
    int luck_bonus = 0;
    int mastery_bonus = 0;

    std::uniform_int_distribution<> main_stat_dist(0, 4);
    int main_stat = main_stat_dist(_rng);

    switch (main_stat)
    {
    case 0:
        health_bonus = stat_dist(_rng);
        break;

    case 1:
        armor_bonus = stat_dist(_rng);
        break;

    case 2:
        accuracy_bonus = stat_dist(_rng);
        break;

    case 3:
        luck_bonus = stat_dist(_rng);
        break;

    case 4:
        mastery_bonus = stat_dist(_rng);
        break;
    }

    std::uniform_int_distribution<> secondary_count_dist(1, 2);
    int secondary_count = secondary_count_dist(_rng);

    for (int i = 0; i < secondary_count; ++i)
    {
        std::uniform_int_distribution<> secondary_stat_dist(0, 4);
        int secondary_stat = secondary_stat_dist(_rng);

        if (secondary_stat == main_stat)
        {
            continue;
        }

        switch (secondary_stat)
        {
        case 0:
            if (health_bonus == 0)
                health_bonus = secondary_dist(_rng);
            break;
        case 1:
            if (armor_bonus == 0)
                armor_bonus = secondary_dist(_rng);
            break;
        case 2:
            if (accuracy_bonus == 0)
                accuracy_bonus = secondary_dist(_rng);
            break;
        case 3:
            if (luck_bonus == 0)
                luck_bonus = secondary_dist(_rng);
            break;
        case 4:
            if (mastery_bonus == 0)
                mastery_bonus = secondary_dist(_rng);
            break;
        }
    }

    return equipment_item(
        slot, name, health_bonus, armor_bonus,
        accuracy_bonus, luck_bonus, mastery_bonus);
}

void game::display_character_list()
{
    if (_characters.empty())
    {
        std::cout << "No characters available." << std::endl;

        return;
    }

    std::cout << "\nAvailable characters:" << std::endl;

    for (size_t i = 0; i < _characters.size(); ++i)
    {
        std::cout << i + 1 << ". " << _characters[i]->get_name()
                  << " (" << _characters[i]->get_class_name() << ")" << std::endl;
    }
}

void game::display_fighters_list()
{
    if (_characters.empty())
    {
        std::cout << "No characters available." << std::endl;

        return;
    }

    std::cout << "\nAvailable fighters (non-healers):" << std::endl;
    bool has_fighters = false;

    for (size_t i = 0; i < _characters.size(); ++i)
    {
        if (_characters[i]->get_class_name() != "Healer")
        {
            std::cout << i + 1 << ". " << _characters[i]->get_name()
                      << " (" << _characters[i]->get_class_name() << ")" << std::endl;
            has_fighters = true;
        }
    }

    if (!has_fighters)
    {
        std::cout << "No fighters available. Create non-healer characters first!" << std::endl;
    }
}

void game::display_healers_list()
{
    if (_characters.empty())
    {
        std::cout << "No characters available." << std::endl;

        return;
    }

    std::cout << "\nAvailable healers:" << std::endl;
    bool has_healers = false;

    for (size_t i = 0; i < _characters.size(); ++i)
    {
        if (_characters[i]->get_class_name() == "Healer")
        {
            std::cout << i + 1 << ". " << _characters[i]->get_name()
                      << " (" << _characters[i]->get_class_name() << ")" << std::endl;
            has_healers = true;
        }
    }

    if (!has_healers)
    {
        std::cout << "No healers available." << std::endl;
    }
}

int game::select_character_from_list()
{
    if (_characters.empty())
    {
        return -1;
    }

    display_character_list();
    std::cout << "Select character (1-" << _characters.size() << "): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > static_cast<int>(_characters.size()))
    {
        std::cout << "Invalid choice!" << std::endl;

        return -1;
    }

    return choice - 1;
}

int game::select_fighter_from_list()
{
    if (_characters.empty())
    {
        return -1;
    }

    display_fighters_list();

    std::vector<int> fighter_indices;

    for (size_t i = 0; i < _characters.size(); ++i)
    {
        if (_characters[i]->get_class_name() != "Healer")
        {
            fighter_indices.push_back(i);
        }
    }

    if (fighter_indices.empty())
    {
        std::cout << "No fighters available!" << std::endl;

        return -1;
    }

    std::cout << "Select fighter (1-" << fighter_indices.size() << "): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > static_cast<int>(fighter_indices.size()))
    {
        std::cout << "Invalid choice!" << std::endl;

        return -1;
    }

    return fighter_indices[choice - 1];
}

int game::select_healer_from_list(
    const std::vector<int> &excluded_indices)
{
    if (_characters.empty())
    {
        return -1;
    }

    display_healers_list();

    std::vector<int> healer_indices;
    for (size_t i = 0; i < _characters.size(); ++i)
    {
        if (_characters[i]->get_class_name() == "Healer")
        {
            if (std::find(excluded_indices.begin(), excluded_indices.end(), i) == excluded_indices.end())
            {
                healer_indices.push_back(i);
            }
        }
    }

    if (healer_indices.empty())
    {
        std::cout << "No healers available!" << std::endl;

        return -1;
    }

    std::cout << "Select healer (1-" << healer_indices.size() << "): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > static_cast<int>(healer_indices.size()))
    {
        std::cout << "Invalid choice!" << std::endl;

        return -1;
    }

    return healer_indices[choice - 1];
}

void game::create_character()
{
    std::cout << "\n    Create New Character" << std::endl;
    std::cout << "Enter character name: ";
    std::string name;
    std::getline(std::cin, name);

    if (find_character_by_name(name))
    {
        std::cout << "Character with this name already exists!" << std::endl;

        return;
    }

    std::cout << "\nSelect character class:" << std::endl;
    std::cout << "1. Defender (High HP, High Armor)" << std::endl;
    std::cout << "2. Healer (Wisdom-based, Can heal allies)" << std::endl;
    std::cout << "3. Melee Fighter (Strength-based, Moderate HP)" << std::endl;
    std::cout << "4. Ranged Fighter (Agility-based, High Accuracy)" << std::endl;
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    character *new_character = nullptr;

    switch (choice)
    {
    case 1:
        new_character = new character_defender(name);
        break;

    case 2:
        new_character = new character_healer(name);
        break;

    case 3:
        new_character = new character_melee(name);
        break;

    case 4:
        new_character = new character_ranged(name);
        break;

    default:
        std::cout << "Invalid choice!" << std::endl;
        return;
    }

    _characters.push_back(new_character);
    std::cout << "\nCharacter created successfully!" << std::endl;
    new_character->display_stats();
}

void game::generate_equipment_for_character()
{
    if (_characters.empty())
    {
        std::cout << "\nNo characters available. Create a character first!" << std::endl;

        return;
    }

    std::cout << "\n    Generate Equipment" << std::endl;
    int char_index = select_character_from_list();

    if (char_index == -1)
    {
        return;
    }

    character *character = _characters[char_index];

    std::cout << "\nHow many items to generate? ";
    int count;
    std::cin >> count;
    std::cin.ignore();

    std::cout << "\nGenerated items:" << std::endl;

    for (int i = 0; i < count; ++i)
    {
        equipment_item item = generate_random_equipment();
        std::cout << "\nItem #" << (i + 1) << ":" << std::endl;
        item.display();

        std::cout << "Equip this item? (y/n): ";
        char answer;
        std::cin >> answer;
        std::cin.ignore();

        if (answer == 'y' || answer == 'Y')
        {
            character->equip_item(item);
            std::cout << "Item equipped!" << std::endl;
        }
    }

    character->display_stats();
    character->display_equipment();
}

void game::display_all_characters()
{
    if (_characters.empty())
    {
        std::cout << "\nNo characters created yet." << std::endl;

        return;
    }

    std::cout << "\n    All Characters" << std::endl;

    for (const auto &character : _characters)
    {
        character->display_stats();
        character->display_equipment();
        std::cout << std::endl;
    }
}

bool game::attack_hits(
    const character &attacker,
    const character &defender)
{
    int hit_chance = attacker.get_total_accuracy() - 50 + attacker.get_total_luck() / 2;

    int dodge_chance = defender.get_total_luck() / 3;

    int final_hit_chance = hit_chance - dodge_chance;

    if (final_hit_chance < 10)
    {
        final_hit_chance = 10;
    }

    if (final_hit_chance > 95)
    {
        final_hit_chance = 95;
    }

    std::uniform_int_distribution<> dist(1, 100);

    return dist(_rng) <= final_hit_chance;
}

bool game::is_critical_hit(
    const character &attacker)
{
    int crit_chance = attacker.get_total_luck() / 2;

    if (crit_chance < 5)
        crit_chance = 5;
    if (crit_chance > 50)
        crit_chance = 50;

    std::uniform_int_distribution<> dist(1, 100);

    return dist(_rng) <= crit_chance;
}

double game::calculate_actual_damage(
    double base_damage,
    const character &defender, bool is_critical)
{
    if (is_critical)
    {
        base_damage *= 2.0;
    }

    double damage_reduction = defender.get_total_armor() * 0.5;
    if (damage_reduction > 75)
    {
        damage_reduction = 75;
    }

    double actual_damage = base_damage * (1.0 - damage_reduction / 100.0);

    if (actual_damage < 1.0)
    {
        actual_damage = 1.0;
    }

    return actual_damage;
}

void game::restore_all_characters()
{
    for (auto character : _characters)
    {
        character->restore_health();
    }
}

void game::start_battle()
{
    if (_characters.size() < 2)
    {
        std::cout << "\nNeed at least 2 characters for battle!" << std::endl;

        return;
    }

    int fighter_count = 0;

    for (const auto &character : _characters)
    {
        if (character->get_class_name() != "Healer")
        {
            fighter_count++;
        }
    }

    if (fighter_count < 2)
    {
        std::cout << "\nNeed at least 2 non-healer characters for battle!" << std::endl;

        return;
    }

    std::cout << "\n    Battle Setup" << std::endl;

    std::cout << "Select first fighter:" << std::endl;
    int fighter1_index = select_fighter_from_list();

    if (fighter1_index == -1)
    {
        return;
    }

    character *fighter1 = _characters[fighter1_index];

    character *healer1 = nullptr;
    std::vector<int> excluded_healers;

    std::cout << "\nSelect healer for " << fighter1->get_name() << ":" << std::endl;
    int healer1_index = select_healer_from_list(excluded_healers);

    if (healer1_index != -1)
    {
        healer1 = _characters[healer1_index];
        excluded_healers.push_back(healer1_index);
    }
    else
    {
        std::cout << "No healer selected for " << fighter1->get_name() << std::endl;
    }

    std::cout << "\nSelect second fighter:" << std::endl;
    int fighter2_index = select_fighter_from_list();

    if (fighter2_index == -1)
    {
        return;
    }

    if (fighter2_index == fighter1_index)
    {
        std::cout << "Cannot select the same character as both fighters!" << std::endl;

        return;
    }
    character *fighter2 = _characters[fighter2_index];

    character *healer2 = nullptr;

    std::cout << "\nSelect healer for " << fighter2->get_name() << ":" << std::endl;
    int healer2_index = select_healer_from_list(excluded_healers);

    if (healer2_index != -1)
    {
        healer2 = _characters[healer2_index];
    }
    else
    {
        std::cout << "No healer selected for " << fighter2->get_name() << std::endl;
    }

    std::cout << "         BATTLE STARTED!" << std::endl;

    std::cout << "\nTeam 1: " << fighter1->get_name();

    if (healer1)
    {
        std::cout << " + " << healer1->get_name() << " (Healer)";
    }
    std::cout << std::endl;

    std::cout << "Team 2: " << fighter2->get_name();
    if (healer2)
    {
        std::cout << " + " << healer2->get_name() << " (Healer)";
    }
    std::cout << std::endl
              << std::endl;

    int round = 1;
    const int max_rounds = 20;

    while (fighter1->is_alive() && fighter2->is_alive() && round <= max_rounds)
    {
        std::cout << "    Round " << round << std::endl;

        if (attack_hits(*fighter1, *fighter2))
        {
            bool crit = is_critical_hit(*fighter1);
            double base_damage = fighter1->calculate_damage();
            double actual_damage = calculate_actual_damage(base_damage, *fighter2, crit);

            fighter2->set_current_hp(fighter2->get_current_hp() - actual_damage);

            std::cout << fighter1->get_name() << " attacks " << fighter2->get_name();
            if (crit)
            {
                std::cout << " with CRITICAL HIT!";
            }
            std::cout << " for " << std::fixed << std::setprecision(1) << actual_damage << " damage!" << std::endl;
            std::cout << fighter2->get_name() << " HP: " << fighter2->get_current_hp()
                      << "/" << fighter2->get_total_health() << std::endl;
        }
        else
        {
            std::cout << fighter1->get_name() << " attacks " << fighter2->get_name()
                      << " but MISSES!" << std::endl;
        }

        if (!fighter2->is_alive())
        {
            break;
        }

        if (healer1 && fighter1->get_current_hp() < fighter1->get_total_health())
        {
            character_healer *healer_ptr = dynamic_cast<character_healer *>(healer1);
            if (healer_ptr)
            {
                double heal_amount = healer_ptr->calculate_heal();
                fighter1->set_current_hp(fighter1->get_current_hp() + heal_amount);

                std::cout << healer1->get_name() << " heals " << fighter1->get_name()
                          << " for " << std::fixed << std::setprecision(1) << heal_amount << " HP!" << std::endl;
                std::cout << fighter1->get_name() << " HP: " << fighter1->get_current_hp()
                          << "/" << fighter1->get_total_health() << std::endl;
            }
        }

        std::cout << std::endl;

        if (attack_hits(*fighter2, *fighter1))
        {
            bool crit = is_critical_hit(*fighter2);
            double base_damage = fighter2->calculate_damage();
            double actual_damage = calculate_actual_damage(base_damage, *fighter1, crit);

            fighter1->set_current_hp(fighter1->get_current_hp() - actual_damage);

            std::cout << fighter2->get_name() << " attacks " << fighter1->get_name();
            if (crit)
            {
                std::cout << " with CRITICAL HIT!";
            }
            std::cout << " for " << std::fixed << std::setprecision(1) << actual_damage << " damage!" << std::endl;
            std::cout << fighter1->get_name() << " HP: " << fighter1->get_current_hp()
                      << "/" << fighter1->get_total_health() << std::endl;
        }
        else
        {
            std::cout << fighter2->get_name() << " attacks " << fighter1->get_name()
                      << " but MISSES!" << std::endl;
        }

        if (!fighter1->is_alive())
        {
            break;
        }

        if (healer2 && fighter2->get_current_hp() < fighter2->get_total_health())
        {
            character_healer *healer_ptr = dynamic_cast<character_healer *>(healer2);
            if (healer_ptr)
            {
                double heal_amount = healer_ptr->calculate_heal();
                fighter2->set_current_hp(fighter2->get_current_hp() + heal_amount);

                std::cout << healer2->get_name() << " heals " << fighter2->get_name()
                          << " for " << std::fixed << std::setprecision(1) << heal_amount << " HP!" << std::endl;
                std::cout << fighter2->get_name() << " HP: " << fighter2->get_current_hp()
                          << "/" << fighter2->get_total_health() << std::endl;
            }
        }

        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        round++;
    }

    std::cout << "         BATTLE ENDED!" << std::endl;

    if (fighter1->is_alive() && fighter2->is_alive())
    {
        std::cout << "\nBattle reached maximum rounds! It's a DRAW!" << std::endl;
    }
    else if (fighter1->is_alive())
    {
        std::cout << "\n"
                  << fighter1->get_name() << " WINS!" << std::endl;
    }
    else
    {
        std::cout << "\n"
                  << fighter2->get_name() << " WINS!" << std::endl;
    }

    std::cout << "\nFinal Stats:" << std::endl;
    std::cout << fighter1->get_name() << " HP: " << fighter1->get_current_hp()
              << "/" << fighter1->get_total_health() << std::endl;
    std::cout << fighter2->get_name() << " HP: " << fighter2->get_current_hp()
              << "/" << fighter2->get_total_health() << std::endl;

    restore_all_characters();
}

void game::run()
{
    while (true)
    {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. Create Character" << std::endl;
        std::cout << "2. Generate Equipment" << std::endl;
        std::cout << "3. Display All Characters" << std::endl;
        std::cout << "4. Start Battle" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between 1 and 5." << std::endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            create_character();

            break;

        case 2:
            generate_equipment_for_character();

            break;

        case 3:
            display_all_characters();

            break;

        case 4:
            start_battle();

            break;

        case 5:
            std::cout << "\nTi igraesh po 22 mmov v den, na**ya, zachem?" << std::endl;

            return;

        default:
            std::cout << "Invalid choice! Please select 1-5." << std::endl;

            break;
        }
    }
}