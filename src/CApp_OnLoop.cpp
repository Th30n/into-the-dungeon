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

#include "AISystem.h"
#include "AnimationSystem.h"
#include "CArea.h"
#include "CHud.h"
#include "CFPS.h"
#include "CCamera.h"
#include "CMusic.h"
#include "DOTSystem.h"
#include "EntityManager.h"
#include "HealthSystem.h"
#include "MovementSystem.h"
#include "ParticleSystem.h"
#include "ProjectileSystem.h"
#include "SpaceComponent.h"
#include "StatsComponent.h"
#include "TrapSystem.h"
#include "TurnSystem.h"
#include "WeaponSystem.h"

void CApp::OnLoop()
{
  EntityManager &em = EntityManager::instance();
  StatsComponent *stats = em.getComponentForEntity<StatsComponent>(player);
  int prevXP = stats->xp;
  //
  //if (LMB.Pressed) {
  //  SDL_GetMouseState(&LMB.X, &LMB.Y);
  //  MovePlayerWithMouse(LMB.X - Player.space->getPosX() +
  //            CCamera::camera_control.GetX() - TILE_SIZE / 2,
  //            LMB.Y - Player.space->getPosY() +
  //            CCamera::camera_control.GetY() - TILE_SIZE / 2);
  //}

  CArea::area_control.OnLoop(-CCamera::camera_control.GetX(),
                -CCamera::camera_control.GetY());
  CHud::HUD.FoWCalculation();

  CFPS::fps_control.OnLoop();
  CHud::HUD.OnLoop();

  if (prevXP != stats->xp && CharacterSheetOn) {
    CHud::HUD.updateStats();
  }
  
  DOTSystem dots;
  dots.update();
  AISystem ai(player);
  ai.update();
  MovementSystem ms;
  ms.update();
  WeaponSystem ws;
  ws.update();
  AnimationSystem as;
  as.update();
  ParticleSystem particleSystem;
  particleSystem.update();
  ProjectileSystem projectileSystem;
  projectileSystem.update();
  TrapSystem traps;
  traps.update();
  HealthSystem hps("./gfx/UI/Text.png", "./gfx/Status.png");
  hps.start();
  hps.update();
  turnSystem.update();
}

void CApp::SaveHighscore()
{
}
