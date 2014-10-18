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
#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <cstdio>

#include "IComponent.h"

class CAnimation;

/*
 * Animation component of a game object.
 * Contains various animation flags (e.g. attacking, casting...),
 * as well as frame and frame rate values.
 */
class AnimationComponent : public IComponent {
  public:
    /**
     * Frame positions within spritesheet.
     * This should change to allow having different spritesheets.
     */
    enum Frames {
      FRAME_LOOK_DOWN = 0,
      FRAME_MOVE_DOWN,
      FRAME_LOOK_UP,
      FRAME_MOVE_UP,
      FRAME_LOOK_RIGHT,
      FRAME_MOVE_RIGHT,
      FRAME_LOOK_LEFT,
      FRAME_MOVE_LEFT,
      FRAME_CORPSE,
      FRAME_CASTING = 13
    };

    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    AnimationComponent();
    virtual ~AnimationComponent();
    virtual void loadIML(const IMLNode &node);
    void save(FILE *file);
    bool load(FILE *file);
  
    int max_frames;
    // Frame rate in ms
    int frame_rate;
    // When the animation starts (used for calculating current frame).
    long start_time;
    // Current Frame Colon used for animation (from loaded GFXFile)
    int current_frame_col;
    // Current Frame Row used for animation (from loaded GFXFile)
    int current_frame_row;
    // Frame Colon used for idle animation (from loaded GFXFile)
    int idle_frame_col;
    // Animation flags.
    bool moving;
    bool attacking;     //is attacking
    bool is_dead;
    bool casting;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, AnimationComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("maxFrames", comp.max_frames);
  archive << MakeNameValuePair("frameRate", comp.frame_rate);
  archive << MakeNameValuePair("currentFrameCol", comp.current_frame_col);
  archive << MakeNameValuePair("currentFrameRow", comp.current_frame_row);
  archive << MakeNameValuePair("idleFrameCol", comp.idle_frame_col);
  archive << MakeNameValuePair("moving", comp.moving);
  archive << MakeNameValuePair("attacking", comp.attacking);
  archive << MakeNameValuePair("isDead", comp.is_dead);
  archive << MakeNameValuePair("casting", comp.casting);
}

template<class Archive>
inline void load(Archive &archive, AnimationComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.max_frames;
  archive >> comp.frame_rate;
  archive >> comp.current_frame_col;
  archive >> comp.current_frame_row;
  archive >> comp.idle_frame_col;
  archive >> comp.moving;
  archive >> comp.attacking;
  archive >> comp.is_dead;
  archive >> comp.casting;
}

} // namespace serialization

#endif

