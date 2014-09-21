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
#include "CSurface.h"
#include "data/dirs.h"
#include "StateMachine.h"

#define HIGHSCORE_ITEMS 10

HighscoreMenu* HighscoreMenu::Instance()
{
  static HighscoreMenu instance;
  
  return &instance;
}

HighscoreMenu::HighscoreMenu() : background_(0), high_score_page_(0)
{
}

HighscoreMenu::~HighscoreMenu()
{
  deleteSurfaces();
}

void HighscoreMenu::deleteSurfaces()
{
  if (background_) SDL_FreeSurface(background_);
  
  background_ = NULL;
}

void HighscoreMenu::initializeSurfaces()
{
  if (background_ == NULL)
    background_ = CSurface::OnLoad(data::FindFile("gfx/UI/MENU_BACKGROUND_800x600.png").c_str());
}
  
void HighscoreMenu::Enter(CApp* app)
{
  initializeSurfaces();
  owner_ = app;
}

void HighscoreMenu::Execute(CApp* app)
{ 
  SDL_Event Event;
  
  while(SDL_PollEvent(&Event))
  {
    Menu::OnEvent(&Event);
  }

  OnRender(app);
}

void HighscoreMenu::Exit(CApp* app)
{
  deleteSurfaces();
}

void HighscoreMenu::OnRender(CApp* app)
{
  CSurface::OnDraw(app->getDisplay(), background_, 0, 0);
  Menu::displayVersion(app->getDisplay());
  
  HighscoreList(app->getDisplay());
  
  SDL_Flip(app->getDisplay());
}

void HighscoreMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym)
  {
    case SDLK_ESCAPE:
    {
      goBack();
      break;
    }
    case SDLK_LEFT:
    {
      high_score_page_--;  
      if (high_score_page_ < 0) high_score_page_ = 0;
      break;
    }
    case SDLK_RIGHT:
    {
      high_score_page_++;    
      break;
    }
    default:
    {
      break;
    }
  }
}

void HighscoreMenu::goBack()
{
  owner_->getFSM()->RevertToPreviousState();
}

void HighscoreMenu::HighscoreList(SDL_Surface *dst)
{
}
