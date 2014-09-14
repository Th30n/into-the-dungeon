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
#include "ParticleSystem.h"

#include <vector>

#include <SDL.h>

#include "CFPS.h"
#include "EntityManager.h"
#include "ParticleComponent.h"
#include "SpaceComponent.h"

void ParticleSystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> particles;
  em.getEntitiesWithComponent<ParticleComponent>(particles);
  std::vector<GameObject>::iterator it = particles.begin();
  for (; it != particles.end(); ++it) {
    updateParticle(*it);
  }
}

void ParticleSystem::updateParticle(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  ParticleComponent *part = em.getComponentForEntity<ParticleComponent>(obj);
  if (part->start_time + part->time_to_live <= SDL_GetTicks()) {
    removeParticle(obj);
    return;
  }
  SpaceComponent *space = em.getComponentForEntity<SpaceComponent>(obj);
  if (space) {
    space->pos += part->velocity * CFPS::fps_control.GetSpeedFactor();
  }
}

void ParticleSystem::removeParticle(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  ParticleComponent *part = em.getComponentForEntity<ParticleComponent>(obj);
  if (part->attack_fun) {
    part->attack_fun(part->attacker, part->target);
  }
  EntityManager::instance().removeEntity(obj);
}
