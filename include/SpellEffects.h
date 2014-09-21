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
#ifndef SPELLEFFECTS_H
#define SPELLEFFECTS_H

#include <string>
#include <vector>

#include "GameObject.h"
#include "Vector2f.h"

class Effect;
class SpellComponent;

/**
 * Handles applying effects in SpellComponent.
 * Based on effect type different behaviours are done.
 * Special visual effects are also created.
 */
class SpellEffects {
  public:
    // Apply the spell by name to target.
    static void apply(GameObject source, GameObject target,
        const std::string &name);
  private:
    static void apply(GameObject spell_obj);
    static void addAreaTargets(SpellComponent *spell,
        std::vector<GameObject> &targets);
    static void targetSquareArea(SpellComponent *spell,
        std::vector<GameObject> &targets);
    static GameObject getTargetAtPos(Vector2f pos);
    static void createEffect(const Effect &effect);
    static void applyDOT(const Effect &effect);
    static void applyDamage(const Effect &effect);
    static void applyHeal(const Effect &effect);
    static void applyTrigger(const Effect &effect);
    static void applyLifesteal(const Effect &effect);
    static void applyTeleportRandom(const Effect &effect);
    static void applyBlessing(const Effect &effect);
    static void applyItemMod(const Effect &effect);
    static void applyIdentify(const Effect &effect);
    static void applySpawnEntity(const Effect &effect);
    static void createSfx(GameObject spell_obj);
    static void createPreSfx(GameObject spell_obj);
    // Callback for particle/projectile
    static void createPostSfx(GameObject attacker, GameObject target);
    static void createPostSfx(GameObject spell_obj);
};
#endif
