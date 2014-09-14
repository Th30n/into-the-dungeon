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
#include "StatsComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int StatsComponent::hreg = ComponentFactory::instance().registerCreator(
    "stats", StatsComponent::creator);

IComponent *StatsComponent::creator()
{
  return new StatsComponent();
}

StatsComponent::StatsComponent()
  : level(1), xp(0), vision_range(0),
    attack(0.0f), armor(0.0f), health(0.0f)
{
}

void StatsComponent::loadIML(const IMLNode &node)
{
  level = atoi(iml::getAttribute(node, "level", "1").c_str());
  xp = atoi(iml::getAttribute(node, "experience", "0").c_str());
  vision_range = atoi(iml::getAttribute(node, "visionRange", "0").c_str());

  attack = atof(iml::getAttribute(node, "attack", "0").c_str());
  armor = atof(iml::getAttribute(node, "armor", "0").c_str());
  health = atof(iml::getAttribute(node, "health", "0").c_str());

  IMLNode *skills_node = node.findByName("skills");
  if (skills_node) {
    std::list<IMLNode*> &skills_iml = skills_node->getChildren();
    std::list<IMLNode*>::iterator it = skills_iml.begin();
    for (; it != skills_iml.end(); ++it) {
      if ((*it)->getName() == "skill") {
        skills.push_back(iml::getAttribute(**it, "name"));
      }
    }
  }
}
