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
#include <string>
#include <sstream>
#include <vector>

#include "iml/IMLNode.h"
#include "iml/IMLTag.h"

namespace iml {

// Returns the value of given IMLNode node. If unable to get the value
// or given node is not IMLTag node, returns the provided def_val.
template<typename T>
T GetTagValue(const IMLNode &node, T def_val = T())
{
  const IMLTag *tag = dynamic_cast<const IMLTag*>(&node);
  if (!tag) {
    return def_val;
  }
  const std::list<IMLNode*> &vals = tag->getChildren();
  if (!vals.empty()) {
    std::string string_val = (*vals.begin())->getName();
    std::istringstream sstream(string_val);
    T val;
    sstream >> val;
    if (!sstream.fail()) {
      return val;
    }
  }
  return def_val;
}

// Returns the attribute value for given tag node with give key. If unable to
// find attribute mapping or given node is not IMLTag node returns provided
// default value.
template <typename T>
T GetAttribute(const IMLNode &node, const std::string &key, T def_val = T())
{
  const IMLTag *tag = dynamic_cast<const IMLTag*>(&node);
  if (!tag) {
    return def_val;
  }
  const AttributesMap &attrs = tag->getAttributes();
  AttributesMap::const_iterator it = attrs.find(key);
  if (it != attrs.end()) {
    std::istringstream sstream(it->second);
    T val;
    sstream >> val;
    if (!sstream.fail()) {
      return val;
    }
  }
  return def_val;
}

IMLNode *OpenIML(const char *path);
void SaveIML(const IMLNode &node, const char *path);
std::vector<IMLTag*> GetChildrenTags(const IMLNode &node);

} // namespace iml
#endif
