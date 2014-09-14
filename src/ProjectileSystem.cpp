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
#include "ProjectileSystem.h"

#include <vector>

#include <SDL.h>

#include "AnimationComponent.h"
#include "CMusic.h"
#include "EntityManager.h"
#include "MovementComponent.h"
#include "ProjectileComponent.h"
#include "SpaceComponent.h"

void ProjectileSystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<ProjectileComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    updateProjectile(*it);
  }
}

static int getFrameColon(Vector2f dir);

void ProjectileSystem::updateProjectile(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  ProjectileComponent *proj = em.
      getComponentForEntity<ProjectileComponent>(obj);
  AnimationComponent *anim = em.
      getComponentForEntity<AnimationComponent>(obj);
  unsigned anim_time = 0;
  if (anim) {
    anim_time = anim->max_frames * anim->frame_rate;
  }
  unsigned expire_time = proj->destroy_time + anim_time;
  if (proj->destroyed && expire_time <= SDL_GetTicks()) {
    removeProjectile(obj);
    return;
  }
  MovementComponent *mc = em.getComponentForEntity<MovementComponent>(obj);
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(obj);
  if (!mc || !sc) {
    removeProjectile(obj);
    return;
  }
  mc->waypoint = proj->end;
  if (sc->pos == proj->end && !proj->destroyed) {
    proj->destroyed = true;
    if (anim) {
      anim->start_time = SDL_GetTicks();
    }
    proj->destroy_time = SDL_GetTicks();
  }
  if (anim) {
    anim->current_frame_col = getFrameColon(proj->end - sc->pos);
    anim->current_frame_row = 0;
    if (proj->destroyed) {
      anim->current_frame_col = PROJECTILE_FRAME_COLON_HIT;
    }
  }
}

static int getFrameColon(Vector2f dir)
{
  int dx = dir.x;
  int dy = dir.y;
  
  if (dx <= 0) { //Target is left of the projectile
    if (dy <= 0) { //Target is left and above the projectile
      if (-dx >= -dy) {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_LEFT;
      } else {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_UP;
      }
    } else { //Target is left and below the projectile
      if (-dx >= dy) {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_LEFT;
      } else {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_DOWN;
      }
    }
  } else { //Target is right of the projectile
    if (dy <= 0) { //Target is right and above the projectile
      if (dx >= -dy) {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_RIGHT;
      } else {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_UP;
      }
    } else { //Target is right and below the projectile
      if (dx >= dy) {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_RIGHT;
      } else {
        return ProjectileSystem::PROJECTILE_FRAME_COLON_DOWN;
      }
    }
  }
}

void ProjectileSystem::removeProjectile(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  ProjectileComponent *proj = em.
      getComponentForEntity<ProjectileComponent>(obj);
  if (proj->attack_fun) {
    proj->attack_fun(proj->attacker, proj->target);
  }
  em.removeEntity(obj);
  CMusic::SoundControl.PlaySFX(CMusic::SoundControl.ProjectileHit, 0);
}
