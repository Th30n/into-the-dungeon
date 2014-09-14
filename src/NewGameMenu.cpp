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
#include "Define.h"
#include "StateMachine.h"

NewGameMenu* NewGameMenu::Instance()
{
  static NewGameMenu instance;
  
  return &instance;
}

NewGameMenu::NewGameMenu()
{
  background_ = NULL;
  logo_ = NULL;
  name_ = "";
}

NewGameMenu::~NewGameMenu()
{
  deleteSurfaces();
}

void NewGameMenu::deleteSurfaces()
{
  if (background_) SDL_FreeSurface(background_);
  if (logo_) SDL_FreeSurface(logo_);
  
  background_ = NULL;
  logo_ = NULL;
}

void NewGameMenu::initializeSurfaces()
{
  if (background_ == NULL)
    background_ = CSurface::OnLoad("./gfx/UI/MENU_BACKGROUND_800x600.png");
  if (logo_ == NULL)
    logo_ = CSurface::OnLoad("./gfx/UI/ITD++_MENU_LOGO.png");
}
  
void NewGameMenu::Enter(CApp* app)
{
  initializeSurfaces();
  owner_ = app;
}

void NewGameMenu::Execute(CApp* app)
{ 
  SDL_Event Event;
  
  while(SDL_PollEvent(&Event))
  {
    Menu::OnEvent(&Event);
  }

  OnRender(app);
}

void NewGameMenu::Exit(CApp* app)
{
  deleteSurfaces();
}

void NewGameMenu::OnRender(CApp* app)
{
  CSurface::OnDraw(app->getDisplay(), background_, 0, 0);
  CSurface::OnDraw(app->getDisplay(), logo_, 0, 0);
  Menu::displayVersion(app->getDisplay());
  
  CText::TextControl.displayBMPText(app->getDisplay(), 200, 150, "Enter character name: ", 219, 55, 38, 90, 47, 21, FONT_MEDIUM_BOLD, 0);
  CText::TextControl.displayBMPText(app->getDisplay(), 250, 200, name_, 219, 55, 38, 90, 47, 21, FONT_MEDIUM_BOLD, 0);
    
  SDL_Flip(app->getDisplay());
}

void NewGameMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym)
  {
    case SDLK_RETURN:
    {
      //strcpy(CEntity::EntityList[0]->Name, name_.c_str());
      startGame();
      break;
    }
    case SDLK_ESCAPE:
    {
      goBack();
      break;
    }
    case SDLK_BACKSPACE:
    {
      if (name_.length() != 0) name_.erase(name_.length() - 1);
      break;
    }
    default:
    {
      if (name_.length() <= MAX_NAME)
      {
        if (unicode == (Uint16)' ') name_ += unicode;
        else if (unicode >= (Uint16)'0' && unicode <= (Uint16)'9') name_ += unicode;
        else if (unicode >= (Uint16)'A' && unicode <= (Uint16)'Z') name_ += unicode;
        else if (unicode >= (Uint16)'a' && unicode <= (Uint16)'z') name_ += unicode;
      }
      break;
    }
  }
}

void NewGameMenu::startGame()
{
  owner_->getFSM()->ChangeState(Gameplay::Instance());
}

void NewGameMenu::goBack()
{
  owner_->getFSM()->RevertToPreviousState();
}
