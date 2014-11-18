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
#ifndef SFX_H
#define SFX_H

#include <string>

#include "serialization/NameValuePair.hpp"

class IMLNode;

/**
 * Describes a special effect.
 * Used for creating particle based animations and projectiles.
 */
struct Sfx {
    Sfx();
    void loadIML(const IMLNode &node);

    // Special effect type (animation, projectile)
    std::string type;
    // Sprite-sheet file.
    std::string sprite;
    // Dimensions of a sprite.
    int width;
    int height;
    // Rendering offsets.
    int x_offset;
    int y_offset;
    // Animation frames.
    int max_frames;
    int frame_rate;
    // Speed of projectile
    float speed;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, Sfx &sfx, unsigned int version)
{
  archive << MakeNameValuePair("type", sfx.type);
  archive << MakeNameValuePair("sprite", sfx.sprite);
  archive << MakeNameValuePair("width", sfx.width);
  archive << MakeNameValuePair("height", sfx.height);
  archive << MakeNameValuePair("xOffset", sfx.x_offset);
  archive << MakeNameValuePair("yOffset", sfx.y_offset);
  archive << MakeNameValuePair("maxFrames", sfx.max_frames);
  archive << MakeNameValuePair("frameRate", sfx.frame_rate);
  archive << MakeNameValuePair("speed", sfx.speed);
}

template<class Archive>
inline void load(Archive &archive, Sfx &sfx, unsigned int version)
{
  archive >> sfx.type;
  archive >> sfx.sprite;
  archive >> sfx.width;
  archive >> sfx.height;
  archive >> sfx.x_offset;
  archive >> sfx.y_offset;
  archive >> sfx.max_frames;
  archive >> sfx.frame_rate;
  archive >> sfx.speed;
}

} // namespace serialization

#endif
