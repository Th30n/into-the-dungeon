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
#include "HudTargeting.h"

#include <cstdio>

#include "CCamera.h"
#include "CHud.h"
#include "COptions.h"
#include "CSurface.h"
#include "CText.h"
#include "data/dirs.h"
#include "Define.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "ItemUsage.h"
#include "SpaceComponent.h"
#include "SpellEffects.h"
#include "TurnComponent.h"
#include "Visibility.h"

HudTargeting::HudTargeting(CHud &hud)
  : owner_(hud), player_(0),
    is_active_(false),
    player_space_(0), player_inventory_(0), player_turn_(0),
    target_image_(0),
    selected_target_(0), spell_item_(0)
{
}

HudTargeting::~HudTargeting()
{
  if (target_image_) {
    SDL_FreeSurface(target_image_);
  }
}

bool HudTargeting::OnLoad()
{
  target_image_ = CSurface::OnLoad(data::FindFile("gfx/UI/Target.png").c_str());
  return true;
}

void HudTargeting::start(GameObject obj)
{
  player_ = obj;
  EntityManager &em = EntityManager::instance();
  player_space_ = em.getComponentForEntity<SpaceComponent>(player_);
  player_inventory_ = em.getComponentForEntity<InventoryComponent>(player_);
  player_turn_ = em.getComponentForEntity<TurnComponent>(player_);
}

void HudTargeting::render(SDL_Surface *display)
{
  if (is_active_) {
    std::string text = "\t\t\tTARGETING MODE!\n";
    text += " Select with arrow keys and press enter to fire. \n";
    text += " Alternatively press the assigned number twice. \n";
    text += "\t\tPress ESC to quit targeting. ";
    CText::TextControl.displayBMPText(display,
          COptions::options.getScreenWidth() / 4, 0,
          text, 255, 0, 0, 0, 0, 0, FONT_SMALL, 0);
    drawValidTargets(display);
  }
}

bool HudTargeting::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  if (is_active_) {
    switch (sym) {
    case SDLK_ESCAPE:
      is_active_ = false;
      return true;
    case SDLK_LEFT:
    case SDLK_DOWN:
      selectTarget(selected_target_ - 1);
      return true;
    case SDLK_RIGHT:
    case SDLK_UP:
      selectTarget(selected_target_ + 1);
      return true;
    case SDLK_RETURN:
      selectTarget(selected_target_);
      return true;
    case SDLK_0:
      selectTarget(0);
      return true;
    case SDLK_1:
      selectTarget(1);
      return true;
    case SDLK_2:
      selectTarget(2);
      return true;
    case SDLK_3:
      selectTarget(3);
      return true;
    case SDLK_4:
      selectTarget(4);
      return true;
    case SDLK_5:
      selectTarget(5);
      return true;
    case SDLK_6:
      selectTarget(6);
      return true;
    case SDLK_7:
      selectTarget(7);
      return true;
    case SDLK_8:
      selectTarget(8);
      return true;
    case SDLK_9:
      selectTarget(9);
      return true;
    default:
      return false;
    }
  }
  return false;
}

void HudTargeting::activateTargeting(std::string &spell, GameObject item)
{
  spell_item_ = item;
  spell_name_ = spell;
  is_active_ = true;
  selected_target_ = 0;
  getValidTargets();
}

void HudTargeting::activateTargeting(std::string &spell)
{
  activateTargeting(spell, 0);
}

void HudTargeting::drawValidTargets(SDL_Surface *display)
{
  EntityManager &em = EntityManager::instance();
  for (unsigned i = 0; i < targets_.size(); ++i) {
    GameObject &obj = targets_[i];
    SpaceComponent *tspace = em.getComponentForEntity<SpaceComponent>(obj);
    int x = tspace->pos.x * TILE_SIZE - CCamera::camera_control.GetX();
    int y = tspace->pos.y * TILE_SIZE - CCamera::camera_control.GetY();
    std::string num  = CText::TextControl.ConvertInt(i);
    CText::TextControl.displayBMPText(display, x, y,
        num, 255, 255, 255, 0, 0, 0, FONT_SMALL, 0);
    if (selected_target_ == i) {
      CSurface::OnDraw(display, target_image_, x, y);
    }
  }
}

void HudTargeting::getValidTargets()
{
  targets_.clear();
  EntityManager &em = EntityManager::instance();
  std::vector<GameObject> entities;
  em.getEntitiesWithComponent<HealthComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    if (it->getId() == player_.getId()) {
      continue;
    }
    HealthComponent *thp = em.getComponentForEntity<HealthComponent>(*it);
    if (thp->is_dead) {
      continue;
    }
    SpaceComponent *tspace = em.getComponentForEntity<SpaceComponent>(*it);
    if (player_space_->pos.manhattanDistance(tspace->pos) > 6) {
      continue;
    }
    if (Visibility::InLOS(player_space_->pos, tspace->pos)) {
      targets_.push_back(*it);
    }
  }
}

void HudTargeting::selectTarget(unsigned n)
{
  if (selected_target_ == n && selected_target_ < targets_.size()) {
    fire();
  } else {
    selected_target_ = n;
    if (selected_target_ >= targets_.size()) {
      selected_target_ = 0;
    }
  }
}

void HudTargeting::fire()
{
  SpellEffects::apply(player_, targets_[selected_target_], spell_name_);
  if (spell_item_.getId() != 0) {
    ItemUsage::removeFromInventory(player_inventory_->items, spell_item_);
    ItemUsage::removeFromInventory(player_inventory_->scrolls, spell_item_);
    EntityManager::instance().removeEntity(spell_item_);
    owner_.updateInventory();
    owner_.updateSpellBook();
  }
  is_active_ = false;
  player_turn_->turn_taken = true;
}
