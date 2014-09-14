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
#ifndef ITEMCOMPONENT_H
#define ITEMCOMPONENT_H

#include "IComponent.h"

#include <string>

#include "StatsComponent.h"

/**
 * Component which describes an item.
 */
class ItemComponent: public IComponent {
  public:
    /**
     * Enumeration of all possible usage types of an item.
     * Each item is either a one time use or a certain equipment type.
     */
    enum UseTypes {
      USE_TYPE_POTION, // one time use potion (gain stats modifier perm.)
      USE_TYPE_TRAP, // it's a trap
      USE_TYPE_SCROLL, // scroll with a spell effect
      EQ_TYPE_WEAPON, // weapon slot
      EQ_TYPE_SHIELD, // shield slot
      EQ_TYPE_HELMET, // helmet slot
      EQ_TYPE_ARMOR, // armor slot
      EQ_TYPE_BOOTS, // boots slot
      EQ_TYPE_RING, // ring slot
      EQ_TYPE_AMULET, // amulet slot
      EQ_TYPE_CBOX // special rare crate box item
    };

    /**
     * Enumeration of rarity of an item.
     */
    enum RareTypes {
      RARE_TYPE_WHITE,
      RARE_TYPE_BLUE,
      RARE_TYPE_GREEN
    };

    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    ItemComponent();
    virtual ~ItemComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Level of an item (used in generation).
    int itemLevel;
    // Item name which is displayed.
    std::string name;
    // Item display description.
    std::string description;
    // Spell name which is triggered on use.
    std::string spell;
    // Stat bonuses (used currently only with equipment).
    StatsComponent statsModifiers;
    UseTypes useType;
    RareTypes rareType;
    // Id number of the image in spritesheet items.png
    int spriteId;
    // Item flags.
    bool identified;
    bool cursed;
    bool equipped;
};
#endif
