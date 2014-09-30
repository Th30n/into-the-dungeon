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
#include "TurnComponent.h"

#include <cstdlib>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int TurnComponent::hreg = ComponentFactory::instance().registerCreator(
    "turnControl", TurnComponent::creator);

IComponent *TurnComponent::creator()
{
  return new TurnComponent();
}

TurnComponent::TurnComponent() : team(0), turn_taken(true)
{
}

void TurnComponent::loadIML(const IMLNode &node)
{
  team = iml::GetAttribute(node, "team", 0);
}
