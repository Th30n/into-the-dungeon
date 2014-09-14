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
#include "Sfx.h"

#include <cstdlib>

#include "iml/IMLNode.h"
#include "iml/utils.h"

Sfx::Sfx()
  : width(0), height(0), x_offset(0), y_offset(0),
    max_frames(0), frame_rate(0), speed(1.0f)
{
}

void Sfx::loadIML(const IMLNode &node)
{
  type = iml::getAttribute(node, "type");
  sprite = iml::getAttribute(node, "sprite");
  width = atoi(iml::getAttribute(node, "width", "0").c_str());
  height = atoi(iml::getAttribute(node, "height", "0").c_str());
  x_offset = atoi(iml::getAttribute(node, "xOffset", "0").c_str());
  y_offset = atoi(iml::getAttribute(node, "yOffset", "0").c_str());
  max_frames = atoi(iml::getAttribute(node, "frames", "0").c_str());
  frame_rate = atoi(iml::getAttribute(node, "frameRate", "0").c_str());
  speed = atof(iml::getAttribute(node, "speed", "1").c_str());
}
