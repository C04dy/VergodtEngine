#include "Engine.h"
#include "Scene.h"
#include <box2d/b2_world.h>

void Engine::EngineStart()
{
    Scene* scene = new Scene;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL_Init failed (%s)", SDL_GetError());
        return;
    }

    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    if (SDL_CreateWindowAndRenderer(scene->GetWindowWidth(), scene->GetWindowHeight(), SDL_WindowFlags::SDL_WINDOW_VULKAN, &window, &renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        SDL_Quit();
        return ;
    }
    SDL_SetWindowTitle(window, scene->GetGameName().c_str());

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


    SDL_Event e;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
    double timeStep = 1.0f / 60.0f;

    m_physicsworld = new b2World({ 0.0f, 9.8f });

    m_physicsworld->SetWarmStarting(true);
    m_physicsworld->SetContinuousPhysics(true);

    unsigned int StartTime = SDL_GetPerformanceCounter();
    unsigned int EndTime = 0;
    double delta = 0;

    scene->SetRenderer(renderer);
    scene->SetPhysicsWorld(m_physicsworld);
    scene->Start();

    while (IsRunning()) {
        EndTime = StartTime;
        StartTime = SDL_GetPerformanceCounter();
        delta = (double)((StartTime - EndTime)*1000 / (double)SDL_GetPerformanceFrequency() );

        while (SDL_PollEvent(&e) > 0) {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                SetRunning(false);
                break;
            case SDL_EVENT_KEY_UP:
                break;
            case SDL_EVENT_KEY_DOWN:
                break;
            }
        }

        scene->Update(delta);

        m_physicsworld->Step(timeStep * delta, velocityIterations, positionIterations);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        scene->Draw();
        SDL_RenderPresent(renderer);
        //if (delta > 1000 / 60) { }
    }

    scene->Clean();

    delete m_physicsworld;
    m_physicsworld = nullptr;
    delete scene;
    scene = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}