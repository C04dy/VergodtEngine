#include "Engine.h"

void Engine::EngineStart()
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVERYTHING);

    Scene scene;

    SDL_Window* window = SDL_CreateWindow(scene.GetGameName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scene.GetWindowWidth(), scene.GetWindowHeight(), 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event e;

    unsigned int StartTime = SDL_GetTicks();
    unsigned int EndTime = SDL_GetTicks();
    double delta = 0;

    scene.Start(renderer);
    while (IsRunning()) {
        StartTime = SDL_GetTicks();
        delta = StartTime - EndTime;


        while (SDL_PollEvent(&e) > 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                SetRunning(false);
                break;
            }
        }

        scene.Update(delta);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        scene.Draw();
        SDL_RenderPresent(renderer);

        if (delta > 1000 / 60)
        {
            //std::cout << "fps: " << 1000 / delta << std::endl;
            scene.PhysicsUpdate(delta);

            EndTime = StartTime;
        }

    }

    scene.Clean();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}