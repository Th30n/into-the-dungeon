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
#include "Button.h"
#include "CSurface.h"
#include "IAction.h"

Button::Button(Rectangle rect, SDL_Surface *image) :
  rect_image_(rect), rect_highlight_(rect), image(image),
  highlight_image_(0), pressed_image_(0),
  pressed_(false), highlighted_(false),
  id_(0),
  action_(0)
{
}

Button::~Button()
{
  if (image) {
    SDL_FreeSurface(image);
    image = NULL;
  }
  if (highlight_image_) {
    SDL_FreeSurface(highlight_image_);
    highlight_image_ = NULL;
  }
  if (pressed_image_) {
    SDL_FreeSurface(pressed_image_);
    pressed_image_ = NULL;
  }
}

void Button::setHighlightImage(SDL_Surface *highlight_image)
{
  if (highlight_image_) {
    SDL_FreeSurface(highlight_image_);
    highlight_image_ = NULL;
  }
  highlight_image_ = highlight_image;
}

void Button::setHighlightImage(SDL_Surface *highlight_image, Rectangle rect)
{
  rect_highlight_ = rect;
  setHighlightImage(highlight_image);
}

void Button::setPressedImage(SDL_Surface *pressed_image)
{
  if (pressed_image_) {
    SDL_FreeSurface(pressed_image_);
    pressed_image_ = NULL;
  }
  pressed_image_ = pressed_image;
}

void Button::setAction(IAction *action)
{
  action_ = action;
}

long Button::getId()
{
  return id_;
}

void Button::setId(long id)
{
  id_ = id;
}

void Button::performAction()
{
  if(action_) {
    action_->performAction();
  }
}

void Button::paint(SDL_Surface *destination)
{
  if (pressed_ && pressed_image_) {
    CSurface::OnDraw(destination, pressed_image_,
              rect_image_.getX(), rect_image_.getY());
  } else {
    CSurface::OnDraw(destination, image,
              rect_image_.getX(), rect_image_.getY());
  }
  if (highlighted_ && highlight_image_) {
    CSurface::OnDraw(destination, highlight_image_,
              rect_highlight_.getX(), rect_highlight_.getY());
  }
}

void Button::mouseMoved(int mouse_x, int mouse_y)
{
  if (rect_image_.intersects(mouse_x, mouse_y)) {
    highlighted_ = true;
  } else {
    highlighted_ = false;
  }
}

void Button::mouseClicked(int mouse_x, int mouse_y)
{
  if (rect_image_.intersects(mouse_x, mouse_y)) {
    pressed_ = true;
  } else {
    pressed_ = false;
  }
}

void Button::mouseReleased(int mouse_x, int mouse_y)
{
  pressed_ = false;
  if (action_ && rect_image_.intersects(mouse_x, mouse_y)) {
    action_->performAction();
  }
}

void Button::keySelected(long id)
{
  if (id_ == id) {
    highlighted_ = true;
  } else {
    highlighted_ = false;
  }
}
