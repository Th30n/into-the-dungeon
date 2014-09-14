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
#include "ControlSystem.h"

#include <cstdio>
#include <vector>

#include "CArea.h"
#include "CHud.h"
#include "CMusic.h"
#include "CTile.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "ItemComponent.h"
#include "InventoryComponent.h"
#include "LevelLoader.h"
#include "MovementComponent.h"
#include "Rectangle.h"
#include "SpaceComponent.h"
#include "TurnComponent.h"
#include "WeaponComponent.h"

ControlSystem::ControlSystem(EntityManager &em, GameObject player)
  : em_(em), player_(player), player_turn_(0), player_weapon_(0),
    player_space_(0), player_movement_(0), player_health_(0), inventory_(0)
{
}

void ControlSystem::start()
{
  player_turn_ = em_.getComponentForEntity<TurnComponent>(player_);
  player_weapon_ = em_.getComponentForEntity<WeaponComponent>(player_);
  player_space_ = em_.getComponentForEntity<SpaceComponent>(player_);
  player_movement_ = em_.getComponentForEntity<MovementComponent>(player_);
  player_health_ = em_.getComponentForEntity<HealthComponent>(player_);
  inventory_ = em_.getComponentForEntity<InventoryComponent>(player_);
}

void ControlSystem::update(Controls control)
{
  if (player_health_->is_dead) {
    return;
  }
  switch (control) {
  case UP:
  case DOWN:
  case LEFT:
  case RIGHT:
    move(control);
    break;
  case PICK_UP_ITEM:
    pickUpItem();
    break;
  case DESCEND:
    descend();
    break;
  case PAUSE_TURN:
    player_turn_->turn_taken = true;
    break;
  default:
    break;
  }
}

void ControlSystem::move(Controls control) const
{
  if (player_turn_->turn_taken) {
    return;
  }
  switch (control) {
  case UP:
    player_movement_->waypoint.y -= 1;
    break;
  case DOWN:
    player_movement_->waypoint.y += 1;
    break;
  case LEFT:
    player_movement_->waypoint.x -= 1;
    break;
  case RIGHT:
    player_movement_->waypoint.x += 1;
    break;
  default:
    break;
  }
  player_turn_->turn_taken = true;
  GameObject attackable = getAttackableObjectAtPos(player_movement_->waypoint);
  if (attackable.getId()) {
    if (player_weapon_) {
      player_weapon_->target_id = attackable.getId();
    }
  } else {
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.Walk, 0);
  }
}

void ControlSystem::pickUpItem() const
{
  if (player_turn_->turn_taken) {
    return;
  }
  GameObject item = getItemAtPos(player_space_->pos);
  if (item.getId()) {
    addToInventory(item);
  }
}

GameObject ControlSystem::getAttackableObjectAtPos(Vector2f pos) const
{
  std::vector<GameObject> entities;
  em_.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    HealthComponent *hc = em_.
                getComponentForEntity<HealthComponent>(*it);
    if (hc->is_dead) {
      continue;
    }
    SpaceComponent *sc = em_.
                getComponentForEntity<SpaceComponent>(*it);
    Rectangle r(sc->pos.x, sc->pos.y, sc->width, sc->height);
    if (r.intersects(pos.x, pos.y)) {
      return *it;
    }
  }
  return 0;
}

GameObject ControlSystem::getItemAtPos(Vector2f pos) const
{
  std::vector<GameObject> items;
  em_.getEntitiesWithComponent<ItemComponent>(items);
  std::vector<GameObject>::iterator it = items.begin();
  for (; it != items.end(); ++it) {
    SpaceComponent *sc = em_.
                getComponentForEntity<SpaceComponent>(*it);
    if (sc && sc->is_active && sc->pos == pos) {
      return *it;
    }
  }
  return 0;
}

bool ControlSystem::addToInventory(GameObject item) const
{
  ItemComponent *ic = em_.
      getComponentForEntity<ItemComponent>(item);
  if (ic->useType == ItemComponent::USE_TYPE_SCROLL) {
    inventory_->scrolls.push_back(item.getId());
    //printf("Added scroll to inventory_\n");
    CHud::HUD.updateSpellBook();
  } else {
    if (inventory_->items.size() >= inventory_->max_size) {
      //printf("Inventory is full\n");
      return false;
    }
    inventory_->items.push_back(item.getId());
    //printf("Added item to inventory_\n");
    CHud::HUD.updateInventory();
  }
  SpaceComponent *sc = em_.
              getComponentForEntity<SpaceComponent>(item);
  sc->is_active = false;
  switch (ic->useType) {
  case ItemComponent::USE_TYPE_POTION:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.PotionSound, 0);
    break;
  case ItemComponent::USE_TYPE_SCROLL:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.ScrollSound, 0);
    break;
  case ItemComponent::EQ_TYPE_WEAPON:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.WeaponSound, 0);
    break;
  case ItemComponent::EQ_TYPE_SHIELD:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.ShieldSound, 0);
    break;
  case ItemComponent::EQ_TYPE_HELMET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.HeadSound, 0);
    break;
  case ItemComponent::EQ_TYPE_ARMOR:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.ChestSound, 0);
    break;
  case ItemComponent::EQ_TYPE_BOOTS:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.FeetSound, 0);
    break;
  case ItemComponent::EQ_TYPE_RING:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.RingSound, 0);
    break;
  case ItemComponent::EQ_TYPE_AMULET:
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.NeckSound, 0);
    break;
  default:
    break;
  }
  return true;
}

void ControlSystem::descend() const
{
  if (player_turn_->turn_taken) {
    return;
  }
  if (CArea::area_control.GetTile(player_space_->pos.x * TILE_SIZE,
      player_space_->pos.y * TILE_SIZE)->tile_id == TILE_ID_LADDER) {
    player_health_->light += player_health_->max_light / 2;
    if (player_health_->light > player_health_->max_light) {
      player_health_->light = player_health_->max_light;
    }
    LevelLoader::nextLevel(player_, CArea::dungeon_level + 1);
    CHud::HUD.updateStats();
  }
}
