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
#ifndef IML_UTILS_H
#define IML_UTILS_H

#include <list>
#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "iml/IMLNode.h"
#include "iml/IMLTag.h"

namespace iml {

// Returns the value of given IMLTag node. If unable to create the value with
// given type T the first element of std::pair is false and value is set
// to initial value for type.
template<typename T>
std::pair<bool, T> GetTagValue(const IMLTag &tag)
{
  const std::list<IMLNode*> &vals = tag.getChildren();
  if (!vals.empty()) {
    std::string string_val = (*vals.begin())->getName();
    std::istringstream sstream(string_val);
    T val;
    sstream >> val;
    if (!sstream.fail()) {
      return std::make_pair(true, val);
    }
  }
  return std::make_pair(false, T());
}

// std::map<std::string, float> getMappedFloats(const IMLNode &node);
// std::map<std::string, int> getMappedInts(const IMLNode &node);
IMLNode *openIML(const char *path);
void saveIML(const IMLNode &node, const char *path);
std::string getAttribute(const IMLNode &node, std::string key);
std::string getAttribute(const IMLNode &node, std::string key, std::string def);
std::vector<IMLTag*> getChildrenTags(const IMLNode &node);

} // namespace iml
#endif
