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
#include "AIComponent.h"

#include <cstdlib>
#include <sstream>

#include "ComponentFactory.h"
#include "iml/utils.h"
#include "serialization/IMLArchive.h"
#include "serialization/IArchive.h"

int AIComponent::hreg = ComponentFactory::instance().registerCreator(
    "ai", AIComponent::creator);

const Vector2f AIComponent::RESET_POSITION(-1.0f, -1.0f);

IComponent *AIComponent::creator()
{
  return new AIComponent();
}

AIComponent::AIComponent()
  : waypoint(RESET_POSITION), cast_rate(0)
{
}

namespace serialization {

template<class Archive>
void save(Archive &archive, AIComponent &comp)
{
  archive << MakeNameValuePair("castRate", comp.cast_rate);
}

template<class Archive>
void load(Archive &archive, AIComponent &comp)
{
  NameValuePair<int> castRate = MakeNameValuePair("castRate", comp.cast_rate);
  archive >> castRate;
}

}

void AIComponent::loadIML(const IMLNode &node)
{
  cast_rate = iml::GetAttribute(node, "castRate", 0);
  serialization::IMLArchive archive(std::cout);
  archive << *this;
  std::istringstream iss("<castRate>25</castRate>");
  serialization::IArchive iarchive(iss);
  iarchive >> *this;
  std::cout << "Loaded: " << cast_rate;
}

void AIComponent::save(FILE *file)
{
}

bool AIComponent::load(FILE *file)
{
  return true;
}
