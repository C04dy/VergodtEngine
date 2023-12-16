#include "Input.h"
#include "VisualScripting/ScriptingNodes.h"
#include <iostream>

void Input::SetKeyDown(SDL_Keycode KeyCode){
    m_key = KeyCode;
    m_iskeydown = true;
    m_iskeyup = false;
    m_keypressed = true;
}

void Input::SetKeyUp(SDL_Keycode KeyCode){
    m_key = KeyCode;
    m_iskeyup = true;
    m_iskeydown = false;
    m_keypressed = false;
}

void Input::SetMousePos(Vector2 MousePos){
    m_mousepos = MousePos;
}

void Input::SetMouseKeyDown(int Button){
    m_mousekey = Button;
    m_ismousekeydown = true;
    m_ismousekeyup = false;
    m_mousekeypressed = true;
}

void Input::SetMouseKeyUp(int Button){
    m_mousekey = Button;
    m_ismousekeyup = true;
    m_ismousekeydown = false;
    m_mousekeypressed = false;
}

bool Input::IsKeyDown(SDL_Keycode Key){
    if(m_iskeydown && Key == m_key){
        m_iskeydown = false;
        return true;
    }
    return false;
}

bool Input::IsKeyUp(SDL_Keycode Key){
    if(m_iskeyup && Key == m_key){
        m_iskeyup = false;
        return true;
    }
    return false;
}

bool Input::IsKeyPressed(SDL_Keycode Key){
    if(keystates[SDL_GetScancodeFromKey(Key)]){
        return true;
    }
    return false;
}

bool Input::IsKeyNotPressed(SDL_Keycode Key){
    if(!keystates[SDL_GetScancodeFromKey(Key)]){
        return true;
    }
    return false;
}

bool Input::IsMouseKeyDown(int Key){
    if(m_ismousekeydown && Key == m_mousekey){
        m_ismousekeydown = false;
        return true;
    }
    return false;
}

bool Input::IsMouseKeyUp(int Key){
    if(m_ismousekeyup && Key == m_mousekey){
        m_ismousekeyup = false;
        return true;
    }
    return false;
}

bool Input::IsMouseKeyPressed(int Key){
    if(m_mousekeypressed && Key == m_mousekey){
        return true;
    }
    return false;
}

bool Input::IsMouseKeyNotPressed(int Key){
    if(m_mousekeypressed == false)
        return true;
    if(m_mousekeypressed == true && Key != m_mousekey)
        return true;
    return false;
}

void Input::UpdateKeyboardInputNodes(){
    for (int i = 0; i < (int)KeyboardInputNodes.size(); i++){
        KeyboardInputNodes[i]->SendSignal();
    }   
}

void Input::UpdateMouseInputNodes(){
    for (int i = 0; i < (int)MouseInputNodes.size(); i++){
        MouseInputNodes[i]->SendSignal();
        MouseInputNodes[i]->X = m_mousepos.x;
        MouseInputNodes[i]->Y = m_mousepos.y;
    }   
}

void Input::SetInputNode(KeyboardInputNode* InputNode){
    KeyboardInputNodes.push_back(InputNode);
}

void Input::SetInputNode(MouseInputNode* InputNode){
    MouseInputNodes.push_back(InputNode);
}