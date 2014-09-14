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
#include "CCamera.h"
#include "Define.h"
#include "COptions.h"
#include "SpaceComponent.h"

CCamera CCamera::camera_control;

CCamera::CCamera()
  : target_mode(TARGET_MODE_NORMAL),
    x_(0), y_(0), target_space_(0), in_boundaries_(true)
{
}

void CCamera::OnMove(int move_x, int move_y)
{
  x_ += move_x;
  y_ += move_y;
}

int CCamera::GetX()
{
  int ww = COptions::options.getScreenWidth();
  
  if (target_space_->pos.x != 0) {
    if (target_mode == TARGET_MODE_CENTER) {
      int ret_x = target_space_->pos.x * TILE_SIZE - (ww / 2);
      if (in_boundaries_) {
        if (ret_x < 0) {
          ret_x = 0;
        }
        if (ret_x > MAP_WIDTH*TILE_SIZE - ww) {
          ret_x = MAP_WIDTH*TILE_SIZE - ww;
        }
      }
      return ret_x;
    }
    return target_space_->pos.x * TILE_SIZE;
  }
  return x_;
}

int CCamera::GetY()
{
  int wh = COptions::options.getScreenHeight();
  
  if (target_space_->pos.y != 0) {
    if (target_mode == TARGET_MODE_CENTER) {
      int ret_y = target_space_->pos.y * TILE_SIZE - (wh / 2);
      if (in_boundaries_) {
        if (ret_y < 0) {
          ret_y = 0;
        }
        if (ret_y > MAP_HEIGHT*TILE_SIZE - wh) {
          ret_y = MAP_HEIGHT*TILE_SIZE - wh;
        }
      }
      return ret_y;
    }
    return target_space_->pos.y * TILE_SIZE;
  }
  return y_;
}

void CCamera::SetPos(int x, int y)
{
  x_ = x;
  y_ = y;
}

void CCamera::SetTarget(SpaceComponent *target)
{
  target_space_ = target;
}

void CCamera::toggleInBoundaries()
{
  in_boundaries_ = !in_boundaries_;
}

