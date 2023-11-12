#include "Input.h"

#include <iostream>

Input::Input()
{

}

Input::~Input()
{

}

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

void Input::SetMouseKeyDown(Uint8 Button){
    std::cout << Button;
    m_mousekey = Button;
    m_ismousekeydown = true;
    m_ismousekeyup = false;
    m_mousekeypressed = true;
}

void Input::SetMouseKeyUp(Uint8 Button){
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
    if(m_keypressed && Key == m_key){
        return true;
    }
    return false;
}

bool Input::IsKeyNotPressed(SDL_Keycode Key){
    if(m_keypressed == false && Key == m_key){
        return true;
    }
    return false;
}

bool Input::IsMouseKeyDown(Uint8 Key){
    if(m_ismousekeydown && Key == m_key){
        m_ismousekeydown = false;
        return true;
    }
    return false;
}

bool Input::IsMouseKeyUp(Uint8 Key){
    if(m_ismousekeyup && Key == m_key){
        m_ismousekeyup = false;
        return true;
    }
    return false;
}

bool Input::IsMouseKeyPressed(Uint8 Key){
    if(m_mousekeypressed && Key == m_key){
        return true;
    }
    return false;
}

bool Input::IsMouseKeyNotPressed(Uint8 Key){
    if(m_mousekeypressed == false && Key == m_key){
        return true;
    }
    return false;
}