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
#include "Rectangle.h"

#include <cstdio>

Rectangle::Rectangle() : x_(0), y_(0), width_(0), height_(0)
{
}

Rectangle::Rectangle(int x, int y, int width, int height)
{
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;
}

bool Rectangle::intersects(int x, int y) const
{
  if (x < x_ || x >= x_ + width_
      || y < y_ || y >= y_ + height_)
    return false;
    
  return true;
}

bool Rectangle::intersects(const Rectangle &r) const
{
  if (r.x_ >= x_ + width_) return false;
  if (r.x_ + r.width_ <= x_) return false;
  if (r.y_ >= y_ + height_) return false;
  if (r.y_ + r.height_ <= y_) return false;

  return true;
}

int Rectangle::getX()
{
  return x_;
}

void Rectangle::setX(int x)
{
  x_ = x;
}

int Rectangle::getY()
{
  return y_;
}

void Rectangle::setY(int y)
{
  y_ = y;
}

int Rectangle::getWidth()
{
  return width_;
}

void Rectangle::setWidth(int w)
{
  width_ = w;
}

int Rectangle::getHeight()
{
  return height_;
}

void Rectangle::setHeight(int h)
{
  height_ = h;
}

void Rectangle::print()
{
  printf("r: (%d, %d) (%d, %d)\n", x_, y_, x_ + width_, y_ + height_);
}
