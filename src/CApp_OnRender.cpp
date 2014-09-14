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

#include "AnimationComponent.h"
#include "CArea.h"
#include "CHud.h"
#include "CText.h"
#include "COptions.h"
#include "CSurface.h"
#include "CCamera.h"
#include "CTile.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "HealthSystem.h"
#include "ItemRenderSystem.h"
#include "RenderSystem.h"
#include "SpaceComponent.h"
#include "Visibility.h"

/*
 * Rendering the whole screen
 * Levels of render go from bottom to top
 */
void CApp::OnRender()
{
  //bool isOnItem = false;
  //bool isOnStairs = false;
  //if (CArea::area_control.GetTile(
  //    Player.space->getPosX(),
  //    Player.space->getPosY())->tile_id == TILE_ID_LADDER)
  //  isOnStairs = true;
  
  CSurface::OnDraw(Surf_Display, Surf_Background, 0, 0);

  CArea::area_control.OnRender(Surf_Display,
      -CCamera::camera_control.GetX(), -CCamera::camera_control.GetY());

  //
  //if (CharacterSheetOn) {
  //  CHud::HUD.renderCharacterSheet(Surf_Display);
  //}
  //if (InventoryOn) {
  //  CHud::HUD.renderNewInventory(Surf_Display);
  //}
  //if (InventoryOn && ItemDetails) {
  //  CHud::HUD.ItemDetails(Surf_Display, WhichItem);
  //}
  //
  //if (SpellBookOn) { //shows the spellbook if opened
  //  CHud::HUD.renderNewSpellBook(Surf_Display);
  //}
  //
  //if (MiniMapOn && !InventoryOn) {
  //  CArea::area_control.MiniMapOnRender(Surf_Display,
  //      CEntity::EntityList[0]->space->getPosX() / TILE_SIZE,
  //      CEntity::EntityList[0]->space->getPosY() / TILE_SIZE);
  //}
  //
  //if (TargetingOn) {
  //  CTarget::Targeting.PossibleTargets(CEntity::EntityList[0], Surf_Display);
  //  CText::TextControl.displayMessage(Surf_Display, "Select your target by pressing the number associated with a possible target.", 0,  0);
  //}
  //
  //if (PickItem) {
  //  CText::TextControl.displayMessage(Surf_Display, "Select the item on which you want to use the scroll.", 0,  0);
  //}
  //
  //if (CEntity::EntityList[0]->Dead &&
  //    CEntity::EntityList[0]->animation->getDeathTime() > 0 &&
  //    CEntity::EntityList[0]->animation->getDeathTime() + 1000 <= SDL_GetTicks()) {
  //  CHud::HUD.DisplayDeathScreen(Surf_Display);
  //}
  //
  //if (GameWon) {
  //  CHud::HUD.DisplayWinScreen(Surf_Display);
  //  InventoryOn = false;
  //  MiniMapOn = false;
  //  SpellBookOn = false;
  //}
  //
  //if (FoVOn) {
  //  CHud::HUD.DisplayFoV(Surf_Display);
  //  EntityManager &em = EntityManager::instance();
  //    SpaceComponent *sc = em.getComponentForEntity<SpaceComponent>(player);
  //    Visibility::drawRay(sc->pos.x * TILE_SIZE, sc->pos.y * TILE_SIZE,
  //          LMB.X + CCamera::camera_control.GetX(),
  //          LMB.Y + CCamera::camera_control.GetY(), Surf_Display);
  //}
  //if (Surf_Text) {
  //  CSurface::OnDraw(Surf_Display, Surf_Text, 0, 0);
  //}
  //if (HelpScreen) {
  //  CHud::HUD.RenderHelpScreen(Surf_Display);
  //}
  //
  //for (unsigned i = 0; i < CEntity::EntityList.size(); i++) {
  //  CEntity::EntityList[i]->renderTextOverlay(Surf_Display, Surf_TextOverlay);
  //}
  //
  ///*
  // * Part where tips for beginners are rendering
  // */
  //if (Player.dungeon_level > 2 && COptions::options.getNewbieTips()) {
  //  COptions::options.setNewbieTips(false);
  //}

  ItemRenderSystem irs;
  irs.render(Surf_Display);
  RenderSystem rs;
  rs.render(Surf_Display);
  HealthSystem hps("./gfx/UI/Text.png", "./gfx/Status.png");
  hps.start();
  hps.render(Surf_Display);
  CHud::HUD.OnRender(Surf_Display);
  if (DebugModeOn) {
    CHud::HUD.DebugMode(Surf_Display);
  }

  SDL_Flip(Surf_Display);
}

void CApp::displayNewbieTips(SDL_Surface *dst, int turn, float hpPerc, int invSize, int bookSize, bool isOnItem, bool isOnStairs, bool invOpen, bool bookOpen)
{
  static int firstTime = 1;
  std::string text;
  int x = COptions::options.getScreenWidth() / 5;
  int y = COptions::options.getScreenHeight() / 5;
  if (turn == 0 && firstTime == 1)
  {
    text = "Welcome to Into The Dungeon++!\n\nTo move your character press arrow key\nor left mouse button in the direction\nyou want to move.\n\nSkip turn with Space or right mouse button.\n\nTo attack a monster just 'walk' into it.\n\nPress Shift + S to Save the game,\nor Shift + L to Load.\nIf you want to quit press CTRL + Q\n\n Press Enter to continue.";
    
    if (CText::TextControl.displayPrompt(dst, text, x, y))
      firstTime++;
    else
      firstTime = 0;
  }
  else if (turn == 0 && firstTime == 2)
  {
    text = "In the lower left you can see your health.\n\nNext to your health globe is a candle,\nit represents how much time you have\nleft for this dungeon level.\n\nBelow it, is the book which tracks the\nnumber of magical scrolls you have found.\n\nOn the right side of health globe\nis a potion that tells you how many\npotions you have in inventory.\n\n Press Enter to continue.";
    
    if (CText::TextControl.displayPrompt(dst, text, x, y))
      firstTime++;
    else
      firstTime = 0;
  }
  else if (turn == 0 && firstTime == 3)
  {
    text = "In the right upper corner you can see\nyour stats.\nThat window is turned on/off with 'c'.\n\nIn the left upper corner is the minimap.\nTo turn it off press TAB\n green - your hero\n blue - stairs to next level\n white - floor\n brown - door\n\n Press Enter to continue.";
    
    CText::TextControl.displayPrompt(dst, text, x,y);
    firstTime = 0;
  }
  if (hpPerc < 0.5)
  {
    text = "Your health is low! Pressing 'q' drinks the potion without the need to open the inventory.";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (isOnStairs)
  {
    text = "You are standing on the stairs to next level. Press 'd' to descend.";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (invOpen)
  {
    text = "Press the number corresponding to the item you want to view. Press 'u' to use the item or 'd' to drop it.";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (bookOpen)
  {
    text = "Press the number corresponding to the scroll you want to read.";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (isOnItem)
  {
    text = "You are standing above an item. Pick it up with Enter or by clicking on your character";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (bookSize && invSize)
  {
    text = "To open inventory, press 'i'. To open spell book, press 'r'.";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (bookSize)
  {
    text = "Open your spell book with 'r' to read the scrolls you collected";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
  else if (invSize)
  {
    text = "Open your inventory with 'i' to view the items you collected";
    CText::TextControl.displayMessage(dst, text, 0, 0);
  }
}
