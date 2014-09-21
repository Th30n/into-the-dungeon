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
#ifndef COMPONENTFACTORY_H_
#define COMPONENTFACTORY_H_

#include <map>
#include <string>
#include <vector>

class IComponent;

/*
 * This is a factory class for various components used in this game.
 * Each concrete component must register itself statically with string id
 * and pointer to create function.
 * Singleton instance is created when requested.
 */
class ComponentFactory {
  public:
    typedef IComponent *(*Creator)();

    static ComponentFactory &instance();

    int registerCreator(const std::string &id, Creator creator);
    IComponent *create(const std::string &id) const;
    std::vector<std::string> getIds() const;

  private:
    typedef std::map<std::string, Creator> ComponentMap;

    ComponentFactory();
    ~ComponentFactory();
    ComponentFactory(const ComponentFactory &ef);
    ComponentFactory &operator=(const ComponentFactory &ef);

    static ComponentFactory *instance_;

    ComponentMap components;
};
#endif
