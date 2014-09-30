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
  level = iml::GetAttribute(node, "level", 1);
  xp = iml::GetAttribute(node, "experience", 0);
  vision_range = iml::GetAttribute(node, "visionRange", 0);

  attack = iml::GetAttribute(node, "attack", 0.0f);
  armor = iml::GetAttribute(node, "armor", 0.0f);
  health = iml::GetAttribute(node, "health", 0.0f);

  IMLNode *skills_node = node.findByName("skills");
  if (skills_node) {
    std::list<IMLNode*> &skills_iml = skills_node->getChildren();
    std::list<IMLNode*>::iterator it = skills_iml.begin();
    for (; it != skills_iml.end(); ++it) {
      if ((*it)->getName() == "skill") {
        skills.push_back(iml::GetAttribute<std::string>(**it, "name"));
      }
    }
  }
}
