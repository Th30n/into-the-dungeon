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

class IMLNode;

/**
 * Describes a special effect.
 * Used for creating particle based animations and projectiles.
 */
class Sfx {
  public:
    Sfx();
    virtual void loadIML(const IMLNode &node);

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

#endif
