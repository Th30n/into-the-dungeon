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
#include "TurnSystem.h"

#include "AnimationComponent.h"
#include "EntityManager.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "SpaceComponent.h"
#include "TurnComponent.h"

TurnSystem::TurnSystem() : current_team_(-1), max_teams_(DEFAULT_TEAM_SIZE)
{
}

void TurnSystem::update()
{
  if (!isAnyObjectMoving() && !waitForAnimations()){
    if (isCurrentTeamDone()) {
      switchTurn();
    }
  }
}

bool TurnSystem::isAnyObjectMoving() const
{
  EntityManager &e = EntityManager::instance();
  std::vector<GameObject> entities;
  e.getEntitiesWithComponent<MovementComponent>(entities);
  std::vector<GameObject>::iterator it = entities.begin();
  for (; it != entities.end(); ++it) {
    MovementComponent *mc = e.getComponentForEntity<MovementComponent>(*it);
    SpaceComponent *sc = e.getComponentForEntity<SpaceComponent>(*it);
    if (sc && mc->waypoint != sc->pos) {
      return true;
    }
  }
  return false;
}

bool TurnSystem::waitForAnimations() const
{
  EntityManager &e = EntityManager::instance();
  std::vector<AnimationComponent*> acs;
  e.getAllComponentsOfType(acs);
  std::vector<AnimationComponent*>::iterator it = acs.begin();
  for (; it != acs.end(); ++it) {
    AnimationComponent *ac = *it;
    if (ac->attacking) {
      return true;
    }
  }
  return false;
}

bool TurnSystem::isCurrentTeamDone() const
{
  EntityManager &e = EntityManager::instance();
  std::vector<TurnComponent*> tcs;
  e.getAllComponentsOfType(tcs);
  std::vector<TurnComponent*>::iterator it = tcs.begin();
  for (; it != tcs.end(); ++it) {
    TurnComponent *tc = *it;
    if (tc->team == current_team_ && !tc->turn_taken) {
      return false;
    }
  }
  return true;
}

void TurnSystem::switchTurn()
{
  current_team_ = (current_team_ + 1) % max_teams_;
  EntityManager &e = EntityManager::instance();
  std::vector<TurnComponent*> tcs;
  e.getAllComponentsOfType(tcs);
  std::vector<TurnComponent*>::iterator it = tcs.begin();
  int max = DEFAULT_TEAM_SIZE;
  for (; it != tcs.end(); ++it) {
    TurnComponent *tc = *it;
    if (tc->team == current_team_) {
      tc->turn_taken = false;
    } else if (tc->team >= max) {
      max = tc->team + 1;
    }
  }
  max_teams_ = max;
}
