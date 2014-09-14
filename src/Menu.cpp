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
#include "GameStates.h"
#include "CApp.h"
#include "CText.h"
#include "Define.h"

void Menu::Enter(CApp* app)
{
  owner_ = app;
}

void Menu::OnEvent(SDL_Event *event)
{
  CEvent::OnEvent(event);
}

void Menu::displayVersion(SDL_Surface* dst)
{
  //Y is Window Height - 16
  CText::TextControl.displayBMPText(dst, 1, 584, VERSION, 178, 27, 2,
                    -1, -1, -1, FONT_SMALL, 0);
}

void Menu::OnExit()
{
  owner_->setRunning(false);
}
