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
#include "AnimationComponent.h"

#include <cstdlib>

#include <SDL.h>

#include "CAnimation.h"
#include "ComponentFactory.h"
#include "iml/IMLNode.h"
#include "iml/utils.h"
#include "SpaceComponent.h"

int AnimationComponent::hreg = ComponentFactory::instance().registerCreator(
    "animation", AnimationComponent::creator);

IComponent *AnimationComponent::creator()
{
  return new AnimationComponent();
}

AnimationComponent::AnimationComponent()
  : max_frames(0), frame_rate(0), start_time(0),
    current_frame_col(0), current_frame_row(0), idle_frame_col(0),
    moving(false), attacking(false), is_dead(false), casting(false)
{
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::loadIML(const IMLNode &node)
{
  max_frames = iml::GetAttribute(node, "frames", 0);
  frame_rate = iml::GetAttribute(node, "frameRate", 0);
}

void AnimationComponent::save(FILE *file)
{
}

bool AnimationComponent::load(FILE *file)
{
  return true;
}
