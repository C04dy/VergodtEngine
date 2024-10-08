#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <string>

class SDL_Window;
class SDL_Renderer;

class Window
{
public:
    Window(const std::string& Title, int Width, int Height, bool UseVsync);
    ~Window();

    SDL_Window* GetWindow() { return m_Window; }

    SDL_Renderer* GetRenderer() { return m_Renderer; }
private:
    SDL_Window* m_Window = nullptr;

    SDL_Renderer* m_Renderer = nullptr;
};

#endif