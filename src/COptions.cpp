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

#include "COptions.h"

#include <cstdlib>

#include "data/dirs.h"
#include "iml/IMLNode.h"
#include "iml/IMLTag.h"
#include "iml/utils.h"

COptions COptions::options;

COptions::COptions()
  : options_iml_(0),
    is_windowed_(true), newbie_tips_on_(false),
    screen_width_(800), screen_height_(600)
{
}

COptions::~COptions()
{
  if (options_iml_) {
    delete options_iml_;
  }
}

void COptions::LoadOptions()
{
  options_iml_ = iml::openIML(data::FindFile("data/options.xml").c_str());
  //options_iml_->print();
  IMLNode &screen = *options_iml_->findByName("screen");
  int val = atoi(iml::getAttribute(screen, "windowed", "1").c_str());
  if (val == 0) {
    is_windowed_ = false;
  }
  
  screen_width_ = atoi(iml::getAttribute(screen, "width", "800").c_str());
  screen_height_ = atoi(iml::getAttribute(screen, "height", "600").c_str());

  IMLNode &gameplay = *options_iml_->findByName("gameplay");
  val = atoi(iml::getAttribute(gameplay, "newGameTips", "0").c_str());
  if (val == 1) {
    newbie_tips_on_ = true;
  }
}

bool COptions::getWindowed()
{
  return is_windowed_;
}

void COptions::setWindowed(bool value)
{
  IMLTag *screen = dynamic_cast<IMLTag*>(options_iml_->findByName("screen"));
  if (value) {
    screen->getAttributes()["windowed"] = "1";
  } else {
    screen->getAttributes()["windowed"] = "0";
  }
  is_windowed_ = value;
  //options_iml_->print();
  iml::saveIML(*options_iml_, data::FindFile("data/options.xml").c_str());
}

int COptions::getScreenWidth()
{
  return screen_width_;
}

int COptions::getScreenHeight()
{
  return screen_height_;
}

bool COptions::getNewbieTips()
{
  return newbie_tips_on_;
}

void COptions::setNewbieTips(bool value)
{
  IMLTag *gameplay = dynamic_cast<IMLTag*>(options_iml_->findByName("gameplay"));
  if (value) {
    gameplay->getAttributes()["newGameTips"] = "1";
  } else {
    gameplay->getAttributes()["newGameTips"] = "0";
  }
  newbie_tips_on_ = value;
  //options_iml_->print();
  iml::saveIML(*options_iml_, data::FindFile("data/options.xml").c_str());
}
