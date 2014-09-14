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
#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

#include "Rectangle.h"

class IAction;

/*
 * This class is a button element of a GUI
 * It can store an image of itself
 */
class Button {
  public:
    Button(Rectangle rect, SDL_Surface *image);
    // Deletes set images.
    ~Button();
    
    // set highlight image that is displayed on top of default image
    void setHighlightImage(SDL_Surface *highlight_image);
    // set highlight image and rectangle as offset
    void setHighlightImage(SDL_Surface *highlight_image, Rectangle rect);
    // Set image which is used when the button is pressed.
    void setPressedImage(SDL_Surface *pressed_image);
    // Set the action to be performed by this button.
    void setAction(IAction *action);
    long getId();
    void setId(long id);
    // Perform the stored action.
    void performAction();
    // Paint itself.
    void paint(SDL_Surface *destination);
    // Mouse event handlers.
    void mouseMoved(int mouse_x, int mouse_y);
    void mouseClicked(int mouse_x, int mouse_y);
    void mouseReleased(int mouse_x, int mouse_y);
    // User used keyboard to select a component with given id
    void keySelected(long id);

  private:
    Rectangle rect_image_;
    Rectangle rect_highlight_;
    SDL_Surface *image;
    SDL_Surface *highlight_image_;
    SDL_Surface *pressed_image_;
    bool pressed_;
    bool highlighted_;
    long id_;
    IAction *action_;
    
};

#endif

