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
#ifndef STATSCOMPONENT_H
#define STATSCOMPONENT_H

#include "IComponent.h"

#include <vector>
#include <string>

/**
 * Stores stats of an object.
 * It is also used in other components as a convenient stat holder.
 */
class StatsComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    StatsComponent();
    virtual ~StatsComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Character level.
    int level;
    // Current experience amount.
    int xp;
    // Vision range in squares.
    int vision_range;
    // Primary stats, perhaps should be a struct of its own.
    float attack;
    float armor;
    float health;
    // List of available skills/spells.
    // Currently used by the AI, should be expanded.
    std::vector<std::string> skills;
};
#endif
