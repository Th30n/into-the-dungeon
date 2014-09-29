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
#include "DOTSystem.h"

#include <algorithm>
#include <vector>

#include "DOTComponent.h"
#include "EntityManager.h"
#include "HealthSystem.h"
#include "SpellEffects.h"
#include "TurnComponent.h"

static void updateDOT(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  DOTComponent *dot = em.getComponentForEntity<DOTComponent>(obj);
  HealthSystem::addAilments(dot->target, dot->ailments);

  TurnComponent *turn = em.getComponentForEntity<TurnComponent>(obj);
  if (!turn || turn->turn_taken) {
    // Don't process DOTs multiple times in one turn or if no turn comp.
    return;
  }
  if (!dot->infinite && dot->duration <= 0) {
    HealthSystem::removeAilments(dot->target, dot->ailments);
    em.removeEntity(obj);
    return;
  }
  if (dot->health_change != 0) {
    HealthSystem::changeHealth(dot->source, dot->target, dot->health_change);
  }
  if (dot->light_change != 0) {
    HealthSystem::changeLight(dot->source, dot->target, dot->light_change);
  }
  if (!dot->infinite) {
    dot->duration--;
  }
  if (!dot->spell.empty()) {
    SpellEffects::apply(dot->source, dot->target, dot->spell);
  }
  turn->turn_taken = true;
}

void DOTSystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> dots;
  em.getEntitiesWithComponent<DOTComponent>(dots);
  std::for_each(dots.begin(), dots.end(), updateDOT);
}
