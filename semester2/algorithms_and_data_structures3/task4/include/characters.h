#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <string>
#include <vector>

enum class equipment_slot
{
    helmet,
    shoulder,
    chest,
    gloves,
    cloak
};

class equipment_item final
{
private:
    equipment_slot _slot;
    std::string _name;
    int _health_bonus;
    int _armor_bonus;
    int _accuracy_bonus;
    int _luck_bonus;
    int _mastery_bonus;

public:
    equipment_item(
        equipment_slot slot,
        const std::string &name,
        int health_bonus,
        int armor_bonus,
        int accuracy_bonus,
        int luck_bonus,
        int mastery_bonus);

    equipment_slot get_slot() const;

    std::string get_name() const;

    int get_health_bonus() const;

    int get_armor_bonus() const;

    int get_accuracy_bonus() const;

    int get_luck_bonus() const;

    int get_mastery_bonus() const;

    void display() const;
};

class character
{
protected:
    std::string _name;
    int _base_health;
    int _base_armor;
    int _base_accuracy;
    int _base_luck;
    int _base_mastery;

    double _current_hp;
    std::vector<equipment_item> _equipment;

public:
    character(
        const std::string &name,
        int health,
        int armor,
        int accuracy,
        int luck,
        int mastery);

    virtual ~character() = default;

    std::string get_name() const;

    double get_current_hp() const;

    void set_current_hp(
        double hp);

    void restore_health();

    bool is_alive() const;

    int get_total_health() const;

    int get_total_armor() const;

    int get_total_accuracy() const;

    int get_total_luck() const;

    int get_total_mastery() const;

    void equip_item(
        const equipment_item &item);

    void display_stats() const;

    void display_equipment() const;

    virtual double calculate_damage() const = 0;

    virtual std::string get_class_name() const = 0;

    virtual int get_main_attribute() const = 0;
};

class character_defender final : public character
{
private:
    int _defense_multiplier;

public:
    character_defender(
        const std::string &name);

    double calculate_damage() const override;

    std::string get_class_name() const override;

    int get_main_attribute() const override;
};

class character_healer : public character
{
private:
    int _wisdom;

public:
    character_healer(
        const std::string &name);

    double calculate_damage() const override;

    double calculate_heal() const;

    std::string get_class_name() const override;

    int get_main_attribute() const override;
};

class character_melee final : public character
{
private:
    int _strength;

public:
    character_melee(
        const std::string &name);

    double calculate_damage() const override;

    std::string get_class_name() const override;

    int get_main_attribute() const override;
};

class character_ranged final : public character
{
private:
    int _agility;

public:
    character_ranged(
        const std::string &name);

    double calculate_damage() const override;

    std::string get_class_name() const override;

    int get_main_attribute() const override;
};

#endif // CHARACTERS_H