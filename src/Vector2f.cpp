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
#include "Vector2f.h"

#include <cmath>
#include <cstdio>

Vector2f Vector2f::normalized() const
{
  const float len = sqrtf(x * x + y * y);
  if (len == 0.0f) {
    return Vector2f();
  } else {
    return Vector2f(x / len, y / len);
  }
}

float Vector2f::distance(const Vector2f &o) const
{
  return sqrtf((o.x - x) * (o.x - x) + (o.y - y) * (o.y - y));
}

float Vector2f::manhattanDistance(const Vector2f &o) const
{
  return std::abs(o.x - x) + std::abs(o.y - y);
}

void Vector2f::print() const
{
  printf("(%f, %f)\n", x, y);
}
