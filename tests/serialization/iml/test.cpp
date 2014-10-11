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
#include "test.hpp"

#include <iostream>
#include <sstream>

#include "serialization/iml/IArchive.hpp"
#include "serialization/iml/OArchive.hpp"

int main(int argc, char *argv[])
{
  C c;
  c.c = 5;
  std::ostringstream oss;
  serialization::iml::OArchive oarchive(oss);
  oarchive << c;
  if (oss.str() != "<B><a>1</a><b>2</b></B><c>5</c>") {
    std::cerr << oss.str() << " != <B><a>1</a><b>2</b></B><c>5</c>"
      << std::endl;
    return -1;
  }

  C c2;
  std::istringstream iss(oss.str());
  serialization::iml::IArchive iarchive(iss);
  iarchive >> c2;
  if (c2 != c) {
    std::cerr << "c2 != c" << std::endl;
    std::cerr << "c2.c = " << c2.c << std::endl;
    return -1;
  }
  return 0;
}
