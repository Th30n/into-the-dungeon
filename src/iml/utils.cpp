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
#include "iml/utils.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "iml/IMLNode.h"
#include "iml/IMLParser.h"
#include "iml/IMLTag.h"

namespace iml {

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

std::vector<IMLTag*> getChildrenTags(const IMLNode &node)
{
  std::vector<IMLTag*> tags;
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

} // namespace iml
