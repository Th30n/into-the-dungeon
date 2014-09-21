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
#ifndef GAME_STATE_H
#define GAME_STATE_H

/*
 * All states for the CApp (the game)
 */

#include <SDL.h>
#include <string>
#include <list>

#include "State.h"
#include "CEvent.h"

class CApp;
class Button;
class IAction;

/*
 * This state controls the gameplay
 */
class Gameplay : public State<CApp>, public CEvent {
  public:
    //this is a singleton
    static Gameplay* Instance();
      
    virtual ~Gameplay();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
    
    //Event menagement method
    virtual void OnEvent(SDL_Event *event);

  private:
    Gameplay();
    //copy ctor and assignment should be private
    Gameplay(const Gameplay&);
    Gameplay& operator=(const Gameplay&);
};

/*
 * Base state for menus
 */
class Menu : public State<CApp>, public CEvent {
  public:
      
    virtual ~Menu(){}
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app)=0;
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app)=0;
    
    //rendering method
    virtual void OnRender(CApp* app)=0;
    
    //Event menagement method
    virtual void OnEvent(SDL_Event *event);
  
  protected:
    CApp* owner_;
    void displayVersion(SDL_Surface* dst);
    //when exit event occurs
    virtual void OnExit();
};

/*
 * This state controls the main menu
 */
class MainMenu : public Menu {
  public:
    //this is a singleton
    static MainMenu* Instance();
      
    virtual ~MainMenu();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
  
  private:
    MainMenu();
    //copy ctor and assignment should be private
    MainMenu(const MainMenu&);
    MainMenu& operator=(const MainMenu&);

    //Sets up the components
    void initGUI();
    void deleteGUI();
    
    //loads the required images
    void initializeSurfaces();
    void deleteSurfaces();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnMouseMove(int mx, int my, int relx, int rely,
            bool left, bool right, bool middle);
    void OnLButtonUp(int mx, int my);
    
    // Sets highlights when using keyboard to press buttons
    void updateSelection();
    // Presses the selected button using keyboard
    void doSelected();

    //Surfaces for main menu
    SDL_Surface *background_;
    SDL_Surface *logo_;
    
    IAction *new_game_action_;
    IAction *load_game_action_;
    IAction *highscore_action_;
    IAction *exit_game_action_;
    IAction *options_action_;
    IAction *credits_action_;
    
    std::list<Button*> buttons_;
    
    //which button is selected
    long selection_;
};

/*
 * This state controls the new game menu
 */
class NewGameMenu : public Menu {
  public:
      //this is a singleton
      static NewGameMenu* Instance();
      
    virtual ~NewGameMenu();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
  
  private:
    NewGameMenu();
    //copy ctor and assignment should be private
    NewGameMenu(const NewGameMenu&);
    NewGameMenu& operator=(const NewGameMenu&);

    //loads the required images
    void initializeSurfaces();
    void deleteSurfaces();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    
    void startGame();
    void goBack();

    //Surfaces for new game menu
    SDL_Surface *background_;
    SDL_Surface *logo_;
    
    //Name of the player
    std::string name_;
};

/*
 * This state controls the highscore menu
 */
class HighscoreMenu : public Menu {
  public:
      //this is a singleton
      static HighscoreMenu* Instance();
      
    virtual ~HighscoreMenu();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
  
  private:
    //Surfaces for new game menu
    SDL_Surface *background_;
    
    int high_score_page_;  //which HighScore Page is displayed
    
    HighscoreMenu();
    //copy ctor and assignment should be private
    HighscoreMenu(const HighscoreMenu&);
    HighscoreMenu& operator=(const HighscoreMenu&);

    //loads the required images
    void initializeSurfaces();
    void deleteSurfaces();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

    void HighscoreList(SDL_Surface *dst);
    void goBack();
};

class CreditsMenu : public Menu {
  public:
    //this is a singleton
    static CreditsMenu* Instance();
      
    virtual ~CreditsMenu();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
  
  private:
    //Surfaces for credits menu
    SDL_Surface *background_;
    SDL_Surface *logo_;
    
    CreditsMenu();
    //copy ctor and assignment should be private
    CreditsMenu(const CreditsMenu&);
    CreditsMenu& operator=(const CreditsMenu&);

    //loads the required images
    void initializeSurfaces();
    void deleteSurfaces();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    
    void goBack();
};

class OptionsMenu : public Menu {
  public:
      //this is a singleton
      static OptionsMenu* Instance();
      
    virtual ~OptionsMenu();
  
    //this will execute when the state is entered
    virtual void Enter(CApp* app);
  
    //this is the states normal update function
    virtual void Execute(CApp* app);
  
    //this will execute when the state is exited. 
    virtual void Exit(CApp* app);
    
    //rendering method
    virtual void OnRender(CApp* app);
  
  private:
    enum
    {
      HIGHLIGHT_NONE = 0,
      HIGHLIGHT_FULLSCREEN,
      HIGHLIGHT_TUTORIAL
    };

    OptionsMenu();
    //copy ctor and assignment should be private
    OptionsMenu(const OptionsMenu&);
    OptionsMenu& operator=(const OptionsMenu&);
    
    //Sets up the components
    void initGUI();
    void deleteGUI();
    
    //loads the required images
    void initializeSurfaces();
    void deleteSurfaces();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnMouseMove(int mx, int my, int relx, int rely, bool Left, bool Right, bool Middle);
    void OnLButtonUp(int mx, int my);
    
    // Sets highlights when using keyboard to press buttons
    void updateSelection();
    // Presses the selected button using keyboard
    void doSelected();
    
    void goBack();

    //Surfaces for credits menu
    SDL_Surface *background_;
    SDL_Surface *logo_;
    SDL_Surface *fullscreen_option_;
    SDL_Surface *tutorial_option_;
    SDL_Surface *checkmark_;
  
    int highlight_; //which option is highlighted

    IAction *toggle_fullscreen_action_;
    IAction *toggle_tutorial_action_;
    
    std::list<Button*> buttons_;
};

#endif
