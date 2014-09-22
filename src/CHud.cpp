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
#include "CHud.h"

#include "AnimationComponent.h"
#include "CText.h"
#include "CArea.h"
#include "CFPS.h"
#include "COptions.h"
#include "CSurface.h"
#include "CCamera.h"
#include "CTile.h"
#include "data/dirs.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "ItemRenderSystem.h"
#include "SpaceComponent.h"
#include "StatsComponent.h"
#include "TrapComponent.h"
#include "TurnComponent.h"
#include "Visibility.h"
#include "WeaponComponent.h"
#include "WeaponSystem.h"

CHud CHud::HUD;

CHud::CHud() : hud_inventory_(*this), hud_targeting_(*this), player_(0)
{
  Surf_HPGlobe = NULL;
  Surf_Interface = NULL;
  Surf_WinScreen = NULL;
  Surf_DeathScreen = NULL;
  Surf_FoV = NULL;
  Surf_HelpScreen = NULL;
  Surf_Candle = NULL;
  Surf_Stats = NULL;
  
  HealthPosX = 110;
  HealthPosY = 482;
  
  DLevelPosX = 680;
  DLevelPosY = 10;
  
  player_health_ = 0;
  player_inventory_ = 0;
  player_space_ = 0;
  player_stats_ = 0;
  player_weapon_ = 0;
  player_animation_ = 0;
  player_turn_ = 0;
  //StatUpdateTime = 0;
}


bool CHud::OnLoad()
{
  hud_inventory_.OnLoad();
  hud_targeting_.OnLoad();
  Surf_Interface = CSurface::OnLoad(data::FindFile("gfx/UI/HUD.png").c_str());
  if (!Surf_Interface) {
    return false;
  }
  Surf_HPGlobe = CSurface::OnLoad(data::FindFile("gfx/UI/health.png").c_str());
  if (!Surf_HPGlobe) {
    return false;
  }
  
  Surf_WinScreen =
      CSurface::OnLoad(data::FindFile("gfx/UI/WinScreen.png").c_str());
  if (!Surf_WinScreen) {
    return false;
  }
  Surf_DeathScreen =
      CSurface::OnLoad(data::FindFile("gfx/UI/DeathScreen.png").c_str());
  if (!Surf_DeathScreen) {
    return false;
  }
  if (!(Surf_FoV = CSurface::OnLoad(data::FindFile("gfx/UI/FoV.png").c_str()))) {
    return false;
  }
  Surf_HelpScreen =
      CSurface::OnLoad(data::FindFile("gfx/UI/HelpScreen.png").c_str());
  if (!Surf_HelpScreen) {
    return false;
  }
  Surf_Candle = CSurface::OnLoad(data::FindFile("gfx/UI/Candle.png").c_str());
  if (!Surf_Candle) {
    return false;
  }

  Surf_Stats = SDL_CreateRGBSurface(SDL_SWSURFACE, 200, 200, 32, 0, 0, 0, 0);
  if (!Surf_Stats) {
    return false;
  }
  CSurface::Transparent(Surf_Stats, 255, 0, 255);
  
  anim_control_.max_frames = 2;
  
  return true;
}

void CHud::start(GameObject player)
{
  player_ = player;
  EntityManager &em = EntityManager::instance();
  player_health_ = em.getComponentForEntity<HealthComponent>(player);
  player_inventory_ = em.getComponentForEntity<InventoryComponent>(player);
  player_space_ = em.getComponentForEntity<SpaceComponent>(player);
  player_stats_ = em.getComponentForEntity<StatsComponent>(player);
  player_weapon_ = em.getComponentForEntity<WeaponComponent>(player);
  player_animation_ = em.getComponentForEntity<AnimationComponent>(player);
  player_turn_ = em.getComponentForEntity<TurnComponent>(player);
  hud_inventory_.start(player_);
  hud_targeting_.start(player_);
}

void CHud::OnCleanup()
{
  if (Surf_Interface) SDL_FreeSurface(Surf_Interface);
  if (Surf_HPGlobe) SDL_FreeSurface(Surf_HPGlobe);
  if (Surf_WinScreen) SDL_FreeSurface(Surf_WinScreen);
  if (Surf_DeathScreen) SDL_FreeSurface(Surf_DeathScreen);
  if (Surf_FoV) SDL_FreeSurface(Surf_FoV);
  if (Surf_HelpScreen) SDL_FreeSurface(Surf_HelpScreen);
  if (Surf_Candle) SDL_FreeSurface(Surf_Candle);
  if (Surf_Stats) SDL_FreeSurface(Surf_Stats);
}

void CHud::OnRender(SDL_Surface *display)
{
  drawHealth(display);
  drawCandle(display);
  drawPotionCount(display);
  drawScrollCount(display);
  drawMiniMap(display);
  drawCharacterSheet(display);
  int y = 20 + (COptions::options.getScreenHeight() - WHEIGHT) / 2;
  int x = 0;
  hud_inventory_.render(display, x, y);
  hud_targeting_.render(display);
  if (player_health_->is_dead) {
    if (!player_animation_ ||
        player_animation_->start_time + 1000 < SDL_GetTicks()) {
      DisplayDeathScreen(display);
    }
  }
  if (CArea::area_control.dungeon_level == 20) {
    DisplayWinScreen(display);
  }
}

void CHud::drawCandle(SDL_Surface *display)
{
  float light_perc = 1.0f * player_health_->light / player_health_->max_light;
  int CurrentFrameRow = 0;
  int CropOffsetY = 32 * light_perc;
  
  int hy = HUD_Y + COptions::options.getScreenHeight() - WHEIGHT;
  
  if (light_perc > 0.01f)
  {
    if (light_perc < 0.33f) {
      CurrentFrameRow = 2;
    } else if (light_perc < 0.66f) {
      CurrentFrameRow = 1;
    }
    
    //Draw the base of the candle
    CSurface::OnDraw(display, Surf_Candle,
        HUD_X + HUD_CANDLE_OFFSET_X, hy + HUD_CANDLE_OFFSET_Y,
        0, CurrentFrameRow * 32, 32, 32);
    
    //Draw the shaft of the candle
    CSurface::OnDraw(display, Surf_Candle,
        HUD_X + HUD_CANDLE_SHAFT_OFFSET_X,
        hy + HUD_CANDLE_SHAFT_OFFSET_Y + (32 - CropOffsetY),
        32, 32 - CropOffsetY, 32, CropOffsetY);
    
    //Draw the flame of the candle
    if (light_perc < 0.33f)
    {
      CSurface::OnDraw(display, Surf_Candle,
          HUD_X + HUD_CANDLE_FLAME_OFFSET_X,
          hy + HUD_CANDLE_FLAME_OFFSET_Y + (32 - CropOffsetY),
          64, (CurrentFrameRow + 1)*32, 32, 32);
    }
    else
    {
      CSurface::OnDraw(display, Surf_Candle,
          HUD_X + HUD_CANDLE_FLAME_OFFSET_X,
          hy + HUD_CANDLE_FLAME_OFFSET_Y + (32 - CropOffsetY),
          64, (CurrentFrameRow + anim_control_.GetCurrentFrame())*32,
          32, 32);
    }
  }
  else
  {
    CurrentFrameRow = 3;
    
    //Draw the base of the candle
    CSurface::OnDraw(display, Surf_Candle,
      HUD_X + HUD_CANDLE_OFFSET_X, hy + HUD_CANDLE_OFFSET_Y, 0,
      CurrentFrameRow*32, 32, 32);
    
    //Draw the flame of the candle
    CSurface::OnDraw(display, Surf_Candle,
      HUD_X + HUD_CANDLE_FLAME_OFFSET_X,
      hy + HUD_CANDLE_FLAME_OFFSET_Y + 32,
      64, (CurrentFrameRow + 1)*32, 32, 32);
  }
}

void CHud::OnLoop()
{
  anim_control_.OnAnimate();
}

bool CHud::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  if (player_health_->is_dead) {
    return true;
  }
  if (player_health_->ailments & STATUS_IMMOBILE) {
    player_turn_->turn_taken = true;
    return true;
  }
  if (!hud_targeting_.onKeyDown(sym, mod, unicode)) {
    return hud_inventory_.onKeyDown(sym, mod, unicode);
  } else {
    return true;
  }
}

void CHud::updateStats()
{
  SDL_FillRect(Surf_Stats, NULL, SDL_MapRGB(Surf_Stats->format, 255, 0, 255));
  
  //Draw stats
  std::string Text;
  int X = WINDOW_TILE;
  int Y = WINDOW_TILE;
  
  //Draw dungeon level
  int dungeon_level = CArea::dungeon_level;
  std::string DLevel = "Dungeon Level: "
      + CText::TextControl.ConvertInt(dungeon_level);
  
  //Draw Name
  std::string Name = "Name here";

  //Draw Level
  std::string Level = "Level: " +
      CText::TextControl.ConvertInt(player_stats_->level) + " ";

  //Draw Experience
  std::string XP = "XP: " + CText::TextControl.ConvertInt(player_stats_->xp) +
      '/' + CText::TextControl.ConvertInt(player_stats_->level * 1000);

  //Draw Attack
  int stat_bonus = WeaponSystem::getStatBonus(player_stats_, player_weapon_);
  int min_attack = player_weapon_->min_damage + stat_bonus;
  int max_attack = player_weapon_->max_damage + stat_bonus;
  std::string Attack = "Attack: " +
      CText::TextControl.ConvertInt(min_attack) +
      '-' + CText::TextControl.ConvertInt(max_attack);
  
  //Draw Armor
  std::string Armor = "Armor: " +
      CText::TextControl.ConvertInt(player_stats_->armor);
  
  Text = ' ' + DLevel + " \n\n " + Name + " \n\n " + Level + " \n\n " +
      XP + " \n\n " + Attack + " \n\n " + Armor + ' ';
  CText::TextControl.displayBMPText(Surf_Stats, X, Y,
      Text, 29, 90, 49, -2, -2, -2, FONT_SMALL, 1);
}

void CHud::activateTargeting(Targeting targeting, std::string &spell)
{
  activateTargeting(targeting, spell, 0);
}

void CHud::activateTargeting(Targeting targeting, std::string &spell,
    GameObject item)
{
  switch (targeting) {
  case TARGETING_ENTITY:
    hud_targeting_.activateTargeting(spell, item);
    break;
  case TARGETING_ITEM:
    hud_inventory_.activateTargeting(spell, item);
    break;
  default:
    break;
  }
}

void CHud::drawCharacterSheet(SDL_Surface *display)
{
  int X = DLevelPosX - WINDOW_TILE +
      COptions::options.getScreenWidth() - WWIDTH;
  int Y = DLevelPosY - WINDOW_TILE;
  CSurface::OnDraw(display, Surf_Stats, X, Y);
}

void CHud::updateInventory(int Highlight)
{
  return hud_inventory_.updateInventory(Highlight);
}

void CHud::updateSpellBook(int Highlight)
{
  return hud_inventory_.updateScrollBook(Highlight);
}

void CHud::drawPotionCount(SDL_Surface *display)
{
  int potion_counter = 0;
  std::vector<unsigned> &items = player_inventory_->items;
  for (unsigned i = 0; i < items.size(); ++i)
  {
    if (isPotion(items[i])) {
      potion_counter++;
    }
  }
  
  int PosX = 110;
  if (potion_counter > 9) PosX = 110;
  int PosY = 557 + COptions::options.getScreenHeight() - WHEIGHT;
  
  CText::TextControl.displayBMPText(display, PosX, PosY,
      CText::TextControl.ConvertInt(potion_counter),
      255, 255, 255, -1, -1, -1, FONT_SMALL, 0);
}

bool CHud::isPotion(GameObject obj)
{
  EntityManager &em = EntityManager::instance();
  ItemComponent *item = em.getComponentForEntity<ItemComponent>(obj);
  if (item && item->useType == ItemComponent::USE_TYPE_POTION) {
    return true;
  } else {
    return false;
  }
}

void CHud::drawScrollCount(SDL_Surface *display)
{
  //Draw number of scrolls on the bookmark
  int NumberOfScrolls = (player_inventory_->scrolls).size();
  int X = HUD_X + HUD_SCROLLS_OFFSET_X;
  int Y = HUD_Y + HUD_SCROLLS_OFFSET_Y +
    COptions::options.getScreenHeight() - WHEIGHT;
  CText::TextControl.displayBMPText(display, X, Y,
      CText::TextControl.ConvertInt(NumberOfScrolls),
      255, 255, 255, -1, -1, -1, FONT_SMALL, 0);
}

void CHud::drawMiniMap(SDL_Surface *display)
{
  CArea::area_control.MiniMapOnRender(display,
      player_space_->pos.x, player_space_->pos.y);
}

void CHud::DebugMode(SDL_Surface *display)
{ 
  char DebugText[255] = {0};
  strcat(DebugText, " FPS: ");
  strcat(DebugText,
    CText::TextControl.ConvertInt(CFPS::fps_control.GetFPS()).c_str());
  
  CText::TextControl.displayBMPText(display, 0, 0,
      DebugText, 255, 255, 255, 0, 0, 0, FONT_SMALL, 0);
}

void CHud::DisplayWinScreen(SDL_Surface *display)
{
  CSurface::OnDraw(display, Surf_WinScreen, 0, 0);
}

void CHud::DisplayDeathScreen(SDL_Surface *display)
{
  CSurface::OnDraw(display, Surf_DeathScreen, 0, 0);
}

void CHud::DisplayFoV(SDL_Surface *display)
{
}

void CHud::FoWCalculation()
{ 
  unsigned short circle_mask[13*13] = {
    FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_CNW, FOW_NNN, FOW_CNE, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL,
    FOW_ALL, FOW_ALL, FOW_ALL, FOW_CNW, FOW_NNN, FOW_JNW, FOW_NON, FOW_JNE, FOW_NNN, FOW_CNE, FOW_ALL, FOW_ALL, FOW_ALL,
    FOW_ALL, FOW_ALL, FOW_CNW, FOW_JNW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JNE, FOW_CNE, FOW_ALL, FOW_ALL,
    FOW_ALL, FOW_CNW, FOW_JNW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JNE, FOW_CNE, FOW_ALL,
    FOW_ALL, FOW_WWW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_EEE, FOW_ALL,
    FOW_CNW, FOW_JNW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JNE, FOW_CNE,
    FOW_WWW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_EEE,
    FOW_CSW, FOW_JSW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JSE, FOW_CSE,
    FOW_ALL, FOW_WWW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_EEE, FOW_ALL,
    FOW_ALL, FOW_CSW, FOW_JSW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JSE, FOW_CSE, FOW_ALL,
    FOW_ALL, FOW_ALL, FOW_CSW, FOW_JSW, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_NON, FOW_JSE, FOW_CSE, FOW_ALL, FOW_ALL,
    FOW_ALL, FOW_ALL, FOW_ALL, FOW_CSW, FOW_SSS, FOW_JSW, FOW_NON, FOW_JSE, FOW_SSS, FOW_CSE, FOW_ALL, FOW_ALL, FOW_ALL,
    FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_CSW, FOW_SSS, FOW_CSE, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL, FOW_ALL };
  
  int Range = 6;
  int X = player_space_->pos.x;
  int Y = player_space_->pos.y;
  
  int mask_index = 0;
  for (int j = Y - Range; j <= Y + Range; ++j) {
    for (int i = X - Range; i <= X + Range; ++i) {
      if (i < 0 || i >= MAP_WIDTH || j < 0 || j >= MAP_HEIGHT) {
        //prevents getting out of the map
        mask_index++;
        continue;
      }
      if (circle_mask[mask_index] != FOW_ALL) {
        CTile *tile = CArea::area_control.GetTile(i * TILE_SIZE, j * TILE_SIZE);
        tile->is_explored = true;
        tile->fow_mask &= circle_mask[mask_index];
        if (Visibility::InLOS(X, Y, i, j)) {
          tile->is_visible = true;
        }
      }
      mask_index++;
    }
  }
}

void CHud::RenderHelpScreen(SDL_Surface *display)
{
  CSurface::OnDraw(display, Surf_HelpScreen, 0, 0);
}

void CHud::drawHealth(SDL_Surface *display)
{
  int X = HUD_X;
  int Y = HUD_Y + COptions::options.getScreenHeight() - WHEIGHT;
  CSurface::OnDraw(display, Surf_Interface, X, Y);
  float HPPercentage = 0;
  HPPercentage = (float)player_health_->health / player_health_->max_health;
  int GlobeHeight = 64 * HPPercentage;
  int GlobeY = 64 - 64 * HPPercentage;
  X = HUD_X + HUD_HEALTH_OFFSET_X;
  Y = HUD_Y + HUD_HEALTH_OFFSET_Y + GlobeY +
    COptions::options.getScreenHeight() - WHEIGHT;
  CSurface::OnDraw(display, Surf_HPGlobe, X, Y, 0, GlobeY, 64, GlobeHeight);
  
  //Draw player Health/MaxHealth
  std::string health_text = CText::TextControl.
                ConvertInt(player_health_->health);
  health_text = health_text + " / " +
          CText::TextControl.ConvertInt(player_health_->max_health);
  X = HealthPosX;
  Y = HealthPosY + COptions::options.getScreenHeight() - WHEIGHT;
  CText::TextControl.displayBMPText(
      display, X, Y, health_text, 255, 255, 255, 0, 0, 0, FONT_SMALL, 0);
  
}
