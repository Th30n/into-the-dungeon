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
#include "Visibility.h"

#include "CArea.h"
#include "CCamera.h"
#include "CSurface.h"
#include "CTile.h"
#include "SpaceComponent.h"

static void calculatePoints(int x1, int y1, int x2, int y2,
    std::vector<Vector2f> &points);

bool Visibility::InLOS(Vector2f start, Vector2f end)
{
  return InLOS(start.x, start.y, end.x, end.y);
}

bool Visibility::InLOS(int x1, int y1, int x2, int y2)
{
  std::vector<Vector2f> points;
  calculatePoints(x1, y1, x2, y2, points);

  for (unsigned i = 0; i < points.size(); ++i) {
    CTile *tile = CArea::area_control.GetTile(
        points[i].x * TILE_SIZE, points[i].y * TILE_SIZE);
        if (tile && tile->type_id == TILE_TYPE_BLOCK) {
      return false;
    }
  }
    return true;
}

void Visibility::drawRay(int x1, int y1, int x2, int y2, SDL_Surface *dest)
{
  std::vector<Vector2f> points;
  calculatePoints(x1 / TILE_SIZE, y1 / TILE_SIZE, x2 / TILE_SIZE, y2 / TILE_SIZE, points);

  SDL_Surface *point = CSurface::OnLoad("./gfx/UI/Target.png");
  for (unsigned i = 0; i < points.size(); ++i) {
    CSurface::OnDraw(dest, point,
        points[i].x * TILE_SIZE - CCamera::camera_control.GetX(),
        points[i].y * TILE_SIZE - CCamera::camera_control.GetY());
  }
  SDL_FreeSurface(point);
}

static void calculatePoints(int x1, int y1, int x2, int y2,
    std::vector<Vector2f> &points)
{
  signed char ix;
    signed char iy;
  
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x = (x2 > x1 ? (ix = 1, x2 - x1) : (ix = -1, x1 - x2)) << 1;
    int delta_y = (y2 > y1 ? (iy = 1, y2 - y1) : (iy = -1, y1 - y2)) << 1;
 
    if (delta_x >= delta_y) {
        // error may go below zero
        int error = delta_y - (delta_x >> 1);
        while (x1 != x2) {
            if (error >= 0) {
                //if (error || (ix > 0)) {
                    y1 += iy;
                    error -= delta_x;
                //}
            }
            x1 += ix;
            error += delta_y;
      points.push_back(Vector2f(x1, y1));
        }
    } else {
        // error may go below zero
        int error = delta_x - (delta_y >> 1);
        while (y1 != y2) {
            if (error >= 0) {
                //if (error || (iy > 0)) {
                    x1 += ix;
                    error -= delta_y;
                //}
            }
            y1 += iy;
            error += delta_x;
      points.push_back(Vector2f(x1, y1));
        }
    }
}
