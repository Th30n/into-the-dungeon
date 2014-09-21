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
#ifndef _CSURFACE_H_
#define _CSURFACE_H_

#include <SDL.h>

/**
 * Offers static methods for loading images, blitting and color keying.
 */
class CSurface {
  public:
    CSurface();
    static SDL_Surface *OnLoad(const char *file);
    static bool OnDraw(SDL_Surface *dest, SDL_Surface *src, int x, int y);
    static bool OnDraw(SDL_Surface *dest, SDL_Surface *src,
            int dest_x, int dest_y, int src_x, int src_y, int w, int h);
    static bool Transparent(SDL_Surface *dest, int r, int g, int b);
    static Uint32 getPixel32(SDL_Surface *src, int x, int y);
    //you may have to LOCK the SURFACE before calling this method
    static void setPixel32(SDL_Surface *src, int x, int y,
                int r, int g, int b);
};

#endif
