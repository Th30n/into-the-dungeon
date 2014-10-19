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
#include "TrapComponent.h"

#include <list>

#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"

int TrapComponent::hreg = ComponentFactory::instance().registerCreator(
    "trap", TrapComponent::creator);

IComponent *TrapComponent::creator()
{
  return new TrapComponent();
}

TrapComponent::TrapComponent()
  : team(1), is_triggered(false), is_destroyed(false), is_visible(false)
{
}

void TrapComponent::loadIML(const IMLNode &node)
{
  spell = iml::GetAttribute(node, "spell");
}
