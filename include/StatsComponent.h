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
#ifndef STATSCOMPONENT_H
#define STATSCOMPONENT_H

#include "IComponent.h"

#include <vector>
#include <string>

/**
 * Stores stats of an object.
 * It is also used in other components as a convenient stat holder.
 */
class StatsComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    StatsComponent();
    virtual ~StatsComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Character level.
    int level;
    // Current experience amount.
    int xp;
    // Vision range in squares.
    int vision_range;
    // Primary stats, perhaps should be a struct of its own.
    float attack;
    float armor;
    float health;
    // List of available skills/spells.
    // Currently used by the AI, should be expanded.
    std::vector<std::string> skills;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, StatsComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("level", comp.level);
  archive << MakeNameValuePair("xp", comp.xp);
  archive << MakeNameValuePair("visionRange", comp.vision_range);
  archive << MakeNameValuePair("attack", comp.attack);
  archive << MakeNameValuePair("armor", comp.armor);
  archive << MakeNameValuePair("health", comp.health);
  typedef std::vector<std::string> Skills;
  Skills::size_type skills = comp.skills.size();
  archive << MakeNameValuePair("skills", skills);
  for (Skills::iterator it = comp.skills.begin();
      it != comp.skills.end(); ++it) {
    archive << MakeNameValuePair("skill", *it);
  }
}

template<class Archive>
inline void load(Archive &archive, StatsComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.level;
  archive >> comp.xp;
  archive >> comp.vision_range;
  archive >> comp.attack;
  archive >> comp.armor;
  archive >> comp.health;
  typedef std::vector<std::string> Skills;
  Skills::size_type skills = 0;
  archive >> skills;
  for (Skills::size_type i = 0; i < skills; ++i) {
    std::string skill;
    archive >> skill;
    comp.skills.push_back(skill);
  }
}

} // namespace serialization

#endif
