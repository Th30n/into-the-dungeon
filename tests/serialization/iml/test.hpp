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
#ifndef SERIALIZATION_IML_TEST_HPP
#define SERIALIZATION_IML_TEST_HPP

#include "serialization/NameValuePair.hpp"

struct A
{
  A() : a(1) {}
  int a;
};

struct B : public A
{
  B() : b(2) {}
  int b;
};

struct C
{
  C() : c(3) {}
  int c;
  B b;
};

bool operator==(const B &b1, const B &b2)
{
  return (b1.b == b2.b && b1.a == b2.a);
}

bool operator!=(const B &b1, const B &b2)
{
  return !(b1 == b2);
}

bool operator==(const C &c1, const C &c2)
{
  return (c1.c == c2.c && c1.b == c2.b);
}

bool operator!=(const C &c1, const C &c2)
{
  return !(c1 == c2);
}

namespace serialization {

template<class Archive>
void save(Archive &archive, C &c, unsigned int version)
{
  archive << MakeNameValuePair("B", c.b);
  archive << MakeNameValuePair("c", c.c);
}

template<class Archive>
void load(Archive &archive, C &c, unsigned int version)
{
  int empty_val_tag;
  archive >> empty_val_tag;
  archive >> c.b;
  archive >> c.c;
}

template<class Archive>
void save(Archive &archive, B &b, unsigned int version)
{
  archive << *static_cast<A*>(&b);
  archive << MakeNameValuePair("b", b.b);
}

template<class Archive>
void load(Archive &archive, B &b, unsigned int version)
{
  archive >> *static_cast<A*>(&b);
  archive >> b.b;
}

template<class Archive>
void save(Archive &archive, A &a, unsigned int version)
{
  archive << MakeNameValuePair("a", a.a);
}

template<class Archive>
void load(Archive &archive, A &a, unsigned int version)
{
  archive >> a.a;
}

} // namespace serialization
#endif
