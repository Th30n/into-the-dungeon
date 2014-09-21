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
#ifndef _CCAMERA_H_
#define _CCAMERA_H_

enum CameraModes {
  TARGET_MODE_NORMAL = 0,
  TARGET_MODE_CENTER
};

class SpaceComponent;

/**
 * Follows the given target SpaceComponent.
 * Has 2 modes: normal which stop at the map boundary
 * and center which always has the target in center.
 */
class CCamera {
  public:
    static CCamera camera_control;

    int target_mode;

    CCamera();
    void OnMove(int move_x, int move_y);
    int GetX();
    int GetY();
    void SetPos(int x, int y);
    void SetTarget(SpaceComponent *target);
    void toggleInBoundaries();

  private:
    int x_;
    int y_;
    SpaceComponent *target_space_;
    //float *targetX;
    //float *targetY;
    bool in_boundaries_;
};

#endif
