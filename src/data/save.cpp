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
#include "data/save.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "AIComponent.h"
#include "AnimationComponent.h"
#include "CArea.h"
#include "CollisionComponent.h"
#include "DamageIndicatorComponent.h"
#include "DOTComponent.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "MovementComponent.h"
#include "ParticleComponent.h"
#include "ProjectileComponent.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "SpellComponent.h"
#include "StatsComponent.h"
#include "TrapComponent.h"
#include "TurnComponent.h"
#include "WeaponComponent.h"

#include "serialization/iml/IArchive.hpp"
#include "serialization/iml/OArchive.hpp"
#include "serialization/NameValuePair.hpp"
#include "utility/typelist.hpp"

namespace data
{

// This is just a placeholder solution.
// ORDER IS IMPORTANT!!!
// TODO: Look into type traits and resolving pointer to base template.
// That should provide for a much more decoupled solution than listing all
// the component types that need to be serialized.
typedef TYPELIST_18(AIComponent, AnimationComponent, CollisionComponent,
    DamageIndicatorComponent, DOTComponent, HealthComponent,
    InventoryComponent, ItemComponent, MovementComponent, ParticleComponent,
    ProjectileComponent, RendererComponent, SpaceComponent, SpellComponent,
    StatsComponent, TrapComponent, TurnComponent, WeaponComponent)
  ComponentList;

template<class Component, class Archive>
void saveComponents(Archive &archive, EntityManager &em)
{
  std::vector<Component*> comps;
  em.getAllComponentsOfType(comps);
  typename std::vector<Component*>::size_type size = comps.size();
  archive << serialization::MakeNameValuePair("components", size);
  typename std::vector<Component*>::iterator it;
  for (it = comps.begin(); it != comps.end(); ++it) {
    archive << **it;
  }
}

template<class Component, class Archive>
void loadComponents(Archive &archive, EntityManager &em)
{
  typedef typename std::vector<Component*>::size_type SizeType;
  SizeType size = 0;
  archive >> size;
  for (SizeType i = 0; i < size; ++i) {
    Component *comp = new Component;
    archive >> *comp;
    em.addComponentToEntity(comp, comp->parent());
  }
}

// Traverses the TypeList and fetches from EntityManager components for each
// type in the list and saves it to given Archive.
// Passing invalid TypeList as first template parameter causes compile time
// error.
template<class TL, class Archive>
class ComponentSaver {};

template<class T, class Archive>
class ComponentSaver<utility::TypeList<T, utility::NullType>, Archive>
{
  public:
    void operator()(Archive &archive, EntityManager &em)
    {
      saveComponents<T>(archive, em);
    }
};

template<class T, class U, class Archive>
class ComponentSaver<utility::TypeList<T, U>, Archive>
{
  public:
    void operator()(Archive &archive, EntityManager &em)
    {
      saveComponents<T>(archive, em);
      ComponentSaver<U, Archive> saver;
      return saver(archive, em);
    }
};

template<class TL, class Archive>
class ComponentLoader {};

template<class T, class Archive>
class ComponentLoader<utility::TypeList<T, utility::NullType>, Archive>
{
  public:
    void operator()(Archive &archive, EntityManager &em)
    {
      loadComponents<T>(archive, em);
    }
};

template<class T, class U, class Archive>
class ComponentLoader<utility::TypeList<T, U>, Archive>
{
  public:
    void operator()(Archive &archive, EntityManager &em)
    {
      loadComponents<T>(archive, em);
      ComponentLoader<U, Archive> loader;
      return loader(archive, em);
    }
};

void SaveGame(EntityManager &em)
{
  using serialization::iml::OArchive;
  std::ofstream save_file("./save.sav");
  OArchive oarchive(save_file);
  oarchive << CArea::area_control;
  oarchive << em;
  ComponentSaver<ComponentList, OArchive> saver;
  saver(oarchive, em);
}

void LoadGame(EntityManager &em)
{
  using serialization::iml::IArchive;
  std::ifstream save_file("./save.sav");
  IArchive iarchive(save_file);
  iarchive >> CArea::area_control;
  iarchive >> em;
  ComponentLoader<ComponentList, IArchive> loader;
  loader(iarchive, em);
}

} // namespace data
