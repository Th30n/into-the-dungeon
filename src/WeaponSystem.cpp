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
#include "WeaponSystem.h"

#include <cstdlib>
#include <vector>

#include <SDL.h>

#include "AnimationComponent.h"
#include "CMusic.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "HealthSystem.h"
#include "MovementComponent.h"
#include "ProjectileComponent.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "SpellEffects.h"
#include "StatsComponent.h"
#include "TurnComponent.h"
#include "WeaponComponent.h"

static void dealDamage(GameObject attacker, GameObject target);
static int calculateDamage(GameObject attacker, GameObject target);
static bool isHit(int accuracy);
static int getDamageReduction(StatsComponent *stats);
static GameObject createProjectile(const Sfx &sfx,
    GameObject attacker, GameObject target);
static void addTurnComponent(GameObject source, GameObject target);

void WeaponSystem::update() {
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<WeaponComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    WeaponComponent *wc = em.getComponentForEntity<WeaponComponent>(*it);
    if (wc->target_id) {
      startAttackAnimation(*it, wc->target_id);
      if (wc->max_range > 1) {
        GameObject sfx = createProjectile(wc->projectile_sfx,
            *it, wc->target_id);
        if (sfx.getId() == 0) {
          dealDamage(*it, wc->target_id);
        }
      } else {
        dealDamage(*it, wc->target_id);
      }
    }
    wc->target_id = 0;
  }
}

int WeaponSystem::getStatBonus(StatsComponent *stats, WeaponComponent *weapon)
{
  if (!stats) {
    return 0;
  }
  int bonus = stats->attack * weapon->stats_multipliers.attack;
  return bonus;
}

void WeaponSystem::startAttackAnimation(GameObject attacker, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  AnimationComponent *ac =
      em.getComponentForEntity<AnimationComponent>(attacker);
  if (ac && !ac->attacking) {
    ac->start_time = SDL_GetTicks();
    ac->attacking = true;
  }
}

void dealDamage(GameObject attacker, GameObject target)
{
  int dmg = calculateDamage(attacker, target);
  HealthSystem::changeHealth(attacker, target, -dmg);
  if (dmg == 0) {
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.Miss, 0);
  } else {
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.SwordHit, 0);
  }
}

int calculateDamage(GameObject attacker, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  WeaponComponent *wc = em.getComponentForEntity<WeaponComponent>(attacker);
  int dmg = 0;
  if (isHit(wc->accuracy)) {
    StatsComponent *stats = em.
        getComponentForEntity<StatsComponent>(attacker);
    int stat_bonus = WeaponSystem::getStatBonus(stats, wc);
    int min_damage = wc->min_damage + stat_bonus;
    int max_damage = wc->max_damage + stat_bonus;
    dmg = rand() % (max_damage - min_damage + 1) + min_damage;
    // If is critical hit, damage reduction is ignored.
    if (!isHit(wc->crit_chance)) {
      StatsComponent *target_stats = em.
          getComponentForEntity<StatsComponent>(target);
      int dmg_reduction = getDamageReduction(target_stats);
      dmg -= dmg_reduction;
    }
    if (!wc->spell.empty()) {
      SpellEffects::apply(attacker, target, wc->spell);
    }
    //Prevets heal by negative dmg.
    if (dmg < 0) {
      dmg = 0;
    }
  }
  return dmg;
}

bool isHit(int accuracy)
{
  return (rand() % 100) < accuracy;
}

int getDamageReduction(StatsComponent *stats)
{
  return stats->armor;
}

static void addTurnComponent(GameObject source, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  TurnComponent *turn = em.getComponentForEntity<TurnComponent>(source);
  if (turn) {
    TurnComponent *part_turn = new TurnComponent();
    part_turn->team = turn->team;
    part_turn->turn_taken = false;
    em.addComponentToEntity(part_turn, target);
  }
}

static GameObject createProjectile(const Sfx &sfx,
    GameObject attacker, GameObject target)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *attacker_space =
      em.getComponentForEntity<SpaceComponent>(attacker);
  SpaceComponent *target_space =
      em.getComponentForEntity<SpaceComponent>(target);
  if (!attacker_space || !target_space) {
    return 0;
  }
  GameObject proj_obj = em.createEntity();
  if (proj_obj.getId() == 0) {
    fprintf(stderr, "Unable to create projectile.\n");
    return 0;
  }
  SpaceComponent *proj_space = new SpaceComponent();
  proj_space->pos = attacker_space->pos;
  RendererComponent *sprite = new RendererComponent(sfx.sprite.c_str());
  sprite->width = sfx.width;
  sprite->height = sfx.height;
  sprite->x_offset = sfx.x_offset;
  sprite->y_offset = sfx.y_offset;
  sprite->start();
  AnimationComponent *anim = new AnimationComponent();
  anim->max_frames = sfx.max_frames;
  anim->frame_rate = sfx.frame_rate;
  anim->start_time = SDL_GetTicks();
  ProjectileComponent *proj = new ProjectileComponent();
  proj->end = target_space->pos;
  MovementComponent *mov = new MovementComponent();
  mov->speed = sfx.speed;
  em.addComponentToEntity(proj_space, proj_obj);
  em.addComponentToEntity(sprite, proj_obj);
  em.addComponentToEntity(anim, proj_obj);
  em.addComponentToEntity(proj, proj_obj);
  em.addComponentToEntity(mov, proj_obj);
  proj->attacker = attacker;
  proj->target = target;
  proj->attack_fun = dealDamage;
  addTurnComponent(attacker, proj_obj);
  return proj_obj;
}
