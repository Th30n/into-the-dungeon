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

#include <map>
#include <string>
#include <vector>

class IMLNode;

namespace iml {

std::map<std::string, float> getMappedFloats(const IMLNode &node);
std::map<std::string, int> getMappedInts(const IMLNode &node);
IMLNode *openIML(const char *path);
void saveIML(const IMLNode &node, const char *path);
std::string getAttribute(const IMLNode &node, std::string key);
std::string getAttribute(const IMLNode &node, std::string key, std::string def);
std::vector<IMLNode*> getChildrenTags(const IMLNode &node);

};
#endif
