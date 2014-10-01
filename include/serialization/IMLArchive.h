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
#ifndef SERIALIZATION_IMLARCHIVE_H
#define SERIALIZATION_IMLARCHIVE_H

#include <iostream>
#include <map>
#include <ostream>
#include <stack>
#include <sstream>
#include <string>
#include <utility>

#include "iml/IMLTag.h"
#include "iml/IMLValue.h"

namespace serialization
{

class IMLArchive {
  public:
    IMLArchive(std::ostream &os) : os_(os)
    {
      node_stack_.push(new IMLTag("Document", false));
    }
    ~IMLArchive() { delete node_stack_.top(); }

    template<typename T>
    IMLArchive &operator<<(const T &t)
    {
      std::ostringstream oss;
      oss << t;
      IMLValue *val_node = new IMLValue(oss.str());
      node_stack_.top()->addChild(val_node);
      //val_node->writeToStream(os_);
      return *this;
    }

    template<typename T, typename U>
    IMLArchive &operator<<(const std::pair<T, U> &tag)
    {
      std::ostringstream oss;
      oss << tag.first;
      IMLTag *tag_node = new IMLTag(oss.str(), false);
      node_stack_.top()->addChild(tag_node);
      node_stack_.push(tag_node);
      *this << tag.second;
      node_stack_.pop();
      if (node_stack_.size() == 1) {
        tag_node->writeToStream(os_);
      }
      return *this;
    }

    template<typename T>
    IMLArchive &operator<<(const std::map<std::string, T> &attr)
    {
      typename std::map<std::string, T>::const_iterator it;
      for (it = attr.begin(); it != attr.end(); ++it) {
        std::ostringstream oss;
        oss << it->second;
        node_stack_.top()->getAttributes()[it->first] = oss.str();
      }
      return *this;
    }

  private:
    std::ostream &os_;
    std::stack<IMLTag*> node_stack_;
};

} // namespace serialization
#endif
