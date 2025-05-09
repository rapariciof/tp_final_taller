#ifndef MONSTER_H
#define MONSTER_H
#include <vector>

#include "base_character.h"
#include "helper_functions.h"
#include "stdint.h"
class Monster : public BaseCharacter {
 public:
  Monster(unsigned int unique_id, int x, int y, int id, char repr, int hp,
          int level, int dps, Map *map, const std::string &name,
          const float critical_damage_multiplier, const unsigned int auto_attack_distance,
          const unsigned int auto_follow_distance);
  Monster(const Monster &) = delete;
  // Moverse si no hay enemigos cercanos (O si hay muy cerca acercarse a ellos)
  // Atacar si hay enemigo cercano (crear proyectil)
  void auto_move();
  void try_to_move_to_position(int x, int y);
  virtual ~Monster();
  virtual unsigned int receive_damage(unsigned int damage, bool critical,
                                      unsigned int weapon_origin) override;
  virtual bool is_death() override;
  virtual void notify_damage_done(BaseCharacter *other,
                                  unsigned int damage_done) override;
  const Attack attack();
  bool is_next_to(int other_x, int other_y);
  bool is_close_to(int other_x, int other_y);
  void move_closer_to(int other_x, int other_y);

 private:
  unsigned int dps;
  std::vector<std::tuple<int, int>> moves;
  std::size_t current_move = 0;
  std::vector<std::tuple<int, int>> get_possible_next_moves();
  float critical_damage_multiplier;
  unsigned int auto_attack_distance;
  unsigned int auto_follow_distance;
};

#endif  // MONSTER_H
