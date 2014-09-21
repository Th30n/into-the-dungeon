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
#ifndef _COPTIONS_H_
#define _COPTIONS_H_

class IMLNode;

/**
 * Loads game options from data/options.xml file.
 * Allows for changing and saving options.
 */
class COptions {
  public:
    static COptions options;
    
    COptions();
    ~COptions();
    void LoadOptions();
    bool getWindowed();
    void setWindowed(bool value);
    int getScreenHeight();
    int getScreenWidth();
    bool getNewbieTips();
    void setNewbieTips(bool value);

  private:
    IMLNode *options_iml_;
    bool is_windowed_;
    bool newbie_tips_on_;
    int screen_width_;
    int screen_height_;
};

#endif
