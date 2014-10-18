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
#ifndef TRAPCOMPONENT_H
#define TRAPCOMPONENT_H

#include "IComponent.h"

#include <string>

/**
 * Trap component of an entity.
 */
class TrapComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    TrapComponent();
    virtual void loadIML(const IMLNode &node);

    // Team on which this trap is on.
    int team;
    // True if triggered.
    bool is_triggered;
    // True if destroyed and cannot be used anymore.
    bool is_destroyed;
    // Spell name which is triggered.
    std::string spell;
    // False if not visible by the player.
    bool is_visible;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, TrapComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("team", comp.team);
  archive << MakeNameValuePair("isTriggered", comp.is_triggered);
  archive << MakeNameValuePair("isDestroyed", comp.is_destroyed);
  archive << MakeNameValuePair("spell", comp.spell);
  archive << MakeNameValuePair("isVisible", comp.is_visible);
}

template<class Archive>
inline void load(Archive &archive, TrapComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.team;
  archive >> comp.is_triggered;
  archive >> comp.is_destroyed;
  archive >> comp.spell;
  archive >> comp.is_visible;
}

} // namespace serialization

#endif
