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
#include "CMap.h"

#include <string>

#include "CSurface.h"
#include "COptions.h"
#include "data/dirs.h"

CMap::CMap() : surf_tileset(0), surf_fog(0), surf_mini_tileset(0), fow_on(true)
{
}

bool CMap::OnLoad()
{
  tile_list_.clear();
  int tile_row = rand() % TILESET_ROWS;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      CTile tile;
      tile.tile_row = tile_row;
      tile.tile_id = TILE_ID_WALL;
      tile.type_id = TILE_TYPE_BLOCK;
      tile_list_.push_back(tile);
    }
  }
  GenerateRoom();
  int room_limit = rand() % MAX_ROOMS + MIN_ROOMS;
  for (int i = 0; i < room_limit; i++) {
    AddRoom();
  }
  SpawnLadder();
  fow_on = true;
  return true;
}

void CMap::OnLoop(int map_x, int map_y)
{
  int wwidth = COptions::options.getScreenWidth();
  int wheight = COptions::options.getScreenHeight();
  
  int max_y = ((-map_y + wheight) / TILE_SIZE) + 1;
  if (max_y > MAP_HEIGHT) max_y = MAP_HEIGHT;
  
  int max_x = ((-map_x + wwidth) / TILE_SIZE) + 1;
  if (max_x > MAP_WIDTH) max_x = MAP_WIDTH;
  
  for (int y = -map_y / TILE_SIZE; y < max_y; y++) {
    for (int x = -map_x / TILE_SIZE; x < max_x; x++) {
      tile_list_[x + y*MAP_WIDTH].is_visible = false;
    }
  }
}

void CMap::OnRender(SDL_Surface *display, int map_x, int map_y)
{
  if (surf_tileset == NULL) {
    return;
  }
  
  int tileset_width = surf_tileset->w / TILE_SIZE;
  
  int wwidth = COptions::options.getScreenWidth();
  int wheight = COptions::options.getScreenHeight();
  
  int id = 0;
  int max_y = ((-map_y + wheight) / TILE_SIZE) + 1;
  if (max_y > MAP_HEIGHT) max_y = MAP_HEIGHT;
  
  int max_x = ((-map_x + wwidth) / TILE_SIZE) + 1;
  if (max_x > MAP_WIDTH) max_x = MAP_WIDTH;
  
  for (int y = -map_y/TILE_SIZE; y < max_y; y++) {
    id = (-map_x / TILE_SIZE) + (y * MAP_WIDTH);
    
    for (int x = -map_x/TILE_SIZE; x < max_x; x++) {
      if (tile_list_[id].type_id == TILE_TYPE_NONE) {
        id++;
        continue;
      }
      
      int tx = map_x + (x * TILE_SIZE);
      int ty = map_y + (y * TILE_SIZE);
      
      int tileset_x = (tile_list_[id].tile_id % tileset_width) * TILE_SIZE;
      int tileset_y = (tile_list_[id].tile_id / tileset_width) * TILE_SIZE +
              tile_list_[id].tile_row * TILE_SIZE;
      
      int gimmick_x = (tile_list_[id].layer_id % tileset_width) * TILE_SIZE;
      int gimmick_y = (tile_list_[id].layer_id / tileset_width) * TILE_SIZE +
              TILESET_ROWS*TILE_SIZE;
      
      if (tile_list_[id].is_explored || !fow_on) {
        CSurface::OnDraw(display, surf_tileset, tx, ty,
                  tileset_x, tileset_y, TILE_SIZE, TILE_SIZE);
        if (tile_list_[id].layer_id != LAYER_ID_NONE) {
          CSurface::OnDraw(display, surf_tileset, tx, ty,
                  gimmick_x, gimmick_y, TILE_SIZE, TILE_SIZE);
        }
      }
      if (fow_on) {
        if (!tile_list_[id].is_visible) {
          drawFog(display, tx, ty, tile_list_[id].fow_mask);
        } else {
          drawLightFog(display, x, y, tx, ty);
        }
      }
      id++;
    }
  }
}

void CMap::drawLightFog(SDL_Surface *display, int target_x, int target_y,
        int display_x, int display_y)
{
  short mask = FOW_NON;
  int c = 0;
  for (int y = target_y - 1; y <= target_y + 1; y++) {
    for (int x = target_x - 1; x <= target_x + 1; x++) {
      CTile *tile = GetTile(x*TILE_SIZE, y*TILE_SIZE);
      if (!tile) {
        c++;
        continue;
      } else if (tile->is_visible == false) {
        mask |= (1 << c);
      }
      c++;
    }
  }
  if (mask & FOW_BIT_N) {
    CSurface::OnDraw(display, surf_fog, display_x, display_y,
              64, 160, TILE_SIZE, TILE_SIZE);
  }
  if (mask & FOW_BIT_S) {
    CSurface::OnDraw(display, surf_fog, display_x, display_y,
              64, 128, TILE_SIZE, TILE_SIZE);
  }
  if (mask & FOW_BIT_E) {
    CSurface::OnDraw(display, surf_fog, display_x, display_y,
              0, 160, TILE_SIZE, TILE_SIZE);
  }
  if (mask & FOW_BIT_W) {
    CSurface::OnDraw(display, surf_fog, display_x, display_y,
              32, 160, TILE_SIZE, TILE_SIZE);
  }
}

void CMap::drawFog(SDL_Surface *display, int target_x, int target_y,
        short fow_mask)
{
  switch(fow_mask) {
  case FOW_CNW:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              0, 0, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_NNN:
  case FOW_NNN | FOW_BIT_C:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              32, 0, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_CNE:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              64, 0, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_WWW:
  case FOW_WWW | FOW_BIT_C:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              0, 32, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_EEE:
  case FOW_EEE | FOW_BIT_C:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              64, 32, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_CSW:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              0, 64, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_SSS:
  case FOW_SSS | FOW_BIT_C:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              32, 64, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_CSE:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              64, 64, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_ALL:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              0, 96, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_JNW:
  case FOW_JNW & FOW_WWW:
  case FOW_JNW & FOW_NNN:
  case FOW_BIT_NW:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              32, 96, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_JNE:
  case FOW_JNE & FOW_EEE:
  case FOW_JNE & FOW_NNN:
  case FOW_BIT_NE:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              64, 96, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_JSW:
  case FOW_JSW & FOW_WWW:
  case FOW_JSW & FOW_SSS:
  case FOW_BIT_SW:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              0, 128, TILE_SIZE, TILE_SIZE);
    break;
  case FOW_JSE:
  case FOW_JSE & FOW_EEE:
  case FOW_JSE & FOW_SSS:
  case FOW_BIT_SE:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              32, 128, TILE_SIZE, TILE_SIZE);
    break;
  default:
    CSurface::OnDraw(display, surf_fog, target_x, target_y,
              32, 32, TILE_SIZE, TILE_SIZE);
    break;
  }
}
void CMap::MiniMapOnRender(SDL_Surface *display, int player_x, int player_y)
{
  if (surf_mini_tileset == NULL) {
    return;
  }
  int mini_tileset_width = surf_mini_tileset->w / MINITILE_SIZE;
  
  int id = 0;
  
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      if (tile_list_[id].type_id == TILE_TYPE_NONE) {
        id++;
        continue;
      }
      
      int mini_x = x*MINITILE_SIZE;
      int mini_y = y*MINITILE_SIZE;
      int tile_id = tile_list_[id].tile_id;
      if (player_x == x && player_y == y) tile_id = 7;
      int mini_tileset_x = (tile_id % mini_tileset_width) * MINITILE_SIZE;
      int mini_tileset_y = (tile_id / mini_tileset_width) * MINITILE_SIZE;
      
      if (tile_list_[id].is_explored || !fow_on) {
        CSurface::OnDraw(display, surf_mini_tileset,
                mini_x, mini_y, mini_tileset_x, mini_tileset_y,
                MINITILE_SIZE, MINITILE_SIZE);
      }
      id++;
    }
  }
}

CTile *CMap::GetTile(int x, int y)
{
  unsigned id = 0;
  id = x / TILE_SIZE;
  id = id + (MAP_WIDTH * (y / TILE_SIZE));

  if (id >= tile_list_.size()) {
    return NULL;
  }
  
  return &tile_list_[id];
}

void CMap::GenerateRoom()
{ 
  int room_length = rand() % ROOM_SIZE + 1;
  int room_height = rand() % ROOM_SIZE + 1;
  
  for (int y = 0; y < room_height; ++y) {
    for (int x = 0; x < room_length; ++x) {
      //center of map
      int i = (y - room_length / 2) + (MAP_WIDTH / 2) + 
          ((x - (room_height / 2) + (MAP_HEIGHT / 2)) * MAP_WIDTH);
      tile_list_[i].tile_id = TILE_ID_FLOOR;
      tile_list_[i].type_id = TILE_TYPE_NORMAL;
    }
  }
}

void CMap::AddRoom()
{
  int room_length = 0;
  int room_height = 0;
  int px = 0;
  int py = 0;
  int x = 0;
  int y = 0;
  char direction = 0;
  
  bool door = false;
  bool space = false;
  
  do {
    px = rand() % MAP_WIDTH; //pick starting x coordinate
    py = rand() % MAP_HEIGHT; //pick starting y coordinate
    
    door = false;
    space = false;
    
    if (tile_list_[px + py*MAP_WIDTH].type_id == TILE_TYPE_BLOCK) {
      y = py + 1;
      
      for (int i = (py - 1); i < (py + 2); ++i) {
        if (!door) {
          x = px + 1;
        }
        int j = px - 1;
        do {
          if (i > 0 && i < (MAP_HEIGHT - 1) && 
              j > 0 && j < (MAP_WIDTH - 1))
          {
            //eliminates corners
            if (!(i == (py - 1) && j == (px - 1)) &&
                !(i == (py + 1) && j == (px - 1)) &&
                !(i == (py - 1) && j == (px + 1)) &&
                !(i == (py + 1) && j == (px + 1))) {
              if (tile_list_[j + i*MAP_WIDTH].type_id != TILE_TYPE_BLOCK) {
                door = true; //door available
              }
            }
          }
          if (!door) {
            x--;
          }
        } while (++j < (px + 2));
        if (!door) {
          y--;
        }
      }
    } else {
      continue;
    }
    
    room_length = rand() % ROOM_SIZE;
    room_height = rand() % ROOM_SIZE;
    
    if (x > px) direction = 'r';
    else        direction = 'l';
    if (y > py) direction = 'd';
    else        direction = 'u';
    
    space = CheckSpace(x, y, room_length, room_height, direction);
    
  } while (!door || !space);
  tile_list_[x + y*MAP_WIDTH].tile_id = TILE_ID_FLOOR;
  tile_list_[x + y*MAP_WIDTH].type_id = TILE_TYPE_NORMAL;
  tile_list_[x + y*MAP_WIDTH].layer_id = LAYER_ID_NONE;
  
  tile_list_[px + py*MAP_WIDTH].tile_id = TILE_ID_HDOOR; //door
  if (direction == 'l' || direction == 'r') {
    tile_list_[px + py*MAP_WIDTH].tile_id = TILE_ID_VDOOR; //rotated door
  }
  tile_list_[px + py*MAP_WIDTH].layer_id = LAYER_ID_NONE;
  tile_list_[px + py*MAP_WIDTH].type_id = TILE_TYPE_BLOCK;
  
  Expand(x, y, room_height, room_length, direction);
}

bool CMap::CheckSpace(int x, int y, int length, int height, int direction)
{
  //direction (u)p, (d)own, (l)eft, (r)ight of room expand
  int i, j, ylimit, xlimit, k;
  
  if (direction == 'u') {
    ylimit = y + 1;
    i = y - height - 1;
  } else {
    ylimit = y + height + 1;
    i = y - 1;
  }
  if (direction == 'l') {
    xlimit = x + 1;
    j = x - length - 1;
  } else {
    xlimit = x + length + 1;
    j = x - 1;
  }
  
  for (; i < ylimit; i++) {
    k = j;
    do {
      if (i < 1 || k < 1 || i > (MAP_HEIGHT-2) || k > (MAP_WIDTH-2)) {
        return false; //keeps in map boundary
      }
      if (tile_list_[k + i*MAP_WIDTH].type_id != TILE_TYPE_BLOCK) {
        return false;
      }
    } while (k++ < xlimit);
  }
  return true;
}

void CMap::Expand(int x, int y, int room_height, int room_length, int direction)
{
  int i, j, xlimit, ylimit, k;
  
  if (direction == 'u') {
    ylimit = y;
    i = y - room_height;
  } else {
    ylimit = y + room_height;
    i = y;
  }
  if (direction == 'l') {
    xlimit = x;
    j = x - room_length;
  } else {
    xlimit = x + room_length;
    j = x;
  }
  
  for (; i < ylimit; i++) {
    k = j;
    do {
      tile_list_[k + i*MAP_WIDTH].tile_id = TILE_ID_FLOOR;
      tile_list_[k + i*MAP_WIDTH].type_id = TILE_TYPE_NORMAL;
      tile_list_[k + i*MAP_WIDTH].layer_id = LAYER_ID_NONE;
    } while (++k < xlimit);
  }
  
  for (k = x - 1; k < xlimit + 1; k++) {
    for (i = y - 1; i < ylimit + 1; i++) {
      AddGimmick(k, i);
    }
  }
}

bool CMap::Save(FILE *file)
{
  for (unsigned int i = 0; i < tile_list_.size(); i++) {
    tile_list_[i].save(file);
  }
  return true;
}

bool CMap::Load(FILE *file)
{
  tile_list_.clear();
  for (int i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++) {
    CTile tile;
    if (tile.load(file) == false)
        return false;
    tile_list_.push_back(tile);
  }
  return true;
}

void CMap::SpawnLadder()
{
  bool set = false;
  int x, y;
  do {
    x = rand() % MAP_WIDTH;
    y = rand() % MAP_HEIGHT;
    if (tile_list_[x + y*MAP_WIDTH].type_id == TILE_TYPE_NORMAL) {
      tile_list_[x + y*MAP_WIDTH].tile_id = TILE_ID_LADDER;
      tile_list_[x + y*MAP_WIDTH].layer_id = LAYER_ID_NONE;
      set = true;
    }
  } while (!set);
}

void CMap::NextLevel()
{
  OnLoad();
}

bool CMap::BossLevel()
{
  FILE *fBossLevel;
  std::string path = data::FindFile("data/boss1.map");
  
  if (path.empty() || !(fBossLevel = fopen(path.c_str(), "r"))) {
    fprintf(stderr, "BossLevel(): Unable to open %s\n", path.c_str());
    return false;
  }
  
  tile_list_.clear();
  int tile_row = rand() % TILESET_ROWS;
  int tile_id = TILE_ID_WALL;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      if (fscanf(fBossLevel, "%1d", &tile_id) != 1) {
        fprintf(stderr, "BossLevel(): fscanf returned != 1\n");
          return false;
      }
      CTile tile;
      tile.tile_row = tile_row;
      tile.tile_id = tile_id;
      if (tile.tile_id == TILE_ID_WALL ||
          tile.tile_id == TILE_ID_HDOOR ||
          tile.tile_id == TILE_ID_VDOOR) {
        tile.type_id = TILE_TYPE_BLOCK;
      } else {
        tile.type_id = TILE_TYPE_NORMAL;
      }
      tile_list_.push_back(tile);
    }
    if (fscanf(fBossLevel, "\n") != 0) {
        return false;
    }
  }
  fclose(fBossLevel);
  fow_on = false;
  return true;
}

void CMap::AddGimmick(int x, int y)
{
  if ((rand() % 101) > GIMMICK_CHANCE) return;
  
  if (tile_list_[x + y*MAP_WIDTH].tile_id == TILE_ID_FLOOR) {
    int rand_gimmick = rand() % 2;
    switch (rand_gimmick) {
      case 0:
      {
        if (!WillGimmickBlock(x, y)) {
          tile_list_[x + y*MAP_WIDTH].type_id = TILE_TYPE_BLOCK;
          tile_list_[x + y*MAP_WIDTH].layer_id = LAYER_ID_STATUE;
        }
        break;
      }
      case 1:
      {
        tile_list_[x + y*MAP_WIDTH].layer_id = LAYER_ID_SKELETON;
        break;
      }
      default:
      {
        break;
      }
    }
  } else if (tile_list_[x + y*MAP_WIDTH].tile_id == TILE_ID_WALL) {
    int rand_gimmick = LAYER_ID_TORCH;
    switch (rand_gimmick) {
      case LAYER_ID_TORCH:
      {
        tile_list_[x + y*MAP_WIDTH].layer_id = LAYER_ID_TORCH;
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

bool CMap::WillGimmickBlock(int x, int y)
{
  bool Return = false;
  
  //check left
  if (tile_list_[x - 1 + y * MAP_WIDTH].type_id == TILE_TYPE_NORMAL ||
      tile_list_[x - 1 + y * MAP_WIDTH].tile_id == TILE_ID_HDOOR ||
      tile_list_[x - 1 + y * MAP_WIDTH].tile_id == TILE_ID_VDOOR) {
    if ((tile_list_[x - 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x + 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
      if ((tile_list_[x - 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
        Return = true;
      }
    }
  }
  
  //check right
  if (tile_list_[x + 1 + y * MAP_WIDTH].type_id == TILE_TYPE_NORMAL ||
      tile_list_[x + 1 + y * MAP_WIDTH].tile_id == TILE_ID_HDOOR ||
      tile_list_[x + 1 + y * MAP_WIDTH].tile_id == TILE_ID_VDOOR) {
    if ((tile_list_[x + 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x - 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
      if ((tile_list_[x + 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x-1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
        Return = true;
      }
    }
  }

  //check up
  if (tile_list_[x + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_NORMAL ||
      tile_list_[x + (y - 1) * MAP_WIDTH].tile_id == TILE_ID_HDOOR ||
      tile_list_[x + (y - 1) * MAP_WIDTH].tile_id == TILE_ID_VDOOR) {
    if ((tile_list_[x - 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x - 1 + y*MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x - 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
      if ((tile_list_[x + 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + 1 + y * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
        Return = true;
      }
    }
  }
  
  //check down
  if (tile_list_[x + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_NORMAL ||
      tile_list_[x + (y + 1) * MAP_WIDTH].tile_id == TILE_ID_HDOOR ||
      tile_list_[x + (y + 1) * MAP_WIDTH].tile_id == TILE_ID_VDOOR) {
    if ((tile_list_[x - 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x - 1 + y * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
        (tile_list_[x - 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
      if ((tile_list_[x + 1 + (y + 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + 1 + y * MAP_WIDTH].type_id == TILE_TYPE_BLOCK) ||
          (tile_list_[x + 1 + (y - 1) * MAP_WIDTH].type_id == TILE_TYPE_BLOCK)) {
        Return = true;
      }
    }
  }
  
  return Return;
}
