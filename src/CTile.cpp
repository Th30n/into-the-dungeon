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
#include "CTile.h"

CTile::CTile()
{
  tile_id = 0;
  type_id = TILE_TYPE_NONE;
  layer_id = LAYER_ID_NONE;
  fow_mask = FOW_ALL;
  is_visible = false;
  is_explored = false;
  tile_row = 0;
}

bool CTile::isDoor()
{
  if (tile_id == TILE_ID_HDOOR || tile_id == TILE_ID_HOPENDOOR
        || tile_id == TILE_ID_VDOOR || tile_id == TILE_ID_VOPENDOOR) {
    return true;
  }
  return false;
}

void CTile::openDoor()
{
  if (tile_id == TILE_ID_HDOOR) {
    tile_id = TILE_ID_HOPENDOOR;
    type_id = TILE_TYPE_NORMAL;
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.DoorOpen, 0);
  } else if (tile_id == TILE_ID_VDOOR) {
    tile_id = TILE_ID_VOPENDOOR;
    type_id = TILE_TYPE_NORMAL;
    CMusic::SoundControl.PlaySFX(CMusic::SoundControl.DoorOpen, 0);
  }
}

bool CTile::isBlocking()
{
  return type_id == TILE_TYPE_BLOCK;
}

void CTile::save(FILE *file)
{
  int save_version = 2;
  fwrite(&save_version, sizeof(save_version), 1, file);
  fwrite(&tile_id, sizeof(tile_id), 1, file);
  fwrite(&layer_id, sizeof(layer_id), 1, file);
  fwrite(&type_id, sizeof(type_id), 1, file);
  fwrite(&tile_row, sizeof(tile_row), 1, file);
  fwrite(&is_visible, sizeof(is_visible), 1, file);
  fwrite(&is_explored, sizeof(is_explored), 1, file);
  fwrite(&fow_mask, sizeof(fow_mask), 1, file);
}

bool CTile::load(FILE *file)
{
  int save_version;

  if (fread(&save_version, sizeof(save_version), 1, file) != 1) {
    return false;
  }
  if (fread(&tile_id, sizeof(tile_id), 1, file) != 1) {
    return false;
  }
  if (fread(&layer_id, sizeof(layer_id), 1, file) != 1) {
    return false;
  }
  if (fread(&type_id, sizeof(type_id), 1, file) != 1) {
    return false;
  }
  if (fread(&tile_row, sizeof(tile_row), 1, file) != 1) {
    return false;
  }
  if (fread(&is_visible, sizeof(is_visible), 1, file) != 1) {
    return false;
  }
  if (fread(&is_explored, sizeof(is_explored), 1, file) != 1) {
    return false;
  }
  if (save_version == 2) {
    if (fread(&fow_mask, sizeof(fow_mask), 1, file) != 1) {
      return false;
    }
  }
  return true;
}
