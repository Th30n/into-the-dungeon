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

#include "AnimationComponent.h"
#include "CArea.h"
#include "CCamera.h"
#include "CFPS.h"
#include "CHud.h"
#include "CMap.h"
#include "ControlSystem.h"
#include "COptions.h"
#include "CSurface.h"
#include "CText.h"
#include "data/save.hpp"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "LevelLoader.h"
#include "SpaceComponent.h"

void CApp::OnEvent(SDL_Event *Event)
{
  CEvent::OnEvent(Event);
}

void CApp::OnExit()
{
  Running = false;
}

void CApp::OnRButtonDown(int mX, int mY)
{
}

void CApp::OnLButtonDown(int mX, int mY)
{
  LMB.Pressed = true;
  LMB.X = mX;
  LMB.Y = mY;
  
  //Not needed -> implemented in CPlayer::MovePlayer
  //if (CTime::TimeControl.Turn != PLAYER_TURN || Player.TurnTaken || Player.attacking || Player.Dead) return;
  
  //int pX = Player.space->getPosX() - CCamera::camera_control.GetX() + TILE_SIZE/2; //Player center coord X on screen (not on map)
  //int pY = Player.space->getPosY() - CCamera::camera_control.GetY() + TILE_SIZE/2; //Player center coor Y on screen
  
  //int dX = mX - pX; //difference between mouse X coord and player X
  //int dY = mY - pY; //difference between mouse Y coord and player Y

//  if (isMouseOnPlayer(mX, mY))
//  {
//    if (ItemPickup() == false && !PickItem)
//    {
//      if (ScrollPickup() == false && CArea::area_control.GetTile(Player.space->getPosX(), Player.space->getPosY())->layer_id == LAYER_ID_SKELETON)
//        RenderTip();
//    }
//    LMB.Pressed = false;
//  }
//  else MovePlayerWithMouse(dX, dY);
}

bool CApp::isMouseOnPlayer(int mX, int mY)
{
  bool Return = false;
  
  //int pX = Player.space->getPosX() - CCamera::camera_control.GetX();
  //int pY = Player.space->getPosY() - CCamera::camera_control.GetY();
  
  //if (mX >= pX && mX <= pX + TILE_SIZE && mY >= pY && mY <= pY + TILE_SIZE) Return = true;
  
  return Return;
}
void CApp::MovePlayerWithMouse(int dX, int dY)
{
}

void CApp::OnLButtonUp(int mX, int my)
{
  LMB.Pressed = false;
}

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
  LMB.X = mX;
  LMB.Y = mY;
}

void CApp::newOnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  EntityManager &em = EntityManager::instance();
  if (sym == SDLK_q && (mod & KMOD_CTRL)) {
    if (CText::TextControl.displayPrompt(Surf_Display,
        "Do you want to quit? y/n", 80, 20)) {
      Running = false;
      return;
    }
  } else if (sym == SDLK_RETURN) {
    HealthComponent *hp = em.getComponentForEntity<HealthComponent>(player);
    if (hp->is_dead || CArea::area_control.dungeon_level == 20) {
      player = LevelLoader::startNewGame();
      return;
    }
  } else if (sym == SDLK_l && (mod & KMOD_SHIFT)) {
    if (CText::TextControl.displayPrompt(Surf_Display,
        "Do you want to load the game? y/n", 80, 20)) {
      LevelLoader::LoadGame();
    }
  } else if (sym == SDLK_s && (mod & KMOD_SHIFT)) {
    if (CText::TextControl.displayPrompt(Surf_Display,
        "Do you want to save the game? y/n", 80, 20)) {
      data::SaveGame(em);
    }
  }
  if (CHud::HUD.onKeyDown(sym, mod, unicode)) {
    return;
  }
  ControlSystem cs(em, player);
  cs.start();
  switch (sym) {
  case SDLK_LEFT:
    cs.update(ControlSystem::LEFT);
    break;
  case SDLK_RIGHT:
    cs.update(ControlSystem::RIGHT);
    break;
  case SDLK_UP:
    cs.update(ControlSystem::UP);
    break;
  case SDLK_DOWN:
    cs.update(ControlSystem::DOWN);
    break;
  case SDLK_RETURN:
    cs.update(ControlSystem::PICK_UP_ITEM);
    break;
  case SDLK_SPACE:
    cs.update(ControlSystem::PAUSE_TURN);
    break;
  case SDLK_F10: //turns Fog of War on/off
    CArea::area_control.GetMap(0,0)->fow_on = !CArea::area_control.GetMap(0,0)->fow_on;
    break;
  case SDLK_d:
    cs.update(ControlSystem::DESCEND);
    break;
  default:
    break;
  }
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  return newOnKeyDown(sym, mod, unicode);
}

void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym)
  {
    case SDLK_LEFT:
    {
      //Player.MoveLeft = false;
      break;
    }
    case SDLK_RIGHT:
    {
      //Player.MoveRight = false;
      break;
    }
    case SDLK_UP:
    {
      //Player.MoveUp = false;
      break;
    }
    case SDLK_DOWN:
    {
      //Player.MoveDown = false;
      break;
    }
    default:
    {
    }
  }
}

bool CApp::ItemPickup()
{
  return false;
}

bool CApp::ScrollPickup()
{
  return false;
}

bool CApp::QuaffPotion()
{
  bool Return = false;
  return Return;
}

bool CApp::SaveGame()
{
  return true;
}
bool CApp::LoadGame()
{
  return true;
}

bool CApp::SaveEntities(FILE *file)
{
  return true;
}

bool CApp::LoadEntities(FILE *file)
{
  return true;
}

bool CApp::SaveItems(FILE *file)
{
  return true;
}

bool CApp::LoadItems(FILE *file)
{
  return true;
}

bool CApp::SaveDOTs(FILE *file)
{
  return true;
}

bool CApp::LoadDOTs(FILE *file)
{
  return true;
}

bool CApp::SaveSpells(FILE *file)
{
  return true;
}

bool CApp::LoadSpells(FILE *file)
{
  return true;
}

bool CApp::SaveAOEs(FILE *file)
{
  return true;
}

bool CApp::LoadAOEs(FILE *file)
{
  return true;
}

bool CApp::SaveTraps(FILE *file)
{
  return true;
}

bool CApp::LoadTraps(FILE *file)
{
  return true;
}

void CApp::enchantItem()
{
}

void CApp::setTrap(int index)
{
}

void CApp::RenderTip()
{
  SDL_Surface *Surf_Icon = NULL;
  
  //Get the skeleton icon from tileset
  Surf_Icon = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 32, 0, 0, 0, 0);
  CSurface::OnDraw(Surf_Icon, CArea::area_control.surf_tileset, 0, 0, 32 * 6, 32 * 4, 32, 32);
  
  int MaxTips = 12;
  int Tip = 0;
  std::string Text;
  std::string SpacesFromIcon = "          "; //10 spaces to get the text away from icon
  Text = SpacesFromIcon;
  Tip = rand() % MaxTips;
  switch(Tip)
  {
    case 0:
    {
      Text += "I'm telling you this only because Rarefied Horse Meat made me do it!\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 1:
    {
      Text += "Dead people can't share tips... Perhaps dead souls can?\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 2:
    {
      Text += "My candle burned out thus corruption got me.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 3:
    {
      Text += "Beware of the cursed items! You cannot unequip those.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 4:
    {
      Text += "The Undead are immune to poison and your LifeSteal attacks don't work on them.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 5:
    {
      Text += "Magical poisoning can go through poison immunity.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 6:
    {
      Text += "Certain Skeleton Monsters have poisonous tips on their blades.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 7:
    {
      Text += "Some extremely powerful creatures can absorb Chain Lightning Spell. They still take damage but it makes Chaining stop.\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 8:
    {
      Text += "When I was alive I drank a lot of potions. Unfortunately, one turned out to be cursed... and here I am...\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 9:
    {
      Text += "Have you seen ThunderBirds? I heard their attacks can shock you in place!\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 10:
    {
      Text += "I once hid from the monsters in a cardboard box... Alas, I lost my box in this dungeon!\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    case 11:
    {
      Text += "There are riches here no man has seen before! Though, they have a low chance of spawning...\n" + SpacesFromIcon + "~Enter~";
      break;
    }
    default:
    {
      Text += "Something bad happened with hints & tips generating!\n" + SpacesFromIcon + "~Enter~";
      break;
    }
  }
  CText::TextControl.displayDialogue(Surf_Display, Surf_Icon, Text);
  
  if (Surf_Icon) SDL_FreeSurface(Surf_Icon);
}

void CApp::toggleFullscreen()
{
  bool Windowed = COptions::options.getWindowed();
  COptions::options.setWindowed(!Windowed);
  int Width = COptions::options.getScreenWidth();
  int Height = COptions::options.getScreenHeight();
  if (Windowed)
  {
    if (Surf_Display) SDL_FreeSurface(Surf_Display);
    Surf_Display = SDL_SetVideoMode(Width, Height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  else
  {
    if (Surf_Display) SDL_FreeSurface(Surf_Display);
    Surf_Display = SDL_SetVideoMode(Width, Height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  }
}
