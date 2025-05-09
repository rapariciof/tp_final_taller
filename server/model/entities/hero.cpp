#include "hero.h"

#include "defensive_item.h"
#include "drop.h"
#include "staff.h"
#include "weapon.h"

Hero::Hero(
    unsigned int unique_id, int x, int y, unsigned int race_id, char repr,
    unsigned int level, unsigned int strength, unsigned int intelligence,
    unsigned int agility, unsigned int constitution, unsigned int f_class_hp,
    unsigned int f_race_hp, unsigned int f_race_recovery,
    unsigned int f_race_mana, unsigned int f_class_mana,
    unsigned int f_class_meditation, unsigned int gold, unsigned int class_id,
    Map *map, const std::string &name, const float critical_damage_multiplier,
    const unsigned int inventory_size, const float critical_damage_probability,
    const float evasion_probability, const float max_safe_gold_multiplier,
    const float level_up_limit_power, const float starting_xp_cap,
    const unsigned int bank_size, const int amount_of_experience_to_update,
    const bool is_new)
    : BaseCharacter(unique_id, x, y, race_id, repr,
                    constitution * f_class_hp * f_race_hp * level, level, map,
                    name),
      strength(strength),
      intelligence(intelligence),
      agility(agility),
      constitution(constitution),
      f_class_hp(f_class_hp),
      f_race_hp(f_race_hp),
      f_race_recovery(f_race_recovery),
      f_race_mana(f_race_mana),
      f_class_mana(f_class_mana),
      f_class_meditation(f_class_meditation),
      class_id(class_id),
      experience(0),
      respawn_x(0),
      respawn_y(0),
      speed_x(0),
      speed_y(0),
      meditating(false),
      ghost_mode(false),
      close_to_npc(false),
      blocked(false),
      critical_damage_multiplier(critical_damage_multiplier),
      critical_damage_probability(critical_damage_probability),
      evasion_probability(evasion_probability),
      max_safe_gold_multiplier(max_safe_gold_multiplier),
      level_up_limit_power(level_up_limit_power),
      starting_xp_cap(starting_xp_cap),
      blocked_seconds_duration(0),
      amount_of_experience_to_update(amount_of_experience_to_update) {
  if (is_new) {
    level_up();
  } else {
    max_hp = constitution * f_class_hp * f_race_hp * level;
    max_mana = intelligence * f_class_mana * f_race_mana * level;
    std::cout << "level: " << level << " int: " << intelligence
              << " f_class_mana " << f_class_mana << " f_race_mana "
              << f_race_mana << std::endl;
    max_safe_gold = max_safe_gold_multiplier * level;
  }
  equipment = new Equipment();
  inventory = new Inventory(inventory_size, gold);
  bank = new Inventory(bank_size, 0);
}

void Hero::set_speed_x(int x) { speed_x = x; }
void Hero::set_speed_y(int y) { speed_y = y; }

void Hero::auto_move() {
  if (blocked) return;
  move(x_position + speed_x, y_position + speed_y);
}

void Hero::try_to_unblock() {
  if (!blocked) return;
  auto current_time = std::chrono::high_resolution_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::seconds>(
      current_time - wait_starting_time);
  if (diff.count() >= blocked_seconds_duration) {
    revive();
    bool moved = false;
    moved = move(respawn_x, respawn_y);
    int y_diff = 0;
    int x_diff = 0;
    while (!moved) {
      // intento moverlo lo mas cercano posible
      y_diff += HelperFunctions::random_int(-1, 1);
      x_diff += HelperFunctions::random_int(-1, 1);
      moved = move(respawn_x + x_diff, respawn_y + y_diff);
    }
    respawn_x = 0;
    respawn_y = 0;
    blocked = false;
  }
}

void Hero::block(unsigned int seconds, int x, int y) {
  if (!ghost_mode) throw ModelException("Estas vivo!");
  blocked = true;
  blocked_seconds_duration = seconds;
  wait_starting_time = std::chrono::high_resolution_clock::now();
  respawn_x = x;
  respawn_y = y;
}

void Hero::regenerate() {
  if (ghost_mode) return;
  set_hp(current_hp + f_race_recovery);
  if (!meditating)
    set_mana(current_mana + f_race_recovery);
  else
    set_mana(current_mana + (f_class_meditation * intelligence));
}

void Hero::heal(unsigned int hp, unsigned int mana) {
  set_hp(current_hp + hp);
  set_mana(current_mana + mana);
}

void Hero::set_hp(unsigned int hp) { current_hp = std::min(hp, max_hp); }

void Hero::set_mana(unsigned int mana) {
  current_mana = std::min(mana, max_mana);
}

void Hero::equip_weapon(unsigned int weapon_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  meditating = false;
  if (inventory->has_item(weapon_id) && equipment->can_hold_weapon()) {
    Item *w = inventory->remove_item(weapon_id);
    equipment->equip_weapon(dynamic_cast<Weapon *>(w));
  }
}
void Hero::equip_staff(unsigned int staff_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  meditating = false;
  if (inventory->has_item(staff_id) && equipment->can_hold_staff()) {
    Item *w = inventory->remove_item(staff_id);
    equipment->equip_staff(dynamic_cast<Staff *>(w));
  }
}
void Hero::equip_shield(unsigned int shield_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  meditating = false;
  if (inventory->has_item(shield_id)) {
    Item *w = inventory->remove_item(shield_id);
    equipment->equip_shield(dynamic_cast<DefensiveItem *>(w));
  }
}
void Hero::equip_helmet(unsigned int helmet_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  meditating = false;
  if (inventory->has_item(helmet_id)) {
    Item *w = inventory->remove_item(helmet_id);
    equipment->equip_helmet(dynamic_cast<DefensiveItem *>(w));
  }
}
void Hero::equip_armour(unsigned int armour_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  meditating = false;
  if (inventory->has_item(armour_id)) {
    Item *w = inventory->remove_item(armour_id);
    equipment->equip_armour(dynamic_cast<DefensiveItem *>(w));
  }
}

void Hero::unequip(unsigned int item_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full()) throw ModelException("El inventario esta lleno!");
  meditating = false;
  Item *item = equipment->unequip(item_id);
  if (!item) throw ModelException("No tenes ese item!");
  inventory->add_item(item);
}

void Hero::unequip_weapon() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full() && equipment->has_weapon())
    throw ModelException("El inventario esta lleno!");
  meditating = false;
  Weapon *weapon = equipment->unequip_weapon();
  if (weapon) {
    inventory->add_item(weapon);
  }
}
void Hero::unequip_staff() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full() && equipment->has_staff())
    throw ModelException("El inventario esta lleno!");
  meditating = false;
  Staff *staff = equipment->unequip_staff();
  if (staff) inventory->add_item(staff);
}
void Hero::unequip_shield() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full() && equipment->has_shield())
    throw ModelException("El inventario esta lleno!");
  meditating = false;
  DefensiveItem *shield = equipment->unequip_shield();
  if (shield) inventory->add_item(shield);
}
void Hero::unequip_helmet() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full() && equipment->has_helmet())
    throw ModelException("El inventario esta lleno!");
  meditating = false;
  DefensiveItem *helmet = equipment->unequip_helmet();
  if (helmet) inventory->add_item(helmet);
}
void Hero::unequip_armour() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden equiparse o desequiparse items!");
  if (inventory->is_full() && equipment->has_armour())
    throw ModelException("El inventario esta lleno!");
  meditating = false;
  DefensiveItem *armour = equipment->unequip_armour();
  if (armour) inventory->add_item(armour);
}

void Hero::use_item(unsigned int item_id) {
  if (ghost_mode) throw ModelException("Los fantasmas no pueden usar items!");
  meditating = false;
  if (!inventory->has_item(item_id)) throw ModelException("No tiene este item!");;

  Item *i = inventory->item_with_id(item_id);
  i->use(this);
}
void Hero::use_special_staff() {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden utilizar ataques especiales!");
  Staff *s = equipment->staff;
  if (s)
    s->special_use(this);
  else
    throw ModelException("No tienes equipada una vara!");
}

void Hero::unbank_gold(unsigned int ammount) {
  if (ghost_mode)
    throw ModelException("Los fantasmas no pueden utilizar el banco!");
  meditating = false;
  if (bank->current_gold() < ammount)
    throw ModelException("No tenes esa cantidad de monedas en tu banco!");
  if (gold_space_remaining() < ammount)
    throw ModelException(
        "No podes guardar esa cantidad de monedas en tu inventario!");
  inventory->add_gold(bank->remove_gold(ammount));
}
void Hero::bank_gold(unsigned int ammount) {
  if (ghost_mode)
    throw ModelException("Los fantasmas no pueden utilizar el banco!");
  meditating = false;
  if (inventory->current_gold() < ammount)
    throw ModelException("No tenes esa cantidad de dinero en tu inventario!");
  bank->add_gold(inventory->remove_gold(ammount));
}

bool Hero::has_gold(unsigned int q) { return (inventory->current_gold() >= q); }

void Hero::remove_gold(unsigned int q) { inventory->remove_gold(q); }

void Hero::unbank_item(unsigned int item_id) {
  if (ghost_mode)
    throw ModelException("Los fantasmas no pueden utilizar el banco!");
  meditating = false;
  if (inventory->is_full())
    throw ModelException(
        "Tu inventario esta lleno, no podes retirar mas items!");
  Item *i = bank->remove_item(item_id);
  if (!i) throw ModelException("No tenes ese item en tu banco!");
  inventory->add_item(i);
}
void Hero::bank_item(unsigned int item_id) {
  if (ghost_mode)
    throw ModelException("Los fantasmas no pueden utilizar el banco!");
  meditating = false;
  if (bank->is_full())
    throw ModelException(
        "El banco esta lleno, ya no podes depositar mas items!");
  Item *i = inventory->remove_item(item_id);
  if (!i) throw ModelException("No tenes ese item en tu inventario!");
  bank->add_item(i);
}

void Hero::add_item_to_bank(Item *item) { bank->add_item(item); }
void Hero::add_gold_to_bank(unsigned int ammount) { bank->add_gold(ammount); }

Item *Hero::remove_item(unsigned int item_id) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden remover items de su inventario!");
  meditating = false;
  Item *i = inventory->remove_item(item_id);
  return i;
}

void Hero::add_item(Item *item) {
  if (ghost_mode)
    throw ModelException(
        "Los fantasmas no pueden agregar items a su inventario!");
  meditating = false;
  inventory->add_item(item);
}

bool Hero::has_free_space() { return (!inventory->is_full()); }

bool Hero::has_items_in_inventory() { return (!inventory->is_empty()); }

void Hero::add_gold(unsigned int gold) { inventory->add_gold(gold); }

void Hero::pick_up_drop(Drop *drop) {
  if (ghost_mode)
    throw ModelException("Los fantasmas no pueden agarrar cosas del suelo!");
  if ((drop->size() > 0) && (this->has_free_space())) {
    // siempre tomo el ultimo item en el drop
    Item *item = drop->take_item(drop->size());
    if (!item) return;
    this->add_item(item);
  } else if ((drop->ammount_of_gold() > 0) && this->can_hold_more_gold()) {
    unsigned int hero_gold_space = this->gold_space_remaining();
    unsigned int taken_gold = drop->take_gold(hero_gold_space);
    this->add_gold(taken_gold);
  }
}

bool Hero::can_hold_more_gold() {
  return (inventory->current_gold() < (max_safe_gold + max_safe_gold / 2));
}

unsigned int Hero::gold_space_remaining() {
  return ((max_safe_gold + max_safe_gold / 2) - inventory->current_gold());
}

bool Hero::has_excedent_coins() {
  return (inventory->current_gold() > max_safe_gold);
}

void Hero::notify_damage_done(BaseCharacter *other, unsigned int damage_done) {
  update_experience(damage_done, other);
}

const Attack Hero::attack() {
  if (ghost_mode) throw ModelException("Los fantasmas no pueden atacar!");
  if (equipment->has_weapon() || equipment->has_staff()) {
    if (!equipment->can_use_primary_weapon(this))
      throw ModelException("Mana insuficiente!");
  }
  meditating = false;
  // mover a json!
  bool critical = false;
  unsigned int dmg = calculate_damage();
  float p = rand() / double(RAND_MAX);
  if (p < critical_damage_probability) critical = true;
  Attack attack = {dmg, critical, equipment->primary_weapon_id(),
                   equipment->range()};
  equipment->attack_use_primary_weapon(this);
  return std::move(attack);
}

unsigned int Hero::receive_damage(unsigned int damage, bool critical,
                                  unsigned int weapon_origin) {
  // std::cout << "Received damage!! " << damage << " is critical? " << critical
  //           << std::endl;
  if (ghost_mode) throw ModelException("Los fantasmas no son atacables!");
  meditating = false;
  int actual_damage = damage;
  float p = pow(rand() / double(RAND_MAX), agility);
  if (critical) {
    actual_damage *= critical_damage_multiplier;
  } else if (p < evasion_probability) {
    actual_damage = 0;
  } else {
    actual_damage =
        std::max((int)damage - (int)equipment->get_defense_bonus(), 0);
  }  // Hacer chequeos si esta vivo etc?
  if (actual_damage > 0) {
    affected_by = weapon_origin;
  }

  current_hp -= actual_damage;
  if (current_hp <= 0) {
    current_hp = 0;
    ghost_mode = true;
    experience = experience/2;
  }
  return actual_damage;
}

void Hero::set_ghost_mode(const bool ghost_mode) {
  this->ghost_mode = ghost_mode;
}

void Hero::meditate() {
  if (ghost_mode) throw ModelException("No se puede meditar si estas muerto!");
  meditating = true;
}

bool Hero::is_death() { return ghost_mode; }

void Hero::revive() {
  set_hp(round(max_hp / 2));
  set_mana(round(max_mana / 2));
  ghost_mode = false;
}

class_t Hero::get_class_id() { return (class_t)class_id; }

Hero::~Hero() {
  if (inventory) delete inventory;
  if (equipment) delete equipment;
  if (bank) delete bank;
}

/* private methods */

unsigned int Hero::calculate_damage() {
  return strength * equipment->get_attack_bonus();
}

void Hero::update_experience(unsigned int dmg_done, BaseCharacter *other) {
  experience += floor(dmg_done * std::max((other->level - this->level) +
                                              amount_of_experience_to_update,
                                          (unsigned int)0));
  if (other->is_death()) {
    float p = rand() / double(RAND_MAX);
    experience += floor(
        p * other->max_hp *
        std::max((other->level - this->level) + amount_of_experience_to_update,
                 (unsigned int)0));
  }
  while (experience >= next_level_xp_limit) {
    experience -= next_level_xp_limit;
    level_up();
  }
}

void Hero::level_up() {
  level++;
  max_hp = current_hp = constitution * f_class_hp * f_race_hp * level;
  max_mana = current_mana = intelligence * f_class_mana * f_race_mana * level;
  // std::cout << "level: " << level << " int: " << intelligence
  //           << " f_class_mana " << f_class_mana << " f_race_mana "
  //           << f_race_mana << std::endl;
  next_level_xp_limit =
      starting_xp_cap * floor(pow(level, level_up_limit_power));
  max_safe_gold = max_safe_gold_multiplier * level;
}

unsigned int Hero::remove_excess_gold() {
  unsigned int excess_gold = 0;

  if (inventory->current_gold() > max_safe_gold) {
    excess_gold = inventory->current_gold() - max_safe_gold;
    inventory->remove_gold(excess_gold);
  }
  return excess_gold;
}

void Hero::set_close_to_npc(bool val) { close_to_npc = val; }

void Hero::set_next_level_xp_limit(unsigned int next_level_xp_limit) {
  this->next_level_xp_limit = next_level_xp_limit;
}

void Hero::set_experience(unsigned int experience) {
  this->experience = experience;
}
