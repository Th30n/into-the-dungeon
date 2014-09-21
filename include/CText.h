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
#ifndef _CTEXT_H_
#define _CTEXT_H_

#include <string>

#include <SDL.h>

#define WINDOW_TILE 16

enum
{
  WINDOW_TOP_LEFT = 0,
  WINDOW_TOP,
  WINDOW_TOP_RIGHT,
  WINDOW_LEFT,
  WINDOW_CENTER,
  WINDOW_RIGHT,
  WINDOW_BOT_LEFT,
  WINDOW_BOT,
  WINDOW_BOT_RIGHT
};

enum
{
  FONT_MEDIUM_BOLD = 0,
  FONT_SMALL
};

enum
{
  COLOR_WHITE = 0,
  COLOR_BLUE,
  COLOR_GREEN,
  COLOR_RED,
  COLOR_BLACK
};

/**
 * Offers method for creating text images using the custom bmp_font images.
 * This should be a bit reorganized.
 */
class CText {
  public:
    static CText TextControl;

    CText();
    bool OnLoad();
    void OnCleanup();
    std::string ConvertInt(int number);
    bool displayPrompt(SDL_Surface *display, std::string text, int x, int y);
    
    // R, G, B, if set to -1 won't draw the window,
    // if set to -2 window tiles will be used
    void drawTextWindow(SDL_Surface *display, int x, int y,
            int width, int height, int R, int G, int B);
    
    void displayMessage(SDL_Surface *display, std::string text, int x, int y);
    
    void displayDialogue(SDL_Surface *display, SDL_Surface *icon,
            std::string text);
    void buildBMPFont(SDL_Surface *bmp_font, int *bmp_space, int *bmp_newline,
            SDL_Rect *bmp_chars);
    
    // fg_r, fg_g, fg_b are letter colors (-1 is default),
    // bg_r, bg_g, bg_b are background (passed to drawTextWindow),
    // border - 0 off, 1 on
    void displayBMPText(SDL_Surface *display, int x, int y, std::string text,
            Uint32 fg_r, Uint32 fg_g, Uint32 fg_b, int bg_r, int bg_g, int bg_b,
            int font, int border);
    void displayBMPText(SDL_Surface *display, int x, int y, std::string text,
            SDL_Color fg, SDL_Color bg, int font, int border);
    void drawWindowBorders(SDL_Surface *display, int x, int y, int width,
            int height);
    void drawWindowTiles(SDL_Surface *display, int x, int y, int width,
            int height, int tile);
    void getTextDimensions(std::string text, int font, int *width, int *height);

  private:
    static const int TAB_WIDTH = 8; // Tab equals n spaces

    SDL_Surface *getColoredFont(SDL_Surface *bmp_font, int r, int g, int b);

    SDL_Surface *window_tiles_;
    SDL_Surface *bmp_font_small_;
    SDL_Surface *bmp_font_medium_bold_;
    SDL_Rect bmp_chars_cmall_[256];
    SDL_Rect bmp_chars_medium_bold_[256];
    int bmp_newline_small_;
    int bmp_newline_medium_bold_;
    int bmp_space_small_;
    int bmp_space_medium_bold_;
};

#endif
