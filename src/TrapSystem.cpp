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
#include "TrapSystem.h"

#include <algorithm>
#include <cstdio>
#include <vector>

#include "CollisionComponent.h"
#include "data/dirs.h"
#include "EntityManager.h"
#include "ParticleComponent.h"
#include "Rectangle.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "SpellEffects.h"
#include "TrapComponent.h"
#include "TurnComponent.h"
#include "Vector2f.h"

static void createParticle(Vector2f pos)
{
  EntityManager &em = EntityManager::instance();
  GameObject part_obj = em.createEntity();
  if (part_obj.getId() == 0) {
    fprintf(stderr, "Unable to create particle.\n");
    return;
  }
  SpaceComponent *part_space = new SpaceComponent();
  part_space->pos = pos;
  RendererComponent *sprite =
      new RendererComponent(data::FindFile("gfx/UI/TrapTrig.png").c_str());
  sprite->width = 40;
  sprite->height = 16;
  sprite->x_offset = 0;
  sprite->y_offset = 0;
  sprite->start();
  ParticleComponent *part = new ParticleComponent();
  part->time_to_live = 2000;
  part->velocity = Vector2f(0.0f, -0.01f);
  part->start_time =  SDL_GetTicks();
  em.addComponentToEntity(part_space, part_obj);
  em.addComponentToEntity(sprite, part_obj);
  em.addComponentToEntity(part, part_obj);
}

static void spawnEffect(GameObject trap, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  TrapComponent *tc = em.getComponentForEntity<TrapComponent>(trap);
  SpellEffects::apply(trap, target, tc->spell);
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(trap);
  createParticle(sc->pos);
}

static bool triggersTrap(GameObject entity, GameObject trap)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *entity_space =
      em.getComponentForEntity<SpaceComponent>(entity);
  if (!entity_space) {
    return false;
  }
  TurnComponent *entity_turn = em.getComponentForEntity<TurnComponent>(entity);
  TrapComponent *tc = em.getComponentForEntity<TrapComponent>(trap);
  if (tc->team == entity_turn->team) {
    return false;
  }
  SpaceComponent *trap_space = em.getComponentForEntity<SpaceComponent>(trap);
  Rectangle trap_rect(trap_space->pos.x, trap_space->pos.y,
      trap_space->width, trap_space->height);
  Rectangle entity_rect(entity_space->pos.x, entity_space->pos.y,
      entity_space->width, entity_space->height);
  return trap_rect.intersects(entity_rect);
}

static void updateTrap(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  TrapComponent *trap = em.getComponentForEntity<TrapComponent>(obj);
  if (trap->is_triggered || trap->is_destroyed) {
    // If the trap was already triggered or destroyed.
    return;
  }
  SpaceComponent *trap_space = em.getComponentForEntity<SpaceComponent>(obj);
  if (!trap_space || !trap_space->is_active) {
    // If trap isn't on map.
    return ;
  }
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<CollisionComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    if (triggersTrap(*it, obj)) {
      trap->is_triggered = true;
      trap->is_visible = true;
      spawnEffect(obj, *it);
    }
  }
}

void TrapSystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> traps;
  em.getEntitiesWithComponent<TrapComponent>(traps);
  std::for_each(traps.begin(), traps.end(), updateTrap);
}
