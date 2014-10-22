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
#ifndef _DEFINE_H_
#define _DEFINE_H_

/**
 * Various defines.
 * These should be removed.
 */
#define VERSION "v0.9.2"

#define MAP_WIDTH  60
#define MAP_HEIGHT 40

#define TILE_SIZE 32
#define MINITILE_SIZE 4

//default window size
#define WWIDTH  800
#define WHEIGHT 600

#define TILESET_ROWS 4 //how many different tilesets within tileset.png
#define TILES 7 //how many tiles in a tileset row
#define GIMMICKS 7 //how many gimmicks after tiles in tileset
#define GIMMICK_CHANCE 10 //percentage how often a tile gets a gimmick

#define MAX_NAME 16 //maximum length of character name

#define EASTER_EGGS 5
#define EGG_CHANCE 5 // EGG_CHANCE/1000 => in this case that means 0.5%
#endif
