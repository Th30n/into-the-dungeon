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
#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H

#include <SDL.h>

#include <string>

#include "GameObject.h"

struct Overlay;

/**
 * Controls changing variables in HealthComponent of an object.
 * All changes should go through methods defined here, that way all the
 * indicators can be set up and rendered.
 * Also renders status ailment icons.
 */
class HealthSystem {
  public:
    // Change target health, source gets experience if the target is killed.
    static void changeHealth(GameObject source, GameObject target, int dhp);
    // Change max health.
    static void changeMaxHealth(GameObject target, int dhp);
    // Change max health and set the current health to max if heal is true.
    static void changeMaxHealth(GameObject target, int dhp, bool heal);
    // Changes corruption/light/food.
    static void changeLight(GameObject source, GameObject target, int dval);
    // Add bit flags status ailments to target.
    static void addAilments(GameObject target, int ailments);
    // Remove status ailments from target.
    static void removeAilments(GameObject target, int ailments);

    HealthSystem(std::string overlays_filename, std::string status_filename);
    ~HealthSystem();
    void start();
    void update();
    void render(SDL_Surface *display);
  private:
    static void addIndicator(GameObject obj, int txt, int col, int dhp);
    static bool deleteExpired(Overlay* overlay);
    static void startDeathAnimation(GameObject object);
    void renderStatusIcons(SDL_Surface *display, GameObject obj);
    void renderIndicator(SDL_Surface *display, GameObject obj);

    // Offsets and dimensions used when rendering overlays.
    static const int OVERLAY_OFFSET_X = 10;
    static const int OVERLAY_HEIGHT = 16;
    static const int OVERLAY_MISS_W = 39;
    static const int OVERLAY_BACKSTAB_W = 77;
    static const int OVERLAY_NUMBER_W = 16;
    // How long is an overlay displayed (ms)
    static const unsigned TIME_TO_LIVE = 2000;
    // How long to delay before displaying an overlay (ms)
    static const unsigned DELAY_TIME = 150;

    std::string overlays_filename_;
    std::string status_filename_;
    SDL_Surface *overlay_image_;
    SDL_Surface *status_icons_;
};
#endif
