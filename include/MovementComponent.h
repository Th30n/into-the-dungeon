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
#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <cstdio>

#include "IComponent.h"

#include "Vector2f.h"

/*
 * Movement component of a game object.
 * Stores speed and destination waypoint.
 */
class MovementComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    MovementComponent();
    virtual ~MovementComponent() {}
    virtual void loadIML(const IMLNode &node);
    void save(FILE *file);
    bool load(FILE *file);

    // Speed, technically tiles per second.
    float speed;
    // Destination waypoint.
    Vector2f waypoint;
};
#endif

