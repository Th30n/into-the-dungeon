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
#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "serialization/NameValuePair.hpp"

/**
 * Vector of 2 float values, can be used as a point in space also.
 * Among the usual methods are:
 *   normalize - normalizes this vector.
 *   normalized - returns a new normalized vector from this one.
 *   distance - calculates distance between 2 points.
 *   manhattanDistance - calculates Manhattan distance between 2 points.
 */
struct Vector2f {
  public:
    Vector2f() : x(0.0f), y(0.0f) {}
    Vector2f(float x, float y) : x(x), y(y) {}

    Vector2f &operator+=(const Vector2f &r)
    {
      x += r.x;
      y += r.y;
      return *this;
    }

    Vector2f &operator-=(const Vector2f &r)
    {
      x -= r.x;
      y -= r.y;
      return *this;
    }

    Vector2f &operator*=(const float f)
    {
      x *= f;
      y *= f;
      return *this;
    }
    Vector2f normalized() const;
    float distance(const Vector2f &o) const;
    float manhattanDistance(const Vector2f &o) const;

    void print() const;

    float x;
    float y;
};

inline Vector2f operator+(const Vector2f &l, const Vector2f &r)
{
  return Vector2f(l.x + r.x, l.y + r.y);
}

inline Vector2f operator-(const Vector2f &l, const Vector2f &r)
{
  return Vector2f(l.x - r.x, l.y - r.y);
}

inline Vector2f operator*(const Vector2f &l, const float f)
{
  return Vector2f(l.x * f, l.y * f);
}

inline float operator*(const Vector2f &l, const Vector2f &r)
{
  return l.x * r.x + l.y * r.y;
}

inline bool operator==(const Vector2f &l, const Vector2f &r)
{
  return (l.x == r.x) && (l.y == r.y);
}

inline bool operator!=(const Vector2f &l, const Vector2f &r)
{
  return !operator==(l, r);
}

namespace serialization {

template<class Archive>
inline void save(Archive &archive, Vector2f &vec, unsigned int version)
{
  archive << MakeNameValuePair("x", vec.x);
  archive << MakeNameValuePair("y", vec.y);
}

template<class Archive>
inline void load(Archive &archive, Vector2f &vec, unsigned int version)
{
  archive >> vec.x;
  archive >> vec.y;
}

} // namespace serialization
#endif
