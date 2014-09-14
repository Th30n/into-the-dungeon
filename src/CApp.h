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

#ifndef _CAPP_H_
#define _CAPP_H_

#include <cstdlib>
#include <vector>
#include <ctime>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

#include "CEvent.h"
#include "GameObject.h"
#include "TurnSystem.h"

template <typename T>
class StateMachine;

struct MouseButton {
  bool Pressed;
  int X;
  int Y;
};

class CApp : public CEvent {
  private:
    long OldTime;
    bool IntroScreen; //is Intro Screen
    bool GameWon; //has the player won the game
    bool TargetingOn; //is Targeting Mode on
    bool SpellBookOn; //is SpellBook opened
    bool SpellDetails; //show Spell Details
    bool InventoryOn; //is Inventory opened
    bool ItemDetails; //show Item Details
    bool PickItem; //is Item Picking Mode on
    bool SavedHighscore; //has the game saved highscore upon death
    bool FoVOn; //is Field of View on
    bool FoWCalculated; //has the FoW been calculated (optimization issues);
    bool MiniMapOn;     //is Mini-Map on
    
    // Index of currently selected item/spell
    unsigned int WhichItem;
    unsigned int WhichSpell;
    
    bool HelpScreen; //is HelpScreen on
    bool DebugModeOn; //is DebugMode on
    bool MusicOn; //is Music turned on
    bool CharacterSheetOn;
    
    MouseButton LMB;
    
    StateMachine<CApp> *stateMachine;

    GameObject player;
    TurnSystem turnSystem;

    bool Running; //is the application Running
    SDL_Surface *Surf_Display; //main Display Surface
    SDL_Surface *Surf_Background; //Surface with Background image
    SDL_Surface *Surf_LoadScreen; //Surface with loading screen image
    SDL_Surface *Surf_Text; //Surface for Text output
    //Surface containing numbers and text for text overlay
    SDL_Surface *Surf_TextOverlay;
    SDL_Surface *Surf_WIcon;

  public: // bad hack, music system needs to change
    static Mix_Music *Music; //Background Game Music
    static Mix_Music *MusicBoss; //BGM for Boss fight
    static Mix_Music *MusicIntro; //BGM for Menu/Intro

  public:
    CApp();
    ~CApp();
    int OnExecute(); //Execution method
    void setSurf_Text(const char *newText);
    
    SDL_Surface *getDisplay() const {return Surf_Display;}
    StateMachine<CApp> *getFSM() const {return stateMachine;}
    
    void setRunning(bool val) {Running = val;}
    bool OnInit(); //Initialization method
    void OnEvent(SDL_Event *Event); //Event menagement method
    void OnExit(); //upon Exit method
    //Key Press management method
    void newOnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    //Key Release management method
    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnLButtonDown(int mX, int mY); //Method when LMB is pressed
    void OnLButtonUp(int mX, int my); //Method when LMB is released
    void OnRButtonDown(int mX, int mY); //Method when RMB is pressed
    //Method for handling mouse movement
    void OnMouseMove(int mX, int mY, int relX, int relY,
              bool Left, bool Right, bool Middle);
    void OnLoop(); //Main Loop method
    void OnRender(); //Rendering method
    void OnCleanup(); //Cleanup method (when object ceases to exist)
    void RenderLoading(); //Method that shows the loading screen (in CApp)

    //Following methods are implemented in CApp_OnEvent.cpp
    void MovePlayerWithMouse(int dX, int dY);
    bool isMouseOnPlayer(int mX, int mY);
    void Descend(); //Player Descends down the ladder method
    void Restart(); //Player Restarts the game method
    bool ItemPickup(); //Method for Picking Up Items (items -> inventory)
    bool ScrollPickup(); //Method for Picking Up Scrolls (scrolls -> spellbook)
    bool ReadScroll(); //When Scrolls is Read method
    bool QuaffPotion(); //Method for quick Potion Quaffing
    //Method for Selecting Target in Targeting Mode
    void SelectTarget(int TargetID);
    bool LoadGame(); //Method for Loading the Game
    bool SaveGame(); //Method for Saving the Game
    bool SaveEntities(FILE *file);
    bool LoadEntities(FILE *file);
    bool SaveItems(FILE *file);
    bool LoadItems(FILE *file);
    bool SaveDOTs(FILE *file);
    bool LoadDOTs(FILE *file);
    bool SaveSpells(FILE *file);
    bool LoadSpells(FILE *file);
    bool SaveAOEs(FILE *file);
    bool LoadAOEs(FILE *file);
    bool SaveTraps(FILE *file);
    bool LoadTraps(FILE *file);
    void enchantItem();
    void setTrap(int index);
    void RenderTip();
    void toggleFullscreen();
    
    //Following methods are implemented in CApp_OnLoop.cpp
    void SaveHighscore(); //Method for Saving the Highscore
    
    //Implemented in CApp.cpp
    void exitWithError(std::string Error);
    
    //Implemented in CApp_OnRender.cpp
    void displayNewbieTips(SDL_Surface *dst, int turn, float hpPerc,
                int invSize, int bookSize, bool isOnItem,
                bool isOnStairs, bool invOpen, bool bookOpen);
};

#endif
