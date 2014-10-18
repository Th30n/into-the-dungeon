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
#ifndef RENDERERCOMPONENT_H
#define RENDERERCOMPONENT_H

#include <string>

#include <SDL.h>

#include "IComponent.h"

/*
 * Renderer component of a game object.
 */
class RendererComponent : public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    RendererComponent(const char *filename);
    virtual ~RendererComponent();
    virtual void start();
    virtual void loadIML(const IMLNode &node);
    template<class Archive>
    inline void save(Archive &archive, unsigned int version);
    template<class Archive>
    inline void load(Archive &archive, unsigned int version);

    SDL_Surface *image;
    int x;
    int y;
    int width;
    int height;
    int x_offset;
    int y_offset;

  private:
    std::string gfx_filename_;
};

template<class Archive>
inline void RendererComponent::save(Archive &archive, unsigned int version)
{
  using serialization::MakeNameValuePair;
  archive << *static_cast<IComponent*>(this);
  archive << MakeNameValuePair("x", x);
  archive << MakeNameValuePair("y", y);
  archive << MakeNameValuePair("width", width);
  archive << MakeNameValuePair("height", height);
  archive << MakeNameValuePair("xOffset", x_offset);
  archive << MakeNameValuePair("yOffset", y_offset);
  archive << MakeNameValuePair("gfxFilename", gfx_filename_);
}

template<class Archive>
inline void RendererComponent::load(Archive &archive, unsigned int version)
{
  archive >> *static_cast<IComponent*>(this);
  archive >> x;
  archive >> y;
  archive >> width;
  archive >> height;
  archive >> x_offset;
  archive >> y_offset;
  archive >> gfx_filename_;
  start();
}

#endif
