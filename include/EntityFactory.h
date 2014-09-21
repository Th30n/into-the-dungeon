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
#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include <string>
#include <vector>

#include "GameObject.h"
#include "Vector2f.h"

class CEntity;
class IMLNode;
class ItemComponent;
class Rectangle;

/*
 * This is a factory class for various entities used in this game.
 * Singleton instance is created when requested.
 */
class EntityFactory {
  public:
    static EntityFactory &instance();
    // Create from entities.xml (monsters and player);
    GameObject createEntity(const std::string &name) const;
    // Create from items.xml.
    GameObject createItem(const std::string &name) const;
    // Create from traps.xml.
    GameObject createTrap(const std::string &name) const;
    // Create from spells.xml.
    GameObject createSpell(const std::string &name) const;
    // Creates a basic DOT effect.
    GameObject createDOT() const;
    // Spawn an object.
    // SpaceComponent is added if the object doesn't have one.
    GameObject spawn(GameObject object) const;
    GameObject spawn(GameObject object, Vector2f pos) const;
    // Create from items.xml based on item level and randomize stats.
    GameObject createRandomItem(int min_level, int max_level) const;
    // Create from entities.xml based on level stats.
    GameObject createRandomEntity(int min_level, int max_level) const;
    // Create from traps.xml based on item level.
    GameObject createRandomTrap(int min_level, int max_level) const;

  private:
    EntityFactory();
    ~EntityFactory();
    EntityFactory(const EntityFactory &ef);
    EntityFactory &operator=(const EntityFactory &ef);
    void loadGameData();
    GameObject createObjectFromIML(
        const std::string &name, const IMLNode *iml) const;
    bool colliderAt(const Rectangle &space) const;
    void randomizeItemStats(ItemComponent *item) const;

    static EntityFactory *instance_;
    IMLNode *entities_iml_;
    IMLNode *items_iml_;
    IMLNode *traps_iml_;
    IMLNode *spells_iml_;
    std::vector<IMLNode*> entity_list_;
    std::vector<IMLNode*> item_list_;
};
#endif
