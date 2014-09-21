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
#ifndef _CEVENT_H_
#define _CEVENT_H_

#include <SDL.h>

/**
 * This is an interface for all possible SDL events.
 */
class CEvent {
  public:
    CEvent();
    virtual ~CEvent();

    virtual void OnEvent(SDL_Event *event);
    virtual void OnInputFocus();
    virtual void OnInputBlur();
    virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnMouseFocus();
    virtual void OnMouseBlur();
    virtual void OnMouseMove(int mX, int mY, int relX, int relY,
                  bool Left, bool Right, bool Middle);
    virtual void OnMouseWheel(bool Up, bool Down);
    virtual void OnLButtonDown(int mX, int mY);
    virtual void OnLButtonUp(int mX, int my);
    virtual void OnRButtonDown(int mX, int mY);
    virtual void OnRButtonUp(int mX, int mY);
    virtual void OnMButtonDown(int mX, int mY);
    virtual void OnMButtonUp(int mX, int mY);
    virtual void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
    virtual void OnJoyButtonDown(Uint8 which, Uint8 button);
    virtual void OnJoyButtonUp(Uint8 which, Uint8 button);
    virtual void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
    virtual void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
    virtual void OnMinimize();
    virtual void OnRestore();
    virtual void OnResize(int w, int h);
    virtual void OnExpose();
    virtual void OnExit();
    virtual void OnUser(Uint8 type, int code, void *data1, void *data2);
};

#endif
