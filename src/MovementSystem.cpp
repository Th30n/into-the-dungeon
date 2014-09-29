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
#include "MovementSystem.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "CArea.h"
#include "CFPS.h"
#include "CMap.h"
#include "CollisionComponent.h"
#include "CTile.h"
#include "Define.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "MovementComponent.h"
#include "Rectangle.h"
#include "SpaceComponent.h"
#include "Vector2f.h"

static bool collidesObject(GameObject o1, GameObject o2)
{
  if (o1 == o2) {
    return false;
  }
  EntityManager &em = EntityManager::instance();
  HealthComponent *hc2 = em.getComponentForEntity<HealthComponent>(o2);
  if (hc2 && hc2->is_dead) {
    return false;
  }
  SpaceComponent *sc1 = em.getComponentForEntity<SpaceComponent>(o1);
  MovementComponent *mc1 = em.getComponentForEntity<MovementComponent>(o1);
  SpaceComponent *sc2 = em.getComponentForEntity<SpaceComponent>(o2);
  Rectangle r1(mc1->waypoint.x, mc1->waypoint.y, sc1->width, sc1->height);
  Rectangle r2(sc2->pos.x, sc2->pos.y, sc2->width, sc2->height);
  if (r1.intersects(r2)) {
    return true;
  }
  MovementComponent *mc2 = em.getComponentForEntity<MovementComponent>(o2);
  if (mc2) {
    r2.setX(mc2->waypoint.x);
    r2.setY(mc2->waypoint.y);
    if (r1.intersects(r2)) {
      return true;
    }
  }
  return false;
}

static GameObject getCollidedObject(GameObject object)
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<CollisionComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    if (collidesObject(object, *it)) {
      return *it;
    }
  }
  return 0;
}

static bool collidesMap(const SpaceComponent *sc, const MovementComponent *mc)
{
  int start_x = mc->waypoint.x;
  int start_y = mc->waypoint.y;
  int end_x = start_x + sc->width;
  int end_y = start_y + sc->height;

  for (int i_y = start_y; i_y < end_y; i_y++) {
    for (int i_x = start_x; i_x < end_x; i_x++) {
      CTile *tile = CArea::area_control.GetTile(
                  i_x * TILE_SIZE, i_y * TILE_SIZE);
      if (tile && tile->isBlocking()) {
        if (tile->isDoor()) {
          tile->openDoor();
        }
        return true;
      }
    }
  }
  return false;
}

static void move(GameObject object)
{
  EntityManager &em = EntityManager::instance();
  MovementComponent *mc = em.getComponentForEntity<MovementComponent>(object);
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(object);
  if (mc->waypoint != sc->pos) {
    sc->look_dir = mc->waypoint - sc->pos;
  }
  CollisionComponent *col = em.getComponentForEntity<CollisionComponent>(object);
  if (col) {
    if (collidesMap(sc, mc)) {
      mc->waypoint = sc->pos;
    }
    GameObject collided = getCollidedObject(object);
    if (collided.getId() > 0) {
      mc->waypoint = sc->pos;
    }
  }
  Vector2f dir = (mc->waypoint - sc->pos).normalized();
  float speed = mc->speed * CFPS::fps_control.GetSpeedFactor();
  float dist = mc->waypoint.distance(sc->pos);
  CTile *tile1 = CArea::area_control.GetTile(
      sc->pos.x * TILE_SIZE, sc->pos.y * TILE_SIZE);
  CTile *tile2 = CArea::area_control.GetTile(
      mc->waypoint.x * TILE_SIZE, mc->waypoint.y * TILE_SIZE);
  bool is_fow_on = CArea::area_control.GetMap(0, 0)->fow_on;
  bool is_visible = tile1->is_visible || tile2->is_visible || !is_fow_on;
  if (speed > dist || !is_visible) {
    speed = dist;
  }
  sc->pos += dir * speed;
}

void MovementSystem::update() const
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<MovementComponent>(entities);
  std::for_each(entities.begin(), entities.end(), move);
}
