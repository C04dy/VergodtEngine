#include "Window.h"
#include <SDL3/SDL.h>

Window::Window(const std::string& Title, int Width, int Height){
    if(SDL_CreateWindowAndRenderer(Width, Height, SDL_WindowFlags::SDL_WINDOW_VULKAN, &m_window, &m_renderer) < 0){
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        SDL_Quit();
        return ;
    }
    SDL_SetWindowTitle(m_window, Title.c_str());

    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

Window::~Window(){
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}