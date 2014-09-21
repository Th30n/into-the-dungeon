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
#ifndef ITEMUSAGE_H
#define ITEMUSAGE_H

#include <string>
#include <vector>

#include "GameObject.h"

class ItemComponent;

/**
 * This class offers static methods for using an item in an inventory.
 * All item usage should go through here to provide unified handling.
 */
class ItemUsage {
  public:
    // Use an item, the exact effect is determined based on use type.
    static void useItem(GameObject user, GameObject item);
    // Drop an item from user's inventory.
    static void dropItem(GameObject user, GameObject item);
    // Just remove an item from given inventory collection.
    static void removeFromInventory(std::vector<unsigned> &inventory,
                    GameObject item);
  private:
    static void usePotion(GameObject user, GameObject item);
    static void useTrap(GameObject user, GameObject item);
    static bool unequip(GameObject user, ItemComponent *ic);
    static void equip(GameObject user, ItemComponent *ic);
    static void useScroll(GameObject user, GameObject scroll);
    static void targetSpell(std::string targeting, std::string &spell,
                GameObject scroll);
};
#endif
