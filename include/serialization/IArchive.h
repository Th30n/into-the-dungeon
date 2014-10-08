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
#ifndef SERIALIZATION_IARCHIVE_H
#define SERIALIZATION_IARCHIVE_H

#include <istream>
#include <vector>

#include "iml/IMLParser.h"
#include "iml/utils.h"
#include "serialization/NameValuePair.hpp"

namespace serialization {

template<class Archive, class T>
void load(Archive &archive, T &t)
{
  t.load(archive);
}

class IArchive
{
  public:
    IArchive(std::istream &is) : root_node_(0)
    {
      IMLParser parser;
      if (parser.createTree(is)) {
        root_node_ = parser.getRoot();
        tag_nodes_ = iml::GetChildrenTags(*root_node_);
        it_ = tag_nodes_.begin();
      }
    }
    ~IArchive()
    {
      if (root_node_) {
        delete root_node_;
      }
    }

    // Default implementation requires load function or method for given type.
    template<class T>
    IArchive &operator>>(T &t)
    {
      load(*this, t);
      return *this;
    }

    // Specializations for primitive types
    IArchive &operator>>(int &val)
    {
      val = iml::GetTagValue<int>(**it_);
      it_++;
      return *this;
    }

  private:
    IMLNode *root_node_;
    std::vector<IMLTag*> tag_nodes_;
    std::vector<IMLTag*>::iterator it_;
};

} // namespace serialization
#endif
