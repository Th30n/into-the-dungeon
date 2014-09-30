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
#include "ItemComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int ItemComponent::hreg = ComponentFactory::instance().registerCreator(
    "item", ItemComponent::creator);

IComponent *ItemComponent::creator()
{
  return new ItemComponent();
}

ItemComponent::ItemComponent()
  : itemLevel(1), useType(USE_TYPE_POTION), rareType(RARE_TYPE_WHITE),
    spriteId(0), identified(true), cursed(false), equipped(false)
{
}

static ItemComponent::UseTypes stringToUseType(const std::string &name)
{
  ItemComponent::UseTypes useType = ItemComponent::USE_TYPE_POTION;
  if (name == "potion") {
    useType = ItemComponent::USE_TYPE_POTION;
  } else if  (name == "trap") {
    useType = ItemComponent::USE_TYPE_TRAP;
  } else if (name == "scroll") {
    useType = ItemComponent::USE_TYPE_SCROLL;
  } else if (name == "weapon") {
    useType = ItemComponent::EQ_TYPE_WEAPON;
  } else if (name == "shield") {
    useType = ItemComponent::EQ_TYPE_SHIELD;
  } else if (name == "helmet") {
    useType = ItemComponent::EQ_TYPE_HELMET;
  } else if (name == "armor") {
    useType = ItemComponent::EQ_TYPE_ARMOR;
  } else if (name == "boots") {
    useType = ItemComponent::EQ_TYPE_BOOTS;
  } else if (name == "ring") {
    useType = ItemComponent::EQ_TYPE_RING;
  } else if (name == "amulet") {
    useType = ItemComponent::EQ_TYPE_AMULET;
  } else if (name == "cratebox") {
    useType = ItemComponent::EQ_TYPE_CBOX;
  }
  return useType;
}

void ItemComponent::loadIML(const IMLNode &node)
{
  itemLevel = iml::GetAttribute(node, "level", 0);
  name = iml::GetAttribute<std::string>(node, "name");
  spell = iml::GetAttribute<std::string>(node, "spell");
  description = iml::GetAttribute<std::string>(node, "description");
  IMLNode *stats = node.findByName("statsModifiers");
  if (stats) {
    statsModifiers.loadIML(*stats);
  }
  spriteId = iml::GetAttribute(node, "spriteId", 0);
  useType = stringToUseType(iml::GetAttribute<std::string>(node, "type"));
}
