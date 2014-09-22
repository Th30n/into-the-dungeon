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
#include "HealthSystem.h"

#include "AnimationComponent.h"
#include "CCamera.h"
#include "CHud.h"
#include "CMusic.h"
#include "CSurface.h"
#include "DamageIndicatorComponent.h"
#include "data/dirs.h"
#include "Define.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "ParticleComponent.h"
#include "RendererComponent.h"
#include "SpaceComponent.h"
#include "StatsComponent.h"

static void addExperience(GameObject target, GameObject source);

void HealthSystem::changeHealth(GameObject source, GameObject target, int dhp)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *thp = em.getComponentForEntity<HealthComponent>(target);
  if (!thp) {
    return;
  }
  thp->health += dhp;
  if (dhp == 0) {
    addIndicator(target, MISS, GREEN, dhp);
  } else if (dhp > 0) {
    addIndicator(target, NONE, GREEN, dhp);
  } else {
    addIndicator(target, NONE, RED, dhp);
  }
  if (thp->health <= 0) {
    thp->is_dead = true;
    addExperience(source, target);
    startDeathAnimation(target);
  }
}

void HealthSystem::changeMaxHealth(GameObject target, int dhp)
{
  changeMaxHealth(target, dhp, false);
}

void HealthSystem::changeMaxHealth(GameObject target, int dhp, bool heal)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *thp = em.getComponentForEntity<HealthComponent>(target);
  if (!thp) {
    return;
  }
  thp->max_health += dhp;
  if (heal) {
    thp->health = thp->max_health;
  } else {
    thp->health += dhp;
  }
  if (thp->health <= 0) {
    thp->is_dead = true;
    startDeathAnimation(target);
  }
}

void HealthSystem::changeLight(GameObject source, GameObject target, int dval)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *thp = em.getComponentForEntity<HealthComponent>(target);
  if (!thp) {
    return;
  }
  thp->light += dval;
  if (thp->light <= 0) {
    thp->is_dead = true;
    startDeathAnimation(target);
  }
}

void HealthSystem::addAilments(GameObject target, int ailments)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *thp = em.getComponentForEntity<HealthComponent>(target);
  if (!thp) {
    return;
  }
  thp->ailments |= ailments;
}

void HealthSystem::removeAilments(GameObject target, int ailments)
{
  EntityManager &em = EntityManager::instance();
  HealthComponent *thp = em.getComponentForEntity<HealthComponent>(target);
  if (!thp) {
    return;
  }
  thp->ailments &= ~ailments;
}

HealthSystem::HealthSystem(
        std::string overlays_filename, std::string status_filename)
  : overlays_filename_(overlays_filename), status_filename_(status_filename),
    overlay_image_(0), status_icons_(0)
{
}

HealthSystem::~HealthSystem()
{
  if (overlay_image_) {
    SDL_FreeSurface(overlay_image_);
  }
  if (status_icons_) {
    SDL_FreeSurface(status_icons_);
  }
}

void HealthSystem::start()
{
  overlay_image_ = CSurface::OnLoad(overlays_filename_.c_str());
  status_icons_ = CSurface::OnLoad(status_filename_.c_str());
}

void HealthSystem::update()
{
  EntityManager &em = EntityManager::instance();
  std::vector<DamageIndicatorComponent*> indicators;
  em.getAllComponentsOfType<DamageIndicatorComponent>(indicators);
  std::vector<DamageIndicatorComponent*>::iterator it = indicators.begin();
  for (; it != indicators.end(); ++it) {
    (*it)->overlays.remove_if(deleteExpired);
  }
}

void HealthSystem::render(SDL_Surface *display)
{
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    renderStatusIcons(display, *it);
    DamageIndicatorComponent *dic = em.
        getComponentForEntity<DamageIndicatorComponent>(*it);
    if (dic) {
      renderIndicator(display, *it);
    }
  }
}

void HealthSystem::addIndicator(GameObject obj, int txt, int col, int dhp)
{
  Overlay *overlay = new Overlay;
  overlay->text = txt;
  overlay->color = col;
  if (dhp < 0) {
    dhp = -dhp;
  }
  overlay->health_change = dhp;
  overlay->start_time = SDL_GetTicks();
  
  EntityManager &em = EntityManager::instance();
  DamageIndicatorComponent *dic =
      em.getComponentForEntity<DamageIndicatorComponent>(obj);
  if (!dic) {
    dic = new DamageIndicatorComponent();
    em.addComponentToEntity(dic, obj);
  }
  dic->overlays.push_back(overlay);
}

bool HealthSystem::deleteExpired(Overlay* overlay)
{
  if (overlay->start_time + HealthSystem::TIME_TO_LIVE < SDL_GetTicks()) {
    delete overlay;
    return true;
  }
  return false;
}

void HealthSystem::startDeathAnimation(GameObject object)
{
  EntityManager &em = EntityManager::instance();
  AnimationComponent *ac = em.getComponentForEntity<AnimationComponent>(object);
  if (ac && !ac->is_dead) {
    ac->start_time = SDL_GetTicks();
    ac->is_dead = true;
  }
}

void HealthSystem::renderStatusIcons(SDL_Surface *display, GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(obj);
  if (!sc) {
    return;
  }
  HealthComponent *hc = em.getComponentForEntity<HealthComponent>(obj);
  int icon_x = sc->pos.x * TILE_SIZE - CCamera::camera_control.GetX();
  int icon_y = sc->pos.y * TILE_SIZE - CCamera::camera_control.GetY();
  if (hc->ailments & STATUS_POISONED) {
    CSurface::OnDraw(display, status_icons_, icon_x, icon_y, 0, 0, 8, 8);
    icon_y += 8;
  }
  if (hc->ailments & STATUS_REGEN) {
    CSurface::OnDraw(display, status_icons_, icon_x, icon_y, 8, 0, 8, 8);
    icon_y += 8;
  }
  if (hc->ailments & STATUS_IMMOBILE) {
    CSurface::OnDraw(display, status_icons_, icon_x, icon_y, 16, 0, 8, 8);
    icon_y += 8;
  }
  if (hc->ailments & STATUS_INVISIBLE) {
    CSurface::OnDraw(display, status_icons_, icon_x, icon_y, 24, 0, 8, 8);
    icon_y += 8;
  }
  if (hc->ailments & STATUS_CONFUSED) {
    CSurface::OnDraw(display, status_icons_, icon_x, icon_y, 32, 0, 8, 8);
    icon_y += 8;
  }
}

void HealthSystem::renderIndicator(SDL_Surface *display, GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(obj);
  if (!sc) {
    return;
  }
  DamageIndicatorComponent *dic =
      em.getComponentForEntity<DamageIndicatorComponent>(obj);
  int i = 0;
  for (OverlayListIterator it = dic->overlays.begin();
      it != dic->overlays.end(); ++it) {
    Overlay *overlay = *it;
    if (overlay->start_time + DELAY_TIME > SDL_GetTicks()) {
      continue; //Displays text a bit later(after the attack swing)
    }
    //The First element is rendered highest 
    int vert_lvl = dic->overlays.size() - i;
    int target_y = sc->pos.y * TILE_SIZE + vert_lvl * -OVERLAY_HEIGHT;
    
    if (overlay->text == NONE) {
      int target_x = sc->pos.x * TILE_SIZE + OVERLAY_OFFSET_X + 5;
      int num = overlay->health_change;
      int target_num = num % 10;
      do {
        CSurface::OnDraw(display, overlay_image_,
            target_x - CCamera::camera_control.GetX(),
            target_y - CCamera::camera_control.GetY(),
            target_num * OVERLAY_NUMBER_W,
            overlay->color * OVERLAY_HEIGHT,
            OVERLAY_NUMBER_W, OVERLAY_HEIGHT);
        num = num / 10;
        target_num = num % 10;
        target_x -= OVERLAY_OFFSET_X;
      } while (num > 0);
    } else if (overlay->text == MISS) {
      int target_x = sc->pos.x * TILE_SIZE;
      CSurface::OnDraw(display, overlay_image_,
          target_x - CCamera::camera_control.GetX(),
          target_y - CCamera::camera_control.GetY(),
          overlay->color * OVERLAY_MISS_W,
          2*OVERLAY_HEIGHT, OVERLAY_MISS_W, OVERLAY_HEIGHT);
    } else if (overlay->text == BACKSTAB) {
      int target_x = sc->pos.x * TILE_SIZE - 16;
      CSurface::OnDraw(display, overlay_image_,
          target_x - CCamera::camera_control.GetX(),
          target_y - CCamera::camera_control.GetY(),
          OVERLAY_MISS_W * 2, 2*OVERLAY_HEIGHT,
          OVERLAY_BACKSTAB_W, OVERLAY_HEIGHT);
    }
    i++;
  }
}

static void createParticleEffect(Vector2f pos)
{
  EntityManager &em = EntityManager::instance();
  GameObject game_obj = em.createEntity();
  if (game_obj.getId() == 0) {
    fprintf(stderr, "Unable to create game_obj.\n");
    return;
  }
  SpaceComponent *space = new SpaceComponent();
  space->pos = pos;
  RendererComponent *sprite =
      new RendererComponent(data::FindFile("gfx/UI/LevelUp.png").c_str());
  sprite->width = 114;
  sprite->height = 24;
  sprite->x_offset = -42;
  sprite->y_offset = -24;
  sprite->start();
  ParticleComponent *particle = new ParticleComponent();
  particle->time_to_live = 2000;
  particle->velocity = Vector2f(0.0f, -0.01f);
  particle->start_time =  SDL_GetTicks();
  em.addComponentToEntity(space, game_obj);
  em.addComponentToEntity(sprite, game_obj);
  em.addComponentToEntity(particle, game_obj);
  CMusic::SoundControl.PlaySFX(CMusic::SoundControl.LevelUp, 0);
  return;
}

static void addExperience(GameObject target, GameObject source)
{
  EntityManager &em = EntityManager::instance();
  StatsComponent *target_stats =
      em.getComponentForEntity<StatsComponent>(target);
  if (!target_stats) {
    return;
  }
  StatsComponent *src_stats = em.getComponentForEntity<StatsComponent>(source);
  if (!src_stats) {
    return;
  }
  target_stats->xp += src_stats->xp;
  //printf("XP: %d\n", target_stats->xp);
  if (target_stats->xp >= target_stats->level * 1000) {
    target_stats->xp = 0;
    target_stats->level++;
    target_stats->attack++;
    HealthSystem::changeMaxHealth(target, 5, true);
    SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(target);
    createParticleEffect(sc->pos);
  }
  CHud::HUD.updateStats();
}
