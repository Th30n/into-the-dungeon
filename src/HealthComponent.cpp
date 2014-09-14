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
#include "HealthComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int ailmentFromString(const std::string &name)
{
  if (name == "invisible") {
    return STATUS_INVISIBLE;
  } else if (name == "immobile") {
    return STATUS_IMMOBILE;
  } else if (name == "poisoned") {
    return STATUS_POISONED;
  } else if (name == "regen") {
    return STATUS_REGEN;
  } else if (name == "confused") {
    return STATUS_CONFUSED;
  } else {
    return STATUS_NONE;
  }
}

int HealthComponent::hreg = ComponentFactory::instance().registerCreator(
    "health", HealthComponent::creator);

IComponent *HealthComponent::creator()
{
  return new HealthComponent();
}

HealthComponent::HealthComponent()
  : health(0), max_health(0), is_dead(false), ailments(STATUS_NONE),
    light(0), max_light(0)
{
}

void HealthComponent::loadIML(const IMLNode &node)
{
  max_health = atoi(iml::getAttribute(node, "max", "0").c_str());
  health = max_health;
  max_light = atoi(iml::getAttribute(node, "light", "0").c_str());
  light = max_light;
  //stats_multipliers = getMapFromIML(*node.findByName("statsMultipliers"));
}

void HealthComponent::save(FILE *file)
{
}

bool HealthComponent::load(FILE *file)
{
  return true;
}
