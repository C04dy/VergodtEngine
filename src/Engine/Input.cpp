#include "Input.h"
#include "Scripting/VisualScripting/ScriptingNodes.h"
#include <iostream>

bool InputManager::DoInput(SDL_Event* Event)
{
    m_IsKeyDown = false;
    m_IsKeyUp = false;
    m_IsMouseKeyDown = false;
    m_IsMouseKeyUp = false;
    while (SDL_PollEvent(Event) > 0)
    {
        switch (Event->type)
        {
        case SDL_EVENT_QUIT:
            return false;
            break;
        case SDL_EVENT_MOUSE_MOTION:
            SetMousePos(Vector2(Event->motion.x, Event->motion.y));
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            SetMouseKeyUp(Event->button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SetMouseKeyDown(Event->button.button);
            break;
        case SDL_EVENT_KEY_DOWN:
            if (Event->key.repeat == 0)
            {
                SetKeyDown(Event->key.key);
            }
            break;
        case SDL_EVENT_KEY_UP:
            SetKeyUp(Event->key.key);
            break;
        }
    }
    m_KeyStates = SDL_GetKeyboardState(NULL);
    UpdateMouseInputNodes();
    UpdateKeyboardInputNodes();
    return true;
}

void InputManager::SetKeyDown(SDL_Keycode KeyCode){
    m_Key = KeyCode;
    m_IsKeyUp = false;
    m_IsKeyDown = true;
    m_KeyPressed = true;
}

void InputManager::SetKeyUp(SDL_Keycode KeyCode){
    m_Key = KeyCode;
    m_IsKeyDown = true;
    m_IsKeyDown = false;
    m_KeyPressed = false;
}

void InputManager::SetMousePos(Vector2 MousePos){
    m_Mousepos = MousePos;
}

void InputManager::SetMouseKeyDown(int Button){
    m_MouseKey = Button;
    m_IsMouseKeyUp = false;
    m_IsMouseKeyDown = true;
    m_MouseKeyPressed = true;
}

void InputManager::SetMouseKeyUp(int Button){
    m_MouseKey = Button;
    m_IsMouseKeyUp = true;
    m_IsMouseKeyDown = false;
    m_MouseKeyPressed = false;
}

bool InputManager::IsKeyJustPressed(SDL_Keycode Key){
    if (m_IsKeyDown && m_Key == Key)
    {
        return true;
    }
    return false;
}

bool InputManager::IsKeyJustReleased(SDL_Keycode Key){
    if (m_IsKeyUp && Key == m_Key)
    {
        return true;
    }
    return false;
}

bool InputManager::IsKeyPressed(SDL_Keycode Key){
    if (m_KeyStates[SDL_GetScancodeFromKey(Key, NULL)])
    {
        return true;
    }
    return false;
}

bool InputManager::IsKeyNotPressed(SDL_Keycode Key){
    if (!m_KeyStates[SDL_GetScancodeFromKey(Key, NULL)])
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseKeyJustPressed(int Key){
    if (m_IsMouseKeyDown && Key == m_MouseKey)
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseKeyJustReleased(int Key){
    if (m_IsMouseKeyUp && Key == m_MouseKey)
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseKeyPressed(int Key){
    if (m_MouseKeyPressed && Key == m_MouseKey)
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseKeyNotPressed(int Key){
    if(m_MouseKeyPressed == false)
        return true;
    if(m_MouseKeyPressed == true && Key != m_MouseKey)
        return true;
    return false;
}

void InputManager::UpdateKeyboardInputNodes(){
    for (int i = 0; i < static_cast<int>(m_KeyboardInputNodes.size()); i++)
    {
        m_KeyboardInputNodes[i]->SendSignal();
    }   
}

void InputManager::UpdateMouseInputNodes(){
    for (int i = 0; i < static_cast<int>(m_MouseInputNodes.size()); i++)
    {
        m_MouseInputNodes[i]->SendSignal();
        m_MouseInputNodes[i]->X = m_Mousepos.x;
        m_MouseInputNodes[i]->Y = m_Mousepos.y;
    }   
}