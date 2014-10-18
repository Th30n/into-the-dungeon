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
#define TYPELIST_3(T1, T2, T3) utility::TypeList<T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4) \
  utility::TypeList<T1, TYPELIST_3(T2, T3, T4) >
#define TYPELIST_5(T1, T2, T3, T4, T5) \
  utility::TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
  utility::TypeList<T1, TYPELIST_5(T2, T3, T4, T5, T6) >
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
  utility::TypeList<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
  utility::TypeList<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
  utility::TypeList<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
  utility::TypeList<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >
#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
  utility::TypeList<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >
#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
  utility::TypeList<T1, \
    TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >
#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
  utility::TypeList<T1, \
    TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >
#define TYPELIST_14( \
  T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
    utility::TypeList<T1, \
      TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >
#define TYPELIST_15( \
  T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
    utility::TypeList<T1, TYPELIST_14( \
      T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >
#define TYPELIST_16( \
  T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) \
    utility::TypeList<T1, TYPELIST_15( \
      T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) >
#define TYPELIST_17( \
  T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) \
    utility::TypeList<T1, TYPELIST_16( \
      T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) >
#define TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, \
    T14, T15, T16, T17, T18) \
  utility::TypeList<T1, TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, \
      T12, T13, T14, T15, T16, T17, T18) >
#define TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, \
    T14, T15, T16, T17, T18, T19) \
  utility::TypeList<T1, TYPELIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, \
      T12, T13, T14, T15, T16, T17, T18, T19) >
#define TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, \
    T14, T15, T16, T17, T18, T19, T20) \
  utility::TypeList<T1, TYPELIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, \
      T12, T13, T14, T15, T16, T17, T18, T19, T20) >

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
