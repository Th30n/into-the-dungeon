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
#include "CEvent.h"

CEvent::CEvent()
{
}

CEvent::~CEvent()
{
}

void CEvent::OnEvent(SDL_Event *event)
{
  switch(event->type) {
    case SDL_ACTIVEEVENT: {
      switch(event->active.state) {
        case SDL_APPMOUSEFOCUS: {
          if (event->active.gain) {
            OnMouseFocus();
          } else {
            OnMouseBlur();
          }
          break;
        }
        case SDL_APPINPUTFOCUS: {
          if (event->active.gain) {
            OnInputFocus();
          } else {
            OnInputBlur();
          }
          break;
        }
        case SDL_APPACTIVE: {
          if (event->active.gain) {
            OnRestore();
          } else {
            OnMinimize();
          }
          break;
        }
      }
      break;
    }
    case SDL_KEYDOWN: {
      OnKeyDown(event->key.keysym.sym, event->key.keysym.mod,
          event->key.keysym.unicode);
      break;
    }
    case SDL_KEYUP: {
      OnKeyUp(event->key.keysym.sym, event->key.keysym.mod,
          event->key.keysym.unicode);
      break;
    }
    case SDL_MOUSEMOTION: {
      OnMouseMove(event->motion.x, event->motion.y, event->motion.xrel,
          event->motion.yrel,
          (event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)) != 0,
          (event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0,
          (event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      switch(event->button.button) {
        case SDL_BUTTON_LEFT: {
          OnLButtonDown(event->button.x, event->button.y);
          break;
        }
        case SDL_BUTTON_RIGHT: {
          OnRButtonDown(event->button.x, event->button.y);
          break;
        }
        case SDL_BUTTON_MIDDLE: {
          OnMButtonDown(event->button.x, event->button.y);
          break;
        }
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      switch(event->button.button) {
        case SDL_BUTTON_LEFT: {
          OnLButtonUp(event->button.x, event->button.y);
          break;
        }
        case SDL_BUTTON_RIGHT: {
          OnRButtonUp(event->button.x, event->button.y);
          break;
        }
        case SDL_BUTTON_MIDDLE: {
          OnMButtonUp(event->button.x, event->button.y);
          break;
        }
      }
      break;
    }
    case SDL_JOYAXISMOTION: {
      OnJoyAxis(event->jaxis.which, event->jaxis.axis, event->jaxis.value);
      break;
    }
    case SDL_JOYBALLMOTION: {
      OnJoyBall(event->jball.which, event->jball.ball, event->jball.xrel, event->jball.yrel);
      break;
    }
    case SDL_JOYHATMOTION: {
      OnJoyHat(event->jhat.which, event->jhat.hat, event->jhat.value);
      break;
    }
    case SDL_JOYBUTTONDOWN: {
      OnJoyButtonDown(event->jbutton.which, event->jbutton.button);
      break;
    }
    case SDL_JOYBUTTONUP: {
      OnJoyButtonUp(event->jbutton.which, event->jbutton.button);
      break;
    }
    case SDL_QUIT: {
      OnExit();
      break;
    }
    case SDL_SYSWMEVENT: {
      //ignore
      break;
    }
    case SDL_VIDEORESIZE: {
      OnResize(event->resize.w, event->resize.h);
      break;
    }
    case SDL_VIDEOEXPOSE: {
      OnExpose();
      break;
    }
    default: {
      OnUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
      break;
    }
  }
}

void CEvent::OnInputFocus()
{
  //pure virtual, do nothing
}

void CEvent::OnInputBlur()
{
  //pure virtual
}

void CEvent::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  //pure virtual
}

void CEvent::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  //pure virtual
}

void CEvent::OnMouseFocus()
{
  //pure virtual
}

void CEvent::OnMouseBlur()
{
  //pure virtual
}

void CEvent::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
  //pure virtual
}

void CEvent::OnMouseWheel(bool Up, bool Down)
{
  //pure virtual
}

void CEvent::OnLButtonDown(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnLButtonUp(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnRButtonDown(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnRButtonUp(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnMButtonDown(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnMButtonUp(int mX, int mY)
{
  //pure virtual
}

void CEvent::OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value)
{
  //pure virtual
}

void CEvent::OnJoyButtonDown(Uint8 which, Uint8 button)
{
  //pure virtual
}

void CEvent::OnJoyButtonUp(Uint8 which, Uint8 button)
{
  //pure virtual
}

void CEvent::OnJoyHat(Uint8 which, Uint8 hat, Uint8 value)
{
  //pure virtual
}

void CEvent::OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel)
{
  //pure virtual
}

void CEvent::OnMinimize()
{
  //pure virtual
}

void CEvent::OnRestore()
{
  //pure virtual
}

void CEvent::OnResize(int w, int h)
{
  //pure virtual
}

void CEvent::OnExpose()
{
  //pure virtual
}

void CEvent::OnExit()
{
  //pure virtual
}

void CEvent::OnUser(Uint8 type, int code, void *data1, void *data2)
{
  //pure virtual
}

