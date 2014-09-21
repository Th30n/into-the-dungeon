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
#include <sstream>

#include "CFPS.h"
#include "CText.h"
#include "CSurface.h"
#include "data/dirs.h"

CText CText::TextControl;

CText::CText()
{ 
  window_tiles_ = NULL;
  
  bmp_font_small_ = NULL;
  bmp_font_medium_bold_ = NULL;
  
  bmp_newline_small_ = 0;
  bmp_newline_medium_bold_ = 0;
  bmp_space_small_ = 0;
  bmp_space_medium_bold_ = 0;
}

bool CText::OnLoad()
{
  bmp_font_small_ = CSurface::OnLoad(data::FindFile("gfx/UI/font_small.png").c_str());
  if (!bmp_font_small_) {
    return false;
  }
  
  bmp_font_medium_bold_ = CSurface::OnLoad(data::FindFile("gfx/UI/font_medium_bold.png").c_str());
  if (!bmp_font_medium_bold_) {
    return false;
  }
  
  window_tiles_ = CSurface::OnLoad(data::FindFile("gfx/UI/ITD_windows.png").c_str());
  if (!window_tiles_) {
    return false;
  }
  
  buildBMPFont(bmp_font_small_, &bmp_space_small_, &bmp_newline_small_,
      bmp_chars_cmall_);
  buildBMPFont(bmp_font_medium_bold_, &bmp_space_medium_bold_,
      &bmp_newline_medium_bold_, bmp_chars_medium_bold_);
  
  //CSurface::Transparent(bmp_font_small_, 0, 0, 0);
  //CSurface::Transparent(bmp_font_medium_bold_, 255, 255, 255);
  return true;
}

void CText::buildBMPFont(SDL_Surface *bmp_font, int *bmp_space,
        int *bmp_newline, SDL_Rect *bmp_chars)
{
  Uint32 bg_color = SDL_MapRGB(bmp_font->format, 0, 0, 0);
  
  int cell_w = bmp_font->w / 16;
  int cell_h = bmp_font->h / 8;
  
  int top = cell_h;
  int base = cell_h;
  
  int current_char = 0;
  
  for (int rows = 0; rows < 8; rows++) {
    for (int cols = 0; cols < 16; cols++) {
      bmp_chars[current_char].x = cell_w * cols;
      bmp_chars[current_char].y = cell_h * rows;
      
      bmp_chars[current_char].w = cell_w;
      bmp_chars[current_char].h = cell_h;
      
      //Find where the letter begins from left
      for (int px_col = 0; px_col < cell_w; px_col++) {
        for (int px_row = 0; px_row < cell_h; px_row++) {
          int px_x = (cell_w * cols) + px_col;
          int px_y = (cell_h * rows) + px_row;
          
          if (CSurface::getPixel32(bmp_font, px_x, px_y) != bg_color) {
            bmp_chars[current_char].x = px_x;
            px_col = cell_w;
            px_row = cell_h;
          }
        }
      }
      //Find where the letter begins from right & calculate letter width
      for (int px_col_w = cell_w - 1; px_col_w >= 0; px_col_w--) {
        for (int px_row_w = 0; px_row_w < cell_h; px_row_w++) {
          int px_x = (cell_w * cols) + px_col_w;
          int px_y = (cell_h * rows) + px_row_w;
          
          if (CSurface::getPixel32(bmp_font, px_x, px_y) != bg_color) {
            bmp_chars[current_char].w = (px_x - bmp_chars[current_char].x) + 1;
            px_col_w = -1;
            px_row_w = cell_h;
          }
        }
      }
      //Find where the letter begins from top
      for (int px_row = 0; px_row < cell_h; px_row++) {
        for (int px_col = 0; px_col < cell_w; px_col++) {
          int px_x = (cell_w * cols) + px_col;
          int px_y = (cell_h * rows) + px_row;
          
          if (CSurface::getPixel32(bmp_font, px_x, px_y) != bg_color) {
            if (px_row < top) {
              top = px_row;
            }
            px_col = cell_w;
            px_row = cell_h;
          }
        }
      }
      //Find bottom of A -> base of the letter
      if (current_char == 'A') {
        for (int px_row = cell_h - 1; px_row >= 0; px_row--) {
          for (int px_col = 0; px_col < cell_w; px_col++) {
            int px_x = (cell_w * cols) + px_col;
            int px_y = (cell_h * rows) + px_row;
            
            if (CSurface::getPixel32(bmp_font, px_x, px_y) != bg_color) {
              base = px_row;
              px_col = cell_w;
              px_row = -1;
            }
          }
        }
      }
      current_char++;
    }
  }
  
  *bmp_space = cell_w / 2;
  *bmp_newline = base - top;
  
  //Cut off excess top pixels
  for (int t = 0; t < 16 * 8; t++) {
    bmp_chars[t].y += top;
    bmp_chars[t].h -= top;
  }
}

void CText::OnCleanup()
{
  if (bmp_font_small_) {
    SDL_FreeSurface(bmp_font_small_);
    bmp_font_small_ = NULL;
  }
  if (bmp_font_medium_bold_) {
    SDL_FreeSurface(bmp_font_medium_bold_);
    bmp_font_medium_bold_ = NULL;
  }
  if (window_tiles_) {
    SDL_FreeSurface(window_tiles_);
    window_tiles_ = NULL;
  }
}

void CText::displayBMPText(SDL_Surface *display, int x, int y,
      std::string text, Uint32 fg_r, Uint32 fg_g, Uint32 fg_b,
      int bg_r, int bg_g, int bg_b, int font, int border)
{
  SDL_Surface *bmp_font = NULL;
  int bmp_space = 0;
  int bmp_newline = 0;
  SDL_Rect *bmp_chars = NULL;
  
  if (font == FONT_SMALL) {
    bmp_font = bmp_font_small_;
    bmp_space = bmp_space_small_;
    bmp_newline = bmp_newline_small_;
    bmp_chars = bmp_chars_cmall_;
  } else if (font == FONT_MEDIUM_BOLD) {
    bmp_font = bmp_font_medium_bold_;
    bmp_space = bmp_space_medium_bold_;
    bmp_newline = bmp_newline_medium_bold_;
    bmp_chars = bmp_chars_medium_bold_;
  }
  
  int tmp_x = x;
  int tmp_y = y;
  int text_width = 0;
  int text_height = 0;
  getTextDimensions(text, font, &text_width, &text_height);
  drawTextWindow(display, x, y, text_width, text_height, bg_r, bg_g, bg_b);
  
  SDL_Surface *Surf_Font = getColoredFont(bmp_font, fg_r, fg_g, fg_b);
  
  if (Surf_Font != NULL)
  {
    for (int i = 0; text[i] != '\0'; i++)
    {
      if (text[i] == ' ') {
        tmp_x += bmp_space;
      } else if (text[i] == '\n') {
        tmp_y += bmp_newline;
        tmp_x = x;
      } else if (text[i] == '\t') {
        tmp_x += TAB_WIDTH * bmp_space;
      } else {
        int ascii = (unsigned char)text[i];
        if (ascii > (16 * 8)) {
          continue;
        }
        CSurface::OnDraw(display, Surf_Font, tmp_x, tmp_y,
                  bmp_chars[ascii].x, bmp_chars[ascii].y,
                            bmp_chars[ascii].w, bmp_chars[ascii].h);
        tmp_x += bmp_chars[ascii].w + 1;
      }
    }
  }
  
  if ((border > 0) && bg_r != -1 && bg_g != -1 && bg_b != -1)
  {
    drawWindowBorders(display, x, y, text_width, text_height);
  }
  
  if (Surf_Font)
  {
    SDL_FreeSurface(Surf_Font);
    Surf_Font = NULL;
  }
}

void CText::displayBMPText(SDL_Surface *display, int x, int y,
        std::string text, SDL_Color fg, SDL_Color bg, int font, int border)
{
  displayBMPText(display, x, y, text, fg.r, fg.g, fg.b, bg.r, bg.g, bg.b,
      font, border);
}

SDL_Surface *CText::getColoredFont(SDL_Surface *bmp_font, int r, int g, int b)
{
  SDL_Surface *colored = SDL_CreateRGBSurface(SDL_SWSURFACE,
    bmp_font->w, bmp_font->h, 32, 0, 0, 0, 0);

  SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,
    bmp_font->w, bmp_font->h, 32, 0, 0, 0, 0);
  CSurface::OnDraw(tmp, bmp_font, 0, 0);
  // Fill with desired color
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = colored->w;
  rect.h = colored->h;
  Uint32 color = SDL_MapRGB(colored->format, r, g, b);
  SDL_FillRect(colored, &rect, color);

  // Fill with bmp_font, letters are transparent, background is black
  CSurface::Transparent(tmp, 255, 255, 255);
  CSurface::OnDraw(colored, tmp, 0, 0);
  // Black bacground is now transparent.
  CSurface::Transparent(colored, 0, 0, 0);
  SDL_FreeSurface(tmp);
  return colored;
}

std::string CText::ConvertInt(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream

}

void CText::displayMessage(SDL_Surface *display, std::string text, int x, int y)
{
  displayBMPText(display, x, y, text, 255, 255, 255, 0, 0, 0, FONT_SMALL, 1);
}

bool CText::displayPrompt(SDL_Surface *display, std::string text, int x, int y)
{
  bool Return = false;
  
  SDL_Surface *Surf_Text = NULL;
  
  displayBMPText(display, x, y, text, 29, 90, 49, -2, -2, -2,
      FONT_MEDIUM_BOLD, 1);
  
  SDL_Flip(display);
  
  if (Surf_Text) SDL_FreeSurface(Surf_Text);
  
  bool is_prompt_running = true;
  SDL_Event event;
  
  while (is_prompt_running)
  {
    CFPS::fps_control.OnLoop();
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
        {
          switch (event.key.keysym.sym)
          {
            case SDLK_RETURN:
            case SDLK_SPACE:
            case SDLK_y:
            {
              is_prompt_running = false;
              Return = true;
              break;
            }
            case SDLK_ESCAPE:
            case SDLK_n:
            {
              is_prompt_running = false;
              Return = false;
              break;
            }
            default:
            {
              break;
            }
          }
          break;
        }
        default:
        {
          break;
        }
      }
    }
  }
  
  return Return;
}

void CText::displayDialogue(SDL_Surface *display, SDL_Surface *icon,
        std::string text)
{ 
  SDL_Surface *Surf_Text = NULL;
  
  displayBMPText(display, 0, 0, text, 29, 90, 49, -2, -2, -2, FONT_SMALL, 1);
  CSurface::OnDraw(display, icon, 0, 0);
  SDL_Flip(display);
  
  if (Surf_Text) {
    SDL_FreeSurface(Surf_Text);
  }
  
  bool is_prompt_running = true;
  SDL_Event event;
  while (is_prompt_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN: {
          switch (event.key.keysym.sym) {
            case SDLK_RETURN:
            case SDLK_SPACE:
            case SDLK_y:
            case SDLK_ESCAPE:
            case SDLK_n: {
              is_prompt_running = false;
              break;
            }
            default: {
              break;
            }
          }
          break;
        }
        default: {
          break;
        }
      }
    }
  }
  
  return;
}

void CText::drawTextWindow(SDL_Surface *display, int x, int y, int width,
        int height, int R, int G, int B)
{
  
  if (R == -1 || G == -1 || B == -1) {
    return;
  } else if (R == -2 || G == -2 || B == -2) {
    drawWindowTiles(display, x, y, width, height, WINDOW_CENTER);
  } else {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    Uint32 color = SDL_MapRGB(display->format, R, G, B);
    SDL_FillRect(display, &rect, color);
  }
}

void CText::drawWindowTiles(SDL_Surface *display, int x, int y, int width,
        int height, int tile)
{
  int tile_x = (tile % (window_tiles_->w / WINDOW_TILE)) * WINDOW_TILE;
  int tile_y = (tile / (window_tiles_->w / WINDOW_TILE)) * WINDOW_TILE;
  
  for (int tmp_x = x; tmp_x - x < width; tmp_x += WINDOW_TILE) {
    for (int tmp_y = y; tmp_y - y < height; tmp_y += WINDOW_TILE) {
      int draw_width = WINDOW_TILE;
      int draw_height = WINDOW_TILE;
      if ((tmp_x - x + WINDOW_TILE) > (width - tmp_x + x)) {
        draw_width = width - tmp_x + x;
        if (draw_width > WINDOW_TILE) {
          draw_width = WINDOW_TILE;
        }
      }
      if ((tmp_y - y + WINDOW_TILE) > (height - tmp_y + y)) {
        draw_height = height - tmp_y + y;
        if (draw_height > WINDOW_TILE) {
          draw_height = WINDOW_TILE;
        }
      }
      CSurface::OnDraw(display, window_tiles_, tmp_x, tmp_y, tile_x,
          tile_y, draw_width, draw_height);
    }
  }
}

void CText::drawWindowBorders(SDL_Surface *display, int x, int y, int width,
        int height)
{
  //Draw top border
  drawWindowTiles(display, x, y - WINDOW_TILE, width, WINDOW_TILE, WINDOW_TOP);
  
  //Draw top left corner
  drawWindowTiles(display, x - WINDOW_TILE, y - WINDOW_TILE, WINDOW_TILE,
      WINDOW_TILE, WINDOW_TOP_LEFT);
  
  //Draw top right corner
  drawWindowTiles(display, x + width, y - WINDOW_TILE, WINDOW_TILE,
      WINDOW_TILE, WINDOW_TOP_RIGHT);
  
  //Draw bottom border
  drawWindowTiles(display, x, y + height, width, WINDOW_TILE, WINDOW_BOT);
  
  //Draw bottom left corner
  drawWindowTiles(display, x - WINDOW_TILE, y + height, WINDOW_TILE,
      WINDOW_TILE, WINDOW_BOT_LEFT);
  
  //Draw bottom right corner
  drawWindowTiles(display, x + width, y + height, WINDOW_TILE, WINDOW_TILE,
      WINDOW_BOT_RIGHT);
  
  //Draw left border
  drawWindowTiles(display, x - WINDOW_TILE, y, WINDOW_TILE, height,
      WINDOW_LEFT);
  
  //Draw right border
  drawWindowTiles(display, x + width, y, WINDOW_TILE, height, WINDOW_RIGHT);
}

void CText::getTextDimensions(std::string text, int font, int *width,
        int *height)
{ 
  SDL_Surface *bmp_font = NULL;
  int bmp_space = 0;
  int bmp_newline = 0;
  SDL_Rect *bmp_chars = NULL;
  
  if (font == FONT_SMALL) {
    bmp_font = bmp_font_small_;
    bmp_space = bmp_space_small_;
    bmp_newline = bmp_newline_small_;
    bmp_chars = bmp_chars_cmall_;
  } else if (font == FONT_MEDIUM_BOLD) {
    bmp_font = bmp_font_medium_bold_;
    bmp_space = bmp_space_medium_bold_;
    bmp_newline = bmp_newline_medium_bold_;
    bmp_chars = bmp_chars_medium_bold_;
  }
  
  int cell_h = bmp_font->h / 8;
  int text_width = 0;
  int text_height = 0;
  int tmp_x = 0;
  int tmp_y = cell_h;
  if (bmp_font != NULL) {
    for (int i = 0; text[i] != '\0'; i++) {
      if (text[i] == ' ') {
        tmp_x += bmp_space;
      } else if (text[i] == '\t') {
        tmp_x += TAB_WIDTH * bmp_space;
      } else if (text[i] == '\n') {
        if (tmp_x > text_width) {
          text_width = tmp_x;
        }
        tmp_y += bmp_newline;
        tmp_x = 0;
      } else {
        int ascii = (unsigned char)text[i];
        if (ascii > (16 * 8)) continue;
        tmp_x += bmp_chars[ascii].w + 1;
      }
    }
  }
  if (tmp_x > text_width) {
    text_width = tmp_x;
  }
  text_height = tmp_y;
  
  *width = text_width;
  *height = text_height;
}
