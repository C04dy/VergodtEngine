#ifndef INPUT_H
#define INPUT_H

#pragma once

#include <SDL3/SDL_events.h>
#include "Vector2.h"

class Input
{
public:
    Input();
    ~Input();

    void SetKeyDown(SDL_Keycode KeyCode);
    void SetKeyUp(SDL_Keycode KeyCode);
    
    void SetMousePos(Vector2 MousePos);

    void SetMouseKeyDown(Uint8 Button);
    void SetMouseKeyUp(Uint8 Button);

    bool IsKeyDown(SDL_Keycode Key);
    bool IsKeyUp(SDL_Keycode Key);

    bool IsKeyPressed(SDL_Keycode Key);
    bool IsKeyNotPressed(SDL_Keycode Key);

    bool IsMouseKeyDown(Uint8 Key);
    bool IsMouseKeyUp(Uint8 Key);

    bool IsMouseKeyPressed(Uint8 Key);
    bool IsMouseKeyNotPressed(Uint8 Key);

    Vector2 GetMousePos() { return m_mousepos; }

private:
    int m_key = 0;
    bool m_iskeyup = false;
    bool m_iskeydown = false;
    bool m_keypressed = false;

    Uint8 m_mousekey = 0;
    bool m_ismousekeyup = false;
    bool m_ismousekeydown = false;
    bool m_mousekeypressed = false;

    Vector2 m_mousepos = Vector2(0, 0);
};

#endif