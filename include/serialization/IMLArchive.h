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
#ifndef SERIALIZATION_IMLARCHIVE_H
#define SERIALIZATION_IMLARCHIVE_H

#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>

namespace serialization
{

class IMLArchive {
  public:
    IMLArchive(std::ostream &os) : os_(os) {}

    template<typename T>
    IMLArchive &operator<<(const T &t)
    {
      os_ << t;
      return *this;
    }

    template<typename T, typename U>
    IMLArchive &operator<<(const std::pair<T, U> &tag)
    {
      *this << "Tag: " << tag.first;
      *this << " Val: " << tag.second;
      *this << " EndTag: " << tag.first;
      return *this;
    }

    template<typename T>
    IMLArchive &operator<<(const std::map<std::string, T> &attr)
    {
      typename std::map<std::string, T>::const_iterator it;
      for (it = attr.begin(); it != attr.end(); ++it) {
        *this << "Attr: " << it->first;
        *this << " Val: " << it->second;
      }
      return *this;
    }

  private:
    std::ostream &os_;
};

} // namespace serialization
#endif
