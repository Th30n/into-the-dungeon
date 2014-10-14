/*
 * Into the Dungeon++
 *
 * Copyright (C) 2014 Teon Banek
 *
 * This file is part of Into the Dungeon++.
 * Into the Dungeon++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Into the Dungeon++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Into the Dungeon++. If not, see <http://www.gnu.org/licenses/>
 *
 * Author(s): Teon Banek <intothedungeon@gmail.com>
 */
#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <cstdio>
#include <map>
#include <string>

#include "IComponent.h"

class DamageIndicatorComponent;

/**
 * Bit flags of status ailments and possibly buffs.
 */
enum StatusAilments {
  STATUS_NONE      = 0x00000000,
  STATUS_INVISIBLE = 0x00000001,
  STATUS_IMMOBILE  = 0x00000002,
  STATUS_POISONED  = 0x00000004,
  STATUS_REGEN     = 0x00000008,
  STATUS_CONFUSED  = 0x00000010,
};

int ailmentFromString(const std::string &name);

/*
 * Health component of a game object.
 * Stores health, light (corruption/food) and 
 * status ailments bit flags.
 */
class HealthComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    HealthComponent();
    virtual ~HealthComponent() {}
    virtual void loadIML(const IMLNode &node);
    void save(FILE *file);
    bool load(FILE *file);

    int health;
    int max_health;
    bool is_dead;
    int ailments;
    int light;
    int max_light;
    // Not used at the moment
    //std::map<std::string, float> stats_multipliers;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, HealthComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("health", comp.health);
  archive << MakeNameValuePair("maxHealth", comp.max_health);
  archive << MakeNameValuePair("isDead", comp.is_dead);
  archive << MakeNameValuePair("ailments", comp.ailments);
  archive << MakeNameValuePair("light", comp.light);
  archive << MakeNameValuePair("maxLight", comp.max_light);
}

template<class Archive>
inline void load(Archive &archive, HealthComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.health;
  archive >> comp.max_health;
  archive >> comp.is_dead;
  archive >> comp.ailments;
  archive >> comp.light;
  archive >> comp.max_light;
}

} // namespace serialization
#endif

