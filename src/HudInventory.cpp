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
#include "HudInventory.h"

#include "CHud.h"
#include "CMusic.h"
#include "COptions.h"
#include "CSurface.h"
#include "CText.h"
#include "Define.h"
#include "EntityManager.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "ItemRenderSystem.h"
#include "ItemUsage.h"
#include "SpellEffects.h"
#include "TrapComponent.h"
#include "TurnComponent.h"

HudInventory::HudInventory(CHud &hud)
  : owner_(hud), player_(0), is_active_(false), is_scroll_book_on_(false),
    is_item_details_on_(false), is_scroll_details_on_(false), spell_item_(0),
    is_targeting_item_(false), is_targeted_item_(false),
    surf_inventory_(0), surf_new_inventory_(0), surf_item_details_(0),
    surf_spell_book_(0), surf_spell_list_(0),
    player_inventory_(0), player_turn_(0), inventory_page_(0), spell_book_page_(0),
    which_item_(0), which_scroll_(0)
{
}

HudInventory::~HudInventory()
{
  if (surf_inventory_) {
    SDL_FreeSurface(surf_inventory_);
  }
  if (surf_new_inventory_) {
    SDL_FreeSurface(surf_new_inventory_);
  }
  if (surf_item_details_) {
    SDL_FreeSurface(surf_item_details_);
  }
  if (surf_spell_book_) {
    SDL_FreeSurface(surf_spell_book_);
  }
  if (surf_spell_list_) {
    SDL_FreeSurface(surf_spell_list_);
  }
}

bool HudInventory::OnLoad()
{
  if (!(surf_inventory_ = CSurface::OnLoad("./gfx/UI/Inventory.png"))) {
    return false;
  }

  if (!(surf_item_details_ = CSurface::OnLoad("./gfx/UI/ItemDetails.png"))) {
    return false;
  }
  
  if (!(surf_new_inventory_ = SDL_CreateRGBSurface(SDL_SWSURFACE,
      surf_inventory_->w, surf_inventory_->h,
      surf_inventory_->format->BitsPerPixel, 0, 0, 0, 0))) {
    return false;
  }
  CSurface::Transparent(surf_new_inventory_, 255, 0, 255);

  if (!(surf_spell_book_ = CSurface::OnLoad("./gfx/UI/Spellbook.png"))) {
    return false;
  }
  if ((surf_spell_list_ = SDL_CreateRGBSurface(SDL_SWSURFACE, surf_spell_book_->w, surf_spell_book_->h, surf_spell_book_->format->BitsPerPixel, 0, 0, 0, 0)) == NULL)
  {
    return false;
  }
  CSurface::Transparent(surf_spell_list_, 255, 0, 255);

  return true;
}

void HudInventory::start(GameObject obj)
{
  player_ = obj;
  EntityManager &em = EntityManager::instance();
  player_inventory_ = em.getComponentForEntity<InventoryComponent>(player_);
  player_turn_ = em.getComponentForEntity<TurnComponent>(player_);
}

void HudInventory::render(SDL_Surface *display, int x, int y)
{
  if (is_active_) {
    CSurface::OnDraw(display, surf_inventory_, x, y);
    CSurface::OnDraw(display, surf_new_inventory_, x, y);
    if (is_item_details_on_) {
      drawItemDetails(display, x, y);
    }
  }
  if (is_scroll_book_on_) {
    int X = 540 + COptions::options.getScreenWidth() - WWIDTH;
    int Y = 20 + (COptions::options.getScreenHeight() - WHEIGHT) / 2;
    CSurface::OnDraw(display, surf_spell_book_, X, Y);
    CSurface::OnDraw(display, surf_spell_list_, X, Y);
    if (is_scroll_details_on_) {
      drawScrollDetails(display, X, Y);
    }
  }
  if (is_targeting_item_) {
    std::string text = " Select an item to apply the spell on it.\n";
    text += " Selection is done by clicking the number twice. ";
    int x = COptions::options.getScreenWidth() / 4;
    int y = 0;
    CText::TextControl.displayBMPText(display, x, y,
          text, 255, 0, 0, 0, 0, 0, FONT_SMALL, 0);
    if (is_targeted_item_) {
      std::string prompt = "Apply scroll?";
      if (CText::TextControl.displayPrompt(display, prompt, x, y)) {
        applyScrollOnItem();
      } else {
        is_targeted_item_ = false;
      }
    }
  }
}

bool HudInventory::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch (sym) {
  case SDLK_i:
    toggleInventory();
    return true;
    break;
  case SDLK_r:
    toggleScrollBook();
    return true;
  case SDLK_n:
    if (is_active_) {
      inventory_page_ = (inventory_page_ + 1) % INV_PAGES;
      is_item_details_on_ = false;
      CHud::HUD.updateInventory();
      //play sound
      CMusic::SoundControl.PlaySFX(CMusic::SoundControl.TurnPage, 0);
    } else if (is_scroll_book_on_) {
      spell_book_page_ = (spell_book_page_ + 1) % INV_PAGES;
      is_scroll_details_on_ = false;
      CHud::HUD.updateSpellBook();
      //play sound
      CMusic::SoundControl.PlaySFX(CMusic::SoundControl.TurnPage, 0);
    }
    break;
  case SDLK_0:
    if (is_active_) {
      toggleItemDetails(0);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(0);
      return true;
    }
    break;
  case SDLK_1:
    if (is_active_) {
      toggleItemDetails(1);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(1);
      return true;
    }
    break;
  case SDLK_2:
    if (is_active_) {
      toggleItemDetails(2);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(2);
      return true;
    }
    break;
  case SDLK_3:
    if (is_active_) {
      toggleItemDetails(3);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(3);
      return true;
    }
    break;
  case SDLK_4:
    if (is_active_) {
      toggleItemDetails(4);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(4);
      return true;
    }
    break;
  case SDLK_5:
    if (is_active_) {
      toggleItemDetails(5);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(5);
      return true;
    }
    break;
  case SDLK_6:
    if (is_active_) {
      toggleItemDetails(6);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(6);
      return true;
    }
    break;
  case SDLK_7:
    if (is_active_) {
      toggleItemDetails(7);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(7);
      return true;
    }
    break;
  case SDLK_8:
    if (is_active_) {
      toggleItemDetails(8);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(8);
      return true;
    }
    break;
  case SDLK_9:
    if (is_active_) {
      toggleItemDetails(9);
      return true;
    } else if (is_scroll_book_on_) {
      toggleScrollDetails(9);
      return true;
    }
    break;
  case SDLK_u:
    if (is_active_) {
      useSelectedItem();
      return true;
    } else if (is_scroll_book_on_) {
      useSelectedScroll();
      return true;
    }
  case SDLK_d:
    if (is_active_) {
      dropSelectedItem();
      return true;
    }
  default:
    break;
  }
  return false;
}

void HudInventory::updateInventory(int highlight)
{
  ItemRenderSystem item_renderer;
  int Y = 35;
  int X = 3;
  std::string ItemText;
  
  SDL_FillRect(surf_new_inventory_, NULL,
      SDL_MapRGB(surf_new_inventory_->format, 255, 0, 255));
  
  std::vector<unsigned> &inventory = player_inventory_->items;
  unsigned size = inventory.size();
  
  //checks if there is enough space to show on one page
  if (inventory.size() > MAX_ITEMS_PAGE && inventory_page_ == 0) {
    size = MAX_ITEMS_PAGE;
  }
  
  EntityManager &em = EntityManager::instance();
  for (unsigned i = inventory_page_ * MAX_ITEMS_PAGE; i < size; i++) {
    int name_width = 0;
    int name_height = 0;

    int fg_r = 0;
    int fg_g = 0;
    int fg_b = 0;
    
    int bg_r = -1;
    int bg_g = -1;
    int bg_b = -1;
    
    if (i == highlight) {
      bg_r = 250;
      bg_g = 200;
      bg_b = 0;
    } else {
      bg_r = bg_g = bg_b = -1;
    }

    ItemComponent *item = em.
        getComponentForEntity<ItemComponent>(inventory[i]);
    if (item->useType == ItemComponent::USE_TYPE_TRAP) {
      ItemText = " [" + CText::TextControl.ConvertInt(i % 10) + "] "
        + item->name;
      TrapComponent *trap = em.
          getComponentForEntity<TrapComponent>(inventory[i]);
      if (trap->is_destroyed)
      {
        fg_r = 150;
        fg_g = 0;
        fg_b = 0;
      } else {
        fg_r = 0;
        fg_g = 0;
        fg_b = 255;
      }
      CText::TextControl.displayBMPText(surf_new_inventory_, X, Y,
          ItemText, fg_r, fg_g, fg_b, bg_r, bg_g, bg_b, FONT_SMALL, 0);
      Y = Y + 40;
      CText::TextControl.getTextDimensions(ItemText, FONT_SMALL,
          &name_width, &name_height);
      item_renderer.drawItem(surf_new_inventory_, item,
          X + name_width, Y - 40 - 8);
      continue;
    }
    
    ItemText = " [" + CText::TextControl.ConvertInt(i % 10) + "] " +
        item->name + ' ';
    
    if (item->equipped) {
      ItemText += "[e] ";
    }
    
    switch (item->rareType) {
      case ItemComponent::RARE_TYPE_WHITE:
        fg_r = fg_g = fg_b = 255;
        break;
      case ItemComponent::RARE_TYPE_BLUE:
        fg_r = 0;
        fg_g = 0;
        fg_b = 255;
        break;
      case ItemComponent::RARE_TYPE_GREEN:
        fg_r = 0;
        fg_g = 255;
        fg_b = 0;
        break;
      default:
        break;
    }
    if (item->identified && item->cursed) {
      fg_r = 150;
      fg_g = 0;
      fg_b = 0;
    }
    CText::TextControl.displayBMPText(surf_new_inventory_, X, Y,
        ItemText, fg_r, fg_g, fg_b, bg_r, bg_g, bg_b, FONT_SMALL, 0);
    Y = Y + 40;
    
    CText::TextControl.getTextDimensions(ItemText, FONT_SMALL,
        &name_width, &name_height);
    item_renderer.drawItem(surf_new_inventory_, item,
        X + name_width, Y - 40 - 8);
  }
}

void HudInventory::updateScrollBook(int highlight)
{
  ItemRenderSystem item_renderer;
  int Y = 35;
  int X = 3;

  std::string SpellText;
  
  SDL_FillRect(surf_spell_list_, NULL,
      SDL_MapRGB(surf_spell_list_->format, 255, 0, 255));
  
  std::vector<unsigned> &inventory = player_inventory_->scrolls;
  unsigned size = inventory.size();
  
  //checks if there is enough space to show on one page
  if (inventory.size() > MAX_ITEMS_PAGE && spell_book_page_ == 0) {
    size = MAX_ITEMS_PAGE;
  }
  
  EntityManager &em = EntityManager::instance();
  for (unsigned i = spell_book_page_ * MAX_ITEMS_PAGE; i < size; i++) {
    ItemComponent *scroll= em.
        getComponentForEntity<ItemComponent>(inventory[i]);
    SpellText = " [" + CText::TextControl.ConvertInt(i % 10) + "] " +
        scroll->name + ' ';

    int bg_r = -1;
    int bg_g = -1;
    int bg_b = -1;
    
    if (i == highlight) {
      bg_r = 250;
      bg_g = 200;
      bg_b = 0;
    } else {
      bg_r = bg_g = bg_b = -1;
    }
    
    CText::TextControl.displayBMPText(surf_spell_list_, X, Y,
        SpellText, 255, 255, 255, bg_r, bg_g, bg_b, FONT_SMALL, 0);
    Y = Y + 40;
    
    int name_width = 0;
    int name_height = 0;
    CText::TextControl.getTextDimensions(SpellText, FONT_SMALL,
        &name_width, &name_height);
    //renders icon next to name
    item_renderer.drawItem(surf_spell_list_, scroll,
        X + name_width, Y - 40 - 8);
  }
}

void HudInventory::activateTargeting(std::string &spell, GameObject item)
{
  if (!is_active_) {
    toggleInventory();
  }
  is_targeting_item_ = true;
  spell_name_ = spell;
  spell_item_ = item;
}

void HudInventory::toggleInventory()
{
  is_active_ = !is_active_;
  if (is_active_) {
    updateInventory(-1);
  }
  is_item_details_on_ = false;
  is_targeting_item_ = false;
}

void HudInventory::toggleScrollBook()
{
  is_scroll_book_on_ = !is_scroll_book_on_;
  if (is_scroll_book_on_) {
    updateScrollBook();
  }
  is_scroll_details_on_ = false;
}

void HudInventory::toggleItemDetails(unsigned item_no)
{
  unsigned prev_details = which_item_;
  which_item_ = item_no + inventory_page_ * MAX_ITEMS_PAGE;
  if (prev_details == which_item_) {
    if (is_item_details_on_ && is_targeting_item_) {
      is_targeted_item_ = true;
    } else {
      is_item_details_on_ = !is_item_details_on_;
      is_targeted_item_ = false;
    }
  } else {
    is_item_details_on_ = true;
  }
  if (is_item_details_on_) {
    updateInventory(which_item_);
  } else {
    updateInventory(-1);
  }
}

void HudInventory::toggleScrollDetails(unsigned item_no)
{
  unsigned prev_details = which_scroll_;
  which_scroll_ = item_no + spell_book_page_ * MAX_ITEMS_PAGE;
  if (prev_details == which_scroll_) {
    is_scroll_details_on_ = !is_scroll_details_on_;
  } else {
    is_scroll_details_on_ = true;
  }
  if (is_scroll_details_on_) {
    updateScrollBook(which_scroll_);
  } else {
    updateScrollBook(-1);
  }
}

void HudInventory::drawItemDetails(SDL_Surface *display, int tx, int ty)
{
  std::vector<unsigned> &inventory = player_inventory_->items;
  if (which_item_ < 0 || which_item_ >= inventory.size()) {
    is_item_details_on_ = false;
    return;
  }
  GameObject item_obj(inventory[which_item_]);
  EntityManager &em = EntityManager::instance();
  ItemComponent *item = em.getComponentForEntity<ItemComponent>(item_obj);
  
  ItemComponent *equipped_item = 0;
  
  for (unsigned j = 0; j < inventory.size(); j++) {
    ItemComponent *ic = em.
        getComponentForEntity<ItemComponent>(inventory[j]);
    if (j != which_item_ && ic->useType == item->useType && ic->equipped) {
      equipped_item = ic;
      break;
    }
  }
  
  int x = tx + 3;
  int y = ty + 75 + (which_item_ % MAX_ITEMS_PAGE)*40;
  CSurface::OnDraw(display, surf_item_details_, x-3, y-3);
  std::string Text = getItemDescription(item);
  CText::TextControl.displayBMPText(display, x, y, Text,
      255, 255, 255, -1, -1, -1, FONT_SMALL, 0);
  
  if (equipped_item && equipped_item->identified)
  {
    x += 260;
    CSurface::OnDraw(display, surf_item_details_, x-3, y-3);
    Text = getItemDescription(equipped_item);
    CText::TextControl.displayBMPText(display, x, y,
        Text, 255, 255, 255, -1, -1, -1, FONT_SMALL, 0);
  }
}

void HudInventory::drawScrollDetails(SDL_Surface *display, int tx, int ty)
{
  std::vector<unsigned> &inventory = player_inventory_->scrolls;
  if (which_scroll_ < 0 || which_scroll_ >= inventory.size()) {
    is_scroll_details_on_ = false;
    return;
  }
  GameObject scroll_obj(inventory[which_scroll_]);
  EntityManager &em = EntityManager::instance();
  ItemComponent *scroll = em.getComponentForEntity<ItemComponent>(scroll_obj);
  
  int x = tx + 3;
  int y = ty + 75 + (which_scroll_ % MAX_ITEMS_PAGE)*40;
  CSurface::OnDraw(display, surf_item_details_, x-3, y-3);
  std::string Text = getItemDescription(scroll);
  CText::TextControl.displayBMPText(display, x, y, Text,
      255, 255, 255, -1, -1, -1, FONT_SMALL, 0);
}

void HudInventory::useSelectedItem()
{
  if (!is_item_details_on_ || player_turn_->turn_taken) {
    return;
  }
  std::vector<unsigned> &inventory = player_inventory_->items;
  ItemUsage::useItem(player_, inventory[which_item_]);
  updateInventory(which_item_);
  owner_.updateStats();
}

void HudInventory::useSelectedScroll()
{
  if (!is_scroll_details_on_ || player_turn_->turn_taken) {
    return;
  }
  ItemUsage::useItem(player_, player_inventory_->scrolls[which_scroll_]);
  updateScrollBook(which_scroll_);
  owner_.updateStats();
}

void HudInventory::dropSelectedItem()
{
  if (!is_item_details_on_) {
    return;
  }
  ItemUsage::dropItem(player_, player_inventory_->items[which_item_]);
  updateInventory(-1);
  owner_.updateStats();
}

std::string HudInventory::getItemDescription(ItemComponent *item)
{
  std::string text;
  if (item->identified) {
    text = item->description + "\n";
    if (item->cursed) {
      text += "Cursed! \n";
    }
    StatsComponent &stats = item->statsModifiers;
    if (stats.health != 0.0f) {
      text += "Health: " + CText::TextControl.ConvertInt(stats.health) +
          " \n";
    }
    if (stats.attack != 0.0f) {
      text += "Attack: " + CText::TextControl.ConvertInt(stats.attack) +
          " \n";
    }
    if (stats.armor != 0.0f) {
      text += "Armor: " + CText::TextControl.ConvertInt(stats.armor) +
          " \n";
    }
    if (!item->spell.empty()) {
      text += "Casts spell: " + item->spell + " \n";
    }
  } else {
    text = "Unidentified!";
  }
  return text;
}

void HudInventory::applyScrollOnItem()
{
  SpellEffects::apply(player_, player_inventory_->items[which_item_], spell_name_);
  if (spell_item_.getId() != 0) {
    ItemUsage::removeFromInventory(player_inventory_->items, spell_item_);
    ItemUsage::removeFromInventory(player_inventory_->scrolls, spell_item_);
    EntityManager::instance().removeEntity(spell_item_);
    updateInventory(which_item_);
    updateScrollBook();
    owner_.updateStats();
  }
  is_targeting_item_ = false;
  is_targeted_item_ = false;
}
