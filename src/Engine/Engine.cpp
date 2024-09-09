#include "Engine.h"
#include "Scene.h"
#include "Input.h"
#include "Window.h"
#include <box2d/b2_world.h>

void Engine::EngineStart()
{
    Scene scene;

    InputManager input; 

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
    {
        std::cout << "SDL_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP) < 0)
    {
        std::cout << "IMG_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    Window* window_pointer = new Window(scene.GetGameName(), scene.GetWindowWidth(), scene.GetWindowHeight(), true);

    SDL_Event event;

	int32 velocity_iterations = 6;
	int32 position_iterations = 2;

    m_PhysicsWorld = new b2World({ 0.0f, 9.8f });

    m_PhysicsWorld->SetWarmStarting(true);
    m_PhysicsWorld->SetContinuousPhysics(true);

    double end_time = 0;
    double delta = 0;

    scene.SetRenderer(window_pointer->GetRenderer());
    scene.SetPhysicsWorld(m_PhysicsWorld);
    scene.SetInput(&input);
    scene.Start();

    while (IsRunning())
    {
        float time = (float)(SDL_GetTicks()) / 1000.0f;
        delta = time - end_time;
        end_time = time;

        if (input.DoInput(&event) == false)
        {
            SetRunning(false);
            goto Quit;
        }

        scene.Update(delta);

        m_PhysicsWorld->Step(delta, velocity_iterations, position_iterations);

        SDL_SetRenderDrawColor(window_pointer->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(window_pointer->GetRenderer());
        scene.Draw();
        SDL_RenderPresent(window_pointer->GetRenderer());
    }
    
Quit:

    scene.Clean();

    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
    delete window_pointer;
    window_pointer = nullptr;

    SDL_Quit();
    IMG_Quit();
}