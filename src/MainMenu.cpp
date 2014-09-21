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
#include "CSurface.h"
#include "CText.h"
#include "data/dirs.h"
#include "Rectangle.h"
#include "Button.h"
#include "IAction.h"
#include "StateMachine.h"

/*
 * This is an action for New Game button
 * Author: Teon Banek
 */
class NewGameAction : public IAction {
  public:
    NewGameAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~NewGameAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->getFSM()->ChangeState(NewGameMenu::Instance());
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for Load Game button
 */
class LoadGameAction : public IAction {
  public:
    LoadGameAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~LoadGameAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->LoadGame();
        owner_->getFSM()->ChangeState(Gameplay::Instance());
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for Highscore button
 */
class HighscoreAction : public IAction {
  public:
  
    HighscoreAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~HighscoreAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->getFSM()->ChangeState(HighscoreMenu::Instance());
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for Exit Game button
 */
class ExitGameAction : public IAction {
  public:
    ExitGameAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~ExitGameAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->setRunning(false);
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for options button
 */
class OptionsAction : public IAction {
  public:
    OptionsAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~OptionsAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->getFSM()->ChangeState(OptionsMenu::Instance());
      }
    }
  private:
    CApp *owner_;
};

/*
 * This is an action for Credits button
 */
class CreditsAction : public IAction {
  public:
    CreditsAction(CApp *owner)
    {
      this->owner_ = owner;
    }
    
    virtual ~CreditsAction(){}
    
    virtual void performAction()
    {
      if (owner_) {
        owner_->getFSM()->ChangeState(CreditsMenu::Instance());
      }
    }
  private:
    CApp *owner_;
};

enum
{
  BUTTON_NONE = 0,
  BUTTON_NEW_GAME,
  BUTTON_LOAD_GAME,
  BUTTON_HIGHSCORE,
  BUTTON_EXIT_GAME,
  BUTTON_OPTIONS,
  BUTTON_CREDITS
};

MainMenu* MainMenu::Instance()
{
  static MainMenu instance;
  
  return &instance;
}

MainMenu::MainMenu()
{
  background_    = NULL;
  logo_          = NULL;
  
  new_game_action_   = NULL;
  load_game_action_  = NULL;
  highscore_action_ = NULL;
  exit_game_action_  = NULL;
  options_action_   = NULL;
  credits_action_   = NULL;
    
  selection_ = BUTTON_NEW_GAME;
  
  initGUI();
}

MainMenu::~MainMenu()
{
  deleteGUI();
  deleteSurfaces();
}

void MainMenu::deleteGUI()
{
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    delete b;
  }
  buttons_.clear();
  
  if (new_game_action_) {
    delete new_game_action_;
    new_game_action_ = NULL;
  }
  if (load_game_action_) {
    delete load_game_action_;
    load_game_action_ = NULL;
  }
  if (highscore_action_) {
    delete highscore_action_;
    highscore_action_ = NULL;
  }
  if (exit_game_action_) {
    delete exit_game_action_;
    exit_game_action_ = NULL;
  }
  if (options_action_) {
    delete options_action_;
    options_action_ = NULL;
  }
  if (credits_action_) {
    delete credits_action_;
    credits_action_ = NULL;
  }
}

void MainMenu::deleteSurfaces()
{ 
  if (background_)    SDL_FreeSurface(background_);
  if (logo_)          SDL_FreeSurface(logo_);
  
  background_    = NULL;
  logo_          = NULL;
}

void MainMenu::initGUI()
{
  if (owner_ == NULL || !buttons_.empty()) return;
  
  new_game_action_   = new NewGameAction(owner_);
  load_game_action_  = new LoadGameAction(owner_);
  highscore_action_ = new HighscoreAction(owner_);
  exit_game_action_  = new ExitGameAction(owner_);
  options_action_   = new OptionsAction(owner_);
  credits_action_   = new CreditsAction(owner_);
  
  Rectangle rect(497, 381, 286, 42);
  Button *button = new Button(rect,
      CSurface::OnLoad(data::FindFile("gfx/UI/new_game_button.png").c_str()));
  Rectangle r(484, 373, 311, 57);
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_BIG.png").c_str()), r);
  button->setAction(new_game_action_);
  button->setId(BUTTON_NEW_GAME);
  buttons_.push_back(button);
  
  rect.setX(449);
  rect.setY(437);
  r.setX(436);
  r.setY(429);
  button = new Button(rect, CSurface::OnLoad(data::FindFile("gfx/UI/load_game_button.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_BIG.png").c_str()), r);
  //button->setAction(load_game_action_);
  button->setId(BUTTON_LOAD_GAME);
  buttons_.push_back(button);
  
  rect.setX(401);
  rect.setY(493);
  r.setX(388);
  r.setY(485);
  button = new Button(rect, CSurface::OnLoad(data::FindFile("gfx/UI/highscore_button.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_BIG.png").c_str()), r); 
  //button->setAction(highscore_action_);
  button->setId(BUTTON_HIGHSCORE);
  buttons_.push_back(button);
  
  rect.setX(353);
  rect.setY(549);
  r.setX(340);
  r.setY(541);
  button = new Button(rect, CSurface::OnLoad(data::FindFile("gfx/UI/exit_game_button.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_BIG.png").c_str()), r);
  button->setAction(exit_game_action_);
  button->setId(BUTTON_EXIT_GAME);
  buttons_.push_back(button);
  
  rect.setWidth(57);
  rect.setHeight(33);
  rect.setX(733);
  rect.setY(518);
  r.setWidth(71);
  r.setHeight(41);
  r.setX(726);
  r.setY(514);
  button = new Button(rect, CSurface::OnLoad(data::FindFile("gfx/UI/options_button.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_SMALL.png").c_str()), r);
  button->setAction(options_action_);
  button->setId(BUTTON_OPTIONS);
  buttons_.push_back(button);
  
  rect.setX(707);
  rect.setY(558);
  r.setX(700);
  r.setY(554);
  button = new Button(rect, CSurface::OnLoad(data::FindFile("gfx/UI/credits_button.png").c_str()));
  button->setHighlightImage(
      CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_SELECTOR_SMALL.png").c_str()), r);
  button->setAction(credits_action_);
  button->setId(BUTTON_CREDITS);
  buttons_.push_back(button);
  
  updateSelection();
}

void MainMenu::initializeSurfaces()
{
  if (background_ == NULL)
    background_ = CSurface::OnLoad(data::FindFile("gfx/UI/MENU_BACKGROUND_800x600.png").c_str());
  if (logo_ == NULL)
    logo_ = CSurface::OnLoad(data::FindFile("gfx/UI/ITD++_MENU_LOGO.png").c_str());
}
  
void MainMenu::Enter(CApp* app)
{
  owner_ = app;
  initializeSurfaces();
  initGUI();
}

void MainMenu::Execute(CApp* app)
{
  SDL_Event Event;
  
  while(SDL_PollEvent(&Event))
  {
    Menu::OnEvent(&Event);
  }

  OnRender(app);
}

void MainMenu::Exit(CApp* app)
{
  deleteSurfaces();
}

void MainMenu::OnRender(CApp* app)
{

  CSurface::OnDraw(app->getDisplay(), background_, 0, 0);
  CSurface::OnDraw(app->getDisplay(), logo_, 0, 0);
  
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    b->paint(app->getDisplay());
  }
  
  Menu::displayVersion(app->getDisplay());
  
  SDL_Flip(app->getDisplay());
}

void MainMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym)
  {
    case SDLK_RETURN:
    {
      doSelected();
      break;
    }
    case SDLK_UP:
    {
      if (selection_ > 4)
        selection_ = BUTTON_OPTIONS;
      else
        selection_--;
      if (selection_ < 1)
        selection_ = BUTTON_NEW_GAME;
      updateSelection();
      break;
    }
    case SDLK_DOWN:
    {
      if (selection_ > 4)
        selection_ = BUTTON_CREDITS;
      else {
        selection_++;
        if (selection_ > 4)
          selection_ = 4;
      }
      updateSelection();
      break;
    }
    case SDLK_LEFT:
    {
      selection_ = BUTTON_NEW_GAME;
      updateSelection();
      break;
    }
    case SDLK_RIGHT:
    {
      selection_ = BUTTON_OPTIONS;
      updateSelection();
      break;
    }
    default:
    {
      break;
    }
  }
}

void MainMenu::OnMouseMove(int mx, int my, int relx, int rely,
        bool left, bool right, bool middle)
{
  selection_ = BUTTON_NONE;
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    b->mouseMoved(mx, my);
  }
}

void MainMenu::OnLButtonUp(int mx, int my)
{
  selection_ = BUTTON_NONE;
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    b->mouseReleased(mx, my);
  }
}

void MainMenu::updateSelection()
{
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    b->keySelected(selection_);
  }
}

void MainMenu::doSelected()
{
  std::list<Button*>::iterator it;
  for(it = buttons_.begin(); it != buttons_.end(); it++) {
    Button *b = *it;
    if (b == NULL) continue;
    if (b->getId() == selection_) {
      b->performAction();
      break;
    }
  }
}
