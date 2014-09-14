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
#include "CHud.h"
#include "CArea.h"
#include "CText.h"
#include "COptions.h"
#include "CSurface.h"
#include "CCamera.h"
#include "CMusic.h"
#include "DOTComponent.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "ItemRenderSystem.h"
#include "LevelLoader.h"
#include "SpaceComponent.h"

bool CApp::OnInit()
{
  COptions::options.LoadOptions();
  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    return false;
  }
  
  if ((Surf_WIcon = SDL_LoadBMP("./gfx/ITD++.bmp")) == NULL)
  {
    return false;
  }
  
  SDL_WM_SetIcon(Surf_WIcon, NULL);
  int ScreenWidth = COptions::options.getScreenWidth();
  int ScreenHeight = COptions::options.getScreenHeight();
  
  Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
  if ((Surf_Display = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 32, flags)) == NULL)
  {
    return false;
  }
  
  if (CText::TextControl.OnLoad() == false)
  {
    return false;
  }
  
  SDL_WM_SetCaption("Into The Dungeon++, v0.9", "ITD++");
  
  if (COptions::options.getWindowed() == false)
  {
    Surf_Display = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  
  if ((Surf_LoadScreen = CSurface::OnLoad("./gfx/UI/MENU_BACKGROUND_800x600.png")) == NULL)
  {
    return false;
  }
  RenderLoading();
  
  Surf_Background = SDL_CreateRGBSurface(SDL_SWSURFACE, ScreenWidth, ScreenHeight, 32, 0, 0, 0, 0);
  /*
  if ((Surf_Background = CSurface::OnLoad("./gfx/Background.png")) == NULL)
  {
    return false;
  }
  */
  
  if (CArea::area_control.OnLoad() == false)
  {
    return false;
  }
  
  if (!(ItemRenderSystem::surf_items = CSurface::OnLoad("./gfx/Items.png")))
  {
    return false;
  }
  
  if (CHud::HUD.OnLoad() == false)
  {
    return false;
  }

  player = LevelLoader::startNewGame();

  //setSurf_Text("Game Loaded");
  
  /*Initialize audio*/
  if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) < 0)
  {
    setSurf_Text("NoSound");
  }
  
  if (((Music = CMusic::OnLoad("./audio/BGM/ITDInGame.ogg")) == NULL) ||
      ((MusicBoss = CMusic::OnLoad("./audio/BGM/ITDBossFight.ogg")) == NULL) ||
        ((MusicIntro = CMusic::OnLoad("./audio/BGM/ITDIntro.ogg")) == NULL))
  {
    setSurf_Text("Unable to open music");
  }
  
  if (!CMusic::SoundControl.LoadSounds())
  {
    setSurf_Text("Unable to open sounds");
  }   
  
  CMusic::SoundControl.Play(MusicIntro, -1);
  
  if ((Surf_TextOverlay = CSurface::OnLoad("./gfx/UI/Text.png")) == NULL) return false;
  
  //SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  SDL_EnableUNICODE(SDL_ENABLE);
  
  return true;
}
