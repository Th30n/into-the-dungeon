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
#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "GameObject.h"

/**
 * Handles loading the level.
 * Cleans up the objects and spawns new ones.
 */
class LevelLoader {
  public:
    // Starts new game and returns player object.
    static GameObject startNewGame();
    // Loads a game from standard save file.
    static void LoadGame();
    // Changes to the next level.
    // Player and current level are needed for handling deleting
    // and creating new objects.
    static void nextLevel(GameObject player, int lvl);

  private:
    static const int BOSS_LEVEL = 5;
    static const int MAX_MONSTERS_LEVEL = 28;
    static const int MIN_MONSTERS_LEVEL = 12;
    static const int MAX_ITEMS_LEVEL = 8;
    static const int MIN_ITEMS_LEVEL = 4;
    static const int MAX_TRAPS_LEVEL = 6;
    static const int MIN_TRAPS_LEVEL = 4;
    static void clearObjects(GameObject player);
    static void spawnObjects(GameObject player, int lvl);
};
#endif
