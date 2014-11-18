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
#include "CAnimation.h"

CAnimation::CAnimation() : max_frames(0), oscillate(false),
  current_frame_(0), frame_inc_(1), frame_rate_(200), old_time_(0)
{
}

void CAnimation::ResetOldTime()
{
  old_time_ = 0;
}

void CAnimation::UpdateTime()
{
  old_time_ = SDL_GetTicks();
}

void CAnimation::OnAnimate()
{
  if ((old_time_ + frame_rate_) > SDL_GetTicks())
  {
    return;
  }
  
  old_time_ = SDL_GetTicks();
  
  current_frame_ += frame_inc_;
  
  if (oscillate)
  {
    if (frame_inc_ > 0)
    {
      if (current_frame_ >= (max_frames - 1))
      {
        frame_inc_ = -frame_inc_;
      }
    }
    else
    {
      if (current_frame_ <= 0)
      {
        frame_inc_ = -frame_inc_;
      }
    }
  }
  else
  {
    if (current_frame_ > (max_frames - 1))
    {
      current_frame_ = 0;
    }
  }
}

void CAnimation::SetFrameRate(unsigned rate)
{
  frame_rate_ = rate;
}

void CAnimation::SetCurrentFrame(int frame)
{
  if ((frame < 0) || (frame >= max_frames))
    return;
    
  current_frame_ = frame;
}

int CAnimation::GetCurrentFrame()
{
  return current_frame_;
}

