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
#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "GameObject.h"

/**
 * Handles all objects with ProjectileComponent.
 * Moves, animates and destroys projectiles. If they have
 * an attack callback it is called.
 */
class ProjectileSystem {
  public:
    /**
     * Animation frames of a projectile sprite-sheet.
     * Animations should be reorganized and this removed.
     */
    enum Frames {
      PROJECTILE_FRAME_COLON_DOWN = 0,
      PROJECTILE_FRAME_COLON_UP,
      PROJECTILE_FRAME_COLON_RIGHT,
      PROJECTILE_FRAME_COLON_LEFT,
      PROJECTILE_FRAME_COLON_HIT
    };

    void update();
  private:
    void updateProjectile(GameObject obj);
    void removeProjectile(GameObject obj);
};

#endif
