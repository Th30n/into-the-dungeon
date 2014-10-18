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
#ifndef SPACECOMPONENT_H
#define SPACECOMPONENT_H

#include <cstdio>

#include "IComponent.h"

#include "Vector2f.h"

enum Facing {
  FACING_UP,
  FACING_DOWN,
  FACING_LEFT,
  FACING_RIGHT
};

/*
 * Space (position, scale, rotation) component of a game object.
 */
class SpaceComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    SpaceComponent();
    virtual ~SpaceComponent() {}
    virtual void loadIML(const IMLNode &node);
    void save(FILE *file);
    bool load(FILE *file);

    bool is_active;
    // In game position.
    Vector2f pos;
    // In game scales (how many squares are occupied)
    int width;
    int height;
    // Look direction, might not be normalized.
    Vector2f look_dir;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, SpaceComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("isActive", comp.is_active);
  archive << comp.pos;
  archive << MakeNameValuePair("width", comp.width);
  archive << MakeNameValuePair("height", comp.height);
  archive << comp.look_dir;
}

template<class Archive>
inline void load(Archive &archive, SpaceComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.is_active;
  archive >> comp.pos;
  archive >> comp.width;
  archive >> comp.height;
  archive >> comp.look_dir;
}

} // namespace serialization

#endif
