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
#include "RenderSystem.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "AnimationComponent.h"
#include "CArea.h"
#include "CCamera.h"
#include "CMap.h"
#include "CSurface.h"
#include "CTile.h"
#include "EntityManager.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"

static void drawObject(SDL_Surface *display, GameObject go)
{
  EntityManager &em = EntityManager::instance();
  RendererComponent *rc = em.getComponentForEntity<RendererComponent>(go);
  int disp_x = rc->x;
  int disp_y = rc->y;
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(go);
  if (sc) {
    disp_x = sc->pos.x * TILE_SIZE - CCamera::camera_control.GetX();
    disp_y = sc->pos.y * TILE_SIZE - CCamera::camera_control.GetY();
  }
  if (!CArea::area_control.GetTile(sc->pos.x * TILE_SIZE,
      sc->pos.y * TILE_SIZE)->is_visible &&
      CArea::area_control.GetMap(0,0)->fow_on) {
    return; //dont render if not visible
  }
  int img_x = 0;
  int img_y = 0;
  AnimationComponent *ac = em.getComponentForEntity<AnimationComponent>(go);
  if (ac && ac->frame_rate > 0) {
    int frame = (SDL_GetTicks() - ac->start_time) / ac->frame_rate;
    if (ac->is_dead && frame >= ac->max_frames) {
      frame = ac->max_frames - 1;
    } else {
      frame %= ac->max_frames;
    }
    img_x = ac->current_frame_col * rc->width;
    img_y = (ac->current_frame_row + frame) * rc->height;
  }
  CSurface::OnDraw(display, rc->image,
      disp_x + rc->x_offset, disp_y + rc->y_offset,
      img_x, img_y, rc->width, rc->height); 
}

void RenderSystem::render(SDL_Surface *display) const
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<RendererComponent>(entities);
  std::for_each(entities.begin(), entities.end(),
      std::bind1st(std::ptr_fun(drawObject), display));
}
