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
#ifndef SERIALIZATION_IML_OARCHIVE_HPP
#define SERIALIZATION_IML_OARCHIVE_HPP

#include <iostream>
#include <map>
#include <stack>
#include <sstream>
#include <string>
#include <utility>

#include "iml/IMLTag.h"
#include "iml/IMLValue.h"
#include "serialization/common.hpp"
#include "serialization/NameValuePair.hpp"

namespace serialization
{

namespace iml
{

// Writes an IML (almost XML) representation.
class OArchive
{
  public:
    OArchive(std::ostream &os) : os_(os)
    {
      node_stack_.push(new IMLTag("Document", false));
    }
    ~OArchive() { delete node_stack_.top(); }

    // Default implementation requires save function or method for given type.
    template<class T>
    OArchive &operator<<(T &t)
    {
      using serialization::save;
      int version = 0;
      *this << MakeNameValuePair("version", version);
      save(*this, t, version);
      return *this;
    }

    // Specializations for primitive types
    OArchive &operator<<(short val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(unsigned short val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(int val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(long int val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(unsigned int val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(long unsigned int val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(bool val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(float val)
    {
      writeTagValue(val);
      return *this;
    }

    OArchive &operator<<(std::string val)
    {
      node_stack_.top()->addChild(new IMLValue(val));
      return *this;
    }

    // Writes a tag node.
    template<class T>
    OArchive &operator<<(const NameValuePair<T> &tag)
    {
      IMLTag *tag_node = new IMLTag(tag.name, false);
      node_stack_.top()->addChild(tag_node);
      node_stack_.push(tag_node);
      *this << tag.value;
      node_stack_.pop();
      if (node_stack_.size() == 1) {
        tag_node->writeToStream(os_);
        // For better readability.
        os_ << std::endl;
      }
      return *this;
    }

  private:
    template<typename T>
    void writeTagValue(const T &val)
    {
      std::ostringstream oss;
      oss << val;
      node_stack_.top()->addChild(new IMLValue(oss.str()));
    }

    std::ostream &os_;
    std::stack<IMLTag*> node_stack_;
};

} // namespace iml

} // namespace serialization
#endif
