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
#ifndef SERIALIZATION_IML_IARCHIVE_HPP
#define SERIALIZATION_IML_IARCHIVE_HPP

#include <istream>
#include <list>
#include <vector>

#include "iml/IMLParser.h"
#include "iml/utils.h"
#include "serialization/common.hpp"
#include "serialization/NameValuePair.hpp"

namespace serialization {

namespace iml {

// Reads an IML (almost XML) representation.
class IArchive
{
  public:
    IArchive(std::istream &is) : root_node_(0)
    {
      IMLParser parser;
      if (parser.createTree(is)) {
        root_node_ = parser.getRoot();
        std::vector<IMLTag*> children = ::iml::GetChildrenTags(*root_node_);
        tag_nodes_.insert(tag_nodes_.begin(), children.begin(), children.end());
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
      using serialization::load;
      load(*this, t, 0);
      return *this;
    }

    // Specializations for primitive types
    IArchive &operator>>(int &val)
    {
      readTagValue(val);
      return *this;
    }

  private:
    // Reads a tag value into given argument and inserts current tags
    // children to tag nodes list.
    // This realizes depth first traversal.
    template<typename T>
    void readTagValue(T &val)
    {
      IMLTag *tag_node = tag_nodes_.front();
      val = ::iml::GetTagValue<T>(*tag_node);
      tag_nodes_.pop_front();
      std::vector<IMLTag*> children(::iml::GetChildrenTags(*tag_node));
      tag_nodes_.insert(tag_nodes_.begin(), children.begin(), children.end());
    }

    IMLNode *root_node_;
    std::list<IMLTag*> tag_nodes_;
};

} // namespace iml

} // namespace serialization
#endif
