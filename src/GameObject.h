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
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

/*
 * Base game object. Game Entity is defined as a sum of components that
 * are tied to ID represented by instance of this class.
 * Everything should be done through components.
 */
class GameObject {
  public:
    GameObject(unsigned id) : eid(id) {}
    virtual ~GameObject() {}
    unsigned getId() const { return eid; }

  private:
    unsigned eid;
};

inline bool operator==(const GameObject &l, const GameObject &r)
{
  return l.getId() == r.getId();
}

inline bool operator!=(const GameObject &l, const GameObject &r)
{
  return !operator==(l, r);
}
#endif
