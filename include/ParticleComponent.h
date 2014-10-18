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
#ifndef PARTICLECOMPONENT_H
#define PARTICLECOMPONENT_H

#include "IComponent.h"

#include "GameObject.h"
#include "Vector2f.h"

typedef void (*AttackFun)(GameObject, GameObject);

/**
 * Represents a particle.
 * This should be the basis for new particle system.
 */
class ParticleComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    ParticleComponent();

    // Time this particle was created.
    unsigned start_time;
    // Time before it is destroyed.
    unsigned time_to_live;
    // Velocity of the particle
    Vector2f velocity;
    // Arguments to attack function.
    GameObject attacker;
    GameObject target;
    // Function which is called upon particle destruction.
    // Useful for delayed attack callbacks (e.g. after an animation).
    AttackFun attack_fun;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, ParticleComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("startTime", comp.start_time);
  archive << MakeNameValuePair("timeToLive", comp.time_to_live);
  archive << comp.velocity;
}

template<class Archive>
inline void load(Archive &archive, ParticleComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.start_time;
  archive >> comp.time_to_live;
  archive >> comp.velocity;
}

} // namespace serialization

#endif
