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
#include "ItemRenderSystem.h"

#include <vector>

#include "CArea.h"
#include "CCamera.h"
#include "CMap.h"
#include "CSurface.h"
#include "Define.h"
#include "EntityManager.h"
#include "ItemComponent.h"
#include "SpaceComponent.h"
#include "TrapComponent.h"

SDL_Surface *ItemRenderSystem::surf_items = 0;

void ItemRenderSystem::render(SDL_Surface *display)
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> items;
  em.getEntitiesWithComponent<ItemComponent>(items);
  std::vector<GameObject>::iterator it = items.begin();
  for (; it != items.end(); ++it) {
    drawItem(display, *it);
  }
}

void ItemRenderSystem::drawItem(SDL_Surface *display, ItemComponent *item,
    int x, int y)
{
  int tileset_width = surf_items->w / TILE_SIZE;
  int tileset_x = (item->spriteId % tileset_width) * TILE_SIZE;
  int tileset_y = (item->spriteId / tileset_width) * TILE_SIZE;
  
  CSurface::OnDraw(display, surf_items, x, y,
          tileset_x, tileset_y, TILE_SIZE, TILE_SIZE);
}

void ItemRenderSystem::drawItem(SDL_Surface *display, GameObject item)
{

  EntityManager &em = EntityManager::instance();
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(item);
  if (!sc || !sc->is_active) {
    return;
  }
  bool explored = CArea::area_control.GetTile(
      sc->pos.x * TILE_SIZE, sc->pos.y * TILE_SIZE)->is_explored;
  if (!explored && CArea::area_control.GetMap(0,0)->fow_on) {
    return; //dont render if not visible
  }

  ItemComponent *ic = em.getComponentForEntity<ItemComponent>(item);
  if (ic->useType == ItemComponent::USE_TYPE_TRAP) {
    TrapComponent *trap = em.getComponentForEntity<TrapComponent>(item);
    if (!trap->is_visible) {
      return; // don't render invisible traps
    }
  }

  drawItem(display, ic,
      sc->pos.x * TILE_SIZE - CCamera::camera_control.GetX(),
      sc->pos.y * TILE_SIZE - CCamera::camera_control.GetY());
}
