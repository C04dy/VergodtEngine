#include "Window.h"
#include <SDL3/SDL.h>
#include <stdexcept>
#include <iostream>

Window::Window(const std::string& Title, int Width, int Height, bool UseVsync)
{
    if (SDL_CreateWindowAndRenderer(Title.c_str(), Width, Height, SDL_WINDOW_VULKAN, &m_Window, &m_Renderer) < 0)
    {
        std::cout << "SDL_CreateWindowAndRenderer failed :";
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_SetRenderVSync(m_Renderer, UseVsync);
}

Window::~Window()
{
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
}