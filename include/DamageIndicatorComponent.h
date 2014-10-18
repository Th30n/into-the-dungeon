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
#ifndef DAMAGEINDICATORCOMPONENT_H
#define DAMAGEINDICATORCOMPONENT_H

#include <list>
#include <cstdio>

#include <SDL.h>

#include "IComponent.h"

enum OverlayText {
  NONE,
  MISS,
  BACKSTAB,
  ARMORED
};

enum OverlayColor {
  RED,
  GREEN
};

struct Overlay {
  int text;
  unsigned start_time;
  int health_change;
  int color;
};

typedef std::list<Overlay*> OverlayList;
typedef OverlayList::iterator OverlayListIterator;

/*
 * This component displays health change of an entity, upon
 * damage or smth. else. (Healing etc.)
 */
class DamageIndicatorComponent : public IComponent {
  public:

    DamageIndicatorComponent();
    virtual ~DamageIndicatorComponent();
    void save(FILE *file);
    bool load(FILE *file);
  
    OverlayList overlays;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, Overlay &overlay, unsigned int version)
{
  archive << MakeNameValuePair("text", overlay.text);
  archive << MakeNameValuePair("startTime", overlay.start_time);
  archive << MakeNameValuePair("healthChange", overlay.health_change);
  archive << MakeNameValuePair("color", overlay.color);
}

template<class Archive>
inline void load(Archive &archive, Overlay &overlay, unsigned int version)
{
  archive >> overlay.text;
  archive >> overlay.start_time;
  archive >> overlay.health_change;
  archive >> overlay.color;
}

template<class Archive>
inline void save(Archive &archive, DamageIndicatorComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  OverlayList::size_type overlays = comp.overlays.size();
  archive << MakeNameValuePair("overlays", overlays);
  for (OverlayListIterator it = comp.overlays.begin();
      it != comp.overlays.end(); ++it) {
    archive << **it;
  }
}

template<class Archive>
inline void load(Archive &archive, DamageIndicatorComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  OverlayList::size_type overlays = 0;
  archive >> overlays;
  for (OverlayList::size_type i = 0; i < overlays; ++i) {
    Overlay *overlay = new Overlay;
    archive >> *overlay;
    comp.overlays.push_back(overlay);
  }
}

} // namespace serialization

#endif

