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
#include "ItemUsage.h"

#include <algorithm>
#include <cstdio>

#include "CHud.h"
#include "CMusic.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "HealthSystem.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "SpaceComponent.h"
#include "SpellComponent.h"
#include "SpellEffects.h"
#include "StatsComponent.h"
#include "TrapComponent.h"
#include "TurnComponent.h"

void ItemUsage::useItem(GameObject user, GameObject item)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *ic = em.getComponentForEntity<ItemComponent>(item);
  //printf("Using item: %s\n", ic->name.c_str());
  switch (ic->useType) {
    case ItemComponent::USE_TYPE_POTION:
      usePotion(user, item);
      //play drinking sound
      CMusic::SoundControl.PlaySFX(CMusic::SoundControl.Drink, 0);
      break;
    case ItemComponent::USE_TYPE_TRAP:
      useTrap(user, item);
      break;
    case ItemComponent::EQ_TYPE_WEAPON:
    case ItemComponent::EQ_TYPE_SHIELD:
    case ItemComponent::EQ_TYPE_HELMET:
    case ItemComponent::EQ_TYPE_ARMOR:
    case ItemComponent::EQ_TYPE_BOOTS:
    case ItemComponent::EQ_TYPE_RING:
    case ItemComponent::EQ_TYPE_AMULET:
    case ItemComponent::EQ_TYPE_CBOX:
      if (ic->equipped) {
        unequip(user, ic);
      } else {
        equip(user, ic);
      }
      break;
    case ItemComponent::USE_TYPE_SCROLL:
      useScroll(user, item);
      break;
    default:
      break;
  }
}

void ItemUsage::dropItem(GameObject user, GameObject item)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *ic = em.getComponentForEntity<ItemComponent>(item);
  if (ic->equipped && !unequip(user, ic)) {
    return;
  }
  SpaceComponent *space = em.getComponentForEntity<SpaceComponent>(item);
  if (!space) {
    space = new SpaceComponent();
    em.addComponentToEntity(space, item);
  }
  SpaceComponent *user_space = em.getComponentForEntity<SpaceComponent>(user);
  space->pos = user_space->pos;
  space->is_active = true;
  InventoryComponent *inventory = em.
      getComponentForEntity<InventoryComponent>(user);
  removeFromInventory(inventory->items, item);
  CMusic::SoundControl.PlaySFX(CMusic::SoundControl.Drop, 0);
}

void ItemUsage::removeFromInventory(std::vector<unsigned> &inventory,
    GameObject item)
{
  inventory.erase(remove(inventory.begin(), inventory.end(), item.getId()),
      inventory.end());
}

void ItemUsage::usePotion(GameObject user, GameObject item)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *ic = em.getComponentForEntity<ItemComponent>(item);
  SpellEffects::apply(user, user, ic->spell);
  InventoryComponent *inventory = em.
      getComponentForEntity<InventoryComponent>(user);
  removeFromInventory(inventory->items, item);
  em.removeEntity(item);
  TurnComponent *user_turn = em.getComponentForEntity<TurnComponent>(user);
  user_turn->turn_taken = true;
}

void ItemUsage::useTrap(GameObject user, GameObject item)
{
  EntityManager &em = EntityManager::instance();
  TrapComponent *trap = em.getComponentForEntity<TrapComponent>(item);
  trap->is_triggered = false;
  TurnComponent *turn = em.getComponentForEntity<TurnComponent>(user);
  if (turn) {
    trap->team = turn->team;
  }
  dropItem(user, item);
  TurnComponent *user_turn = em.getComponentForEntity<TurnComponent>(user);
  user_turn->turn_taken = true;
}

bool ItemUsage::unequip(GameObject user, ItemComponent *ic)
{
  if (ic->cursed) {
    return false;
  }
  ic->equipped = false;
  EntityManager &em = EntityManager::instance();
  StatsComponent *stats = em.getComponentForEntity<StatsComponent>(user);
  stats->vision_range -= ic->statsModifiers.vision_range;
  stats->attack -= ic->statsModifiers.attack;
  stats->armor -= ic->statsModifiers.armor;
  stats->health -= ic->statsModifiers.health;
  HealthSystem::changeMaxHealth(user, -ic->statsModifiers.health);
  switch (ic->useType) {
  case ItemComponent::EQ_TYPE_WEAPON:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipWeapon, 0);
    break;
  case ItemComponent::EQ_TYPE_SHIELD:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipShield, 0);
    break;
  case ItemComponent::EQ_TYPE_HELMET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipHead, 0);
    break;
  case ItemComponent::EQ_TYPE_ARMOR:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipChest, 0);
    break;
  case ItemComponent::EQ_TYPE_BOOTS:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipFeet, 0);
    break;
  case ItemComponent::EQ_TYPE_RING:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipRing, 0);
    break;
  case ItemComponent::EQ_TYPE_AMULET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipNeck, 0);
    break;
  default:
    break;
  }

  return true;
}

void ItemUsage::equip(GameObject user, ItemComponent *ic)
{
  EntityManager &em = EntityManager::instance();
  InventoryComponent *inventory = em.
      getComponentForEntity<InventoryComponent>(user);
  std::vector<unsigned> &items = inventory->items;
  for (unsigned i = 0; i < items.size(); ++i) {
    ItemComponent *eq = em.getComponentForEntity<ItemComponent>(items[i]);
    if (ic->useType == eq->useType && eq->equipped) {
      if (unequip(user, eq)) {
        break;
      } else {
        return;
      }
    }
  }
  ic->equipped = true;
  StatsComponent *stats = em.getComponentForEntity<StatsComponent>(user);
  stats->vision_range += ic->statsModifiers.vision_range;
  stats->attack += ic->statsModifiers.attack;
  stats->armor += ic->statsModifiers.armor;
  stats->health += ic->statsModifiers.health;
  HealthSystem::changeMaxHealth(user, ic->statsModifiers.health);

  switch (ic->useType) {
  case ItemComponent::EQ_TYPE_WEAPON:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipWeapon, 0);
    break;
  case ItemComponent::EQ_TYPE_SHIELD:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipShield, 0);
    break;
  case ItemComponent::EQ_TYPE_HELMET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipHead, 0);
    break;
  case ItemComponent::EQ_TYPE_ARMOR:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipChest, 0);
    break;
  case ItemComponent::EQ_TYPE_BOOTS:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipFeet, 0);
    break;
  case ItemComponent::EQ_TYPE_RING:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipRing, 0);
    break;
  case ItemComponent::EQ_TYPE_AMULET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.EquipNeck, 0);
    break;
  default:
    break;
  }
}

void ItemUsage::useScroll(GameObject user, GameObject scroll)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *ic = em.getComponentForEntity<ItemComponent>(scroll);
  GameObject spell = EntityFactory::instance().createSpell(ic->spell);
  if (spell.getId() == 0) {
    return;
  }
  SpellComponent *sp = em.getComponentForEntity<SpellComponent>(spell);
  if (sp) {
    if (sp->targeting == "user") {
      SpellEffects::apply(user, user, ic->spell);
      InventoryComponent *inventory = em.
          getComponentForEntity<InventoryComponent>(user);
      removeFromInventory(inventory->scrolls, scroll);
      em.removeEntity(scroll);
      TurnComponent *user_turn = em.
          getComponentForEntity<TurnComponent>(user);
      user_turn->turn_taken = true;
    } else {
      targetSpell(sp->targeting, ic->spell, scroll);
    }
  }
  em.removeEntity(spell);
}

void ItemUsage::targetSpell(std::string targeting, std::string &spell,
              GameObject scroll)
{
  CHud::Targeting target = CHud::TARGETING_NONE;
  if (targeting == "target") {
    target = CHud::TARGETING_ENTITY;
  } else if (targeting == "item") {
    target = CHud::TARGETING_ITEM;
  }
  CHud::HUD.activateTargeting(target, spell, scroll);
}
