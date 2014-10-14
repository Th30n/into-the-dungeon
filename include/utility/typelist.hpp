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
#ifndef UTILITY_TYPELIST_HPP
#define UTILITY_TYPELIST_HPP

// TypeList, inspired by Modern C++ Design by Andrei Alexandrescu.
namespace utility
{

class NullType {};

struct EmptyType {};

// U = NullType signifies the end of the list
template<class T, class U>
struct TypeList
{
  typedef T Head;
  typedef U Tail;
};

// Helper macros for creating TypeLists
#define TYPELIST_1(T1) utility::TypeList<T1, utility::NullType>
#define TYPELIST_2(T1, T2) utility::TypeList<T1, TYPELIST_1(T2) >

namespace tl
{
// To better understand the algorithms think functional (e.g. Haskell).

// Calculates the length of a TypeList (at compile time).
template<class TL> struct Length;
template<> struct Length<NullType>
{
  // Enum hack
  enum { value = 0 };
};
template<class T, class U>
struct Length<TypeList<T, U> >
{
  enum { value = 1 + Length<U>::value };
};

// Returns the type at given index in a TypeList.
// Compiler error on access out of bounds.
template<class TL, unsigned int i> struct TypeAt;
template<class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0>
{
  typedef Head Result;
};
template<class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i>
{
  typedef typename TypeAt<Tail, i - 1>::Result Result;
};

// Returns index of given type T in TypeList, otherwise -1.
template<class TL, class T> struct IndexOf;
template<class T>
struct IndexOf<NullType, T>
{
  enum { value = -1 };
};
template<class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T>
{
  enum { value = 0 };
};
template<class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T>
{
  private:
    enum { temp = IndexOf<Tail, T>::value };
  public:
    enum { value = temp == -1 ? -1 : 1 + temp };
};

// Appends a type T to typelist
template<class TL, class T> struct Append;
template<> struct Append<NullType, NullType>
{
  typedef NullType Result;
};
template<class T> struct Append<NullType, T>
{
  typedef TYPELIST_1(T) Result;
};
template<class Head, class Tail>
struct Append<NullType, TypeList<Head, Tail> >
{
  typedef TypeList<Head, Tail> Result;
};
template<class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T>
{
  typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
};

// TODO: Removing and sorting TypeLists.

} // namespace tl

} // namespace utility

#endif
