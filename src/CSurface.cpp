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
#include "CSurface.h"

#include <SDL_image.h>

#include "COptions.h"
#include "Define.h"

CSurface::CSurface()
{
}

SDL_Surface *CSurface::OnLoad(const char *file)
{
  SDL_Surface *surf = NULL;
  SDL_Surface *ret_surf = NULL;
  
  if ((surf = IMG_Load(file)) == NULL) {
    return NULL;
  }
  
  ret_surf = SDL_DisplayFormatAlpha(surf);
  SDL_FreeSurface(surf);
  return ret_surf;
}

bool CSurface::OnDraw(SDL_Surface *dest, SDL_Surface *src, int x, int y)
{
  if (dest == NULL || src == NULL) {
    return false;
  }
  
  int screen_width = COptions::options.getScreenWidth();
  int screen_height = COptions::options.getScreenHeight();
  
  if (x < -TILE_SIZE || x > screen_width ||
      y < -TILE_SIZE || y > screen_height) {
    return false; //don't render if not in window
  }
  
  SDL_Rect dest_rect;
  dest_rect.x = x;
  dest_rect.y = y;
  SDL_BlitSurface(src, NULL, dest, &dest_rect);
  return true;
}

bool CSurface::OnDraw(SDL_Surface *dest, SDL_Surface *src,
        int dest_x, int dest_y, int src_x, int src_y, int w, int h)
{
  if (dest == NULL || src == NULL) {
    return false;
  }

  int screen_width = COptions::options.getScreenWidth();
  int screen_height = COptions::options.getScreenHeight();
  
  if (dest_x < -TILE_SIZE || dest_x > screen_width ||
      dest_y < -TILE_SIZE || dest_y > screen_height) {
    return false; //don't render if not in window
  }
  
  SDL_Rect dest_rect;
  dest_rect.x = dest_x;
  dest_rect.y = dest_y;
  SDL_Rect src_rect;
  src_rect.x = src_x;
  src_rect.y = src_y;
  src_rect.w = w;
  src_rect.h = h;
  SDL_BlitSurface(src, &src_rect, dest, &dest_rect);
  return true;
}

bool CSurface::Transparent(SDL_Surface *dest, int r, int g, int b)
{
  if (dest == NULL) {
    return false;
  }
  if (SDL_SetColorKey(dest, SDL_SRCCOLORKEY | SDL_RLEACCEL,
            SDL_MapRGB(dest->format, r, g, b)) < 0) {
    return false;
  }
  return true;
}

Uint32 CSurface::getPixel32(SDL_Surface *src, unsigned x, unsigned y) {
  Uint32 *pixels = (Uint32 *)src->pixels;
  return pixels[(y * src->w) + x];
}

void CSurface::setPixel32(SDL_Surface *src, int x, int y, int r, int g, int b)
{
  Uint32 *pixels = (Uint32 *)src->pixels;
  pixels[(y * src->w) + x] = SDL_MapRGB(src->format, r, g, b);
}
