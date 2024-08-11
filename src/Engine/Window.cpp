#include "Window.h"
#include <SDL3/SDL.h>

Window::Window(const std::string& Title, int Width, int Height, bool UseVsync){
    if(SDL_CreateWindowAndRenderer(Title.c_str(), Width, Height, SDL_WINDOW_VULKAN, &m_window, &m_renderer) < 0){
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        SDL_Quit();
        return ;
    }
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_SetRenderVSync(m_renderer, UseVsync);
}

Window::~Window(){
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}