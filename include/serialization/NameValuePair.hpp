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
#ifndef SERIALIZATION_NAMEVALUEPAIR_HPP
#define SERIALIZATION_NAMEVALUEPAIR_HPP

namespace serialization {

template<class T>
struct NameValuePair
{
  explicit NameValuePair(const char *name_, T &val_) : name(name_), value(val_)
  {
  }

  // TODO: Create assignment operator.

  template<class Archive>
  void save(Archive &archive, unsigned int version) const
  {
    archive << value;
  }

  template<class Archive>
  void load(Archive &archive, unsigned int version)
  {
    archive.operator>>(value);
  }

  const char *name;
  T &value;
};

template<class T>
NameValuePair<T> MakeNameValuePair(const char *name, T &val)
{
  return NameValuePair<T>(name, val);
}

} // namespace serialization
#endif
