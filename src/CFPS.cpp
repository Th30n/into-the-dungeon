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
#include <SDL.h>

#include "CFPS.h"

CFPS CFPS::fps_control;

CFPS::CFPS()
  : old_time_(0), last_time_(0), speed_factor_(0.0f), num_frames_(0),
    fps_cap_(true)
{
}

void CFPS::OnLoop()
{
  if (old_time_ + 1000 < SDL_GetTicks()) {
    old_time_ = SDL_GetTicks();
    
    num_frames_ = frames_;
    frames_ = 0;
  }
  
  speed_factor_ = ((SDL_GetTicks() - last_time_) / 1000.0f) * 32.0f;
  
  last_time_ = SDL_GetTicks();
  frames_++;
  
  /*
  if (fps_cap_ && (num_frames_ > 30))
  {
    SDL_Delay((1000 / 30) - SDL_GetTicks());
  }
  */
}

int CFPS::GetFPS()
{
  return num_frames_;
}

float CFPS::GetSpeedFactor()
{
  return speed_factor_;
}

void CFPS::ToggleFpsCap()
{
  fps_cap_ = !fps_cap_;
}
