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
#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <typeinfo>

#include "IComponent.h"
#include "GameObject.h"
#include "serialization/NameValuePair.hpp"

/**
 * Manages objects and components.
 */
class EntityManager {
  public:
    static EntityManager &instance();
    // Creates a new entity and returns reference to it.
    GameObject createEntity();
    // Removes (and deletes) the referenced entity from manager.
    void removeEntity(const GameObject &entity);
    // Add component to object, previous component of same type is deleted.
    void addComponentToEntity(IComponent *comp, const GameObject &entity);
    // Returns the component of T type or null.
    template<class T>
    T *getComponentForEntity(const GameObject &entity);
    // Fills the vector with objects having component of T type.
    template<class T>
    void getEntitiesWithComponent(std::vector<GameObject> &entities);
    // Fills the vector with all components of T type.
    template<class T>
    void getAllComponentsOfType(std::vector<T*> &components);
    // Clear every entity and start from 0.
    void reset();
    // Serializes the entity manager to archive.
    template<class Archive>
    inline void save(Archive &archive, unsigned int version);
    // Deserializes the entity manager from archive.
    template<class Archive>
    inline void load(Archive &archive, unsigned int version);
  
  private:
    typedef std::map<unsigned, IComponent*> ComponentEntityMap;
    typedef std::map<std::string, ComponentEntityMap> ComponentClassMap;
    typedef std::vector<unsigned> EntityArray;
    typedef EntityArray::iterator EntityArrayIt;

    EntityManager();
    EntityManager(const EntityManager &em);
    ~EntityManager();
    EntityManager &operator=(const EntityManager &em);
    unsigned generateEid();
    void removeAllEntities();

    static EntityManager *instance_;
    unsigned lowest_unassigned_eid_;
    EntityArray entities_; // entity IDs
    ComponentClassMap components_by_class_;
};

template<class T>
T *EntityManager::getComponentForEntity(const GameObject &entity)
{
  ComponentClassMap::iterator ccIt = components_by_class_.find(typeid(T).name());
  if (ccIt != components_by_class_.end()) {
    ComponentEntityMap &components = ccIt->second;
    ComponentEntityMap::iterator compIt = components.find(entity.getId());
    if (compIt != components.end()) {
      return static_cast<T*>(compIt->second);
    }
  }
  return 0;
}

template<class T>
void EntityManager::getEntitiesWithComponent(std::vector<GameObject> &entities)
{
  ComponentClassMap::iterator ccIt = components_by_class_.find(typeid(T).name());
  if (ccIt != components_by_class_.end()) {
    ComponentEntityMap &components = ccIt->second;
    ComponentEntityMap::iterator compIt = components.begin();
    for (; compIt != components.end(); ++compIt) {
      entities.push_back(GameObject(compIt->first));
    }
  }
}

template<class T>
void EntityManager::getAllComponentsOfType(std::vector<T*> &component)
{
  ComponentClassMap::iterator ccIt = components_by_class_.find(typeid(T).name());
  if (ccIt != components_by_class_.end()) {
    ComponentEntityMap &componentsByEntity = ccIt->second;
    ComponentEntityMap::iterator compIt = componentsByEntity.begin();
    for (; compIt != componentsByEntity.end(); ++compIt) {
      component.push_back(static_cast<T*>(compIt->second));
    }
  }
}

// Serializes the entity manager to archive.
template<class Archive>
inline void EntityManager::save(Archive &archive, unsigned int version)
{
  using serialization::MakeNameValuePair;
  archive << MakeNameValuePair("lowestUnassignedId", lowest_unassigned_eid_);
  EntityArray::size_type entities = entities_.size();
  archive << MakeNameValuePair("entities", entities);
  for (EntityArrayIt it = entities_.begin(); it != entities_.end(); ++it) {
    archive << MakeNameValuePair("id", *it);
  }
}
// Deserializes the entity manager from archive.
template<class Archive>
inline void EntityManager::load(Archive &archive, unsigned int version)
{
  reset();
  archive >> lowest_unassigned_eid_;
  EntityArray::size_type entities = 0;
  archive >> entities;
  for (EntityArray::size_type i = 0; i < entities; ++i) {
    unsigned id = 0;
    archive >> id;
    entities_.push_back(id);
  }
}

#endif
