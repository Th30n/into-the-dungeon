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
#ifndef DOTCOMPONENT_H
#define DOTCOMPONENT_H

#include "IComponent.h"

#include <string>

#include "GameObject.h"

/**
 * Represents damage over time effect.
 */
class DOTComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    DOTComponent();

    // Source of the DOT (who cast it).
    GameObject source;
    // Target of the DOT (who is affected).
    GameObject target;
    // Duration in turns.
    int duration;
    // True if infinite duration (e.g. hunger)
    bool infinite;
    // Corruption damage each turn (this game's food system).
    int light_change;
    // Health damage or bonus each turn.
    int health_change;
    // Status ailment bit flags.
    int ailments;
    // Which spell is triggered each turn.
    std::string spell;
};
#endif
