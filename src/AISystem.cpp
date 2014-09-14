/*
 * into the dungeon++
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
#include "AISystem.h"

#include <cstdlib>
#include <vector>

#include "AIComponent.h"
#include "AnimationComponent.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "HealthComponent.h"
#include "MovementComponent.h"
#include "Rectangle.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "SpellComponent.h"
#include "SpellEffects.h"
#include "StatsComponent.h"
#include "TurnComponent.h"
#include "Visibility.h"
#include "WeaponComponent.h"

AISystem::AISystem(GameObject player)
  : player_(player)
{
}

void AISystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<AIComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    TurnComponent *tc = em.getComponentForEntity<TurnComponent>(*it);
    if (tc && !tc->turn_taken) {
      decide(*it);
      tc->turn_taken = true;
    }
  }
}

void AISystem::decide(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *health = em.getComponentForEntity<HealthComponent>(obj);
  if (health && health->is_dead) {
    return;
  } else if (health && (health->ailments & STATUS_IMMOBILE)) {
    return;
  }
  AIComponent *ai = em.getComponentForEntity<AIComponent>(obj);
  SpaceComponent *aispace = em.getComponentForEntity<SpaceComponent>(obj);
  if (aispace->pos == ai->waypoint) {
    ai->waypoint = AIComponent::RESET_POSITION;
  }
  AnimationComponent *anim = em.
      getComponentForEntity<AnimationComponent>(obj);
  if (anim) {
    anim->casting = false;
  }
  GameObject target = getValidTarget(obj);
  if (canSee(obj, target)) {
    SpaceComponent *tsp = em.getComponentForEntity<SpaceComponent>(target);
    ai->waypoint = tsp->pos;
    if (canCastSpell(obj)) {
      if (anim) {
        anim->casting = true;
      }
      return castSpell(obj, target);
    } else if (inAttackRange(obj, target)) {
      aispace->look_dir = tsp->pos - aispace->pos;
      return attack(obj, target);
    }
  }
  if (ai->waypoint != AIComponent::RESET_POSITION) {
    moveToPos(obj, ai->waypoint);
  } else {
    moveRandom(obj);
  }
}

static GameObject getNearestTarget(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sp = em.getComponentForEntity<SpaceComponent>(obj);
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  float min_dist = 100.0f;
  std::vector<GameObject> targets;
  for (; it != entities.end(); ++it) {
    if (obj == *it) {
      continue;
    }
    HealthComponent *thp = em.getComponentForEntity<HealthComponent>(*it);
    if (thp->is_dead) {
      continue;
    }
    SpaceComponent *tsp = em.getComponentForEntity<SpaceComponent>(*it);
    if (tsp) {
      float d = tsp->pos.manhattanDistance(sp->pos);
      if (d < min_dist) {
        targets.clear();
        targets.push_back(*it);
        min_dist = d;
      } else if (d == min_dist) {
        targets.push_back(*it);
      }
    }
  }
  return targets[rand() % targets.size()];
}

GameObject AISystem::getValidTarget(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *hp = em.getComponentForEntity<HealthComponent>(obj);
  if (hp->ailments & STATUS_CONFUSED) {
    return getNearestTarget(obj);
  }
  return player_;
}

static float getManhattanDistance(GameObject o1, GameObject o2)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sp1 = em.getComponentForEntity<SpaceComponent>(o1);
  SpaceComponent *sp2 = em.getComponentForEntity<SpaceComponent>(o2);
  return sp1->pos.manhattanDistance(sp2->pos);
}

bool AISystem::canSee(GameObject obj, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *aisp = em.getComponentForEntity<SpaceComponent>(obj);
  SpaceComponent *tsp = em.getComponentForEntity<SpaceComponent>(target);
  Vector2f dir = tsp->pos - aisp->pos;
  float cos_angle = aisp->look_dir.normalized() * dir.normalized();
  if (cos_angle < 0.0f) {
    // view angle is 180 deg
    return false;
  }
  StatsComponent *aistats = em.getComponentForEntity<StatsComponent>(obj);
  int dist = getManhattanDistance(obj, target);
  if (dist > aistats->vision_range) {
    return false;
  }
  HealthComponent *hp = em.getComponentForEntity<HealthComponent>(target);
  if (hp->ailments & STATUS_INVISIBLE) {
    return false;
  }
  return Visibility::InLOS(aisp->pos, tsp->pos);
}

bool AISystem::canCastSpell(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  StatsComponent *aistats = em.getComponentForEntity<StatsComponent>(obj);
  AIComponent *ai = em.getComponentForEntity<AIComponent>(obj);
  if (aistats && !aistats->skills.empty()) {
    int chance = rand() % 100;
    if (chance < ai->cast_rate) {
      return true;
    }
  }
  return false;
}

void AISystem::castSpell(GameObject obj, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  StatsComponent *aistats = em.getComponentForEntity<StatsComponent>(obj);
  int picker = rand() % aistats->skills.size();
  GameObject spell_target = targetSpell(obj, target, aistats->skills[picker]);
  SpellEffects::apply(obj, spell_target, aistats->skills[picker]);
}

GameObject AISystem::targetSpell(GameObject obj, GameObject target,
    std::string spell_name)
{
  GameObject spell_obj = EntityFactory::instance().createSpell(spell_name);
  if (spell_obj.getId() == 0) {
    return 0;
  }
  EntityManager &em = EntityManager::instance();
  SpellComponent *spell = em.getComponentForEntity<SpellComponent>(spell_obj);
  GameObject spell_target(0);
  if (spell->targeting == "user") {
    spell_target = obj;
  } else if (spell->targeting == "target") {
    spell_target = target;
  } else if (spell->targeting == "targetField") {
    spell_target = getAdjacentField(target);
  } else if (spell->targeting == "adjacentField") {
    spell_target = getAdjacentField(obj);
  }
  em.removeEntity(spell_obj);
  return spell_target;
}

GameObject AISystem::getAdjacentField(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *tsp = em.getComponentForEntity<SpaceComponent>(obj);
  int min_x = tsp->pos.x - 1;
  int max_x = tsp->pos.x + tsp->width;
  int min_y = tsp->pos.y - 1;
  int max_y = tsp->pos.y + tsp->height;
  std::vector<Vector2f> adjacent_fields;
  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      Rectangle r(tsp->pos.x, tsp->pos.y, tsp->width, tsp->height);
      if (r.intersects(x, y)) {
        continue;
      } else {
        adjacent_fields.push_back(Vector2f(x, y));
      }
    }
  }
  int picker = rand() % adjacent_fields.size();
  GameObject field = em.createEntity();
  SpaceComponent *field_space = new SpaceComponent();
  field_space->pos = adjacent_fields[picker];
  RendererComponent *field_image = new RendererComponent("gfx/UI/Target.png");
  field_image->width = 32;
  field_image->height = 32;
  field_image->start();
  em.addComponentToEntity(field_space, field);
  em.addComponentToEntity(field_image, field);
  return field;
}

bool AISystem::inAttackRange(GameObject obj, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  WeaponComponent *wep = em.getComponentForEntity<WeaponComponent>(obj);
  float dist = getManhattanDistance(obj, target);
  if (dist <= wep->max_range && dist >= wep->min_range) {
    return true;
  }
  return false;
}

void AISystem::attack(GameObject obj, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  WeaponComponent *wep = em.getComponentForEntity<WeaponComponent>(obj);
  wep->target_id = target.getId();
}

void AISystem::moveToPos(GameObject obj, Vector2f pos)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *aispace = em.getComponentForEntity<SpaceComponent>(obj);
  Vector2f dir = pos - aispace->pos;
  MovementComponent *mov = em.getComponentForEntity<MovementComponent>(obj);
  if (dir.x > 0) {
    mov->waypoint.x += 1;
  } else if (dir.x < 0) {
    mov->waypoint.x -= 1;
  } else if (dir.y > 0) {
    mov->waypoint.y += 1;
  } else if (dir.y < 0) {
    mov->waypoint.y -= 1;
  }
}

void AISystem::moveRandom(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  MovementComponent *mov = em.getComponentForEntity<MovementComponent>(obj);
  if (!mov) {
    return;
  }
  int dir = rand() % 4;
  switch (dir) {
    case 0:
      mov->waypoint.y -= 1;
      break;
    case 1:
      mov->waypoint.y += 1;
      break;
    case 2:
      mov->waypoint.x -= 1;
      break;
    case 3:
      mov->waypoint.x += 1;
      break;
    default:
      break;
  }
}
