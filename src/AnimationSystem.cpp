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
#include "AnimationSystem.h"

#include <algorithm>
#include <vector>

#include <SDL.h>

#include "AnimationComponent.h"
#include "EntityManager.h"
#include "MovementComponent.h"
#include "SpaceComponent.h"
#include "WeaponComponent.h"

static void changeFacing(AnimationComponent *ac, Facing facing)
{
  switch (facing) {
  case FACING_DOWN:
    ac->idle_frame_col = AnimationComponent::FRAME_LOOK_DOWN;
    break;
  case FACING_UP:
    ac->idle_frame_col = AnimationComponent::FRAME_LOOK_UP;
    break;
  case FACING_LEFT:
    ac->idle_frame_col = AnimationComponent::FRAME_LOOK_LEFT;
    break;
  case FACING_RIGHT:
    ac->idle_frame_col = AnimationComponent::FRAME_LOOK_RIGHT;
    break;
  default:
    break;
  }
}

static void attackAnimation(AnimationComponent *ac)
{
  ac->current_frame_col = ac->idle_frame_col / 2 + 9;
  if (ac->start_time + ac->max_frames * ac->frame_rate <= SDL_GetTicks()) {
    ac->attacking = false;
    ac->current_frame_col = ac->idle_frame_col;
    if (ac->current_frame_row >= 2) {
      ac->current_frame_row = 0;
    }
  } 
}

static Facing getFacing(Vector2f dir)
{
  Vector2f look_dir = dir.normalized();
  if (look_dir.x >= 0.5) {
    return FACING_RIGHT;
  } else if (look_dir.x <= -0.5) {
    return FACING_LEFT;
  } else if (look_dir.y < 0) {
    return FACING_UP;
  } else {
    return FACING_DOWN;
  }
}

static void animate(GameObject object)
{
  EntityManager &em = EntityManager::instance();
  AnimationComponent *ac = em.getComponentForEntity<AnimationComponent>(object);
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(object);
  MovementComponent *mc = em.getComponentForEntity<MovementComponent>(object);
  bool moving = false;
  if (sc && mc) {
    Vector2f dir = mc->waypoint - sc->pos;
    if (dir.x != 0.0f || dir.y != 0.0f) {
      moving = true;
    }
  }
  changeFacing(ac, getFacing(sc->look_dir));
  if (ac->attacking) {
    attackAnimation(ac);
  } else if (ac->is_dead) {
    ac->current_frame_col = AnimationComponent::FRAME_CORPSE;
  } else if (moving) {
    switch(ac->idle_frame_col) {
    case AnimationComponent::FRAME_LOOK_LEFT:
      ac->current_frame_col = AnimationComponent::FRAME_MOVE_LEFT;
      break;
    case AnimationComponent::FRAME_LOOK_RIGHT:
      ac->current_frame_col = AnimationComponent::FRAME_MOVE_RIGHT;
      break;
    case AnimationComponent::FRAME_LOOK_DOWN:
      ac->current_frame_col = AnimationComponent::FRAME_MOVE_DOWN;
      break;
    case AnimationComponent::FRAME_LOOK_UP:
      ac->current_frame_col = AnimationComponent::FRAME_MOVE_UP;
      break;
    default:
      break;
    }
  } else if (ac->casting) {
    ac->current_frame_col = AnimationComponent::FRAME_CASTING;
  } else {
    ac->current_frame_col = ac->idle_frame_col;
  }
}

void AnimationSystem::update() const
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<AnimationComponent>(entities);
  std::for_each(entities.begin(), entities.end(), animate);
}
