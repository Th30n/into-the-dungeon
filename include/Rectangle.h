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
#ifndef RECTANGLE_H
#define RECTANGLE_H

/*
 * This is a basic rectangle class.
 */
class Rectangle {
  public:
    Rectangle();
    Rectangle(int x, int y, int width, int height);
    
    // True if (x, y) inside rectangle.
    bool intersects(int x, int y) const;
    // True if Rectangle r overlaps with this rectangle.
    bool intersects(const Rectangle &r) const;
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getWidth();
    void setWidth(int w);
    int getHeight();
    void setHeight(int h);
    // Print debug info to stdout.
    void print();

  private:
    int x_;
    int y_;
    int width_;
    int height_;
};

#endif

