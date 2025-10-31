#include "../include/characters.h"

#include <iostream>
#include <iomanip>

equipment_item::equipment_item(
    equipment_slot slot,
    const std::string &name,
    int health_bonus,
    int armor_bonus,
    int accuracy_bonus,
    int luck_bonus,
    int mastery_bonus)
    : _slot(slot),
      _name(name),
      _health_bonus(health_bonus),
      _armor_bonus(armor_bonus),
      _accuracy_bonus(accuracy_bonus),
      _luck_bonus(luck_bonus),
      _mastery_bonus(mastery_bonus)
{
}

equipment_slot equipment_item::get_slot() const
{
    return _slot;
}

std::string equipment_item::get_name() const
{
    return _name;
}

int equipment_item::get_health_bonus() const
{
    return _health_bonus;
}

int equipment_item::get_armor_bonus() const
{
    return _armor_bonus;
}

int equipment_item::get_accuracy_bonus() const
{
    return _accuracy_bonus;
}

int equipment_item::get_luck_bonus() const
{
    return _luck_bonus;
}

int equipment_item::get_mastery_bonus() const
{
    return _mastery_bonus;
}

void equipment_item::display() const
{
    std::cout << "  " << _name;
    if (_health_bonus > 0)
        std::cout << " [HP+" << _health_bonus << "]";
    if (_armor_bonus > 0)
        std::cout << " [Armor+" << _armor_bonus << "]";
    if (_accuracy_bonus > 0)
        std::cout << " [Acc+" << _accuracy_bonus << "]";
    if (_luck_bonus > 0)
        std::cout << " [Luck+" << _luck_bonus << "]";
    if (_mastery_bonus > 0)
        std::cout << " [Mast+" << _mastery_bonus << "]";
    std::cout << std::endl;
}

character::character(
    const std::string &name,
    int health,
    int armor,
    int accuracy,
    int luck,
    int mastery)
    : _name(name),
      _base_health(health),
      _base_armor(armor),
      _base_accuracy(accuracy),
      _base_luck(luck),
      _base_mastery(mastery),
      _current_hp(0)
{
    _current_hp = get_total_health();
}

std::string character::get_name() const
{
    return _name;
}

double character::get_current_hp() const
{
    return _current_hp;
}

void character::set_current_hp(
    double hp)
{
    _current_hp = hp;
    if (_current_hp > get_total_health())
    {
        _current_hp = get_total_health();
    }

    if (_current_hp < 0)
    {
        _current_hp = 0;
    }
}

void character::restore_health()
{
    _current_hp = get_total_health();
}

bool character::is_alive() const
{
    return _current_hp > 0;
}

int character::get_total_health() const
{
    int total = _base_health;

    for (const auto &item : _equipment)
    {
        total += item.get_health_bonus();
    }

    return total;
}

int character::get_total_armor() const
{
    int total = _base_armor;

    for (const auto &item : _equipment)
    {
        total += item.get_armor_bonus();
    }

    return total;
}

int character::get_total_accuracy() const
{
    int total = _base_accuracy;

    for (const auto &item : _equipment)
    {
        total += item.get_accuracy_bonus();
    }

    return total;
}

int character::get_total_luck() const
{
    int total = _base_luck;

    for (const auto &item : _equipment)
    {
        total += item.get_luck_bonus();
    }

    return total;
}

int character::get_total_mastery() const
{
    int total = _base_mastery;

    for (const auto &item : _equipment)
    {
        total += item.get_mastery_bonus();
    }

    return total;
}

void character::equip_item(
    const equipment_item &item)
{
    for (auto it = _equipment.begin(); it != _equipment.end(); ++it)
    {
        if (it->get_slot() == item.get_slot())
        {
            _equipment.erase(it);

            break;
        }
    }

    double hp_percentage = _current_hp / get_total_health();
    _equipment.push_back(item);
    _current_hp = get_total_health() * hp_percentage;
}

void character::display_stats() const
{
    std::cout << "\n    " << _name << " (" << get_class_name() << ")" << std::endl;
    std::cout << "HP: " << std::fixed << std::setprecision(1) << _current_hp
              << "/" << get_total_health() << std::endl;
    std::cout << "Armor: " << get_total_armor() << std::endl;
    std::cout << "Accuracy: " << get_total_accuracy() << std::endl;
    std::cout << "Luck: " << get_total_luck() << std::endl;
    std::cout << "Mastery: " << get_total_mastery() << std::endl;
    std::cout << "Main Attribute: " << get_main_attribute() << std::endl;
}

void character::display_equipment() const
{
    std::cout << "\nEquipment:" << std::endl;

    if (_equipment.empty())
    {
        std::cout << "  (none)" << std::endl;
    }
    else
    {
        for (const auto &item : _equipment)
        {
            item.display();
        }
    }
}

character_defender::character_defender(
    const std::string &name)
    : character(name, 300, 50, 70, 50, 40),
      _defense_multiplier(2)
{
}

double character_defender::calculate_damage() const
{
    return 15.0 + get_main_attribute() * 0.3 + get_total_mastery() * 0.2;
}

std::string character_defender::get_class_name() const
{
    return "Defender";
}

int character_defender::get_main_attribute() const
{
    return get_total_health() / 10;
}

character_healer::character_healer(
    const std::string &name)
    : character(name, 200, 20, 75, 60, 50),
      _wisdom(80)
{
}

double character_healer::calculate_damage() const
{
    return 10.0 + get_main_attribute() * 0.2 + get_total_mastery() * 0.15;
}

double character_healer::calculate_heal() const
{
    return 25.0 + get_main_attribute() * 0.6 + get_total_mastery() * 0.3;
}

std::string character_healer::get_class_name() const
{
    return "Healer";
}

int character_healer::get_main_attribute() const
{
    return _wisdom;
}

character_melee::character_melee(
    const std::string &name)
    : character(name, 250, 30, 80, 55, 45),
      _strength(70)
{
}

double character_melee::calculate_damage() const
{
    return 20.0 + get_main_attribute() * 0.4 + get_total_mastery() * 0.25;
}

std::string character_melee::get_class_name() const
{
    return "Melee Fighter";
}

int character_melee::get_main_attribute() const
{
    return _strength;
}

character_ranged::character_ranged(
    const std::string &name)
    : character(name, 220, 25, 85, 65, 50),
      _agility(75)
{
}

double character_ranged::calculate_damage() const
{
    return 18.0 + get_main_attribute() * 0.35 + get_total_mastery() * 0.22;
}

std::string character_ranged::get_class_name() const
{
    return "Ranged Fighter";
}

int character_ranged::get_main_attribute() const
{
    return _agility;
}