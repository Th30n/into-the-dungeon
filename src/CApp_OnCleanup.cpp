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
#include "CArea.h"
#include "CHud.h"
#include "CText.h"
#include "CMusic.h"
#include "ItemRenderSystem.h"

void CApp::OnCleanup()
{
  SDL_EnableUNICODE(SDL_DISABLE);

  if (Mix_PlayingMusic()) Mix_HaltMusic();
  Mix_FreeMusic(Music);
  Mix_FreeMusic(MusicBoss);
  Mix_FreeMusic(MusicIntro);
  
  //revert to original window when exiting
  //if (Windowed) Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
  CArea::area_control.OnCleanup();
  
  //if (boss) delete(boss);
  
  CHud::HUD.OnCleanup();
  
  if (ItemRenderSystem::surf_items) {
    SDL_FreeSurface(ItemRenderSystem::surf_items);
  }
  SDL_FreeSurface(Surf_Background);
  SDL_FreeSurface(Surf_Display);
  if (Surf_Text) {
    SDL_FreeSurface(Surf_Text);
    Surf_Text = NULL;
  }
  SDL_FreeSurface(Surf_WIcon);
  if (Surf_LoadScreen) SDL_FreeSurface(Surf_LoadScreen);
  if (Surf_TextOverlay) SDL_FreeSurface(Surf_TextOverlay);
  
  CText::TextControl.OnCleanup();
  
  CMusic::SoundControl.OnCleanup();
  Mix_CloseAudio();
  SDL_Quit();
}
