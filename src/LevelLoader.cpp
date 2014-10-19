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
#include "LevelLoader.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "CApp.h"
#include "CArea.h"
#include "CCamera.h"
#include "CHud.h"
#include "CMusic.h"
#include "data/save.hpp"
#include "DOTComponent.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "SpaceComponent.h"

GameObject LevelLoader::startNewGame()
{
  // Clear everything.
  EntityManager &em = EntityManager::instance();
  em.reset();

  EntityFactory &ef = EntityFactory::instance();
  GameObject player = ef.createEntity("player");
  CCamera::camera_control.target_mode = TARGET_MODE_CENTER;
  CCamera::camera_control.SetTarget(
      em.getComponentForEntity<SpaceComponent>(player));

  CHud::HUD.start(player);

  /***********initiate food DOT**************/
  GameObject hunger = ef.createDOT();
  DOTComponent *dot = em.getComponentForEntity<DOTComponent>(hunger);
  dot->target = player;
  dot->light_change = -2;
  dot->infinite = true;
  /****************************************/
  
  LevelLoader::nextLevel(player, 1);
  CHud::HUD.updateStats();
  return player;
}

bool LevelLoader::LoadGame()
{
  EntityManager &em = EntityManager::instance();
  if (!data::LoadGame(em)) {
    return false;
  }
  GameObject player(1);

  CCamera::camera_control.target_mode = TARGET_MODE_CENTER;
  CCamera::camera_control.SetTarget(
      em.getComponentForEntity<SpaceComponent>(player));
  CHud::HUD.start(player);
  CHud::HUD.updateStats();

  if (!(CArea::dungeon_level % BOSS_LEVEL)) {
    //When on Boss level change to Boss music
    CMusic::SoundControl.ChangeMusic(CApp::MusicBoss);
  } else {
    CMusic::SoundControl.ChangeMusic(CApp::Music);
  }
  return true;
}

void LevelLoader::nextLevel(GameObject player, int lvl)
{
  //printf(stderr, "LevelLoader::nextLevel\n");
  CMusic::SoundControl.PlaySFX(CMusic::SoundControl.Descend, 0);

  CArea::dungeon_level = lvl;
  if (!(lvl % BOSS_LEVEL)) {
    if (!CArea::area_control.BossLevel()) {
      fprintf(stderr, "BossLevel() returned false\n");
    }
    //When on Boss level change to Boss music
    CMusic::SoundControl.ChangeMusic(CApp::MusicBoss);
  } else {
    CArea::area_control.NextLevel();
    //When leaving Boss level change to normal music
    if ((lvl - 1) % BOSS_LEVEL == 0) {
      CMusic::SoundControl.ChangeMusic(CApp::Music);
    }
  }
  
  clearObjects(player);
  spawnObjects(player, lvl);

  //printf(stderr, "Exiting: LevelLoader::nextLevel()\n");
}

void LevelLoader::clearObjects(GameObject player)
{
  EntityManager &em = EntityManager::instance();

  // Remove DOTs which don't target the player.
  std::vector<GameObject> dots;
  em.getEntitiesWithComponent<DOTComponent>(dots);
  std::vector<GameObject>::iterator it = dots.begin();
  for (; it != dots.end(); ++it) {
    DOTComponent *dot = em.getComponentForEntity<DOTComponent>(*it);
    if (dot->target != player) {
      em.removeEntity(*it);
    }
  }

  // Remove everything within game space (monsters, items...) except player
  std::vector<GameObject> objects;
  em.getEntitiesWithComponent<SpaceComponent>(objects);
  for (it = objects.begin(); it != objects.end(); ++it) {
    SpaceComponent *space = em.getComponentForEntity<SpaceComponent>(*it);
    if (space->is_active && *it != player) {
      em.removeEntity(*it);
    }
  }
}

void LevelLoader::spawnObjects(GameObject player, int lvl)
{
  int MaxEnemies = 1;
  EntityFactory &ef = EntityFactory::instance();

  // spawn new monsters
  if (!(lvl % BOSS_LEVEL)) {
    GameObject boss = ef.createEntity("dragonBoss");
    ef.spawn(boss, Vector2f(24, 12));
    ef.spawn(player, Vector2f(25, 28));
  } else {
    MaxEnemies = rand() % (MAX_MONSTERS_LEVEL - MIN_MONSTERS_LEVEL + 1)
        + MIN_MONSTERS_LEVEL;
    for (int i = 0; i < MaxEnemies; i++) {
      GameObject go = ef.createRandomEntity(0, lvl);
      ef.spawn(go);
    }
    ef.spawn(player);
  }
  
  // spawn new Items
  int MaxItems = rand() % (MAX_ITEMS_LEVEL - MIN_ITEMS_LEVEL + 1)
      + MIN_ITEMS_LEVEL;
  for (int i = 0; i < MaxItems; i++) {
    GameObject go = ef.createRandomItem(0, lvl);
    ef.spawn(go);
  }
  
  //spawn new Traps
  int MaxTraps = rand() % (MAX_TRAPS_LEVEL - MIN_TRAPS_LEVEL + 1)
      + MIN_TRAPS_LEVEL;
  for (int i = 0; i < MaxTraps; i++) {
    GameObject go = ef.createRandomTrap(0, lvl);
    ef.spawn(go);
  }
}
