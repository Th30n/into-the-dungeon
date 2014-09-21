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
#ifndef AISYSTEM_H
#define AISYSTEM_H

#include <string>

#include "GameObject.h"
#include "Vector2f.h"

/**
 * This class handles all objects with AIComponent.
 * During update it tries to find a valid target
 * and sends the object towards it to attack it.
 * If the object has a spell list, decisions are
 * made which spell is cast.
 */
class AISystem {
  public:
    AISystem(GameObject player);
    void update();
  private:
    void decide(GameObject obj);
    GameObject getValidTarget(GameObject obj);
    bool canSee(GameObject obj, GameObject target);
    bool canCastSpell(GameObject obj);
    void castSpell(GameObject obj, GameObject target);
    GameObject targetSpell(GameObject obj, GameObject target,
        std::string spell_name);
    GameObject getAdjacentField(GameObject obj);
    bool inAttackRange(GameObject obj, GameObject target);
    void attack(GameObject obj, GameObject target);
    void moveToPos(GameObject obj, Vector2f pos);
    void moveRandom(GameObject obj);

    GameObject player_;

};
#endif

