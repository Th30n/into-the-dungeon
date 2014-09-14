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
#include "utils.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "IMLNode.h"
#include "IMLParser.h"
#include "IMLTag.h"

namespace iml {

std::map<std::string, float> getMappedFloats(const IMLNode &node)
{
  std::map<std::string, float> map;
  const std::list<IMLNode*> pairs = node.getChildren();
  std::list<IMLNode*>::const_iterator it = pairs.begin();
  for (; it != pairs.end(); ++it) {
    IMLTag *tag = dynamic_cast<IMLTag*>(*it);
    if (tag) {
      std::string key = tag->getName();
      std::list<IMLNode*> &vals = tag->getChildren();
      float val = atof((*vals.begin())->getName().c_str());
      map[key] = val;
    }
  }
  return map;
}

std::map<std::string, int> getMappedInts(const IMLNode &node)
{
  std::map<std::string, int> map;
  const std::list<IMLNode*> pairs = node.getChildren();
  std::list<IMLNode*>::const_iterator it = pairs.begin();
  for (; it != pairs.end(); ++it) {
    IMLTag *tag = dynamic_cast<IMLTag*>(*it);
    if (tag) {
      std::string key = tag->getName();
      std::list<IMLNode*> &vals = tag->getChildren();
      int val = atoi((*vals.begin())->getName().c_str());
      map[key] = val;
    }
  }
  return map;
}

IMLNode *openIML(const char *path)
{
  IMLParser parser;
  std::ifstream file(path);
  if (!parser.createTree(file)) {
    fprintf(stderr, "Unable to parse: '%s'\n", path);
  }
  return parser.getRoot();
}

void saveIML(const IMLNode &node, const char *path)
{
  std::ofstream file(path);
  node.writeToStream(file);
}

std::string getAttribute(const IMLNode &node, std::string key)
{
  return getAttribute(node, key, "");
}

std::string getAttribute(const IMLNode &node, std::string key, std::string def)
{
  const IMLTag *tag = dynamic_cast<const IMLTag*>(&node);
  if (!tag) {
    return def;
  }
  const AttributesMap &attrs = tag->getAttributes();
  AttributesMap::const_iterator it;
  it = attrs.find(key);
  if (it != attrs.end()) {
    return it->second;
  } else {
    return def;
  }
}

std::vector<IMLNode*> getChildrenTags(const IMLNode &node)
{
  std::vector<IMLNode*> tags;
  const std::list<IMLNode*> &children = node.getChildren();
  std::list<IMLNode*>::const_iterator it = children.begin();
  for (; it != children.end(); ++it) {
    IMLTag *tag = dynamic_cast<IMLTag*>(*it);
    if (tag) {
      tags.push_back(tag);
    }
  }
  return tags;
}

};
