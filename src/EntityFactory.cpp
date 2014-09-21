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
#include "EntityFactory.h"

#include "CArea.h"
#include "ComponentFactory.h"
#include "CollisionComponent.h"
#include "CTile.h"
#include "data/dirs.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "iml/IMLNode.h"
#include "iml/IMLParser.h"
#include "iml/IMLTag.h"
#include "iml/utils.h"
#include "ItemComponent.h"
#include "MovementComponent.h"
#include "Rectangle.h"
#include "SpaceComponent.h"
#include "TurnComponent.h"

EntityFactory *EntityFactory::instance_ = 0;

EntityFactory::EntityFactory()
  : entities_iml_(0), items_iml_(0), traps_iml_(0), spells_iml_(0)
{
}

EntityFactory::~EntityFactory()
{
  if (entities_iml_) {
    delete entities_iml_;
  }
  if (items_iml_) {
    delete items_iml_;
  }
  if (traps_iml_) {
    delete traps_iml_;
  }
  if (spells_iml_) {
    delete spells_iml_;
  }
}

EntityFactory &EntityFactory::instance()
{
  if (!instance_) {
    // I don't think it needs deletion since it lasts till end of program.
    instance_ = new EntityFactory();
    instance_->loadGameData();
  }
  return *instance_;
}

GameObject EntityFactory::createEntity(const std::string &name) const
{
  return createObjectFromIML(name, entities_iml_);
}

GameObject EntityFactory::createItem(const std::string &name) const
{
  return createObjectFromIML(name, items_iml_);
}

GameObject EntityFactory::createTrap(const std::string &name) const
{
  return createObjectFromIML(name, traps_iml_);
}

GameObject EntityFactory::createSpell(const std::string &name) const
{
  return createObjectFromIML(name, spells_iml_);
}

GameObject EntityFactory::createDOT() const
{
  EntityManager &em = EntityManager::instance();
  GameObject entity = em.createEntity();
  ComponentFactory &cf = ComponentFactory::instance();
  IComponent *dot = cf.create("dot");
  em.addComponentToEntity(dot, entity);
  IComponent *turn = cf.create("turnControl");
  em.addComponentToEntity(turn, entity);
  return entity;
}

GameObject EntityFactory::spawn(GameObject object) const
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(object);
  if (!sc) {
    sc = new SpaceComponent();
    em.addComponentToEntity(sc, object);
  }
  CollisionComponent *col = em.
      getComponentForEntity<CollisionComponent>(object);
  bool set = false;
  int x = 0;
  int y = 0;
  do {
    x = rand() % MAP_WIDTH;
    y = rand() % MAP_HEIGHT;

    CTile *tile = CArea::area_control.GetTile(x * TILE_SIZE, y * TILE_SIZE);
    
    if (tile->type_id == 1) {
      if (col && colliderAt(Rectangle(x, y, sc->width, sc->height))) {
        set = false;
      } else {
        sc->pos.x = x;
        sc->pos.y = y;
        set = true;
      }
    }
  } while (!set);
  MovementComponent *mc = em.getComponentForEntity<MovementComponent>(object);
  if (mc) {
    mc->waypoint = sc->pos;
  }
  return object;
}

GameObject EntityFactory::spawn(GameObject object, Vector2f pos) const
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(object);
  if (!sc) {
    sc = new SpaceComponent();
    em.addComponentToEntity(sc, object);
  }
  sc->pos = pos;
  MovementComponent *mc = em.getComponentForEntity<MovementComponent>(object);
  if (mc) {
    mc->waypoint = sc->pos;
  }
  return object;
}

GameObject EntityFactory::createRandomItem(int min_level, int max_level) const
{
  int obj_lvl = 0;
  int picked = 0;
  bool ignore = false;
  //creates based on depth
  do
  {
    ignore = false;
    picked = rand() % item_list_.size();
    // Ignore entities with ignore flag in xml.
    if (iml::getAttribute(*item_list_[picked], "ignore", "0") == "1") {
      ignore = true;
      continue;
    }
    IMLNode *item = item_list_[picked]->findByName("item");
    obj_lvl = atoi(iml::getAttribute(*item, "level", "0").c_str());
  } while (ignore || obj_lvl < min_level || obj_lvl > max_level);

  GameObject go = createItem(item_list_[picked]->getName());
  if (go.getId() == 0) {
    return 0;
  }
  EntityManager &em = EntityManager::instance();
  ItemComponent *item = em.getComponentForEntity<ItemComponent>(go);
  if (!item) {
    fprintf(stderr, "Invalid item created. Missing ItemComponent!\n");
    em.removeEntity(go);
    return 0;
  }
  randomizeItemStats(item);
  return go;
}

GameObject EntityFactory::createRandomEntity(int min_level, int max_level) const
{
  int obj_lvl = 0;
  int picked = 0;
  bool ignore = false;
  //creates based on depth
  do
  {
    ignore = false;
    picked = rand() % entity_list_.size();
    // Ignore entities with ignore flag in xml.
    if (iml::getAttribute(*entity_list_[picked], "ignore", "0") == "1") {
      ignore = true;
      continue;
    }
    IMLNode *stats = entity_list_[picked]->findByName("stats");
    obj_lvl = atoi(iml::getAttribute(*stats, "level", "0").c_str());
  } while (ignore || obj_lvl < min_level || obj_lvl > max_level);
  return createEntity(entity_list_[picked]->getName());
}

GameObject EntityFactory::createRandomTrap(int min_level, int max_level) const
{
  int picker = rand() % 3;
  switch (picker) {
  case 0:
    return createTrap("poisonTrap");
  case 1:
    return createTrap("spikeTrap");
  case 2:
    return createTrap("stunTrap");
  }
  return 0;
}

void EntityFactory::loadGameData()
{
  entities_iml_ = iml::openIML(data::FindFile("data/entities.xml").c_str());
  entity_list_ = iml::getChildrenTags(*entities_iml_->findByName("entities"));
  //entities_iml_->print();

  items_iml_ = iml::openIML(data::FindFile("data/items.xml").c_str());
  item_list_ = iml::getChildrenTags(*items_iml_->findByName("items"));
  //items_iml_->print();

  traps_iml_ = iml::openIML(data::FindFile("data/traps.xml").c_str());
  //traps_iml_->print();

  spells_iml_ = iml::openIML(data::FindFile("data/spells.xml").c_str());
  //spells_iml_->print();
}


GameObject EntityFactory::createObjectFromIML(
    const std::string &name, const IMLNode *iml) const
{
  //printf("EntityFactory::createObjectFromIML: '%s'\n", name.c_str());
  IMLNode *node = iml->findByName(name);
  if (!node || name.empty()) {
    fprintf(stderr, "Unable to create entity: '%s'\n", name.c_str());
    return GameObject(0);
  }
  EntityManager &em = EntityManager::instance();
  GameObject entity = em.createEntity();
  std::list<IMLNode*> &components_iml = node->getChildren();
  std::list<IMLNode*>::iterator it = components_iml.begin();
  for (; it != components_iml.end(); ++it) {
    IMLTag *tag = dynamic_cast<IMLTag*>(*it);
    if (tag) {
      ComponentFactory &cf = ComponentFactory::instance();
      IComponent *comp = cf.create((*it)->getName());
      if (!comp) {
        fprintf(stderr, "Unable to create component: %s\n",
            (*it)->getName().c_str());
      } else {
        comp->loadIML(*tag);
        em.addComponentToEntity(comp, entity);
      }
    }
  }
  return entity;
}

bool EntityFactory::colliderAt(const Rectangle &space) const
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<CollisionComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(*it);
    if (sc) {
      Rectangle r(sc->pos.x, sc->pos.y, sc->width, sc->height);
      if (space.intersects(r)) {
        return true;
      }
    }
  }
  return false;
}

static int getItemStatMod();
static void applyCurse(ItemComponent *item);
static void applyIdentified(ItemComponent *item);

void EntityFactory::randomizeItemStats(ItemComponent *item) const
{
  if (item->useType == ItemComponent::USE_TYPE_TRAP ||
      item->useType == ItemComponent::USE_TYPE_SCROLL) {
    return;
  }
  applyCurse(item);
  applyIdentified(item);
  if (item->useType != ItemComponent::USE_TYPE_POTION) {
    int mod_multiplier = item->cursed ? -1 : 1;
    int health_mod = getItemStatMod() * mod_multiplier;
    int attack_mod = getItemStatMod() * mod_multiplier;
    int armor_mod = getItemStatMod() * mod_multiplier;
    item->statsModifiers.health += health_mod;
    item->statsModifiers.attack += attack_mod;
    item->statsModifiers.armor += armor_mod;
    if ((health_mod >= 3) || (attack_mod >= 3) || (armor_mod >= 3)) {
      item->rareType = ItemComponent::RARE_TYPE_BLUE;
    }
    if (health_mod >= 5) {
      item->name += " of Life";
      item->rareType = ItemComponent::RARE_TYPE_GREEN;
    }
    if (attack_mod >= 5) {
      item->name += " of Doom";
      item->rareType = ItemComponent::RARE_TYPE_GREEN;
    }
    if (armor_mod >= 5) {
      item->name += " of Protection";
      item->rareType = ItemComponent::RARE_TYPE_GREEN;
    }
  }
}

static int getItemStatMod()
{
  int mod = 0;
  
  int chance = rand() % 100;
  /*
  possible outcomes
  +0 40%
  +1 20%
  +2 15%
  +3 10%
  +4 10%
  +5  5%
  */
  if (chance < 40) {
    mod = 0;
  } else  if (chance < 60) {
    mod = 1;
  } else if (chance < 75) {
    mod = 2;
  } else if (chance < 85) {
    mod = 3;
  } else if (chance < 95) {
    mod = 4;
  } else if (chance < 100) {
    mod = 5;
  }
  return mod;
}

static void applyCurse(ItemComponent *item)
{
  int Chance = rand() % 100 + 1;
  //10% chance it will be cursed (lower stats & unequipable)
  if (Chance <= 10) {
    item->cursed = true;
    if (item->useType == ItemComponent::USE_TYPE_POTION) {
      item->spell = "poisonTarget";
    } else {
      int PickStat = rand() % 3; //picks out of 3 possible stats
      switch (PickStat) {
      case 0:
        item->statsModifiers.health *= -1;
        break;
      case 1:
        item->statsModifiers.attack *= -1;
        break;
      case 2:
        item->statsModifiers.armor *= -1;
        break;
      default:
        break;
      }
    }
  }
}

static void applyIdentified(ItemComponent *item)
{
  int chance = rand() % 100;
  if (chance < 25) {
    item->identified = false;
  }
}
