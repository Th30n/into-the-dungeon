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
#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include "IComponent.h"

#include "GameObject.h"
#include "Vector2f.h"

typedef void (*AttackFun)(GameObject, GameObject);

/**
 * Represents a projectile.
 * Stores the target position and callback for an attack function.
 */
class ProjectileComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    ProjectileComponent();

    // Target position.
    Vector2f end;
    // Flag if it is destroyed.
    bool destroyed;
    // Time when it got destroyed.
    unsigned destroy_time;
    // Attack callback arguments.
    GameObject attacker;
    GameObject target;
    // Attack callback upon destruction.
    // Currently all projectiles always hit their targets.
    AttackFun attack_fun;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, ProjectileComponent &comp,
    unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << comp.end;
  archive << MakeNameValuePair("destroyed", comp.destroyed);
  archive << MakeNameValuePair("destroyTime", comp.destroy_time);
}

template<class Archive>
inline void load(Archive &archive, ProjectileComponent &comp,
    unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive << comp.end;
  archive >> comp.destroyed;
  archive >> comp.destroy_time;
}

} // namespace serialization

#endif
