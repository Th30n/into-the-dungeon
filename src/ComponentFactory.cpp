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
#include "ComponentFactory.h"

ComponentFactory *ComponentFactory::instance_ = 0;

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

ComponentFactory &ComponentFactory::instance()
{
  if (!instance_) {
    instance_ = new ComponentFactory();
  }
  return *instance_;
}

int ComponentFactory::registerCreator(const std::string &id, Creator creator)
{
  components[id] = creator;
  return components.size();
}

IComponent *ComponentFactory::create(const std::string &id) const
{
  ComponentMap::const_iterator it = components.find(id);
  if (it != components.end()) {
    return (it->second)();
  } else {
    return 0;
  }
}

std::vector<std::string> ComponentFactory::getIds() const
{
  std::vector<std::string> vec;
  for (ComponentMap::const_iterator it = components.begin();
      it != components.end(); ++it) {
    vec.push_back(it->first);
  }
  return vec;
}
