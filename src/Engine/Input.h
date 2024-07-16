#ifndef INPUT_H
#define INPUT_H

#pragma once

#include <SDL3/SDL_events.h>
#include <vector>
#include "Vector2.h"

class KeyboardInputNode;
class MouseInputNode;

class InputManager
{
public:
    void SetKeyDown(SDL_Keycode KeyCode);
    void SetKeyUp(SDL_Keycode KeyCode);
    
    void SetMousePos(Vector2 MousePos);

    void SetMouseKeyDown(int Button);
    void SetMouseKeyUp(int Button);

    bool IsKeyDown(SDL_Keycode Key);
    bool IsKeyUp(SDL_Keycode Key);

    bool IsKeyPressed(SDL_Keycode Key);
    bool IsKeyNotPressed(SDL_Keycode Key);

    bool IsMouseKeyJustPressed(int Key);
    bool IsMouseKeyJustReleased(int Key);

    bool IsMouseKeyPressed(int Key);
    bool IsMouseKeyNotPressed(int Key);

    void SetInputNode(KeyboardInputNode* InputNode);
    void SetInputNode(MouseInputNode* InputNode);
    
    void UpdateKeyboardInputNodes();
    void UpdateMouseInputNodes();

    bool IsMouseKeyDown() { return m_ismousekeydown; }
    bool IsKeyboardKeyDown() { return m_iskeydown; }

    Vector2 GetMousePos() { return m_mousepos; }

    const Uint8* keystates = nullptr;
private:
    int m_key = 0;
    bool m_iskeyup = true;
    bool m_iskeydown = false;
    bool m_keypressed = false;

    int m_mousekey = 0;
    bool m_ismousekeyup = true;
    bool m_ismousekeydown = false;
    bool m_mousekeypressed = false;

    Vector2 m_mousepos = Vector2(0, 0);

    std::vector<KeyboardInputNode*> KeyboardInputNodes;

    std::vector<MouseInputNode*> MouseInputNodes;
};

#endif