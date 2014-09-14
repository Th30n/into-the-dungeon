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
#include "SpellEffects.h"

#include <cstdio>
#include <cstdlib>

#include "AnimationComponent.h"
#include "DOTComponent.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "HealthSystem.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "MovementComponent.h"
#include "ParticleComponent.h"
#include "ProjectileComponent.h"
#include "Rectangle.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "SpellComponent.h"
#include "TurnComponent.h"

void SpellEffects::apply(GameObject source, GameObject target,
    const std::string &name)
{
  //printf("Applying spell '%s'\n", name.c_str());
  GameObject spell_obj = EntityFactory::instance().createSpell(name);
  EntityManager &em = EntityManager::instance();
  SpellComponent *spell = em.getComponentForEntity<SpellComponent>(spell_obj);
  if (!spell) {
    fprintf(stderr, "Invalid spell: '%s'\n", name.c_str());
    em.removeEntity(spell_obj);
    return;
  }
  spell->source = source;
  spell->target = target;
  createSfx(spell_obj);
}

void SpellEffects::apply(GameObject spell_obj)
{
  EntityManager &em = EntityManager::instance();
  SpellComponent *spell = em.getComponentForEntity<SpellComponent>(spell_obj);
  if (!spell) {
    fprintf(stderr, "Invalid spell object.");
    return;
  }
  std::vector<GameObject> targets;
  targets.push_back(spell->target);
  addAreaTargets(spell, targets);
  std::vector<Effect> &effects = spell->effects;
  std::vector<GameObject>::iterator target = targets.begin();
  for (; target != targets.end(); ++target) {
    std::vector<Effect>::iterator it = effects.begin();
    for (; it != effects.end(); ++it) {
      it->source = spell->source;
      it->target = *target;
      createEffect(*it);
    }
  }
  if (spell->targeting == "targetField" ||
      spell->targeting == "adjacentField") {
    // This is needed while the map isn't component based.
    // Remove target field object.
    em.removeEntity(spell->target);
  }
}

void SpellEffects::addAreaTargets(SpellComponent *spell,
    std::vector<GameObject> &targets)
{
  if (spell->aoe == "square") {
    targetSquareArea(spell, targets);
  }
}

void SpellEffects::targetSquareArea(SpellComponent *spell,
    std::vector<GameObject> &targets)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *space = em.
      getComponentForEntity<SpaceComponent>(spell->target);
  if (!space) {
    return;
  }
  int width = spell->radius;
  int max_x = space->pos.x + (width - 1) / 2;
  int min_x = max_x - (width - 1);
  int max_y = space->pos.y + (width - 1) / 2;
  int min_y = max_y - (width - 1);
  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      GameObject t = getTargetAtPos(Vector2f(x, y));
      if (t.getId() != 0 && t != spell->target) {
        targets.push_back(t);
      }
    }
  }
}

GameObject SpellEffects::getTargetAtPos(Vector2f pos)
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    SpaceComponent *space = em.getComponentForEntity<SpaceComponent>(*it);
    if (space) {
      Rectangle r(space->pos.x, space->pos.y,
          space->width, space->height);
      if (r.intersects(pos.x, pos.y)) {
        return *it;
      }
    }
  }
  return 0;
}

void SpellEffects::createEffect(const Effect &effect)
{
  //printf("Effect: [type: %s]\n", effect.type.c_str());
  if (effect.type == "dot") {
    applyDOT(effect);
  } else if (effect.type == "damage") {
    applyDamage(effect);
  } else if (effect.type == "heal") {
    applyHeal(effect);
  } else if (effect.type == "trigger") {
    applyTrigger(effect);
  } else if (effect.type == "lifesteal") {
    applyLifesteal(effect);
  } else if (effect.type == "teleportRandom") {
    applyTeleportRandom(effect);
  } else if (effect.type == "blessing") {
    applyBlessing(effect);
  } else if (effect.type == "itemMod") {
    applyItemMod(effect);
  } else if (effect.type == "identify") {
    applyIdentify(effect);
  } else if (effect.type == "spawnEntity") {
    applySpawnEntity(effect);
  }
}

void SpellEffects::applyDOT(const Effect &effect)
{
  GameObject dot_obj = EntityFactory::instance().createDOT();
  EntityManager &em = EntityManager::instance();
  DOTComponent *dot = em.getComponentForEntity<DOTComponent>(dot_obj);
  dot->source = effect.source;
  dot->target = effect.target;
  dot->duration = effect.duration;
  dot->health_change = -effect.damage;
  dot->ailments = ailmentFromString(effect.ailment);
  dot->spell = effect.spell;
  TurnComponent *target_turn = em.
      getComponentForEntity<TurnComponent>(effect.target);
  TurnComponent *dot_turn = em.getComponentForEntity<TurnComponent>(dot_obj);
  dot_turn->team = target_turn->team;
}

void SpellEffects::applyDamage(const Effect &effect)
{
  HealthSystem::changeHealth(effect.source, effect.target, -effect.damage);
}

void SpellEffects::applyHeal(const Effect &effect)
{
  HealthSystem::changeHealth(effect.source, effect.target, effect.damage);
}

void SpellEffects::applyTrigger(const Effect &effect)
{
  // Calculate trigger chance, if lower doesn't apply.
  if (rand() % 100 >= effect.chance) {
    return;
  }
  if (!effect.spell.empty()) {
    apply(effect.source, effect.target, effect.spell);
  }
}

void SpellEffects::applyLifesteal(const Effect &effect)
{
  HealthSystem::changeHealth(effect.source, effect.target, -effect.damage);
  HealthSystem::changeHealth(effect.source, effect.source, effect.damage);
}

void SpellEffects::applyTeleportRandom(const Effect &effect)
{
  EntityFactory::instance().spawn(effect.target);
}

void SpellEffects::applyBlessing(const Effect &effect)
{
  EntityManager &em = EntityManager::instance();
  InventoryComponent *inv = em.
      getComponentForEntity<InventoryComponent>(effect.target);
  if (!inv) {
    return;
  }
  for (unsigned i = 0; i < inv->items.size(); ++i) {
    ItemComponent *item = em.
      getComponentForEntity<ItemComponent>(inv->items[i]);
    item->cursed = false;
  }
  for (unsigned i = 0; i < inv->scrolls.size(); ++i) {
    ItemComponent *item = em.
      getComponentForEntity<ItemComponent>(inv->scrolls[i]);
    item->cursed = false;
  }
}

void SpellEffects::applyItemMod(const Effect &effect)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *item = em.
      getComponentForEntity<ItemComponent>(effect.target);
  if (!item) {
    return;
  }
  item->statsModifiers.health += effect.stats.health;
  item->statsModifiers.attack += effect.stats.attack;
  item->statsModifiers.armor += effect.stats.armor;
  if (item->equipped) {
    StatsComponent *stats = em.
        getComponentForEntity<StatsComponent>(effect.source);
    stats->health += effect.stats.health;
    HealthSystem::changeMaxHealth(effect.source, effect.stats.health);
    stats->attack += effect.stats.attack;
    stats->armor += effect.stats.armor;
  }
}

void SpellEffects::applyIdentify(const Effect &effect)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *item = em.
      getComponentForEntity<ItemComponent>(effect.target);
  if (item && !item->identified) {
    item->identified = true;
  }
}

void SpellEffects::applySpawnEntity(const Effect &effect)
{
  GameObject go = EntityFactory::instance().createEntity(effect.entity);
  EntityManager &em = EntityManager::instance();
  SpaceComponent *space = em.
      getComponentForEntity<SpaceComponent>(effect.target);
  EntityFactory::instance().spawn(go, space->pos);
}

void SpellEffects::createSfx(GameObject spell_obj)
{
  createPreSfx(spell_obj);
}

static GameObject createProjectile(const Sfx &sfx, Vector2f start, Vector2f end)
{
  EntityManager &em = EntityManager::instance();
  GameObject proj_obj = em.createEntity();
  if (proj_obj.getId() == 0) {
    fprintf(stderr, "Unable to create projectile.\n");
    return 0;
  }
  SpaceComponent *proj_space = new SpaceComponent();
  proj_space->pos = start;
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
  proj->end = end;
  MovementComponent *mov = new MovementComponent();
  mov->speed = sfx.speed;
  em.addComponentToEntity(proj_space, proj_obj);
  em.addComponentToEntity(sprite, proj_obj);
  em.addComponentToEntity(anim, proj_obj);
  em.addComponentToEntity(proj, proj_obj);
  em.addComponentToEntity(mov, proj_obj);
  return proj_obj;
}

static GameObject createParticle(const Sfx &sfx, Vector2f pos)
{
  EntityManager &em = EntityManager::instance();
  GameObject part_obj = em.createEntity();
  if (part_obj.getId() == 0) {
    fprintf(stderr, "Unable to create particle.\n");
    return 0;
  }
  SpaceComponent *part_space = new SpaceComponent();
  part_space->pos = pos;
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
  ParticleComponent *part = new ParticleComponent();
  part->time_to_live = sfx.max_frames * sfx.frame_rate;
  part->start_time =  SDL_GetTicks();
  em.addComponentToEntity(part_space, part_obj);
  em.addComponentToEntity(sprite, part_obj);
  em.addComponentToEntity(anim, part_obj);
  em.addComponentToEntity(part, part_obj);
  return part_obj;
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

void SpellEffects::createPreSfx(GameObject spell_obj)
{
  EntityManager &em = EntityManager::instance();
  SpellComponent *spell = em.getComponentForEntity<SpellComponent>(spell_obj);
  SpaceComponent *space = em.
      getComponentForEntity<SpaceComponent>(spell->source);
  SpaceComponent *target = em.
      getComponentForEntity<SpaceComponent>(spell->target);
  if (space && spell->pre_sfx.type == "animation") {
    GameObject sfx = createParticle(spell->pre_sfx, space->pos);
    if (sfx.getId() != 0) {
      ParticleComponent *part = em.
          getComponentForEntity<ParticleComponent>(sfx);
      part->attacker = spell_obj;
      part->attack_fun = createPostSfx;
      addTurnComponent(spell->source, sfx);
    } else {
      createPostSfx(spell_obj);
    }
  } else if (space && target && spell->pre_sfx.type == "projectile") {
    GameObject sfx = createProjectile(spell->pre_sfx,
        space->pos, target->pos);
    if (sfx.getId() != 0) {
      ProjectileComponent *proj = em.
          getComponentForEntity<ProjectileComponent>(sfx);
      proj->attacker = spell_obj;
      proj->attack_fun = createPostSfx;
      addTurnComponent(spell->source, sfx);
    } else {
      createPostSfx(spell_obj);
    }
  } else {
    createPostSfx(spell_obj);
  }
}

// Callback for projectile/particle
void SpellEffects::createPostSfx(GameObject attacker, GameObject target)
{
  createPostSfx(attacker);
}

static GameObject getBounceTarget(SpellComponent *spell);

void SpellEffects::createPostSfx(GameObject spell_obj)
{
  apply(spell_obj);

  EntityManager &em = EntityManager::instance();
  SpellComponent *spell = em.getComponentForEntity<SpellComponent>(spell_obj);
  SpaceComponent *space = em.
      getComponentForEntity<SpaceComponent>(spell->target);
  if (space && spell->post_sfx.type == "animation") {
    GameObject sfx = createParticle(spell->post_sfx, space->pos);
    if (sfx.getId() != 0) {
      addTurnComponent(spell->source, sfx);
    }
  } else if (spell->post_sfx.type == "projectile") {
    SpaceComponent *target = em.
        getComponentForEntity<SpaceComponent>(spell->target);
    if (target) {
      GameObject proj = createProjectile(spell->post_sfx,
          space->pos, target->pos);
      if (proj.getId() != 0) {
        addTurnComponent(spell->source, proj);
      }
    }
  }
  if (spell->bounces <= 0) {
    em.removeEntity(spell_obj);
  } else {
    spell->bounces--;
    GameObject bounce_target = getBounceTarget(spell);
    if (bounce_target.getId() != 0) {
      spell->source = spell->target;
      spell->target = bounce_target;
      createPreSfx(spell_obj);
    } else {
      em.removeEntity(spell_obj);
    }
  }
}

static GameObject getBounceTarget(SpellComponent *spell)
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  std::vector<GameObject> targets;
  SpaceComponent *current_space = em.
    getComponentForEntity<SpaceComponent>(spell->target);
  Vector2f current_pos = current_space->pos;
  for (; it != entities.end(); ++it) {
    if (*it == spell->target || *it == spell->source) {
      // Ignore current target and caster
      continue;
    }
    HealthComponent *hp = em.getComponentForEntity<HealthComponent>(*it);
    if (hp->is_dead) {
      // Ignore dead entities
      continue;
    }
    SpaceComponent *sp = em.getComponentForEntity<SpaceComponent>(*it);
    if (current_pos.manhattanDistance(sp->pos) < spell->bounce_range) {
      targets.push_back(*it);
    }
  }
  if (targets.empty()) {
    return 0;
  } else {
    return targets[rand() % targets.size()];
  }
}
