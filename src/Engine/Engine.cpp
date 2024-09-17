#include "Engine.h"
#include "Scene.h"
#include "Input.h"
#include "Window.h"
#include <box2d/box2d.h>

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

	uint32_t velocity_iterations = 6;
	uint32_t position_iterations = 2;

    b2WorldDef world_def = b2DefaultWorldDef();

    world_def.enableContinuous = true;
    world_def.gravity = (b2Vec2){ 0.0f, 9.5f };

    b2WorldId PhysicsWorldID = b2CreateWorld(&world_def);

    double end_time = 0;
    double delta = 0;

    scene.SetRenderer(window_pointer->GetRenderer());
    scene.SetPhysicsWorldID(PhysicsWorldID);
    scene.SetInput(&input);
    scene.Start();

    //SetRunning(false);

    while (IsRunning())
    {
        float time = (float)(SDL_GetTicks()) / 1000.0f;
        delta = time - end_time;
        end_time = time;

        if (input.DoInput(&event) == false)
        {
            SetRunning(false);
            break;
        }

        scene.Update(delta);

        b2World_Step(PhysicsWorldID, delta, 4);

        SDL_SetRenderDrawColor(window_pointer->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(window_pointer->GetRenderer());
        scene.Draw();
        SDL_RenderPresent(window_pointer->GetRenderer());
    }

    scene.Clean();

    b2DestroyWorld(PhysicsWorldID);
    delete window_pointer;
    window_pointer = nullptr;

    SDL_Quit();
    IMG_Quit();
}