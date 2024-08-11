#ifndef APP_H
#define APP_H

#pragma once

#include <stdexcept>
#include <iostream>

class SDL_Window;
class SDL_Renderer;

class App
{
public:
    ~App();
    
    void Init();

    int Run();

    bool IsAppRunning() { return m_running; }

private:
    bool m_running = false;

    SDL_Window* m_window;

    SDL_Renderer* m_renderer;
};

#endif