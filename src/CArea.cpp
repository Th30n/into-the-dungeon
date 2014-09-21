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
#include "CArea.h"
#include "CSurface.h"
#include "CMap.h"
#include "data/dirs.h"

int CArea::dungeon_level = 1;
CArea CArea::area_control;

CArea::CArea() : area_size_(0)
{
}

bool CArea::OnLoad()
{
  map_list.clear();
  
  if (!(surf_tileset = CSurface::OnLoad(data::FindFile("gfx/Tileset.png").c_str())))
  {
    return false;
  }
  if (!(surf_fog_ = CSurface::OnLoad(data::FindFile("gfx/Fog.png").c_str())))
  {
    return false;
  }
  if (!(surf_mini_tileset_ = CSurface::OnLoad(data::FindFile("gfx/MiniTileset.png").c_str())))
  {
    return false;
  }
  area_size_ = 1;
  
  for (int x = 0; x < area_size_; x++)
  {
    for (int y = 0; y < area_size_; y++)
    {
      CMap map;
    
      if (map.OnLoad() == false)
      {
        return false;
      }
      map.surf_tileset = surf_tileset;
      map.surf_mini_tileset = surf_mini_tileset_;
      map.surf_fog = surf_fog_;
      map_list.push_back(map);
    }
  }
  return true;
}

void CArea::OnRender(SDL_Surface *surf_display, int camera_x, int camera_y)
{
  map_list[0].OnRender(surf_display, camera_x, camera_y);
}

void CArea::MiniMapOnRender(SDL_Surface *surf_display, int x, int y)
{
  for (int i = 0; i < 4; i++)
  {
    unsigned int ID = ((i / 2) * area_size_) + (i % 2);
    if (ID < 0 || ID >= map_list.size())
    {
      continue;
    }
    map_list[ID].MiniMapOnRender(surf_display, x, y);
  }
}

void CArea::OnLoop(int camera_x, int camera_y)
{
    map_list[0].OnLoop(camera_x, camera_y);
}

void CArea::OnCleanup()
{
  if (surf_tileset)
  {
    SDL_FreeSurface(surf_tileset);
  }
  if (surf_fog_)
  {
    SDL_FreeSurface(surf_fog_);
  }
  if (surf_mini_tileset_)
  {
    SDL_FreeSurface(surf_mini_tileset_);
  }
  map_list.clear();
}

CMap *CArea::GetMap(int x, int y)
{
  int map_width  = MAP_WIDTH * TILE_SIZE;
  int map_height = MAP_HEIGHT * TILE_SIZE;
  unsigned int ID = x / map_width;
  ID = ID + ((y / map_height) * area_size_);
  if (ID < 0 || ID >= map_list.size())
  {
    return NULL;
  }
  return &map_list[ID];
}

CTile *CArea::GetTile(int x, int y)
{
  int map_width  = MAP_WIDTH * TILE_SIZE;
  int map_height = MAP_HEIGHT * TILE_SIZE;
  
  CMap *map = GetMap(x, y);
  
  if (map == NULL) return NULL;
  
  x = x % map_width;
  y = y % map_height;
  
  return map->GetTile(x, y);
}

bool CArea::Save(FILE *file)
{
  return map_list[0].Save(file);
}

bool CArea::Load(FILE *file)
{
  return map_list[0].Load(file);
}

void CArea::NextLevel()
{
  map_list[0].NextLevel();
}

bool CArea::BossLevel()
{
  return map_list[0].BossLevel();
}
