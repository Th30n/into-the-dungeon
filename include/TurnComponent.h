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
#ifndef TURNCOMPONENT_H
#define TURNCOMPONENT_H

#include "IComponent.h"

/**
 * Each object with this component is bound by turns.
 */
class TurnComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    TurnComponent();
    virtual ~TurnComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Team number of this object.
    int team;
    // True if this object took its action this turn.
    bool turn_taken;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, TurnComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("team", comp.team);
  archive << MakeNameValuePair("turnTaken", comp.turn_taken);
}

template<class Archive>
inline void load(Archive &archive, TurnComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.team;
  archive >> comp.turn_taken;
}

} // namespace serialization

#endif
