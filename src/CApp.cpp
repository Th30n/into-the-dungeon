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
#include "CApp.h"

#include <iostream>

#include "StateMachine.h"
#include "GameStates.h"
#include "CText.h"
#include "CSurface.h"

Mix_Music *CApp::Music = NULL;
Mix_Music *CApp::MusicBoss = NULL;
Mix_Music *CApp::MusicIntro = NULL;

CApp::CApp()
  : player(0)
{
  IntroScreen = true;
  GameWon = false;
  TargetingOn = false;
  SpellBookOn = false;
  SpellDetails = false;
  InventoryOn = false;
  ItemDetails = false;
  PickItem = false;
  SavedHighscore = false;
  FoVOn = false;
  FoWCalculated = false;
  MiniMapOn = true;
  HelpScreen = false;
  DebugModeOn = true;
  MusicOn = true;
  CharacterSheetOn = true;
  
  LMB.Pressed = false;
  LMB.X = 0;
  LMB.Y = 0;
  
  WhichItem = 0;
  WhichSpell = 0;
  
  OldTime = 0;
  
  Surf_Display     = NULL;
  Surf_Background  = NULL;
  Surf_LoadScreen  = NULL;
  Surf_Text        = NULL;
  Surf_TextOverlay = NULL;
  Surf_WIcon       = NULL;
  
  Running = true;
  srand((unsigned int) time(NULL));
  
  stateMachine = new StateMachine<CApp>(this);
  stateMachine->SetCurrentState(Gameplay::Instance());
}

CApp::~CApp()
{
  delete stateMachine;
}

int CApp::OnExecute()
{
  if (OnInit() == false)
  {
    return -1;
  }
  stateMachine->ChangeState(MainMenu::Instance());

  while(Running)
  {
    stateMachine->Update();
  }
  
  OnCleanup();
  
  return 0;
}

void CApp::setSurf_Text(const char *newText)
{
  if (Surf_Text)  {
    SDL_FreeSurface(Surf_Text);
    Surf_Text = NULL;
  }
  int width = 0;
  int height = 0;
  CText::TextControl.getTextDimensions(newText, FONT_SMALL, &width, &height);
  Surf_Text = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  CText::TextControl.displayBMPText(Surf_Text, 0, 0, newText, 255, 255, 255, 0, 0, 0, FONT_SMALL, 0);
}

void CApp::RenderLoading()
{
  if (Surf_LoadScreen)
  {
    CSurface::OnDraw(Surf_Display, Surf_LoadScreen, 0, 0);
    SDL_Flip(Surf_Display);
  }
}

void CApp::exitWithError(std::string Error)
{
}

int main(int argc, char *argv[])
{
  std::cout << argv[0] << std::endl;

  CApp MyRoguelike;
  
  return MyRoguelike.OnExecute();
}
