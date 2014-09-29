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
#ifndef _CHUD_H_
#define _CHUD_H_

#include <SDL.h>

#include "Define.h"
#include "CAnimation.h"
#include "GameObject.h"
#include "HudInventory.h"
#include "HudTargeting.h"

#define HUD_X 0
#define HUD_Y 465
#define HUD_CANDLE_OFFSET_X 31
#define HUD_CANDLE_OFFSET_Y 40
#define HUD_CANDLE_SHAFT_OFFSET_X 34
#define HUD_CANDLE_SHAFT_OFFSET_Y 9
#define HUD_CANDLE_FLAME_OFFSET_X 34
#define HUD_CANDLE_FLAME_OFFSET_Y -1
#define HUD_SCROLLS_OFFSET_X 40
#define HUD_SCROLLS_OFFSET_Y 115
#define HUD_HEALTH_OFFSET_X 45
#define HUD_HEALTH_OFFSET_Y 21

class AnimationComponent;
class HealthComponent;
class InventoryComponent;
class SpaceComponent;
class StatsComponent;
class TurnComponent;
class WeaponComponent;

/**
 * Represents the user interface used in gameplay state.
 * Contains the renderable GUI elements and handles events.
 */
class CHud {
  public:
    static CHud HUD;

    enum Targeting {
      TARGETING_NONE,
      TARGETING_ITEM,
      TARGETING_ENTITY
    };
    
    // Coordinates of health element
    int HealthPosX;
    int HealthPosY;
    
    // Coordinates of dungeon level element
    int DLevelPosX;
    int DLevelPosY;
    
    SDL_Surface *Surf_HPGlobe;
    SDL_Surface *Surf_Interface;
    SDL_Surface *Surf_WinScreen;
    SDL_Surface *Surf_DeathScreen;
    SDL_Surface *Surf_FoV;
    SDL_Surface *Surf_HelpScreen;
    SDL_Surface *Surf_Candle;
    SDL_Surface *Surf_Stats;
    
    CHud();
    void OnCleanup();
    bool OnLoad();
    void start(GameObject player);
    void OnRender(SDL_Surface *display);
    void OnLoop();
    bool onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void DebugMode(SDL_Surface *display);
    void DisplayWinScreen(SDL_Surface *display);
    void DisplayDeathScreen(SDL_Surface *display);
    void DisplayFoV(SDL_Surface *display);
    void FoWCalculation();
    void RenderHelpScreen(SDL_Surface *display);
    void updateInventory(int Highlight = -1);
    void updateSpellBook(int Highlight = -1);
    void updateStats();
    void activateTargeting(Targeting targeting, std::string &spell);
    void activateTargeting(Targeting targeting, std::string &spell,
        GameObject item);

  private:
    void drawHealth(SDL_Surface *display);
    void drawCandle(SDL_Surface *display);
    void drawPotionCount(SDL_Surface *display);
    void drawScrollCount(SDL_Surface *display);
    void drawMiniMap(SDL_Surface *display);
    void drawCharacterSheet(SDL_Surface *display);

    // Animation control for hud elements (candle)
    CAnimation anim_control_;
    HudInventory hud_inventory_;
    HudTargeting hud_targeting_;
    GameObject player_;
    HealthComponent *player_health_;
    InventoryComponent *player_inventory_;
    SpaceComponent *player_space_;
    StatsComponent *player_stats_;
    WeaponComponent *player_weapon_;
    AnimationComponent *player_animation_;
    TurnComponent *player_turn_;
};

#endif
