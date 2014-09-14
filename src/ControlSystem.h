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
#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include "GameObject.h"
#include "Vector2f.h"

class EntityManager;
class HealthComponent;
class InventoryComponent;
class MovementComponent;
class SpaceComponent;
class TurnComponent;
class WeaponComponent;

/**
 * Handles player_ control.
 */
class ControlSystem {
  public:
    enum Controls {
      UP,
      DOWN,
      LEFT,
      RIGHT,
      PICK_UP_ITEM,
      DESCEND,
      PAUSE_TURN
    };

    ControlSystem(EntityManager &em, GameObject player);
    void start();
    void update(Controls control);

  private:
    void move(Controls control) const;
    void pickUpItem() const;
    GameObject getAttackableObjectAtPos(Vector2f pos) const;
    GameObject getItemAtPos(Vector2f pos) const;
    bool addToInventory(GameObject item) const;
    void descend() const;

    EntityManager &em_;
    GameObject player_;
    TurnComponent *player_turn_;
    WeaponComponent *player_weapon_;
    SpaceComponent *player_space_;
    MovementComponent *player_movement_;
    HealthComponent *player_health_;
    InventoryComponent *inventory_;

};
#endif
