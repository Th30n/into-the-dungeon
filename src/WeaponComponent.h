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
#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "IComponent.h"

#include <cstdio>
#include <string>

#include "Sfx.h"
#include "StatsComponent.h"

/*
 * Weapon component of a game object.
 * Stores weapon range, damage and other details.
 */
class WeaponComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    WeaponComponent();
    virtual ~WeaponComponent() {}
    virtual void loadIML(const IMLNode &node);
    void save(FILE *file);
    bool load(FILE *file);

    // Range values in squares.
    int min_range;
    int max_range;
    // Damage range.
    int min_damage;
    int max_damage;
    // How much the stats affect the damage values.
    StatsComponent stats_multipliers;
    // Id of the targeted GameObject.
    unsigned target_id;
    // Accuracy in percentage (0-100).
    int accuracy;
    // Critical hit chance in percentage (0-100).
    int crit_chance;
    // Name of the spell which is triggered on attack.
    std::string spell;
    // Projectile effect which is spawned on attack.
    Sfx projectile_sfx;
};
#endif

