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
#include "SpaceComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int SpaceComponent::hreg = ComponentFactory::instance().registerCreator(
    "space", SpaceComponent::creator);

IComponent *SpaceComponent::creator()
{
  return new SpaceComponent();
}

SpaceComponent::SpaceComponent()
  : is_active(true),
    pos(), width(1), height(1), look_dir(0.0f, 1.0f)
{
}

void SpaceComponent::loadIML(const IMLNode &node)
{
  width = atoi(iml::getAttribute(node, "width", "1").c_str());
  height = atoi(iml::getAttribute(node, "height", "1").c_str());
}

void SpaceComponent::save(FILE *file)
{
}

bool SpaceComponent::load(FILE *file)
{
  return true;
}
