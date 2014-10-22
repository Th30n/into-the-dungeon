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

#include "CMap.h"
#include "Define.h"
#include "serialization/NameValuePair.hpp"

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
    template<class Archive>
    inline void save(Archive &archive, unsigned int version);
    template<class Archive>
    inline void load(Archive &archive, unsigned int version);
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

template<class Archive>
inline void CArea::save(Archive &archive, unsigned int version)
{
  using serialization::MakeNameValuePair;
  archive << MakeNameValuePair("dungeonLevel", dungeon_level);
  archive << MakeNameValuePair("areaSize", area_size_);
  typedef std::vector<CMap> Maps;
  Maps::size_type maps = map_list.size();
  archive << MakeNameValuePair("maps", maps);
  for (Maps::iterator it = map_list.begin(); it != map_list.end(); ++it) {
    archive << *it;
  }
}

template<class Archive>
inline void CArea::load(Archive &archive, unsigned int version)
{
  archive >> dungeon_level;
  bool fow_on = true;
  // Quick hack for restoring fow_on.
  // This should be saved and loaded properly in CMap.
  if (!(dungeon_level % 5)) {
    fow_on = false;
  }
  archive >> area_size_;
  typedef std::vector<CMap> Maps;
  Maps::size_type maps = 0;
  archive >> maps;
  map_list.clear();
  for (Maps::size_type i = 0; i < maps; ++i) {
    CMap map;
    archive >> map;
    map.surf_tileset = surf_tileset;
    map.surf_mini_tileset = surf_mini_tileset_;
    map.surf_fog = surf_fog_;
    map.fow_on = fow_on;
    map_list.push_back(map);
  }
}

#endif
