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
#ifndef _CTILE_H_
#define _CTILE_H_
  
#include "Define.h"
#include "CMusic.h"

enum
{
  TILE_TYPE_NONE = 0,
  TILE_TYPE_NORMAL,
  TILE_TYPE_BLOCK
};

enum
{
  TILE_ID_FLOOR = 0,
  TILE_ID_WALL,
  TILE_ID_HDOOR,
  TILE_ID_HOPENDOOR,
  TILE_ID_VDOOR,
  TILE_ID_VOPENDOOR,
  TILE_ID_LADDER
};

enum
{
  LAYER_ID_TORCH = 0,
  LAYER_ID_CHEST,
  LAYER_ID_STATUE,
  LAYER_ID_TRAP_DAMAGE,
  LAYER_ID_TRAP_DOT,
  LAYER_ID_TRAP_IMMOBILIZE,
  LAYER_ID_SKELETON,
  LAYER_ID_NONE
};

#define FOW_BIT_NW   (1 << 0)
#define FOW_BIT_N    (1 << 1)
#define FOW_BIT_NE   (1 << 2)
#define FOW_BIT_W    (1 << 3)
#define FOW_BIT_C    (1 << 4)
#define FOW_BIT_E    (1 << 5)
#define FOW_BIT_SW   (1 << 6)
#define FOW_BIT_S    (1 << 7)
#define FOW_BIT_SE   (1 << 8)

#define FOW_NON 0
#define FOW_ALL (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE    \
          | FOW_BIT_W | FOW_BIT_C | FOW_BIT_E   \
          | FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
#define FOW_EEE (FOW_BIT_NE | FOW_BIT_E | FOW_BIT_SE)
#define FOW_WWW (FOW_BIT_NW | FOW_BIT_W | FOW_BIT_SW)
#define FOW_NNN (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE)
#define FOW_SSS (FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
#define FOW_CNE (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE  \
          | FOW_BIT_C | FOW_BIT_E | FOW_BIT_SE)
#define FOW_CNW (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE  \
          | FOW_BIT_C | FOW_BIT_W | FOW_BIT_SW)
#define FOW_CSE (FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE  \
          | FOW_BIT_C | FOW_BIT_E | FOW_BIT_NE)
#define FOW_CSW (FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE  \
          | FOW_BIT_C | FOW_BIT_W | FOW_BIT_NW)
#define FOW_JNE (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
#define FOW_JNW (FOW_BIT_W | FOW_BIT_NW | FOW_BIT_N)
#define FOW_JSE (FOW_BIT_E | FOW_BIT_SE | FOW_BIT_S)
#define FOW_JSW (FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S)

/**
 * Stores tile information.
 */
class CTile {
  public:
    int tile_id;
    int layer_id;
    int type_id;
    int tile_row;
    unsigned short fow_mask;
    bool is_visible;
    bool is_explored;
    
    CTile();
    bool isDoor();
    void openDoor();
    bool isBlocking();
    void save(FILE *file);
    bool load(FILE *file);
};

#endif
