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
#include "IAction.h"
#include "Button.h"
#include "Rectangle.h"
#include "COptions.h"
#include "StateMachine.h"

/*
 * This is an action for Fullscreen checkmark button
 */
class ToggleFullscreenAction : public IAction {
  public:
    ToggleFullscreenAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    ~ToggleFullscreenAction(){}
    
    void performAction()
    {
      if (owner_) {
        owner_->toggleFullscreen();
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for Tutorial checkmark button
 */
class ToggleTutorialAction : public IAction {
  public:
    ToggleTutorialAction(){}
    ~ToggleTutorialAction(){}
    
    void performAction()
    {
      bool tut = COptions::options.getNewbieTips();
      COptions::options.setNewbieTips(!tut);
    }
};

OptionsMenu* OptionsMenu::Instance()
{
  static OptionsMenu instance;
  
  return &instance;
}

OptionsMenu::OptionsMenu()
{
  background_ = NULL;
  logo_ = NULL;
  fullscreen_option_ = NULL;
  tutorial_option_ = NULL;
  checkmark_ = NULL;
  
  toggle_fullscreen_action_ = NULL;
  toggle_tutorial_action_   = NULL;
    
  highlight_ = HIGHLIGHT_NONE;
  
  initGUI();
}

OptionsMenu::~OptionsMenu()
{
  deleteGUI();
  deleteSurfaces();
}

void OptionsMenu::deleteGUI()
{
  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) {
      continue;
    }
    delete b;
  }
  buttons_.clear();
  
  if (toggle_fullscreen_action_) {
    delete toggle_fullscreen_action_;
    toggle_fullscreen_action_ = NULL;
  }
  if (toggle_tutorial_action_) {
    delete toggle_tutorial_action_;
    toggle_tutorial_action_ = NULL;
  }
}

void OptionsMenu::deleteSurfaces()
{
  if (background_) SDL_FreeSurface(background_);
  if (logo_) SDL_FreeSurface(logo_);
  if (fullscreen_option_) SDL_FreeSurface(fullscreen_option_);
  if (tutorial_option_)   SDL_FreeSurface(tutorial_option_);
  if (checkmark_)  SDL_FreeSurface(checkmark_);
  
  background_ = NULL;
  logo_ = NULL;
  fullscreen_option_ = NULL;
  tutorial_option_ = NULL;
  checkmark_ = NULL;
}

void OptionsMenu::initGUI()
{
  if (owner_ == NULL || !buttons_.empty()) return;
  
  toggle_fullscreen_action_   = new ToggleFullscreenAction(owner_);
  toggle_tutorial_action_  = new ToggleTutorialAction();
  
  Rectangle rect(756, 402, 16, 16);
  Button *button = new Button(rect,
      CSurface::OnLoad(data::FindFile("gfx/UI/options/checkbox.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/options/checkbox_2.png").c_str()));
  button->setAction(toggle_fullscreen_action_);
  button->setId(HIGHLIGHT_FULLSCREEN);
  buttons_.push_back(button);
  
  rect.setX(727);
  rect.setY(442);
  button = new Button(rect,
      CSurface::OnLoad(data::FindFile("gfx/UI/options/checkbox.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/options/checkbox_2.png").c_str()));
  button->setAction(toggle_tutorial_action_);
  button->setId(HIGHLIGHT_TUTORIAL);
  buttons_.push_back(button);
  
  updateSelection();
}

void OptionsMenu::initializeSurfaces()
{
  if (!background_) {
    background_ = CSurface::OnLoad(
        data::FindFile("gfx/UI/MENU_BACKGROUND_800x600.png").c_str());
  }
  if (!logo_) {
    logo_ =
        CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_LOGO.png").c_str());
  }
  if (!fullscreen_option_) {
    fullscreen_option_ = CSurface::OnLoad(
        data::FindFile("gfx/UI/options/fullscreen.png").c_str());
  }
  if (!tutorial_option_) {
    tutorial_option_ =
        CSurface::OnLoad(data::FindFile("gfx/UI/options/tutorial.png").c_str());
  }

  if (!checkmark_) {
    checkmark_ =
        CSurface::OnLoad(data::FindFile("gfx/UI/options/check.png").c_str());
  }
}
  
void OptionsMenu::Enter(CApp* app)
{
  owner_ = app;
  initializeSurfaces();
  initGUI();
}

void OptionsMenu::Execute(CApp* app)
{ 
  SDL_Event Event;
  
  while(SDL_PollEvent(&Event))
  {
    Menu::OnEvent(&Event);
  }

  OnRender(app);
}

void OptionsMenu::Exit(CApp* app)
{
  deleteSurfaces();
}

void OptionsMenu::OnRender(CApp* app)
{
  CSurface::OnDraw(app->getDisplay(), background_, 0, 0);
  CSurface::OnDraw(app->getDisplay(), logo_, 0, 0);
  Menu::displayVersion(app->getDisplay());
  
  CSurface::OnDraw(app->getDisplay(), fullscreen_option_, 609, 397);
  CSurface::OnDraw(app->getDisplay(), tutorial_option_, 609, 437);

  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) {
      continue;
    }
    b->paint(app->getDisplay());
  }
  
  if (!COptions::options.getWindowed()) {
    CSurface::OnDraw(app->getDisplay(), checkmark_, 756, 398);
  }
      
  if (COptions::options.getNewbieTips()) {
    CSurface::OnDraw(app->getDisplay(), checkmark_, 727, 438);
  }
      
  SDL_Flip(app->getDisplay());
}

void OptionsMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym)
  {
    case SDLK_RETURN:
    {
      doSelected();
      break;
    }
    case SDLK_DOWN:
    {
      highlight_ = HIGHLIGHT_TUTORIAL;
      updateSelection();
      break;
    }
    case SDLK_UP:
    {
      highlight_ = HIGHLIGHT_FULLSCREEN;
      updateSelection();
      break;
    }
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

void OptionsMenu::OnMouseMove(int mx, int my, int relx, int rely,
    bool Left, bool Right, bool Middle)
{
  highlight_ = HIGHLIGHT_NONE;
  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) {
      continue;
    }
    b->mouseMoved(mx, my);
  }
}

void OptionsMenu::OnLButtonUp(int mx, int my)
{
  highlight_ = HIGHLIGHT_NONE;
  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    b->mouseReleased(mx, my);
  }
}

void OptionsMenu::updateSelection()
{
  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) {
      continue;
    }
    b->keySelected(highlight_);
  }
}

void OptionsMenu::doSelected()
{
  std::list<Button*>::iterator it;
  for (it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    if (b->getId() == highlight_) {
      b->performAction();
      break;
    }
  }
}

void OptionsMenu::goBack()
{
  owner_->getFSM()->RevertToPreviousState();
}
