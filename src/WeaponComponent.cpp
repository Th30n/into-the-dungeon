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
#include "WeaponComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int WeaponComponent::hreg = ComponentFactory::instance().registerCreator(
    "weapon", WeaponComponent::creator);

IComponent *WeaponComponent::creator()
{
  return new WeaponComponent();
}

WeaponComponent::WeaponComponent()
  : min_range(1), max_range(1), min_damage(1), max_damage(1), target_id(0),
    accuracy(0), crit_chance(0)
{
}

void WeaponComponent::loadIML(const IMLNode &node)
{
  min_range = iml::GetAttribute(node, "minRange", 0);
  max_range = iml::GetAttribute(node, "maxRange", 0);
  min_damage = iml::GetAttribute(node, "minDamage", 0);
  max_damage = iml::GetAttribute(node, "maxDamage", 0);
  accuracy = iml::GetAttribute(node, "accuracy", 0);
  crit_chance = iml::GetAttribute(node, "critChance", 0);
  spell = iml::GetAttribute<std::string>(node, "spell");
  IMLNode *stats = node.findByName("statsMultipliers");
  if (stats) {
    stats_multipliers.loadIML(*stats);
  }
  IMLNode *proj = node.findByName("projectile");
  if (proj) {
    projectile_sfx.loadIML(*proj);
  }
}

void WeaponComponent::save(FILE *file)
{
}

bool WeaponComponent::load(FILE *file)
{
  return true;
}
