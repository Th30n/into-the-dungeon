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
#include "SpellComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

Effect::Effect()
  : source(0), target(0),
    damage(0), duration(0), chance(0)
{
}

void Effect::loadIML(const IMLNode &node)
{
  type = iml::getAttribute(node, "type");
  damage = atoi(iml::getAttribute(node, "damage", "0").c_str());
  duration = atoi(iml::getAttribute(node, "duration", "0").c_str());
  ailment = iml::getAttribute(node, "ailment");
  chance = atoi(iml::getAttribute(node, "chance", "0").c_str());
  spell = iml::getAttribute(node, "spell");
  stats.loadIML(node);
  entity = iml::getAttribute(node, "entity");
}

int SpellComponent::hreg = ComponentFactory::instance().registerCreator(
    "spell", SpellComponent::creator);

IComponent *SpellComponent::creator()
{
  return new SpellComponent();
}

SpellComponent::SpellComponent()
  : source(0), target(0), radius(0), bounces(0), bounce_range(0)
{
}

void SpellComponent::loadIML(const IMLNode &node)
{
  targeting = iml::getAttribute(node, "targeting");
  name = iml::getAttribute(node, "name");
  description = iml::getAttribute(node, "description");
  std::list<IMLNode*> &effects_iml = node.findByName("effects")->getChildren();
  std::list<IMLNode*>::iterator it = effects_iml.begin();
  for (; it != effects_iml.end(); ++it) {
    if ((*it)->getName() == "effect") {
      Effect effect;
      effect.loadIML(**it);
      effects.push_back(effect);
    }
  }
  IMLNode *pre = node.findByName("preSfx");
  if (pre) {
    pre_sfx.loadIML(*pre);
  }
  IMLNode *post = node.findByName("postSfx");
  if (post) {
    post_sfx.loadIML(*post);
  }
  aoe = iml::getAttribute(node, "aoe");
  radius = atoi(iml::getAttribute(node, "radius", "0").c_str());
  bounces = atoi(iml::getAttribute(node, "bounces", "0").c_str());
  bounce_range = atoi(iml::getAttribute(node, "bounceRange", "0").c_str());
}
