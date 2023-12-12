#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <string>

class SDL_Window;
class SDL_Renderer;

class Window
{
public:
    Window(const std::string& Title, int Width, int Height);
    ~Window();

    SDL_Window* GetWindow() { return m_window; }

    SDL_Renderer* GetRenderer() { return m_renderer; }
private:
    SDL_Window* m_window = nullptr;

    SDL_Renderer* m_renderer = nullptr;
};

#endif