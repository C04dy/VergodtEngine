#include "Engine.h"
#include <box2d/b2_world.h>

void Engine::EngineStart(BaseScene* S)
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVERYTHING);

    BaseScene* scene = S;

    SDL_Window* window = SDL_CreateWindow(scene->GetGameName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scene->GetWindowWidth(), scene->GetWindowHeight(), 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event e;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
    double timeStep = 1.0f / 480.0f;

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
            case SDL_QUIT:
                SetRunning(false);
                break;
            case SDL_KEYUP:
                break;
            case SDL_KEYDOWN:
                break;
            }
        }

        scene->Update(delta);

        m_physicsworld->Step(timeStep, velocityIterations, positionIterations);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        scene->Draw();
        SDL_RenderPresent(renderer);
        //if (delta > 1000 / 60) { }
    }

    scene->Clean();

    delete m_physicsworld;
    m_physicsworld = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}