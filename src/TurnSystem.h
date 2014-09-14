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
#ifndef TURNSYSTEM_H
#define TURNSYSTEM_H

/**
 * Handles objects with TurnComponent.
 * If everything is done for current team switches to the next team.
 * Everything being done means: all has finished moving and everyone
 * on current team did their action.
 */
class TurnSystem {
  public:
    TurnSystem();
    void update();
  private:
    bool isAnyObjectMoving() const;
    bool waitForAnimations() const;
    bool isCurrentTeamDone() const;
    void switchTurn();

    static const int DEFAULT_TEAM_SIZE = 2;
    int current_team_;
    int max_teams_;
};
#endif
