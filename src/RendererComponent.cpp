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
#include "RendererComponent.h"

#include <cstdlib>

#include "CSurface.h"
#include "CCamera.h"
#include "ComponentFactory.h"
#include "data/dirs.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"
#include "SpaceComponent.h"
#include "AnimationComponent.h"

int RendererComponent::hreg = ComponentFactory::instance().registerCreator(
    "spritesheet", RendererComponent::creator);

IComponent *RendererComponent::creator()
{
  return new RendererComponent("");
}

RendererComponent::RendererComponent(const char *filename)
  : image(0), x(0), y(0), width(32), height(32), x_offset(0), y_offset(0),
    gfx_filename_(filename)
{
}

RendererComponent::~RendererComponent()
{
  if (image != 0) {
    SDL_FreeSurface(image);
  }
}

void RendererComponent::start()
{
  image = CSurface::OnLoad(gfx_filename_.c_str());
}

void RendererComponent::loadIML(const IMLNode &node)
{
  gfx_filename_ = data::FindFile(iml::GetAttribute<std::string>(node, "file"));
  width = iml::GetAttribute(node, "width", 32);
  height = iml::GetAttribute(node, "height", 32);
  x_offset = iml::GetAttribute(node, "xOffset", 0);
  y_offset = iml::GetAttribute(node, "yOffset", 0);
  start();
}
