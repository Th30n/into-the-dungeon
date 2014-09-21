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
#ifndef VISIBILITY_H_
#define VISIBILITY_H_

#include <SDL.h>

#include "Vector2f.h"

/*
 * This is a utility class used for various line of sight and fog of war
 * solving within the game.
 */
class Visibility {
  public:
    // Determines if point 2 is within line of sight of point 1.
    // Uses simple Bresenham algorithm as a ray trace.
    static bool InLOS(Vector2f start, Vector2f end);
    static bool InLOS(int x1, int y1, int x2, int y2);
    // Used for debugging.
    static void drawRay(int x1, int y1, int x2, int y2, SDL_Surface *dest);
};

#endif
