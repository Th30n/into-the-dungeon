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
#include "EntityManager.h"

#include <algorithm>
#include <cstdio>
#include <limits>

EntityManager *EntityManager::instance_ = 0;

EntityManager::EntityManager()
  : lowest_unassigned_eid_(1)
{
}

EntityManager::~EntityManager()
{
  removeAllEntities();
}

EntityManager &EntityManager::instance()
{
  if (!instance_) {
    instance_ = new EntityManager();
  }
  return *instance_;
}

GameObject EntityManager::createEntity()
{
  unsigned eid = generateEid();
  entities_.push_back(eid);
  //printf("Created entity: %d\n", eid);
  return GameObject(eid);
}

void EntityManager::removeEntity(const GameObject &entity)
{
  ComponentClassMap::iterator it = components_by_class_.begin();
  for (; it != components_by_class_.end(); ++it) {
    ComponentEntityMap &components = it->second;
    ComponentEntityMap::iterator comp = components.find(entity.getId());
    if (comp != components.end()) {
      //printf("Deleted %s : %d\n", it->first.c_str(), comp->first);
      IComponent *tmp = comp->second;
      components.erase(comp);
      delete tmp;
    }
  }

  entities_.erase(remove(entities_.begin(), entities_.end(), entity.getId()),
          entities_.end());
  //printf("Deleted entity: %d\n", entity.getId());
}

void EntityManager::addComponentToEntity(
    IComponent *comp, const GameObject &entity)
{
  std::string compClass = typeid(*comp).name();
  ComponentEntityMap &components = components_by_class_[compClass];
  ComponentEntityMap::iterator prev = components.find(entity.getId());
  if (prev != components.end()) {
    //printf("Deleted previous: %s\n", typeid(*(prev->second)).name());
    delete prev->second;
  }
  components[entity.getId()] = comp;
  //printf("Added component '%s' to %d\n", compClass.c_str(), entity.getId());
}

void EntityManager::reset()
{
  removeAllEntities();
  lowest_unassigned_eid_ = 1;
}

unsigned EntityManager::generateEid()
{
  if (lowest_unassigned_eid_ < std::numeric_limits<unsigned>::max()) {
    return lowest_unassigned_eid_++;
  } else {
    for (unsigned i = 1; i < std::numeric_limits<unsigned>::max(); i++) {
      EntityArrayIt it = std::find(entities_.begin(), entities_.end(), i);
      if (it != entities_.end()) {
        return i;
      }
    }
    fprintf(stderr, "No available entity IDs!\n");
    return 0;
  }
}

void EntityManager::removeAllEntities()
{
  EntityArray tmp(entities_);
  EntityArray::iterator it = tmp.begin();
  for (; it != tmp.end(); ++it) {
    removeEntity(*it);
  }
}
