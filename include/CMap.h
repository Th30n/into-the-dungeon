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
#ifndef _CMAP_H_
#define _CMAP_H_

#include <SDL.h>

#include <vector>
#include <cstdio>

#include "CTile.h"
#include "serialization/NameValuePair.hpp"

/**
 * Stores the tiles within the map.
 * Offers saving/loading, rendering and map generation methods.
 */
class CMap {
  public:

    // SDL_Surfaces are managed outside this class.
    SDL_Surface *surf_tileset;
    SDL_Surface *surf_fog;
    SDL_Surface *surf_mini_tileset;
    bool fow_on;

    CMap();

    bool OnLoad();
    void SpawnLadder();
    void GenerateRoom();
    void AddRoom();
    bool CheckSpace(int x, int y, int length, int height, int direction);
    void Expand(int x, int y, int room_height, int room_length, int direction);
    void NextLevel();
    bool BossLevel();

    bool Save(FILE *file);
    bool Load(FILE *file);
    template<class Archive>
    inline void save(Archive &archive, unsigned int version);
    template<class Archive>
    inline void load(Archive &archive, unsigned int version);

    void OnRender(SDL_Surface *display, int map_x, int map_y);
    void MiniMapOnRender(SDL_Surface *display, int player_x, int player_y);
    void OnLoop(int map_x, int map_y);
    
    void AddGimmick(int x, int y); //add a random gimmick to a tile

    // checks if the gimmick will block a passage
    // (in case of a blockable gimmick)
    bool WillGimmickBlock(int x, int y);
    
    CTile *GetTile(int x, int y);
  
  private:
    void drawLightFog(SDL_Surface *display, int target_x, int target_y,
              int display_x, int display_y);
    void drawFog(SDL_Surface *display, int target_x, int target_y,
              unsigned short fow_mask);

    static const int MAX_ROOMS = 18;
    static const int MIN_ROOMS = 8;
    static const int ROOM_SIZE = 10;

    std::vector<CTile> tile_list_;

};

template<class Archive>
inline void CMap::save(Archive &archive, unsigned int version)
{
  using serialization::MakeNameValuePair;
  typedef std::vector<CTile> Tiles;
  Tiles::size_type tiles = tile_list_.size();
  archive << MakeNameValuePair("tiles", tiles);
  for (Tiles::iterator it = tile_list_.begin(); it != tile_list_.end(); ++it) {
    archive << *it;
  }
}

template<class Archive>
inline void CMap::load(Archive &archive, unsigned int version)
{
  typedef std::vector<CTile> Tiles;
  Tiles::size_type tiles = 0;
  archive >> tiles;
  tile_list_.clear();
  for (Tiles::size_type i = 0; i < tiles; ++i) {
    CTile tile;
    archive >> tile;
    tile_list_.push_back(tile);
  }
}

#endif
