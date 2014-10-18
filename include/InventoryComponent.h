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
#ifndef INVENTORYCOMPONENT_H
#define INVENTORYCOMPONENT_H

#include "IComponent.h"

#include <vector>

/**
 * Stores GameObject ids of items and scrolls in inventory.
 */
class InventoryComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    InventoryComponent();
    virtual ~InventoryComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Maximum size of the inventory.
    unsigned max_size;
    std::vector<unsigned> items;
    std::vector<unsigned> scrolls;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, InventoryComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("maxSize", comp.max_size);
  typedef std::vector<unsigned> Items;
  Items::size_type items = comp.items.size();
  archive << MakeNameValuePair("items", items);
  for (Items::iterator it = comp.items.begin(); it != comp.items.end(); ++it) {
    archive << MakeNameValuePair("item", *it);
  }
  Items::size_type scrolls = comp.scrolls.size();
  archive << MakeNameValuePair("scrolls", scrolls);
  for (Items::iterator it = comp.scrolls.begin(); it != comp.scrolls.end(); ++it) {
    archive << MakeNameValuePair("scroll", *it);
  }
}

template<class Archive>
inline void load(Archive &archive, InventoryComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.max_size;
  typedef std::vector<unsigned> Items;
  Items::size_type items = 0;
  for (Items::size_type i = 0; i < items; ++i) {
    unsigned item = 0;
    archive >> item;
    comp.items.push_back(item);
  }
  Items::size_type scrolls = 0;
  for (Items::size_type i = 0; i < scrolls; ++i) {
    unsigned scroll = 0;
    archive >> scroll;
    comp.scrolls.push_back(scroll);
  }
}

} // namespace serialization

#endif
