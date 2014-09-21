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
#ifndef HUDTARGETING_H
#define HUDTARGETING_H

#include <string>
#include <vector>

#include <SDL.h>

#include "GameObject.h"

class CHud;
class SpaceComponent;
class InventoryComponent;
class TurnComponent;

/**
 * Interface for selecting the target of the spell.
 * Displays target indicators and allows for selecting a target on the map.
 */
class HudTargeting {
  public:
    HudTargeting(CHud &hud);
    ~HudTargeting();
    bool OnLoad();
    void start(GameObject obj);
    void render(SDL_Surface *display);
    bool onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void activateTargeting(std::string &spell);
    void activateTargeting(std::string &spell, GameObject item);

  private:
    void drawValidTargets(SDL_Surface *display);
    void getValidTargets();
    void selectTarget(unsigned n);
    void fire();

    CHud &owner_;
    GameObject player_;
    bool is_active_;
    SpaceComponent *player_space_;
    InventoryComponent *player_inventory_;
    TurnComponent *player_turn_;
    SDL_Surface *target_image_;
    unsigned selected_target_;
    std::vector<GameObject> targets_;
    GameObject spell_item_;
    std::string spell_name_;
};

#endif
