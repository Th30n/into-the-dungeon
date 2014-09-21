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
#ifndef _CAREA_H_
#define _CAREA_H_
  
#include <vector>

#include <SDL.h>

class CMap;
class CTile;

/**
 * Stores the map(s).
 * Offers map saving/loading, rendering and level generation methods.
 * This should be reorganized.
 */
class CArea {
  public:
    // Current dungeon level;
    static int dungeon_level;
    static CArea area_control;
    
    std::vector<CMap> map_list;
    SDL_Surface *surf_tileset;
  
    CArea();
    bool OnLoad();
    bool Save(FILE *file);
    bool Load(FILE *file);
    void NextLevel();
    bool BossLevel();
    CMap *GetMap(int x, int y);
    CTile *GetTile(int x, int y);
    void OnRender(SDL_Surface *surf_display, int camera_x, int camera_y);
    void MiniMapOnRender(SDL_Surface *surf_display, int x, int y);
    void OnCleanup();
    void OnLoop(int camera_x, int camera_y);
    void CreateTorchParticles();
  
  private:
    int area_size_;
    SDL_Surface *surf_fog_;
    SDL_Surface *surf_mini_tileset_;
};

#endif
