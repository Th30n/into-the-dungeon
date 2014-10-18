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
#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "IComponent.h"

#include <cstdio>

#include "Vector2f.h"

/**
 * This component represents AI of an entity.
 * Contains waypoint which represents the last known
 * target (usually player) position.
 * If the entity has spells, casting them is determined
 * by cast rate variable.
 */
class AIComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;
    
    // Invalid map position.
    static const Vector2f RESET_POSITION;

    AIComponent();
    virtual ~AIComponent() {}
    virtual void loadIML(const IMLNode &node);
    virtual void save(FILE *file);
    virtual bool load(FILE *file);

    // Last known target position.
    Vector2f waypoint;
    // Spell cast rate (0-100) in percentage.
    int cast_rate;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, AIComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << comp.waypoint;
  archive << MakeNameValuePair("castRate", comp.cast_rate);
}

template<class Archive>
inline void load(Archive &archive, AIComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.waypoint;
  archive >> comp.cast_rate;
}

} // namespace serialization
#endif

