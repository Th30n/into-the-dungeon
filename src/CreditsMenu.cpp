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
#include "CText.h"
#include "CApp.h"
#include "CSurface.h"
#include "data/dirs.h"
#include "StateMachine.h"

CreditsMenu* CreditsMenu::Instance()
{
  static CreditsMenu instance;
  
  return &instance;
}

CreditsMenu::CreditsMenu()
{
  background_ = NULL;
  logo_ = NULL;
}

CreditsMenu::~CreditsMenu()
{
  deleteSurfaces();
}

void CreditsMenu::deleteSurfaces()
{
  if (background_) SDL_FreeSurface(background_);
  if (logo_) SDL_FreeSurface(logo_);
  
  background_ = NULL;
  logo_ = NULL;
}

void CreditsMenu::initializeSurfaces()
{
  if (background_ == NULL)
    background_ = CSurface::OnLoad(
        data::FindFile("gfx/UI/MENU_BACKGROUND_800x600.png").c_str());
  if (logo_ == NULL) {
    logo_ =
        CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_LOGO.png").c_str());
  }
}
  
void CreditsMenu::Enter(CApp* app)
{
  initializeSurfaces();
  owner_ = app;
}

void CreditsMenu::Execute(CApp* app)
{ 
  SDL_Event Event;
  
  while(SDL_PollEvent(&Event))
  {
    Menu::OnEvent(&Event);
  }

  OnRender(app);
}

void CreditsMenu::Exit(CApp* app)
{
  deleteSurfaces();
}

void CreditsMenu::OnRender(CApp* app)
{
  CSurface::OnDraw(app->getDisplay(), background_, 0, 0);
  CSurface::OnDraw(app->getDisplay(), logo_, 0, 0);
  Menu::displayVersion(app->getDisplay());
    
  std::string CreditsText =
      "  Programming & Design  \n\n  Teon '7h30n' Banek\n\n\n";
  CreditsText += "  Sound & Music Design\n\n  Luka 'Foox' Fucek\n\n\n";
  CreditsText += "  Graphic Design\n\n  Psiweapon\n";
  CText::TextControl.displayBMPText(app->getDisplay(), 480, 250,
      CreditsText, 219, 55, 38, 90, 47, 21, FONT_MEDIUM_BOLD, 0);
  
  SDL_Flip(app->getDisplay());
}

void CreditsMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym) {
    case SDLK_ESCAPE:
    {
      goBack();
      break;
    }
    default:
    {
      break;
    }
  }
}

void CreditsMenu::goBack()
{
  owner_->getFSM()->RevertToPreviousState();
}
