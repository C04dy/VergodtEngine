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

    Uint64 PerfFreq = SDL_GetPerformanceFrequency();
    Uint64 FpsCounter = SDL_GetPerformanceCounter();
    Uint64 EndTime = 0;
    Uint64 ElapsedTime;
    Uint64 FPS;
    double delta = 0;

    scene->SetRenderer(renderer);
    scene->SetPhysicsWorld(m_physicsworld);
    scene->Start();

    while (IsRunning()) {
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
            case SDL_EVENT_MOUSE_MOTION:
                scene->mx = e.motion.x;
                scene->my = e.motion.y;
                break;
            }
        }

        scene->Update(delta);

        m_physicsworld->Step(timeStep * delta * 1000, velocityIterations, positionIterations);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        scene->Draw();
        SDL_RenderPresent(renderer);

        EndTime = SDL_GetPerformanceCounter();
        ElapsedTime = EndTime - FpsCounter;
        delta = (double)ElapsedTime / (double)PerfFreq;
        FPS = (double)PerfFreq / (double)ElapsedTime;
        FpsCounter = EndTime;
    }

    scene->Clean();

    delete m_physicsworld;
    m_physicsworld = nullptr;
    delete scene;
    scene = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}