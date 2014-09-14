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
#ifndef HUDINVENTORY_H
#define HUDINVENTORY_H

#include <string>

#include <SDL.h>

#include "GameObject.h"

class CHud;
class InventoryComponent;
class ItemComponent;
class TurnComponent;

/**
 * Inventory and scroll book interface class.
 * Handles events and renders inventory.
 */
class HudInventory {
  public:
    HudInventory(CHud &hud);
    ~HudInventory();
    bool OnLoad();
    void start(GameObject obj);
    void render(SDL_Surface *display, int x, int y);
    bool onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    // Update methods when inventory is changed (e.g. item picked up).
    void updateInventory(int highlight);
    void updateScrollBook(int highlight = -1);
    // Activate mode for selecting an item which the spell will affect.
    void activateTargeting(std::string &spell, GameObject item);
  
  private:
    static const unsigned MAX_ITEMS_PAGE = 10;
    static const int INV_PAGES = 2;

    void toggleInventory();
    void toggleScrollBook();
    void toggleItemDetails(unsigned item_no);
    void toggleScrollDetails(unsigned item_no);
    void drawItemDetails(SDL_Surface *display, int tx, int ty);
    void drawScrollDetails(SDL_Surface *display, int tx, int ty);
    void useSelectedItem();
    void useSelectedScroll();
    void dropSelectedItem();
    std::string getItemDescription(ItemComponent *item);
    void applyScrollOnItem();

    CHud &owner_;
    GameObject player_;
    bool is_active_;
    bool is_scroll_book_on_;
    bool is_item_details_on_;
    bool is_scroll_details_on_;
    std::string spell_name_;
    GameObject spell_item_;
    bool is_targeting_item_;
    bool is_targeted_item_;
    SDL_Surface *surf_inventory_;
    SDL_Surface *surf_new_inventory_;
    SDL_Surface *surf_item_details_;
    SDL_Surface *surf_spell_book_;
    SDL_Surface *surf_spell_list_;
    InventoryComponent *player_inventory_;
    TurnComponent *player_turn_;
    unsigned inventory_page_;
    unsigned spell_book_page_;
    unsigned which_item_;
    unsigned which_scroll_;
};

#endif
