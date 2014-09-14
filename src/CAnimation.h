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
#ifndef _CANIMATION_H_
  #define _CANIMATION_H_

#include <SDL.h>

/*
 * Controls the animation by switching frames
 * on given frame rate.
 */
class CAnimation {
  public:
    int max_frames;
    bool oscillate; // True if animation should reverse upon ending

    CAnimation();
    void OnAnimate();
    void SetFrameRate(int rate);
    void SetCurrentFrame(int frame);
    int GetCurrentFrame();
    void ResetOldTime();
    void UpdateTime();

  private:
    int current_frame_;
    int frame_inc_;
    int frame_rate_; //Milliseconds
    long old_time_;
};

#endif
