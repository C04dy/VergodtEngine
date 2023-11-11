#ifndef INPUT_H
#define INPUT_H

#pragma once

#include <SDL3/SDL_events.h>
#include <box2d/b2_math.h>

using Vector2 = b2Vec2;

class Input
{
public:
    Input();
    ~Input();

    void SetKeyDown(SDL_Keycode KeyCode);
    void SetKeyUp(SDL_Keycode KeyCode);
    
    void SetMousePos(Vector2 MousePos);

    bool IsKeyDown(SDL_Keycode Key);
    bool IsKeyUp(SDL_Keycode Key);

    bool IsKeyPressed(SDL_Keycode Key);
    bool IsKeyNotPressed(SDL_Keycode Key);

    bool IsMouseKeyDown(SDL_Keycode Key);
    bool IsMouseKeyUp(SDL_Keycode Key);

    bool IsMouseKeyPressed(SDL_Keycode Key);
    bool IsMouseKeyNotPressed(SDL_Keycode Key);

private:
    int m_key = 0;
    bool m_iskeyup = false;
    bool m_iskeydown = false;
    bool m_keypressed = false;

    Vector2 m_mousepos = Vector2(0, 0);
};

#endif