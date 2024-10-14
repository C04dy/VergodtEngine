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
    ~InputManager();
    
    bool DoInput(SDL_Event* Event);

    void SetKeyDown(SDL_Keycode KeyCode);
    void SetKeyUp(SDL_Keycode KeyCode);
    
    void SetMousePos(Vector2 MousePos);

    void SetMouseKeyDown(int Button);
    void SetMouseKeyUp(int Button);

    bool IsKeyJustPressed(SDL_Keycode Key);
    bool IsKeyJustReleased(SDL_Keycode Key);

    bool IsKeyPressed(SDL_Keycode Key);
    bool IsKeyNotPressed(SDL_Keycode Key);

    bool IsMouseKeyJustPressed(int Key);
    bool IsMouseKeyJustReleased(int Key);

    bool IsMouseKeyPressed(int Key);
    bool IsMouseKeyNotPressed(int Key);

    void UpdateKeyboardInputNodes();
    void UpdateMouseInputNodes();
    
    void SetInputNode(KeyboardInputNode* InputNode) { m_KeyboardInputNodes.push_back(InputNode); }
    void SetInputNode(MouseInputNode* InputNode) { m_MouseInputNodes.push_back(InputNode); }

    bool IsMouseKeyDown() { return m_IsMouseKeyDown; }
    bool IsKeyboardKeyDown() { return m_IsKeyDown; }

    Vector2 GetMousePos() { return m_Mousepos; }
private:
    const bool* m_KeyStates = nullptr;
    SDL_Keycode m_Key = 0;
    bool m_IsKeyUp = true;
    bool m_IsKeyDown = false;
    bool m_KeyPressed = false;

    int m_MouseKey = 0;
    bool m_IsMouseKeyUp = true;
    bool m_IsMouseKeyDown = false;
    bool m_MouseKeyPressed = false;

    Vector2 m_Mousepos = Vector2(0, 0);

    std::vector<KeyboardInputNode*> m_KeyboardInputNodes;

    std::vector<MouseInputNode*> m_MouseInputNodes;
};

#endif